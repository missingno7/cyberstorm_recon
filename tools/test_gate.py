"""Self-tests of the proof machinery: the gate must reject what it should reject. Run after tool changes.

    python tools/test_gate.py
"""
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
    import symbols
    cases = {"W?get$:Foo$n()i": "Foo::get", "W?$ct:Foo$n()_": "Foo::$ct", "W?$dt:Foo$n()_": "Foo::$dt",
             "W?gVar$ni": "gVar", "W?InputNop$n()v": "InputNop", "W?n$ni": "n", "cfunc": "cfunc",
             "W?Load$:ResMgr$n(liii)pnv": "ResMgr::Load", "W?gResMgr$n$ResMgr$$": "gResMgr"}
    check(all(symbols.label(k) == v for k, v in cases.items()), "symbol labels demangle correctly")
    fcases = {"W?f$n(i)v": True, "W?g$ni": False, "W?x$:.0$:?Event$n(uiul)i:Cls$n[]uc": False,
              "W?gResMgr$n$ResMgr$$": False, "W?Lock$:StrPool$n()i": True, "cfunc": True,
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
    check(sum(v == "STRICT" for v in st.values()) == 6, "positive control: input TU strict with profile plain")
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
    print("TEST GATE OK" if not fails else f"TEST GATE FAILED: {fails}")
    return 1 if fails else 0


if __name__ == "__main__":
    sys.exit(main())
