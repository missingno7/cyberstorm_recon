# Blocked or parked functions

One line per function: VA, label, TU, what was tried (with flags), best hypothesis, date.

(none yet)

## Cleanup debt (integrator, 2026-11-09)
Published sources that call runtime internals through invented names (bindings kept so verify passes; rewrite
with the C++ construct, then drop the alias): `BriefModuleConstruct`/`HercModuleConstruct`/`PopObjectConstruct`
= `__wcpp_2_mod_register_` (static-init registration, compiler generated), `MemoryRelease` = `operator delete[]`,
`PopConstructorArray` = `__wcpp_2_ctor_array_`, `MemoryAllocationRetry` = `operator new[]`.
Affected TUs: src/shell/missbrf.c, src/shell/hercbay.c, src/pop/psystem.c, src/terr/tfog.c, src/terr/tfast.c.

## Identity correction (integrator, 2026-11-09)
simctorlst.c: the oracle typesig of ActorRecord gives its default constructor at 00456c80 (constructs a member
object at +0x20 via 00457510, returns this). src currently has `ActorRecord() : id(0) {}` (wrong) and a separate
`ActorCloseRecord` function with the right bytes at 00456c80. Rewrite as the real constructor (member object with
its own constructor) so the COMDAT matches; the old `ActorRecord::$ct`@00467971 identity was removed (identical
trivial constructor of another class).

## Data-model correction (integrator, 2026-11-09, audit v1 R2.2)
multi\link.c, multi\linkapp.c, multi\chat.c model many globals as one fake zero-initialized array `link_data`
(`unsigned char[]`) at 004a0000. The oracle holds initialized, nonzero data there, so references through that
array are no longer accepted; 16 functions were withdrawn (accepted losses). Rework: declare the real globals
(initialized ones in _DATA/CONST, zero ones in BSS) with their own names.

## Open: multi-file objects (lane o6, 2026-11-09)
Object 00450b00..00452950 carries the path strings sim\simturn.c, sim\simout.c, sim\simmini.c, sim\simgame.c
(each followed by that file's own data) and 00452950..00454210 carries sim\simfire.c, sim\simgad.c,
sim\simmisc.c: several .c files compiled as ONE object via #include. tools/cc.py copies only the main file, and
promote.py publishes only that file, so included files cannot be reproduced yet. Needed: cc copies the TU's
sibling sources into the scratch dir (preserving relative paths so __FILE__ of included files reads
"sim\simturn.c"), promote/gate policy-check and publish the included files, ledger records the include set.
Until then those TUs are published as single files and functions whose __FILE__/__LINE__ come from an included
file (e.g. 00452570, asserts on simgame.c lines 113/189) stay open.

## Shared inline COMDAT under two names (2026-11-09, s21)
`src/_unknown/tu_004297d4.c` (s21 candidate) emits `GuiPoint::$ct()` as an inline COMDAT; the linker kept one copy at
0042c9d0, which `src/terr/tdisplay.c` already counts under the working label `DisplayIdentityA`. Promotion refuses
the second name. Fix: make tdisplay.c use the real inline `GuiPoint` constructor (same mangled name), then promote
tu_004297d4 (its shared COMDAT is then skipped as already counted). 7 STRICT functions of s21 wait on this.
