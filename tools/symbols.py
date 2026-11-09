"""Symbol bindings: the single name -> VA table used to resolve fixups (config/symbols.json).

Entry: {"va": "0047abe0", "kind": "func|data", "status": "matched|lib|hypothesis", "evidence": "...",
        "mangled": "W?...", "tu": "src/..."}
  matched     the symbol's own bytes are strictly matched at that VA (set by promote.py only)
  lib         library-member identity (tools/libscan.py), runtime code we do not rewrite
  hypothesis  bound because a caller's fixup or a seed analysis implied it; unproven
Imports are not stored: `__imp_*` externs resolve through the oracle's import table.

    python tools/symbols.py show NAME|VA
    python tools/symbols.py bind NAME VA --kind func|data --evidence TEXT     (hypothesis bindings only)
    python tools/symbols.py stats
    python tools/symbols.py aliases       VAs bound under more than one label (unify names in the sources)
"""
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from common import BUILD, CONFIG, dump_json, load_json  # noqa: E402

PATH = CONFIG / "symbols.json"
# W?name$n...   W?name$:Cls$n...   W?$ct:Cls$n...  (name is greedy; scopes are ':Cls$' groups)
_MANGLED = re.compile(r"^W\?(\$?[A-Za-z_0-9]+)(?:\$((?::[A-Za-z_0-9]+\$)*)|((?::[A-Za-z_0-9]+\$)+))n")


def label(sym):
    """Readable label for a Watcom object symbol: 'W?get$:Foo$n()i' -> 'Foo::get'; C names unchanged;
    special C++ symbols ($Wvf vftables, $Wts typesigs, ...) are returned verbatim."""
    if not sym.startswith("W?"):
        return sym
    m = _MANGLED.match(sym)
    if not m or sym.startswith("W?$W"):
        return sym
    name, scopes = m.group(1), (m.group(2) or "") + (m.group(3) or "")
    parts = [s for s in re.split(r"[:$]+", scopes) if s][::-1]
    return "::".join(parts + [name])


def is_function(sym):
    """Function vs data from the mangled name: Watcom puts the symbol's own type after the LAST `$n`
    (`W?f$n(i)v` function, `W?g$ni` data, a function-local static `W?x$:.0$:?Event$n(uiul)i:Cls$n[]uc` data).
    C names (no `W?`) are treated as functions."""
    if not sym.startswith("W?"):
        return True
    i = sym.rfind("$n")
    return i >= 0 and sym[i + 2:i + 3] == "("


def scope_of_as(as_name):
    """TU scope from the compile-as path: 'multi\mstart.c' -> 'multi/mstart.c'."""
    return (as_name or "").replace("\\", "/").lower()


def scope_of_module(module_name):
    """TU scope from an object's THEADR (absolute path of the compiled copy inside build/cc/<tmp>/)."""
    p = (module_name or "").replace("\\", "/").lower()
    i = p.find("/build/cc/")
    if i >= 0:
        rest = p[i + len("/build/cc/"):]
        return rest.split("/", 1)[1] if "/" in rest else rest
    return p.rsplit("/", 1)[-1]


def is_local(sym, local_flag=False):
    """File-local symbols: LPUBDEF statics and compiler-generated names (`W?.fn_init...`)."""
    return bool(local_flag) or sym.startswith("W?.")


def key_for(sym, local_flag, scope):
    lab = label(sym)
    return f"{lab}@{scope}" if is_local(sym, local_flag) and scope else lab


def import_name(sym):
    """'__imp__GetTickCount@0' / '__imp_GetTickCount' -> 'GetTickCount', else None."""
    if not sym.startswith("__imp_"):
        return None
    s = sym[6:]
    if s.startswith("_"):
        s = s[1:]
    return s.split("@")[0]


def load():
    if PATH.exists():
        return load_json(PATH)
    return {"note": "name -> binding; see tools/symbols.py", "symbols": {}}


def save(db):
    db["symbols"] = dict(sorted(db["symbols"].items(), key=lambda kv: kv[1]["va"]))
    dump_json(PATH, db)


class Resolver:
    """Resolve object extern names to oracle VAs: symbols.json, then imports, then library identity."""

    def __init__(self, img):
        self.img = img
        # symbols.json holds primary-oracle VAs only; other oracles resolve through imports/library identity
        from common import default_oracle
        self.db = load()["symbols"] if img.name == default_oracle() else {}
        self.by_mangled = {e["mangled"]: n for n, e in self.db.items() if e.get("mangled")}
        self.imports = {}
        for dll, n, va in img.imports():
            self.imports.setdefault(n, va)
        self.lib = {}
        p = BUILD / "libsyms" / f"{img.name}.json"
        if p.exists():
            for n, e in json.loads(p.read_text()).items():
                self.lib[n] = int(e["va"], 16)

    extra = None  # {object symbol: va} implied bindings for this comparison only (how='implied')

    def resolve(self, sym, scope=None, local=False):
        """Return (va, how) or (None, None). how: matched|hypothesis|lib|import|implied.
        File-local symbols (statics, generated names) resolve only through their TU-scoped key."""
        imp = import_name(sym)
        if imp is not None:
            return (self.imports[imp], "import") if imp in self.imports else (None, None)
        if is_local(sym, local):
            e = self.db.get(key_for(sym, True, scope)) if scope else None
            return (int(e["va"], 16), e["status"]) if e else (None, None)
        key = self.by_mangled.get(sym) or label(sym)
        e = self.db.get(key)
        if e:
            # a label match with a different mangled name means our prototypes disagree between TUs: the VA
            # binding still holds (bytes decide), but the caller is flagged for a type cleanup
            same = not e.get("mangled") or e["mangled"] == sym
            return int(e["va"], 16), e["status"] if same else e["status"] + "~mangle"
        if sym in self.lib:
            return self.lib[sym], "lib"
        if self.extra and sym in self.extra:
            return self.extra[sym], "implied"
        return None, None


def main(argv):
    db = load()
    syms = db["symbols"]
    if len(argv) < 2 or argv[1] == "stats":
        from collections import Counter
        print(Counter(e["status"] for e in syms.values()), len(syms))
        return 0
    if argv[1] == "aliases":
        by = {}
        for n, e in syms.items():
            by.setdefault((e["va"], e["kind"]), []).append(n)
        for (va, k), ns in sorted(by.items()):
            if len(ns) > 1:
                canon = [n for n in ns if syms[n]["status"] == "matched"]
                print(f"{va} {k}: {', '.join(ns)}" + (f"   (canonical: {canon[0]})" if canon else ""))
        return 0
    if argv[1] == "show":
        q = argv[2]
        for n, e in syms.items():
            if q in (n, e["va"], e.get("mangled")):
                print(n, json.dumps(e))
        return 0
    if argv[1] == "bind":
        name, va = argv[2], f"{int(argv[3], 16):08x}"
        kind = argv[argv.index("--kind") + 1] if "--kind" in argv else "func"
        ev = argv[argv.index("--evidence") + 1] if "--evidence" in argv else ""
        if name in syms and syms[name]["status"] == "matched":
            raise SystemExit(f"{name} is matched at {syms[name]['va']}; refusing to rebind")
        clash = [n for n, e in syms.items() if e["va"] == va and n != name and e["kind"] == kind]
        if clash:
            raise SystemExit(f"{va} already bound to {clash}")
        syms[name] = {"va": va, "kind": kind, "status": "hypothesis", "evidence": ev}
        save(db)
        print(f"bound {name} = {va} (hypothesis)")
        return 0
    print(__doc__)
    return 2


if __name__ == "__main__":
    sys.exit(main(sys.argv))
