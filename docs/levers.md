# Levers by residue class (what closed functions here)

A starting list per residue class (`python tools/parked.py`), ranked by how often the lever closed functions in this
project (`docs/attempts.jsonl`, verdict `fixed`, and the lane reports) and in Rayman (Watcom 10.6, same code
generator, `docs/watcom_playbook.md`). Each row names one example to read (`python tools/notes.py show LABEL`).
Compiled 2026-11-18 from 96 fixed attempts over 38 lanes. Rows are experience, not proof: the proven compiler facts
are in `docs/codegen_rules.md`.

How to use it: try the levers of your function's class roughly top down, but **combine** them. A lever that leaves
the diff unchanged or worse on its own can be the missing half of a coupled edit (fb1: the dead `result=0;` did
nothing until two cases also returned; Rayman wave 7: an abs form that made three functions worse alone closed five
together with nesting and declaration order). Record each family with `notes.py add`; a recorded `invariant` covers
the families its hypothesis names, not the function.

## SHAPE (wrong size): the source says something else
| Lever | Example (lane) |
|---|---|
| One inline function per original operation, taking values the caller has already read (the same-TU body appears inlined; `-oe`/plain-oe TUs) | HercConditionDiagram (j2), Shell::ProcessInput campaign commands (j3), InitApplication (j4), ActorCanAct (u1) |
| `-oe` profile: an uncalled out-of-line copy of a small function whose body also appears at call sites | HercBayOpen (m5) |
| Class parameters by value (a class with a user constructor is passed as a hidden pointer to a caller-built temporary) or by `const &`; check the caller's frame for the temporary | SimPanBy, SimCenterOn (r2), f_0043d2b9 (j2), GridPointToIndex (k2) |
| Control flow of the exits: explicit `else` plus one final return, early return instead of break-through-a-variable, return in guards then one common computation | InitializeWinG (oc1), MultiplayerAttemptStart (j1), GridFindNeighbor (k2), CurveSlope (f1) |
| Switch blocks that the original keeps apart: one case leaves by `return`, another by `break` to a switch end kept as its own label (a statement after the switch) | UpdateHercUpgrade (fb1), codegen_rules cross-jumping rows |
| Case label where the original jump table points (a case body cross-jumped into another block) | Rayman CALC_MOV_ON_BLOC (playbook, wave 7) |
| Typed aggregates instead of byte arithmetic: multi-dimensional arrays in natural index loops (strength reduction gives the pointer walk), function-static initialised arrays, unions where the original reads a member through another view | AnimBitmapSweep (r3), f_00459020 (m2), AnimActionInterpolateDrawPoint (ty1) |
| Shared type details that change code: signed vs unsigned char member, a default constructor present or absent, a member accessor vs a direct byte member | SimGameBlink, SimInvalidateActors, SimDamageActor (ty1) |
| A runtime call the compiler would inline: standard header plus `#pragma function(name)` | ResMgr::Init strchr (k1) |
| Un-merge: duplicate a call in both arms instead of modifying a temporary before one call | Rayman init_move_couteau (247 -> 6) |

## WAIT-SIZE / WAIT-TABLE and shared-tail dependents
| Lever | Example (lane) |
|---|---|
| Decode every differing jump on both sides, find the landing offset inside its owner, sum the size deltas of the publics in between: the root is the function whose tail position is wrong, not always the nearest DIFF | j1 four dependents, f_0043e37c (j2: deltas +5,+2,-7,-5,+5,-3,-2), ReportDrawOther, ReleaseState (j4) |
| The tail position inside the root can matter more than its length | MultiplayerAttemptStart (j1) |
| A carried table: same order and partition as the original = the owner drifts in size; a different order = the owner's case structure is wrong | Rayman carrier rule (playbook) |

## SAME-SIZE (one construct off)
| Lever | Example (lane) |
|---|---|
| Signedness at the operation: signed arithmetic at compares and divisions inside an unsigned public interface, without a separate signed copy | CurveForward, CurveInverse (f1) |
| Increment placement: post-increment inside the call argument; pointer advance at the end of the loop body; `++` inside the condition gives the memory increment | f_0043e04d (j2), Palette::FromRgb (f2), SimPaletteCycle (m3) |
| Index vs pointer traversal (pick by the original's limit test and increments) | Palette::ToRgb (f2), SimPaletteCycle (m3), worker_brief pointer-walk fact |
| Library idiom spelled as the original did: `strlen` as the length and `name[--length]` (DEC before ADD) | f_0043bbcc (j2) |
| Free helper taking the unit vs a member function; an accessor that computes the field address (`lea`) vs a direct member | ActorCrouch, SimDamageActor (ty1) |
| Thresholds and compare spellings (`>9`/`>=10`, `a<b`/`b>a`, `/2`/`>>1`) | playbook W-THRESH, W-OPORD |

## HOMES / REG-SWAP / SLOT-SWAP (same instructions, other registers or slots)
Fix shape first; these levers act on the set of backend records, not on the order of declarations alone.
| Lever | Example (lane) |
|---|---|
| Remove a local that the original never had (re-read the value or nest the expression) | QueueReceive (m2, D179 -> D3), f_00442045 (r1), UnitDamageIndicator (m3) |
| Add a named local the original had (capture a value once and reuse it), or capture through the compound assignment's value | HercBayReturnFromPurchase (r3), HercAdvanceTurn (m1), ActorSelect (u1) |
| Two statements instead of one expression (`int t = x; t = -t;`), or the reverse | BestLocSubCandidate (m6), Rayman RECALE_ALL_OBJECTS |
| Ternary join vs if/else (the join value lands in another register); `if (c) x = 1; else x = 0;` (AH) vs `x = c;` (AL); `!(a == b)` vs `a != b` | f_00441ca7 (r1), Rayman DARK_phase1, DO_SAXO_COUP |
| Statement order: independent stores and reads in the original's order (the scheduler hoists across them); exhaustive search when n! <= 120 | SimDrawBar (m5), ResMgr::Init (k1), BestLocDispatch (u1), Rayman doMOSAMScommand |
| Index expression instead of an incremented variable (`(d + i) % 6`) | UnitDamageIndicator (m3) |
| Declaration order, as the LAST step once the shape is right | f_0043cb47 (j2), Rayman init_move_couteau, setToleranceDist |
| Move a copy before or after a loop exit | MenuFormatNumber (f2) |

## RELOC / OPERANDS / UNPLACED (identity, not shape)
| Lever | Example (lane) |
|---|---|
| Use the existing extern name (`tools/symbols.py show VA`), the native destructor declarations, the implicitly declared derived destructor | PilotGuiModule::$dt, MenuListButton::$dt (f2), Shell::ProcessInput aliases (j3) |
| An exact declaration of a callee or caller places an unplaced extent | rv2 lactor wrappers |
| Check table and fixup ownership against the original before editing source | ScriptEvalOp (xa) |

## ASM
Hand assembly: model a whole object in address order under the right assembler (`asm-tasm50`, `asm-tasm40` when
TASM 5.0 crashes); data workspace and labels inside the object; `ALIGN 4` includes the padding NOP
(f3: RenderImage, RleAlignedFill, VectorLength).
