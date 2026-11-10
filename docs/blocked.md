# Blocked or parked functions

One line per function: VA, label, TU, what was tried (with flags), best hypothesis, date.
Parked functions with their residues are listed in the lane reports (`candidates/<lane>/REPORT.md`); this file
holds the structural blockers. Open items first, resolved history below.

## Open

### Hand-written static initializers (cleanup debt, 2026-11-11)
Three published sources still spell a compiler-generated static initializer by hand: a function (or wrapper
objects) that calls `__wcpp_2_mod_register_` under an invented name and then the constructors one by one.
- `src/shell/hercbay.c`: `ModuleConstruct`/`InitWidget`/`InitA..D`/`State` wrapper objects, `HercModuleConstruct`.
- `src/_unknown/tu_0040ffb8.c`: `PilotBriefStaticInit` with `PopObjectConstruct` and `#pragma aux` name aliases for
  the constructors of ten objects.
- `src/shell/pilotgui.c`: `PilotGuiCtorArray` (= `__wcpp_2_ctor_array_`) and `PilotGuiModuleConstructor` aliases.
Recipe (worked examples: `src/sim/mapalert.c`, `src/pop/pherc.c`): define the real global objects with their
constructor arguments; the compiler then emits the initializer, the 12-byte module header in `_DATA`, the destructor
table in CONST2 and the XI record, and all four become STRICT. Destructor addresses come from the CONST2 table.
`#pragma aux NAME "W?..."` aliases elsewhere are the same kind of debt (census: `candidates/s40/ALIASES.md`).

### One class under two names: SimImageGadget = PilotImageButton (2026-11-11)
Constructor 00453800 (`SimImageGadget::$ct`, src/sim/simgad.c), destructor 0041235c (`PilotImageButton::$dt`,
src/pop/ppilot.c), type signature 0048cdb8 (size 0x2c). `src/sim/mapalert.c` still declares `PilotImageButton` with a
pragma alias to the SimImageGadget constructor; `src/pop/pherc.c` declares `SimImageGadget` and binds its destructor
with `--at`. Unify on one name. The hypothesis bindings of `W?$Wts0gn$SimMapGadget...` and
`W?$Wts0in$SimImageGadget...` to 0048df88 in symbols.json are wrong: that signature is SimValueGadget's
(destructor 0044d710, size 0x28). SimMapGadget's destructor is 00443ed0.

### 12-byte screen destructors attributed to the following object (2026-11-11)
A screen/popup object's inline destructor (`mov eax,[esp+4]; mov dword [eax+0x10],0; ret`) is the LAST function of
its object, placed right before the next object's static initializer (see codegen_rules.md). Fixed for 00448d74
(ReportScreen, was `DeathScreenClose` in pdeath.c). Still attributed to the following object:
`GuiScreen::ResetInput` 004087e0 in src/_unknown/tu_004087e0.c and `GuiScreen::$dt` 0041d774 in
src/_unknown/tu_0041d774.c. In plain-profile objects the bytes match either way; the ownership and names are wrong.

### Unowned data between sim objects (2026-11-11)
004a0360..004a0368 (`b7 00 00 00 00 00 00 00`, unreferenced) lies between simmini.c's last item and simgame.c's
path string: owner unknown (simmini tail or a leading table of simgame.c). simturn.c records only its path string.
sim\target.c: `_DATA` (path) is not placed and `TargetInitializeRecord` (0045a9f0, 660 bytes differ) is parked; the
global behind its empty static initializer is a placeholder (`targetStaticPoint`).

### Objects still published as one file (2026-11-11)
Path strings in `_DATA` and XI records (one per object with static initialization) mark object boundaries. Files
that still cover several original objects: `src/sim/sim.c` + `simdo.c` (paths sim.c, simglob.c, simmain.c,
simalert.c, simdo.c between 004a01a8 and 004a030c), and probably others among the 60+ path strings without a TU
(shell\\promosh.c, vrmenu.c, medlab.c, standby.c, dbrief.c, hbmanage.c, ..., pop\\pmove.c, pweapon.c, pshield.c,
core\\*.c, main\\*.c, graph\\*.c). A census tool (path strings + XI targets + module headers against the ledger's
TU ranges) is the next infrastructure step; `tools/promote.py --retire` moves functions out of a combined file.

## Resolved

- 2026-11-11 **sim\\mapalert.c filler after the popup destructor**: the popup is `class TargetPopup : public
  GuiScreen` with an out-of-line constructor (0045a6e0, was `TargetLoadResource`) and a generated destructor; the
  filler exists because the gadget classes' destructor COMDATs are emitted after it and dropped at link. With the
  gadget classes declared through their real virtual interface the object is COMPLETE (rule in codegen_rules.md,
  "screen/popup module shape"). The same shape applies to the other screen modules.

- 2026-11-11 **Multi-file objects (lane o6)**: not a thing. 00450b00..00452950 is three ordinary objects
  (sim\\simout.c 00450b00, sim\\simmini.c 00451b00 starting with an empty static initializer, sim\\simgame.c 00452340)
  and 00452950..00454210 is three more (simfire.c, simgad.c 00453310, simmisc.c 00453c70). Evidence: XI record
  004a6c60 -> 00451b00, each path string followed (or preceded) by that file's own data, the references from each
  code range. Published as six files; simgame.c and simgad.c reach whole-object strictness, `_DATA` is STRICT in all
  six. No change to cc.py was needed. The simgad|simmisc boundary rests on function grouping only.
- 2026-11-11 **sim\\target.c second path**: split into src/sim/mapalert.c (0045a600..0045a910, all segments STRICT)
  and src/sim/target.c (starts with an empty static initializer at 0045a910, XI 004a6c84).
- 2026-11-11 **ReportScreen destructor vs DeathScreenClose** (00448d74): it is ReportScreen's destructor, last
  function of tu_004481c0's object (destructor table 0048dd70). pdeath.c starts at DeathMovie and is COMPLETE.
- 2026-11-11 **ActorRecord constructor** (00456c80): compiler-generated constructor of a class with an `AnimAction`
  member (constructor 00457510, 0x67 bytes at +0x20); `ActorCloseRecord` retired.
- 2026-11-11 **Runtime internals under invented names, part 1**: `MemoryRelease`/`MemoryAllocationRetry` replaced by
  `delete[]`/`new char[]` in ten TUs with no loss (runtime members opdelarr 0047e044, opnewarr 0047eb98); unused
  declarations of `BriefModuleConstruct`/`PopObjectConstruct` removed from four TUs; pherc.c's hand-written
  initializer replaced by two real static objects (CONST2, _DATA, XI now STRICT).
- 2026-11-09/11 **multi\\link.c, linkapp.c, chat.c fake array `link_data`** (audit v1 R2.2): reworked by s27/s37 with
  real globals (Queue objects, LinkPeerTimer[33]); all 16 withdrawn functions are STRICT again.
- 2026-11-09 (s40) **GuiPoint COMDAT under two names**: identities from the oracle descriptor at 0048daa4 (ctor
  00429a3c, copy ctor 00429a04, dtor 0042c9d0); tdisplay.c uses the real GuiPoint and DisplayRenderItem constructs.
  MCI moved from io\\resource.c to its own object tu_0047d0e0. **sinterp.c publish rollback**: fixed in promote.py
  (BSS items are recorded only when STRICT code references them by name).
- 2026-11-09 (s52) **pop\\pdeath.c spans two objects**: split into src/pop/pdeath.c and src/_unknown/tu_00449148.c.
- 2026-11-09 **io\\memory.c _DATA split**: the error texts belong to debug\\assert.c (seven global messages after its
  path string, 4-byte spacing only with wpp386); memory.c and resource.c reference them as extern.
