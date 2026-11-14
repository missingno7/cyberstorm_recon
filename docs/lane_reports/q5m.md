# Lane q5m report (parked functions, second pass after q5)

## Promote dry runs (only files changed)
python tools/promote.py candidates/q5m/_unknown/tu_00426cdc.c --tu src/_unknown/tu_00426cdc.c --as '_unknown\tu_00426cdc.c' --profile plain
  src/_unknown/tu_00426cdc.c: 14 strict functions (1418 bytes), new 1, lost 0
python tools/promote.py candidates/q5m/noise/noise.c --tu src/noise/noise.c --as 'noise\noise.c' --profile plain-oe
  src/noise/noise.c: 20 strict functions (3596 bytes), new 1, lost 0
python tools/promote.py candidates/q5m/shell/hercbay.c --tu src/shell/hercbay.c --as 'shell\hercbay.c' --profile plain
  src/shell/hercbay.c: 17 strict functions (2297 bytes), new 1, lost 0
python tools/promote.py candidates/q5m/shell/hotseat.c --tu src/shell/hotseat.c --as 'shell\hotseat.c' --profile plain-oe
  src/shell/hotseat.c: 16 strict functions (4440 bytes), new 1, lost 0
python tools/promote.py candidates/q5m/terr/tlos.c --tu src/terr/tlos.c --as 'terr\tlos.c' --profile ot
  src/terr/tlos.c: 2 strict functions (240 bytes), new 1, lost 0
python tools/promote.py candidates/q5m/terr/tfast.c --tu src/terr/tfast.c --as 'terr\tfast.c' --profile ot --at FastBufferEmpty=435010
  src/terr/tfast.c: 7 strict functions (530 bytes), new 0, lost 0
python tools/promote.py candidates/q5m/sim/simturn.c --tu src/sim/simturn.c --as 'sim\simturn.c' --profile ot-oe
  src/sim/simturn.c: 12 strict functions (2148 bytes), new 0, lost 0

## New STRICT (5)
- GuiControlShortcut 00426de7 (tu_00426cdc.c): test `c->shortcut && MenuScan()` on the member, then capture `U32 shortcut=c->shortcut`, key (`&255`) before scan (`>>8&255`), compare with MenuScan() again.
- SoundSetSettings 0046c89c (noise.c): inline SoundRate(buffer,id,percent) computes `soundFrequency[id]*percent/100` itself; caller captures `int percent=settings->frequency` before the call.
- HercBayOpen 00415edc (hercbay.c): body is an inline helper `HercBayResume(bay,callback)` (text identical to HercBayAltStart) followed by the two field stores; the inline parameter copy gives the original `mov esi,ebx`. Review point: the helper text duplicates HercBayAltStart; AltStart calling the helper gives AltStart D127 (inline parameter copies), so AltStart keeps its own spelling. A member inline `HercBay::Resume` gives D64.
- f_004252a1 004252a1 (hotseat.c): `long changed=0;` then `changed+=old!=now;` for all five settings (first one included) instead of `long changed=old!=now;`.
- RayCollect 0042ee30 (tlos.c): `for (int direction=0; RayCount<100; ++direction)` with `RayPoint *delta=&pathDirections[direction]` inside the body (strength-reduced pointer initialised after the entry guard) and `RayLocations[RayCount++]=location`.

## Improved, not STRICT (adopted in the candidates, no loss)
- FastBuffer::BuildPixels 00434f50 (tfast.c): 0 bytes differ once an empty function `void FastBufferEmpty(FastBuffer *){}` follows it (the original has no `ret` after BuildPixels' epilogue: it falls through filler into a 1-byte `ret` at 00435010). Blocked by the function map: 00435010 is not an original function start (verifier F4), so BuildPixels' extent (to 00435020) cannot match. Integrator: add a function start at 00435010 (empty function after BuildPixels), then BuildPixels and FastBufferEmpty should be STRICT.
- FastBuffer::BuildFlags 00434e30 (tfast.c): D165 (wrong 256-byte extent) -> D7 with the correct 288-byte extent: `for (index=1; index<count; index++, item++)`, color via `&targetDefinitionTable[index]`, inner neighbour lookup in an inline `FastNeighborCell(FastCoord *base, long direction)` that copies `pathDirections[direction]` field-wise and adds base (extern `Point pathDirections[6]`, the bound name). Residue: the two stack slots of `item` and the index*12 induction temp are swapped (0x10/0x14); 12 spellings of the declarations/loop/color expression invariant.
- SimApplyLeadership 004507d0 (simturn.c): D52 -> D8. `for(ActorRecord *a=ActorFindByOwner(..);a;a=ActorFindOwnerNext(..))` gives the original `jne/jmp` entry guard and point hoisting; inline `PointDistance(const GridPoint*,const GridPoint*){dx*=dx; dy*=dy; dx+=dy; return ISqrt(dx);}` gives the original square order. Residue: original mutates distance (`sub esi,2; imul edx,esi,10`), but every `distance-=2` spelling swaps the homes of `a`/`distance` (D30); plus ECX/EAX in the final sprintf/SimMessage pushes.
- f_00425815 00425815 (hotseat.c): D120 -> D30 by calling `f_004255bf(param_1)` (inlined by -oe, gives the original `mov ebx,esi` parameter copy) instead of the written-out draw loop, and `for (laterIndex=param_2+1; ...)`. Residue: loop counter/constant-1 homes (EDX/EBX swapped) in the first loop; alias removal variants invariant or worse.

## New compiler facts (codegen_rules.md rows)
| ot / ot-oe loop entry | `for (init; cond; step)` whose condition is not known true at entry vs `if(c){...do{}while(c)}` / `while(c)` | the `for` form gives `jcc body; jmp exit` (short jcc over a near jmp) at entry with loop invariants hoisted after the guard; `while`/`if+do` give a single near `je exit` (1 byte shorter) | verified (RayCollect, SimApplyLeadership guard) (q5m, 2026-11-14) |
| ot, table element pointer in a loop | `T *p=&table[i]` inside the body of `for(i...)` vs a walking pointer `T *p=table; ...; p++` | indexing is strength-reduced to a pointer initialised in the preheader AFTER the entry guard and incremented at the loop bottom; the walking pointer is initialised before the guard and the step block moves to the top | verified (RayCollect STRICT) (q5m, 2026-11-14) |
| plain-oe accumulator | `long changed=0; changed+=a!=b; changed+=...` vs `long changed=a!=b; changed+=...` | the 0-initialised form keeps the sum in EAX and loads old/new into EDX/ECX (original); the first-assignment form swaps to EAX/EDX and sum in EDX | verified (f_004252a1 STRICT) (q5m, 2026-11-14) |
| plain, inline helper with pointer parameter | caller body = inline helper(bay, cb) + own stores | helper gets a parameter copy (`mov esi,ebx`) while the caller keeps its own register for the later stores; the same text written in the caller has no copy | verified (HercBayOpen STRICT); also the f_004255bf expansions in hotseat.c (q5m, 2026-11-14) |
| plain-oe member test then capture | `if (c->m && f()) { U32 v=c->m; ...}` vs capture before the test | testing the member first and capturing inside gives the original `mov edx,[eax+18h]; test; ... mov ebx,edx` sequence; capture-first gives `cmp [eax+18h],0` and a reload | verified (GuiControlShortcut) (q5m, 2026-11-14) |
| inline helper argument shape | `SoundRate(buffer,id,percent)` computing `table[id]*percent/100` inside, `percent` captured by the caller | evaluates percent into a stack temp before the buffer load and holds the buffer in ECX / vtable in EAX (original); passing `table[id]` as the argument or no caller capture keeps the old order (D7..D92) | verified (SoundSetSettings) (q5m, 2026-11-14) |
| ot, fall-through into an empty function | function followed by `void f(T*){}` in the same object | the function loses its `ret` and falls through the 16-byte filler into the empty function's `ret` (original BuildPixels 00434f50 / 00435010); the function map treats both as one function | verified bytes; blocked by funcmap F4 (q5m, 2026-11-14) |
| ot, local struct slot order | neighbour Point local in the caller vs inside an inline helper taking an index | inline-helper local lands above the caller's `base` (original order); every caller-scope declaration order gives the reverse | verified (BuildFlags slots) (q5m, 2026-11-14) |

## Remaining targets, one line each
- tu_00426cdc GuiControlShortcut: STRICT.
- tu_004324d0 BuildTerrainPath (579): not reworked (size; no time left after the smaller targets).
- tu_00435dec ActorTrainSkill / ActorRecoverInjuries / ActorMoraleCost: q5's targets, not repeated.
- tu_00435dec ActorScale (137): original keeps a result variable in EDX (the dead `s` register) with a shared `mov eax,edx` return; 12 result/clamp/default spellings keep EAX homes (best D142 with `result=s->level; result+=value-1`). ActorHealthCost 57 with that variant (not adopted).
- tu_00435dec ActorHealthCost, ActorEffectiveSkill, ActorRest, ActorWork, ActorTick, ActorQuery, SendEvent: not reworked; ActorScale's register residue feeds the inlined copies; SendEvent/ActorQuery are 0x10..0x170 bytes short (large structure work).
- tu_00476ddc ResetMultiplayerInput (4): inline AddSpecialOption id/text EDI/EBP homes; 4 earlier lanes' families recorded, not repeated.
- tu_004774b8 MenuApplyMode (562) / BriefWaitA (69): BriefWaitA contains MenuApplyMode inlined; MenuApplyMode keeps the submenu handle on the stack (frame 0x10 vs original handle in EDI); separate handles / scoped blocks / 4 MenuEnableAll spellings give D537..D558, not the shape.
- tu_0047e150 RandomBiased (126): seed pointer ECX vs EBX, `range*percent` multiply from memory and delta homes; target/delta/abs-helper spellings invariant (D126).
- flic.c DecodePalette / DecodeRegion / $dt: q5's targets, not repeated. Flic::DecodeFrame (623): not reworked.
- sinterp.c ScriptFormatArgs / ScriptEvalOp: q5's. ScriptRef (107): original materialises `&points[ref]` / `&actors[index]` with shl/add and copies fields through EAX; explicit pointer locals fold back to scaled addressing; only `result=s->points[ref]` (movsd) materialises the pointer (D109, but then ScriptCommandTarget's table is right). ScriptFormat (89), ScriptCall (1202), ScriptStep (2882): not reworked.
- noise.c SoundPlay (3): 40 earlier variants (o1l) not repeated. SoundCreateBuffer (143): output alias, pointer element type, decoded/allocated alias and three Unlock/copy spellings invariant (D143) or worse.
- pherc.c f_0044579c (2): input argument EDX vs EAX; resource-load captures, typed savedAudio/Manual stores, condition forms invariant. f_0044540f (4), f_00445cfe (5), f_00446570 (9): families of r1/s26/p3 not repeated. f_004456f4 (130): bounds/clamp/order variants D136+ (original keeps speed in EDX with xor before the branches). f_0044591f (313), f_00445e23 (693): not reworked.
- pmove.c MovementCommand (1173): not reworked.
- hercbay.c HercBayOpen: STRICT.
- hotseat.c f_004255bf (3): standalone wants `movsx edx; add edx,25`; pointer-typed parameter, typed view, arithmetic orders invariant. f_00424a07 (54): its tail is the inlined f_004255bf whose original has a parameter copy (`mov esi,ebx`); int-typed screen parameter and pointer-typed f_004255bf do not produce it. f_004252a1: STRICT. f_00425815: D30 (above).
- actor.c ReleaseState (2): only the jump into ActorFinishMove's epilogue; ActorFinishMove (114): rotation of the target/unit temporaries from +ff and the three sound clamps; location/sound declaration placement, merged conditions, cost capture and inline clamp spellings invariant (D114) or worse.
- simhot.c SimHotCheckMission (308), SimHotBriefGameOver (360), SimHotStartMission (1530): not reworked.
- simturn.c SimApplyLeadership: D8 (above).
- tdisplay.c DisplayResolveCandidate (15): only stack displacements of point temporaries (3 lanes' families). BestLocInitialize (58): needs the `add edx,4; [edx-4]=name; sub edx,4` this-adjust; class BestLoc with point members (in-class/out-of-class ctors, arrays, init lists, 24 combos) never produces it. DisplayConfigure (437), DisplayZoomArea (453): not reworked.
- tfast.c BuildPixels: 0 bytes, function-map blocked (above). BuildFlags: D7 (above).
- tlos.c RayCollect STRICT. RayWalk (19), RayDrawSamples (9): earlier lanes' families (s14/s33/p4) not repeated. RayOcclusion (1226): not reworked.

## Needs a structural rewrite
- src/shell/hotseat.c: raw `param_1 + 0x....` offsets throughout; HotseatDataView exists but only a few functions use it. Not done in this lane (too large next to the target list).
- src/_unknown/tu_00435dec.c: SendEvent and ActorQuery are hundreds of bytes short of the original; ActorState layout and the inline helpers (ActorScale, ActorAdjustFixed) need rework before the 0x40..0x280-byte residues can move.
- src/shell/hercbay.c: still `register` locals and raw offsets in most functions (works, but HercBay members are not typed).

## Integrator notes
- tfast.c: add a function start at 00435010 (empty function after BuildPixels) to the function map / allowed starts; then promote with `--at FastBufferEmpty=435010`.
- hercbay.c: decide whether the HercBayResume helper (duplicate text of HercBayAltStart) is acceptable source.
- Candidate files: candidates/q5m/_unknown/tu_00426cdc.c, noise/noise.c, shell/hercbay.c, shell/hotseat.c, sim/simturn.c, terr/tfast.c, terr/tlos.c. Scratch templates in build/q5m/.
