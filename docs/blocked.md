# Blocked or parked functions

One line per function: VA, label, TU, what was tried (with flags), best hypothesis, date.
Parked functions with their residues are listed in the lane reports (`candidates/<lane>/REPORT.md`); this file
holds the structural blockers. Open items first, resolved history below.

## Open

### Register-assignment residue (the dominant blocker; state 2026-11-11)
About 370 of the ~390 parked functions are roots (`python tools/blockers.py`): only 27 wait for a neighbour
(shared tails, jump distances). Ruled out as global causes, all recorded in codegen_rules.md: compiler release
(10.5, 10.0a, 11.0), CPU switch, every single optimisation/code option per TU (`tools/flagcensus.py`), build noise
(the 1.00A build agrees wherever both can be read), declaration order, types, helper spelling
(`tools/variants.py` runs). What is left is the shape of each expression tree and of the surrounding function; the
r3 allocation model predicts synthetic probes but not these functions. Next evidence worth getting: the exact
conflict-graph order of 10.6 on one two-temporary case (IoClose, src/io/file.c: `esi`/`edi` swapped, 96 bytes).

### Objects not yet named or cut (2026-11-11, after lanes n1-n6)
32 path strings have no published file (`python tools/objcensus.py`): gui\comma.c (object 0042ac40..0042acf0,
no source body yet), terr\tmask.c, tcell.c, tfeature.c, triver.c, ttable.c, clifftex.c, bmprgb.c, core\herc.c,
hercsh.c, hercsim.c, mcpbrfs.c, mcpio.c, mcpcamp.c, mcphmisc.c, sim\actorver.c, multi\gameparm.c, main\main.c,
hint.c, prefs.c, mcursor.c, main\WIN\menus.c, main\testrbx.c, graph\dirty.c, font.c, pal.c, misc\format.c,
graph\gfx\g_func.c, g_pal.c, io\resblock.c. In each group the number of unnamed objects differs from the number
of strings, or several share tables, so link order alone cannot pair them (reports: candidates/n3, n4, n6
REPORT.md list what would decide each). sim\simbug.c and logic\slog.c are published as data-only files (path
string STRICT): simbug's code is either nothing (debug code compiled out) or `SimDistanceToDepot`
(00454180..00454210, now in simmisc.c). Compiler filler after simmisc's `SimSoundSetup` favours "same object as
simmisc"; four strings for four code ranges favours "simbug". Undecided.

### Screen modules still in the old shape (2026-11-11)
`mapalert.c` is the model (codegen_rules.md, "screen/popup module shape"). Still with an opaque popup class or a
free function standing for the destructor, visible as a 12-byte last function without its trailing filler:
src/sim/hotbrief.c (`HotbriefClearActive`, 004625f0), and the `#pragma aux NAME "W?..."` constructor aliases that
survive in dbrief.c, hbmanage.c, hotseat.c, qstart.c and others (census: candidates/s40/ALIASES.md). One class
still has two names: `SimImageGadget` = `PilotImageButton` (constructor 00453800 in simgad.c, destructor 0041235c,
type signature 0048cdb8); the symbols.json hypothesis bindings of `W?$Wts0gn$SimMapGadget...` and
`W?$Wts0in$SimImageGadget...` to 0048df88 are wrong (that signature is SimValueGadget's).

### Unowned data
004a0360..004a0368 (`b7 00 00 00 00 00 00 00`, unreferenced) between simmini.c's last item and simgame.c's path
string. sim\target.c: `_DATA` (path) is not placed and `TargetInitializeRecord` (0045a9f0) is parked; the global
behind its empty static initializer is a placeholder (`targetStaticPoint`).

## Resolved

- 2026-11-11 **Hand-written static initializers, destructor ownership, combined files** (lanes n1-n6, Sol): 43
  objects published under their path names (74 unnamed path strings -> 32), `_DATA` STRICT in each; `PopObjectConstruct`/`HercModuleConstruct`/
  `PilotGuiCtorArray` are gone (real global objects in hercbay.c, standby.c, pilotgui.c); the 12-byte screen
  destructors at 004087e0, 0041072c, 0041d774, 00443f58 moved to the preceding objects; mpcomm.c (two stub
  functions at 004243e4) cut from hotseat.c; opfire.c cut from hotbrief.c.
- 2026-11-11 **Functions failing only through a neighbour's switch table**: verifier rule "table ownership"
  (verification.md); 16 functions STRICT.

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
