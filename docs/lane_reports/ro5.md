# Lane ro5 | Reopening 12 invariant-scoped functions

Two hotseat functions match together, adding 836 STRICT function bytes in the dry run. The two storage callers have fewer differing bytes and remain open. The other eight targets retain their starting sources. All eight full-TU promotion dry runs reported zero losses.

## Promotion dry runs

```text
python tools/promote.py candidates/ro5/shell/hotseat.c --tu src/shell/hotseat.c --as 'shell\hotseat.c' --profile plain-oe
src/shell/hotseat.c: 18 strict functions (5276 bytes), new 2, lost 0

python tools/promote.py candidates/ro5/shell/hbmanage.c --tu src/shell/hbmanage.c --as 'shell\hbmanage.c' --profile plain-oe
src/shell/hbmanage.c: 23 strict functions (2898 bytes), new 0, lost 0
```

The hotseat dry run implies three data bindings, `hotseatLayoutName` at `0048acd0`, `hotseatFontName` at `0048acdc`, and `hotseatImagesName` at `0048ace6`. The call at opener offset +0x7b uses the matched `GuiScreen::Open(InputReq *, StrPool *, long, int)` declaration in the final candidate. The existing source-padding comments still place the later hotseat assertion on line 556. The storage candidate extracts the seven field resets shared by the two open paths. Its dry run adds no functions. Neither candidate was published.

## Hotseat | Full-TU controls

The original opener copies its screen pointer from EBX to ESI immediately before the clip reset. The standalone helper sign-extends the color into EDX and adds 25 there. Watcom can emit both sequences when the loop index is declared first and the typed screen pointer is initialized before the reset. The opener's guard and the frame-address expression determine whether they occur together.

| Opener control flow | Frame lookup in the shared icon helper | Opener at 00424a07 | Standalone helper at 004255bf |
|---|---|---:|---:|
| Early return when active | Typed image pointer plus frame index | STRICT, 749 B | 37 differing bytes |
| Early return when active | Header address plus frame byte offset | 42 differing bytes | STRICT, 87 B |
| Body guarded by inactive screen | Header address plus frame byte offset | STRICT, 749 B | STRICT, 87 B |

The one shared helper computes its frame offset from the signed color and `sizeof(MenuImage)`. The index variable is declared before the typed screen view and initialized to zero in the loop. The last two rows differ only in the opener's guard in the control build. The final build also uses the existing matched Open prototype and spells the 12-byte stride with sizeof.

## Function-by-function findings

D is the diagnostic differing-byte count. Sizes in the final-state column are candidate sizes. The middle columns identify the older scope and the combinations tested outside it. Each baseline listing, full-TU sweep and dry run is in build/ro5. The structured attempt records are in [attempts.jsonl](attempts.jsonl).

| VA and function | Older tested scope | Reopened combinations | Final state and original instruction evidence |
|---|---|---|---|
| 00424a07 f_00424a07 | Typed icon records, helper argument and member forms | All six team/last/value store orders, image operand ownership, loop induction, declaration and binding timing, then enclosing guard | **STRICT, 749 B.** The final source restores the screen copy before clip reset and the original inline stride calculation. |
| 004255bf f_004255bf | Width/cast, image/index captures and split addition | Signed-color walk or index, y induction, typed table versus byte offset, then index declaration before the screen view | **STRICT, 87 B.** The standalone MOVSX EDX and ADD EDX,25 agree, as do the stride and final pushes. |
| 004180e3 ShellStorageOpen | Clear/setup/member interfaces, count widths, name captures | All 120 clear orders, pair of label receivers or pointer walks, then shared reset boundary, phase width, Names predicate and guard | **690 B, D267 → D188**, original 687 B. An int phase on the free inline reset restores the left label receiver in ESI and stride in EDI. The original uses an immediate phase store, separately forms the right receiver in ESI, and widens the name-loop count with MOVZX. D118 loses accepted StorageScreen::Names and ShellStorageRender. |
| 00418392 StorageReopen | Direct/inline clear, manual Names loop, base argument and count capture | All 120 clear orders, offset assignment ownership, Names limit/index arithmetic, then grouped layout and shared phase reset | **421 B, D222 → D215**, original 420 B. The joint candidate still materializes phase 3 in EBX. The original uses the immediate, copies ESI into EBX at +0xaf and spills the screen pointer for the name loop. D170 with a shared Names/LoadLayout helper raises Open to D282. D142 loses accepted Names and Render. |
| 00447fbe PilotScreen::DrawBar | Separate image expressions, clamp, x/y locals and getter | Consistent typed image-table forms in both loops, crossed with coordinate order, y increment and clamp control flow | **414 B, D92**, original 415 B. Original x/y occupy EDI/ESI. Both loops load the image table, form base+4 with LEA and add a scaled index. Best common-loop trial D106. |
| 004475de PilotScreen::DrawDetails | Pointer declarations, status/getter width, resource ID capture | Move pointer construction to the producing calls and cross with y/ID induction, then walk both pointers in both update orders with status capture | **573 B, D13.** Original advances and stores current values (ESI) before original values (EBX). At +0x19d it computes the string ID before pushing zero. |
| 004194c4 f_004194c4 | Stat declarations/types and passing stat results as helper arguments | Refactor existing stat helper around argument evaluation and couple it to name-list indexing, then cross selected-byte ownership with both flag store orders | **945 B, D4.** First and second statistic results still occupy the opposite ESI/EDI homes. The helper/list rewrite also changes unrelated accepted bodies. |
| 00422830 LaunchRefreshCostLabels | Label and y captures, receiver types, row helper | Bind money and limit rows independently, reuse receiver after present, and extract the image/color operation for one or both branches with varied argument order | **634 B, D230.** Original computes player−1 before y. The money block keeps the receiver and spills y. The limit block spills the receiver and keeps y. The retained candidate keeps y in both. |
| 0044540f f_0044540f | Clip getters, field captures and dimension order | All 24 color/map store orders crossed with prior-flag capture, then nested coordinates and dimension getters crossed with guard and index timing | **548 B, D4.** Width/height still occupy EAX/ESI. The original loads ESI/EAX and pushes EAX then ESI. |
| 00445cfe f_00445cfe | Audio types, getters, prototypes, saved-setting and buffer snapshots | Both pool clear orders, buffer ownership and guard, then dimension captures at each preceding surface call | **293 B, D5.** Original restores the second setting through EDX and tests the buffer in ECX. Candidate uses EAX and EDX. |
| 00444fbb SimInventoryEvent | Actor alias, facing width, angle type and setter spelling | Fallback/default exits and selection decrement crossed with angle ownership, then return/break in update arms crossed with facing capture timing | **554 B, D9.** Original facing is in ESI and the inlined aim angle is in EDI. Candidate reverses them. |
| 00449d76 PopWorldDraw | Bounds/aggregate copies, getters, member context and capture order | All 24 field-read orders crossed with the active guard, then selectively nested clip operands crossed with inlined text-ID arithmetic | **319 B, D8.** Original x/y/width/height homes are ESI/EDX/ECX/EAX. Candidate uses EAX/ESI/EDX/ECX. |

The lane scratch adapter routed temporary directories under build/ro5 and limited notes lookup to the archived records plus ro5. It ran the repository's compiler, comparator and promotion logic. The commands above are the integrator's standard dry-run invocations.
