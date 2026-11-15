# Lane oc2 - last function of an object (6 functions)

No function became STRICT; no object completed. Two files improved without loss (dry runs below).
Every hypothesis family is recorded in candidates/oc2/attempts.jsonl (tools/notes.py show VA).

## Changed files (promote dry runs)

python tools/promote.py candidates/oc2/logic/lfire.c --tu src/logic/lfire.c --as 'logic\lfire.c' --flags '-4s -s -bm -oe=90'
    src/logic/lfire.c: 5 strict functions (1161 bytes), new 0, lost 0   (ActorChooseByMode D458 -> D195)

python tools/promote.py candidates/oc2/gui/alert.c --tu src/gui/alert.c --as 'gui\alert.c' --profile plain
    src/gui/alert.c: 11 strict functions (1694 bytes), new 0, lost 0    (GuiScreen::Draw 170 B/D64 -> 172 B/D31, right size)

## Per function

- ActorChooseByMode 00464f50 (logic\lfire.c): D458 -> D195, 681 vs 685 B. NEW: writing the case 8/9 divisor as a
  plain product (`other->count30 * other->value38`) instead of the inline Product() flips the whole allocation to
  the original's: the hidden-ID registers (+b8), the nested Available ECX->EAX copy (+11b), value=EBX/divisor=ESI and
  the case-9 tail merge into case 8's idiv all match. Left: the original keeps value38 in a register
  (`mov eax,[edi+38h]; ... imul esi,eax`, 2 sites, 2 B each) and loads `actor` into EDX for case 9's ActorName.
  Any Product()/accessor/local that gives the reg-reg imul in both cases flips the allocation back (D475-508) unless
  `hidden` is `unsigned char` (then 685 B, D45, but byte xor/test where the original has 32-bit): the original must
  differ in some other temp-producing construct of this function.
- GuiScreen::Draw 00425e1a (gui\alert.c): hidden test moved into the draw helper
  (`DrawControlCore(i){ if (Control(i)->Hidden()) return; ...}`) gives the original size (172 B), D31. The original
  copies the index twice (EDX for Hidden, EDI after the test for both controls[] loads); ~200 two-helper variants
  merge both copies into EDI (D64); helper-internal forms keep a dead `this` copy (EDI) and the index copy in EBP.
  Only a helper taking the list as a value (DrawP(controls,i)) keeps two copies, but then the list is CSE'd
  (original reloads [ebx+14h] per use).
- PopUpdateItem 004441f8 (pop\psystem.c): unchanged D245. The original computes the 10/15 conditional in EAX and
  copies it (`mov edi,eax`); every same-width spelling (enum/char/short/pointer/double arms, comma, chained,
  helpers, declaration forms) coalesces into EDI; only narrowing types make a temp (with movsx). Not found.
- SimApplyLeadership 004507d0 (sim\simturn.c): unchanged D8. Original mutates distance (`sub esi,2; imul edx,esi,10`);
  every mutation form swaps actor/distance homes (EBX<->ESI) and rotates the post-loop pilot temps by one register
  (D30), invariant to PointDistance spelling (7), helper removal (D283+), variable types and declaration placement.
- ExploPosition 00460340 (sim\explo.c): unchanged D6 (invariant). The case -10 unit load is allocated ECX, letting
  the scheduler hoist it above `mov esi,eax`; original allocates EAX. UnitPart default/explicit args, captures,
  casts do not change it.
- MovementCommand 00442af4 (pop\pmove.c): unchanged D990 (SHAPE). Found: case 2 in the original copy-initialises the
  point (field copy, no default-ctor zero stores) as Process does, and gInA4 is tested via an accessor (mov/test);
  applying copy-init reallocates the early selection code (D1350), so not retained. Case 2's IsMoving compiles to
  the single-load form (original: 5-reload form as in cases 0/1); case 7 does not CSE p->target; case 8 placement.

## Scratch
Templates and probes: build/oc2/*_t*.c; diagnostic wrapper build/oc2/varf.py (variants.py with --flags).
