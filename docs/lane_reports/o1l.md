# Lane o1l report: parked functions in anim.c, simdo.c, mcp.c, noise.c

New STRICT: 8 (noise 5, simdo 2, mcp 1). No STRICT function lost (dry runs below).

## Dry runs
python tools/promote.py candidates/o1l/noise/noise.c --tu src/noise/noise.c --as 'noise\noise.c' --profile plain-oe
  src/noise/noise.c: 19 strict functions (3368 bytes), new 5, lost 0
python tools/promote.py candidates/o1l/sim/simdo.c --tu src/sim/simdo.c --as 'sim\simdo.c' --profile ot-oe
  src/sim/simdo.c: 26 strict functions (4482 bytes), new 2, lost 0
python tools/promote.py candidates/o1l/core/mcp.c --tu src/core/mcp.c --as 'core\mcp.c' --profile plain-oe
  src/core/mcp.c: 67 strict functions (7630 bytes), new 1, lost 0
python tools/promote.py candidates/o1l/sim/anim.c --tu src/sim/anim.c --as 'sim\anim.c' --profile ot-oe
  src/sim/anim.c: 14 strict functions (784 bytes), new 0, lost 0   (candidate equals src; nothing to promote)

## New STRICT functions
- SoundAllowed 0046c3fc: drop the `volume` local; `if(category==0)return priority>100-soundEffectsVolume; if(category==1)return ...; return priority>100-soundMusicVolume;`
- SoundCache 0046be17, SoundCacheList 0046bf44, SoundLoad 0046c15e: same fix (inlined SoundAllowed).
- SoundPlayDefault 0046c980: same fix (inlined SoundAllowed).
  Keeping SoundReleaseList/ShellSoundShutdown/SoundStop STRICT (they jump into SoundPlay's tail) needed SoundPlay
  at its original 1083 B: `WAVEFORMATEX format; SoundBuffer *buffer;` declared just before SoundDuplicate (not at the
  top), `int attenuation=soundAttenuation[volume]*-100; SetVolume(buffer,attenuation);`.
- ModeCheck 0044eb00: no locals; the slot status expression written twice in the condition
  (`if(G->slots[G->mode-1].status==1 || G->slots[G->mode-1].status==3) return 1; return 0;`).
- SimFaceTarget 0044e420: follows from the ModeCheck spelling (ModeCheck inlined under -oe).
- f_0043d25f 0043d25f: callee f_0043d1a6 uses `int mask=1<<(index%8)` instead of `U8 mask` (inlined in the loop);
  f_0043d1a6 stays STRICT.
Also in simdo.c (structural, no count change): UnitState item slots are `ItemSlot items[23]` at +0x11 indexed
`items[item+19]` (the original adds 0x98 = 19*8 to base+0x11); SimExecuteItem now has its original 1104 B extent,
diff 938 -> 443.

## Codegen rows (docs/codegen_rules.md)
| plain-oe inline bool helper | `int v; if(c==0)v=..;else if(c==1)v=..;else v=..; return p>v;` vs one `return p>100-x;` per branch | the per-branch return form gives the original registers (volume in edx, priority in al) in the out-of-line body and in all 5 inlined copies | verified o1l, noise.c SoundAllowed (2026-11-14) |
| plain-oe stack slot order | position of a declaration (`WAVEFORMATEX format; SoundBuffer *buffer;` before its first use instead of at block top) and a named temporary (`int attenuation`) | moves frame slots of the function and of locals of inlined callees; fixed SoundPlay's slot layout (1083 B) | verified o1l, SoundPlay (2026-11-14) |
| ot-oe small predicate | aliasing local (`SimGame *s=(SimGame*)object; int status=...`) vs the cast expression repeated in the condition | repeated expression: parameter reloaded from the stack, status in EBX with push/pop; the local forms use EAX and drop the EBX save | verified o1l, simdo.c ModeCheck (2026-11-14) |
| plain-oe bit setter inlined in a loop | `U8 mask=(U8)(1<<n)` vs `int mask=1<<n` with `known[b]|=mask` | int mask swaps the EDI/EBP homes of the caller's parameters into the original order; byte stores unchanged | verified o1l, mcp.c f_0043d25f (2026-11-14) |
| plain-oe C++ default argument | `StrPool::GetString(long,int=1)` called with one argument as an argument of an indirect call (f_0043c5d6, f_0043d591) | produces the original EBP frame and shared `xor eax,eax` tail, but EDI save and the point/session register homes still differ; costs 5 STRICT neighbours, not kept | measured o1l (corroborates s42) |
| C++ COM interface | struct-with-function-pointer vtable vs C++ class with `virtual __stdcall` methods (noise.c) | identical bytes in every function | negative o1l |

## Remaining targets (residue)
noise.c
- SoundPlay 0046c461: D3 at +398: attenuation in EDX, original reuses volume's EBP (`imul ebp,ebp,-64h`); every spelling
  that reuses `volume` reorders the frame slots (D160..D470). ~40 variants (clamp forms, helpers, decl positions).
- SoundSetSettings 0046c89c: D32; original evaluates settings->frequency before v->buffer in SoundRate and holds the
  return in ECX; helper/inline/arg-order variants (16) unchanged. Original holds buffer in ECX, vtable in EAX.
- SoundCreateBuffer 0046bb20: not worked (D143, decoder call reloc + register residue).
simdo.c
- SimExecuteItem 0044ebe0: correct size now, 1-byte longer slot address (`lea edx,[eax+98h]` vs `add edx,98h`) ->
  D443 cascade; 10 spellings of the slot address/statement order tried.
- SimFireFocus 0044eb50: only SimExecuteItem's jump table bytes (D10), waits on it.
- SimUseItem 0044f030: D6 EAX/EDX swap of PopFindItem args; Item()/PopFindItem default/direct spellings no change.
- SimPanBy, SimCenterOn, SimSetFocus, SimCenterTarget, SimSetReturnFire, SimRandomFire, SimReturnShot,
  IntelClearMode, SimBeginTurn: not worked (large structural diffs).
mcp.c
- f_0043c5d6 / f_0043d591: need the EBP frame (GetString default, see row) plus point kept in memory and session in
  EBX/ESI; reference/pointer/macro/helper spellings unchanged. With the frame, d591's failure path jumps into c5d6's
  `xor eax,eax` tail (written as `return result;} return 0;`).
- f_0043c844, f_0043cab9, f_0043e37c: wait on f_0043c5d6's epilogue (c844 became STRICT with the frame variant).
- f_0043d2b9: original point not in a register, window/client RECT slots swapped; blocks f_0043dbb2 (shared epilogue)
  and f_0043e21a/f_0043e37c.
- f_0043bbcc: D5 dec-before-add; 6 more length spellings tried (all worse).
- f_0043cb47, f_0043e04d: not worked.
anim.c (candidate unchanged)
- AnimActionEqual / AnimActionCopyChangedFields: original uses an inline `!=` on an 8-byte pair taking references
  (state first) materialized as 1/0; with `operator!=` the 2nd/3rd blocks match, the first block coalesces the
  parameters differently (D110+). Not kept.
- WorldActionNotify, AnimActionConfigure: bitfield byte at +0Ah (`and 70h` then `or 80h` through DH) and an inline
  state setter shared by both; not worked further. The file still uses raw offsets and `volatile`; it needs a struct
  rewrite. Other anim targets not worked.

Candidate files: candidates/o1l/noise/noise.c, candidates/o1l/sim/simdo.c, candidates/o1l/core/mcp.c, candidates/o1l/sim/anim.c (= src).
