"""Self-tests of the proof machinery: the gate must reject what it should reject. Run after tool changes.

    python tools/test_gate.py
"""
import contextlib
import io
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import compare  # noqa: E402
import gate  # noqa: E402
from common import ROOT  # noqa: E402

TIMER = ROOT / "src" / "_unknown" / "tu_0047abc0.c"


def statuses(src, flags, at=None, find=False, as_name=None):
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        obj = Path(td) / "t.obj"
        rc, text = cc.compile_one(src, obj, flags, as_name=as_name)
        assert rc == 0, text
        reps, _ = compare.compare_object(obj, None, at, find)
    return {e["names"][0]: e["status"] for r in reps for s in r["segments"] for e in s.get("extents", [])}


def audit_v1_f3_f5(check):
    import gate
    import promote
    from common import ROOT

    def build(path, entry):
        reports, _, _ = gate.build_and_compare(path, entry, find=True, imply=True)
        return gate.by_label(gate.extents(reports))

    def dry_promote(path, tu, entry):
        argv = ['promote.py', str(path), '--tu', tu, '--as', entry['as']]
        if entry.get('flags') is not None:
            _, tool, flags = gate.flags_for(entry)
            argv += ['--flags', ' '.join(flags), '--tool', tool]
        else:
            argv += ['--profile', entry['profile']]
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            rc = promote.main(argv)  # no --publish, no --accept-loss
        return rc, output.getvalue()

    ledger = gate.load_ledger()
    shell_tu = 'src/shell/shell.c'
    shell_entry = ledger['tus'][shell_tu]
    shell = ROOT / shell_tu
    good = build(shell, shell_entry)
    check(good.get('Shell::Event', {}).get('status') == 'STRICT',
          'F3 positive: unchanged switch owner is STRICT')
    text = shell.read_text(encoding='latin-1')
    head, marker, tail = text.partition('void Shell::Event(unsigned char)')
    buy = 'case 25:case 29:'
    store = 'case 27:\n            ShellStorageRender'
    shape = bool(marker) and tail.count(buy) == 1 and tail.count(store) == 1
    check(shape, 'F3 fixture still has distinct screen-27 and screen-29 handlers')
    if not shape:
        return
    swapped = head + marker + tail.replace(buy, 'case 25:case 27:', 1).replace(
        store, 'case 29:\n            ShellStorageRender', 1)
    with tempfile.TemporaryDirectory(dir=ROOT / 'build') as td:
        candidate = Path(td) / 'shell.c'
        candidate.write_text(swapped, encoding='latin-1')
        bad = build(candidate, shell_entry)
        check(bad.get('Shell::Event', {}).get('status') != 'STRICT',
              'F3 negative: case swap invalidates Event itself, including its external table')
        check(bad.get('Shell::RestoreMenu', {}).get('status') == 'DIFF',
              'F3 fixture changes the table in the predecessor extent')
        rc, output = dry_promote(candidate, shell_tu, shell_entry)
        check(rc == 1 and 'LOSS' in output and 'Shell::Event' in output,
              'F3 promotion reports loss of the switch owner')

    fog_tu = 'src/terr/tfog.c'
    fog_entry = ledger['tus'][fog_tu]
    fog = ROOT / fog_tu
    original = fog.read_text(encoding='latin-1')
    needle = 'void FogNoOp() {}'
    shape = original.count(needle) == 1 and original.count('\n\n') >= 2
    check(shape, 'F5 fixture has one empty function and spare blank lines')
    if not shape:
        return
    split = original.replace(needle,
        '#pragma code_seg("AUDIT_TEXT", "CODE")\nvoid FogNoOp() {}\n#pragma code_seg()', 1)
    split = split.replace('\n\n', '\n', 2)  # preserve assert __LINE__ values
    check([i for i, line in enumerate(original.splitlines()) if 'AssertFail(' in line] ==
          [i for i, line in enumerate(split.splitlines()) if 'AssertFail(' in line],
          'F5 fixture preserves assertion source lines')
    good = build(fog, fog_entry)
    rc, output = dry_promote(fog, fog_tu, fog_entry)
    check(rc == 0, 'F5 positive: unchanged TU passes dry promotion')
    with tempfile.TemporaryDirectory(dir=ROOT / 'build') as td:
        candidate = Path(td) / 'tfog.c'
        candidate.write_text(split, encoding='latin-1')
        bad = build(candidate, fog_entry)
        before, after = good.get('FogNoOp', {}), bad.get('FogNoOp', {})
        check(before.get('status') == after.get('status') == 'STRICT' and
              before.get('va') == after.get('va') and
              before.get('end', 0) - before.get('off', 0) == 16 and
              after.get('end', 0) - after.get('off', 0) == 1 and after.get('tail_fill') == 15,
              'F5 fixture is a STRICT same-VA 16-to-1-byte size loss')
        other_losses = [n for n, e in fog_entry['functions'].items() if n != 'FogNoOp' and
            (bad.get(n, {}).get('status') != 'STRICT' or bad[n]['va'] != e['va'] or
             bad[n]['end'] - bad[n]['off'] != e['size'])]
        check(not other_losses, 'F5 fixture preserves every other accepted function')
        rc, output = dry_promote(candidate, fog_tu, fog_entry)
        check(rc == 1 and 'LOSS' in output and 'FogNoOp' in output,
              'F5 negative: dry promotion rejects size loss without --accept-loss')


def audit_v1_r2_scalars(check):
    """R2.1/R2.2: a named scalar's initializer is proven in full and a zero (BSS) object must be zero in the oracle."""
    import gate
    from common import ROOT
    tu = "src/_unknown/tu_0042a0e0.c"
    entry = gate.load_ledger()["tus"][tu]
    text = (ROOT / tu).read_text(encoding="latin-1")
    decl = "extern long gGuiFadeStep,"
    check(decl in text, "R2 fixture: fade TU declares gGuiFadeStep extern")
    if decl not in text:
        return
    res = {}
    for tag, init in (("good", "64"), ("high", "65600"), ("zero", "0")):
        with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
            p = Path(td) / "tu_0042a0e0.c"
            p.write_text(text.replace(decl, "long gGuiFadeStep=%s; extern long " % init, 1), encoding="latin-1")
            r, _, _ = gate.build_and_compare(p, entry, find=True, imply=True)
            ext = gate.by_label(gate.extents(r))
            res[tag] = [ext.get(n, {}).get("status") for n in ("GuiFadeOut", "GuiFadeOutRange", "GuiFadeIn")]
    check(all(x == "STRICT" for x in res["good"]), "R2 positive: correctly initialized scalar keeps consumers STRICT")
    check(not any(x == "STRICT" for x in res["high"]), "R2.1 negative: wrong high byte of a scalar is not STRICT")
    check(not any(x == "STRICT" for x in res["zero"]), "R2.2 negative: zero (BSS) object over nonzero oracle data is not STRICT")


def resource_checks(check):
    """tools/rsrc.py: policy and sensitivity (a one-pixel or one-character change is not STRICT)."""
    import shutil
    import rsrc
    from common import CONFIG, load_json
    script = ROOT / load_json(CONFIG / "toolchain.json")["resources"]["script"]
    if not script.exists():
        return
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        d = Path(td)
        for f in script.parent.iterdir():
            shutil.copyfile(f, d / f.name)
        rc = d / script.name
        text = rc.read_text(encoding="cp1252")
        rc.write_text(text + "\nBLOB RCDATA { 1, 2, 3 }\n", encoding="cp1252")
        check(bool(rsrc.policy(rc)), "resource policy rejects RCDATA")
        rc.write_text(text.replace("mcursor7.cur", "mcursor7.bin"), encoding="cp1252")
        check(bool(rsrc.policy(rc)), "resource policy rejects a file without pixel-text source")
        rc.write_text(text.replace("Show Ore Locations", "Show Ore Locationz"), encoding="cp1252")
        cur = d / "mcursor3.cur.txt"
        lines = cur.read_text(encoding="ascii").splitlines()
        i = next(k for k, ln in enumerate(lines) if ln and set(ln) <= set(".#o~") and "#" in ln)
        lines[i] = lines[i].replace("#", ".", 1)
        cur.write_text("\n".join(lines) + "\n", encoding="ascii")
        exe, conf = rsrc.build(script=rc)
        import io
        import contextlib
        with contextlib.redirect_stdout(io.StringIO()) as buf:
            ok, strict, total = rsrc.compare(exe, conf, verbose=True)
        out = buf.getvalue()
        check(not ok and strict == total - 2 and "APPMENU" in out.split("DIFF", 1)[-1],
              "changed menu text and one cursor pixel are not STRICT (resources)")


def main():
    fails = []

    def check(cond, what):
        print(("ok   " if cond else "FAIL ") + what)
        if not cond:
            fails.append(what)

    import py_compile
    bad = []
    for t in sorted((ROOT / "tools").glob("*.py")):
        try:
            py_compile.compile(str(t), doraise=True, cfile=str(ROOT / "build" / "pyc" / (t.name + "c")))
        except py_compile.PyCompileError as e:
            bad.append(t.name)
    check(not bad, f"all tools parse {bad or ''}")
    import gate
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        a = Path(td) / "a.asm"
        a.write_text("f_ proc near\n    ret\nf_ endp\nx db 1,2,3,4\n", encoding="latin-1")
        b = Path(td) / "b.asm"
        b.write_text("f_ proc near\n    enter 0,0\n    leave\n    ret\nf_ endp\nx db 7 ; one value\n", encoding="latin-1")
        check(gate.policy(a) and not gate.policy(b), "assembly policy rejects byte blobs, accepts instructions")
    import symbols
    cases = {"W?get$:Foo$n()i": "Foo::get", "W?$ct:Foo$n()_": "Foo::$ct", "W?$dt:Foo$n()_": "Foo::$dt",
             "W?gVar$ni": "gVar", "W?InputNop$n()v": "InputNop", "W?n$ni": "n", "cfunc": "cfunc",
             "W?Load$:ResMgr$n(liii)pnv": "ResMgr::Load", "W?gResMgr$n$ResMgr$$": "gResMgr"}
    check(all(symbols.label(k) == v for k, v in cases.items()), "symbol labels demangle correctly")
    fcases = {"W?f$n(i)v": True, "W?g$ni": False, "W?x$:.0$:?Event$n(uiul)i:Cls$n[]uc": False,
              "W?gResMgr$n$ResMgr$$": False, "W?Lock$:StrPool$n()i": True, "cfunc": None,
              "W?spot$:.0$:?ExploPosition$n(pnv)rn$Pt$$": False}
    check(all(symbols.is_function(k) == v for k, v in fcases.items()), "function/data classification")
    st = statuses(TIMER, ["-4s", "-s", "-oxt"])
    check(all(v == "STRICT" for v in st.values()) and len(st) == 7, "positive control: timer TU strict (bound placement)")
    st = statuses(TIMER, ["-5s", "-s", "-oxt"])
    check(not any(v == "STRICT" for k, v in st.items() if "TimerSet" in k), "wrong CPU flag (-5s) is not strict")
    st = statuses(TIMER, ["-4s", "-s", "-ox"])
    check(sum(v == "STRICT" for v in st.values()) < 7, "missing -ot is not fully strict")
    inp = ROOT / "src" / "_unknown" / "tu_004279fc.c"
    st = statuses(inp, ["-4s", "-s"], find=True)
    check(sum(v == "STRICT" for v in st.values()) >= 6, "positive control: input TU strict with profile plain")
    st = statuses(inp, ["-4s", "-s", "-ox"], find=True)
    check(sum(v == "STRICT" for v in st.values()) < 6, "plain-profile TU under -ox is not fully strict")
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        p = Path(td) / "a.c"
        p.write_text("int f(int x) { _asm { nop } return x; }\n")
        check(bool(gate.policy(p)), "policy rejects inline asm")
        p.write_text("/* _asm in a comment is fine */ int f(int x) { return x; }\n")
        check(not gate.policy(p), "policy ignores comments")
        # wrong address: shift TimeNow by 16 -> DIFF, never STRICT
        st = statuses(TIMER, ["-4s", "-s", "-oxt"], at={"TimeNow": 0x47abd0, "TimeSleep": 0x47abc0})
        check(st.get("W?TimeNow$n()ul") == "DIFF", "wrong placement is DIFF")
        # a byte change in a constant must be caught
        p2 = Path(td) / "tu_0047abc0.c"
        p2.write_text(TIMER.read_text().replace("if (elapsed < t->duration)", "if (elapsed <= t->duration)"))
        st = statuses(p2, ["-4s", "-s", "-oxt"])
        check(st.get("W?TimerRemaining$n(pn$Timer$$)ul") == "DIFF", "changed comparison is DIFF")
    # item-mode literal proof (CONST + 0 with the offset in the data field): a one-byte change in a literal
    # must not stay STRICT (regression for the 2026-11-09 false positive found by lane b1)
    shell = ROOT / "src" / "shell" / "shell.c"
    text = shell.read_text(encoding="latin-1")
    if "mainmenu.plx" in text:
        with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
            good = statuses(shell, ["-4s", "-s", "-bm", "-oe"], as_name="shell\shell.c")
            p3 = Path(td) / "shell.c"
            p3.write_text(text.replace("mainmenu.plx", "mainmenu.plz"), encoding="latin-1")
            bad = statuses(p3, ["-4s", "-s", "-bm", "-oe"], as_name="shell\shell.c")
            k = next((n for n in good if "PlayTakeoff" in n), None)
            check(k is not None and good[k] == "STRICT" and bad.get(k) != "STRICT",
                  "changed string literal is not STRICT (item-mode proof)")
    # audit v1 negatives (candidates/v1/REPORT.md): own initialized data (F1), placed CONST bytes (F2),
    # interior placement (F4)
    mem = ROOT / "src" / "io" / "memory.c"
    mt = mem.read_text(encoding="latin-1")
    with tempfile.TemporaryDirectory(dir=ROOT / "build") as td:
        good = statuses(mem, ["-4s", "-s", "-oxt", "-bm"], as_name="io\\memory.c")
        k = next((n for n in good if "GlobalBlockAlloc" in n), None)
        if "Ran out of memory" in mt:
            p4 = Path(td) / "memory.c"
            p4.write_text(mt.replace("Ran out of memory", "Ran out of memorz"), encoding="latin-1")
            bad1 = statuses(p4, ["-4s", "-s", "-oxt", "-bm"], as_name="io\\memory.c")
            check(k and good[k] == "STRICT" and bad1.get(k) != "STRICT", "own initialized data change is not STRICT (F1)")
        if "__FILE__" in mt:
            p5 = Path(td) / "memory.c"
            p5.write_text(mt.replace("__FILE__", '"io\\\\memorx.c"', 1), encoding="latin-1")
            bad2 = statuses(p5, ["-4s", "-s", "-oxt", "-bm"], as_name="io\\memory.c")
            check(k and bad2.get(k) != "STRICT", "placed CONST byte change is not STRICT (F2)")
        p6 = Path(td) / "fog.c"
        p6.write_text("void FogNoOp() {}\n")
        st6 = statuses(p6, ["-4s", "-s", "-oxt", "-bm"], at={"FogNoOp": 0x47ac72})
        check(all(v != "STRICT" for v in st6.values()), "placement at an interior address is not STRICT (F4)")
    audit_v1_f3_f5(check)
    audit_v1_r2_scalars(check)
    resource_checks(check)
    print("TEST GATE OK" if not fails else f"TEST GATE FAILED: {fails}")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
