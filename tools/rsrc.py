"""Resource proof: rebuild the .rsrc section from src/res/ and compare it with the oracle.

    python tools/rsrc.py [-v]

Pipeline (pins in config/toolchain.json: installs.wc106, runners.dosbox-x, resources):
1. tools/resgen.py turns the pixel text src/res/*.ico.txt / *.cur.txt into .ico/.cur files.
2. WRC 10.6 (DOS build, under DOSBox-X with the recorded build clock) compiles the script to a WATCOM-format .res.
   The clock matters for one field: VS_FIXEDFILEINFO.dwFileDateLS, which WRC stamps with the compile time.
3. WLINK 10.6 cannot attach resources to an NT image; WRC binds them afterwards (`wrc -bt=nt X.res X.exe`), as the
   original build did (.rsrc is the last section, after .reloc). The bind target here is a stub image linked from a
   generated one-function object, so only the .rsrc section is meaningful.
4. Comparison: every resource (type, name, language) STRICT by its bytes; then the whole section with data-entry RVAs
   rebased. Directory TimeDateStamps are the bind time (an image-level clock field, reported, not compared here) and
   the slack after the last resource is UNKNOWN debt (config resources.unknown).
Exit status 0 when every resource is STRICT and the section differs only in those two named places.
"""
import shutil
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import cc  # noqa: E402
import pe as pemod  # noqa: E402
import resgen  # noqa: E402
from common import BUILD, CONFIG, ROOT, TOOLS_DIR, load_json, sha256_file  # noqa: E402

OUT = BUILD / "rsrc_proof"
CONF = """[sdl]
output=surface
[dosbox]
quit warning=false
[cpu]
cycles=fixed {cycles}
[config]
country=1
[dos]
keyboardlayout=us
[autoexec]
"""


def tree(b):
    """{(type, name, lang): (entry offset, data rva, size)}, directory timestamp offsets."""
    out, ts = {}, []

    def walk(off, path):
        ts.append(off + 4)
        _, _, _, _, nn, ni = struct.unpack_from("<IIHHHH", b, off)
        for i in range(nn + ni):
            name, ptr = struct.unpack_from("<II", b, off + 16 + 8 * i)
            if name & 0x80000000:
                q = name & 0x7FFFFFFF
                n = struct.unpack_from("<H", b, q)[0]
                name = b[q + 2:q + 2 + 2 * n].decode("utf-16le")
            if ptr & 0x80000000:
                walk(ptr & 0x7FFFFFFF, path + [name])
            else:
                rva, size = struct.unpack_from("<II", b, ptr)
                out[tuple(path + [name])] = (ptr, rva, size)
    walk(0, [])
    return out, ts


def policy(script):
    """Source rules for the resource script: no raw data resources, and every file it names must be generated from
    readable pixel text next to it (tools/resgen.py), never a binary copied from the oracle."""
    import re
    text = re.sub(r"/\*.*?\*/|//[^\n]*", "", script.read_text(encoding="cp1252"), flags=re.S)
    bad = []
    if re.search(r"\bRCDATA\b", text, re.I):
        bad.append("RCDATA resource (raw data blob)")
    # single-line resource statements that name a file: `name TYPE file` (any type, quoted or not)
    for typ, f in re.findall(r'^[ \t]*[\w"]+[ \t]+(\w+)[ \t]+(?:\w+[ \t]+)*"?([^\s",]+\.\w+)"?[ \t]*$', text, re.M):
        if (typ.upper() not in ("ICON", "CURSOR") or not f.lower().endswith((".ico", ".cur"))
                or not (script.parent / (Path(f).name + ".txt")).exists()):
            bad.append(f"{typ} file {f} has no pixel-text source")
    return bad


def build(verbose=False, script=None):
    tc = load_json(CONFIG / "toolchain.json")
    rc = tc["resources"]
    ent, base = cc.install("wc106")
    run = tc["runners"]["dosbox-x"]
    dosbox = TOOLS_DIR / run["path"]
    if sha256_file(dosbox) != run["sha256"]:
        raise SystemExit("dosbox-x: hash differs from pin")
    script = Path(script) if script else ROOT / rc["script"]
    bad = policy(script)
    if bad:
        raise SystemExit("resource policy: " + "; ".join(bad))
    if OUT.exists():
        shutil.rmtree(OUT)
    stage = OUT / "stage"
    stage.mkdir(parents=True)
    shutil.copyfile(script, stage / script.name)
    for t in sorted(script.parent.glob("*.txt")):
        if t.name.endswith((".ico.txt", ".cur.txt")):
            out = stage / t.name[:-4]
            out.write_bytes(resgen.build(t.read_text(encoding="ascii"), out.suffix == ".cur"))
    clk = rc["clock"]
    (OUT / "run.conf").write_text(CONF.format(cycles=clk["cycles"]), encoding="ascii")
    wrc = ent["tools"]["wrc_dos"].replace("/", "\\")
    cmd = (f"W:\\{wrc} {' '.join(rc['compile'])} -i=W:\\H\\NT -i=W:\\H {script.name} -fo=unit.res > wrc.txt")
    args = [str(dosbox), "-silent", "-nogui", "-nomenu", "-conf", str(OUT / "run.conf"),
            "-c", f'mount c "{stage}"', "-c", f'mount w "{base}"', "-c", "c:",
            "-c", f"date {clk['date']}", "-c", f"time {clk['time']}", "-c", "set PATH=W:\\BINW",
            "-c", cmd, "-c", "exit"]
    subprocess.run(args, cwd=OUT, stdin=subprocess.DEVNULL, capture_output=True, timeout=300)
    res = next((p for p in stage.iterdir() if p.name.lower() == "unit.res"), None)
    log = next((p for p in stage.iterdir() if p.name.lower() == "wrc.txt"), None)
    if res is None:
        raise SystemExit("WRC produced no .res: " + (log.read_text(errors="replace") if log else "no log"))
    # stub image + bind
    env = {"SYSTEMROOT": "C:\\Windows", "WATCOM": str(base), "PATH": str(base / "BINNT"),
           "LIB": str(base / "LIB386") + ";" + str(base / "LIB386" / "NT")}
    (OUT / "stub.c").write_text("int __stdcall WinMain(void *a, void *b, char *c, int d) { return 0; }\n")
    rc_, text = cc.compile_one(OUT / "stub.c", OUT / "stub.obj", ["-4s", "-s"], "wc106", "wcc386")
    if rc_:
        raise SystemExit(text)
    (OUT / "stub.lnk").write_text("system nt_win\noption quiet, undefsok, stack=48k\nname stub.exe\nfile stub.obj\n")
    for argv in ([str(base / ent["tools"]["wlink"]), "@stub.lnk"],
                 [str(base / ent["tools"]["wrc"]), "-q", "-bt=nt", str(res), "stub.exe"]):
        r = subprocess.run(argv, cwd=OUT, env=env, stdin=subprocess.DEVNULL, capture_output=True, text=True,
                           timeout=120)
        if r.returncode:
            raise SystemExit(f"{Path(argv[0]).name} failed: {r.stdout[-400:]}{r.stderr[-400:]}")
    return OUT / "stub.exe", rc


def compare(exe, rc, verbose=False):
    img = pemod.load(None)
    ob, orva = img.section_bytes(".rsrc"), img.section(".rsrc").rva
    mine = pemod.PE(exe.read_bytes(), "rsrc")
    mb, mrva = bytearray(mine.section_bytes(".rsrc")), mine.section(".rsrc").rva
    otree, ots = tree(ob)
    mtree, mts = tree(mb)
    rows, strict = [], 0
    for k, (_, rva, size) in otree.items():
        m = mtree.get(k)
        got = bytes(mb[m[1] - mrva:m[1] - mrva + m[2]]) if m else None
        want = ob[rva - orva:rva - orva + size]
        ok = got == want
        strict += ok
        rows.append((k, size, "STRICT" if ok else ("MISSING" if m is None else "DIFF")))
    extra = [k for k in mtree if k not in otree]
    # section: rebase data-entry RVAs, clear directory timestamps, leave the slack aside
    layout_ok = sorted(otree) == sorted(mtree) and ots == mts and len(ob) == len(mb)
    diffs = []
    if layout_ok:
        for _, (p, rva, _) in mtree.items():
            struct.pack_into("<I", mb, p, rva - mrva + orva)
        c = bytearray(ob)
        for t in ots:
            struct.pack_into("<I", c, t, 0)
            struct.pack_into("<I", mb, t, 0)
        end = max(rva - orva + size for _, rva, size in otree.values())
        end = (end + 3) & ~3
        diffs = [i for i in range(end) if c[i] != mb[i]]
    stamp_o = struct.unpack_from("<I", ob, 4)[0]
    stamp_m = struct.unpack_from("<I", mine.section_bytes(".rsrc"), 4)[0]
    print(f"resources: {strict} of {len(otree)} STRICT" + (f"; extra {extra}" if extra else ""))
    for k, size, st in rows:
        if verbose or st != "STRICT":
            print(f"    {st:7s} {k} {size} bytes")
    if layout_ok:
        print(f".rsrc section: {len(ob):#x} bytes; directory and data layout "
              + ("equal" if not diffs else f"differ at {len(diffs)} bytes from {diffs[0]:#x}")
              + f" (data-entry RVAs rebased); slack {end:#x}..{len(ob):#x} = {len(ob) - end} bytes UNKNOWN; "
              f"directory stamp {stamp_o:#010x} (bind time, image-level; this run {stamp_m:#010x})")
    else:
        print(".rsrc section: directory shape or size differs")
    return strict == len(otree) and not extra and layout_ok and not diffs, strict, len(otree)


def main(argv):
    exe, rc = build("-v" in argv)
    ok, _, _ = compare(exe, rc, "-v" in argv)
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv))
