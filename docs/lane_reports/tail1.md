# Lane tail1: which copy of identical code Watcom 10.6 keeps (cross-jumping)

Probes: build/tail1/probes/*.c, compiled only through tools/cc.py (runner build/tail1/pr.py; listings
build/tail1/probes/NAME.PROFILE.txt). Model from OW v2 optcom.c/optins.c/optpull.c; every claim below was
checked on the pinned 10.6 compiler.

## The rule

Peephole queue: instructions arrive in block order (= source order; `-od` shows it, probe m0/h0 with -od:
`switch` select after the case bodies, `break` already threaded to the return label). The queue is not flushed
between functions of one segment, so tails are shared across functions.

R1 Later copy jumps into the earlier one. When a copy's closing `jmp`/`ret` arrives, it is compared backwards
   with every other jump to the same label (ret: every ret in the queue); the copy being emitted is deleted and
   replaced by a jump into the best older copy. A case/if label directly in front of a whole duplicate block that
   follows a jmp/ret is simply moved into the older block (no jump left).
   p1.c (case 3 = case 1 -> table entry points at case 1's block), q1.c, q3.c, q8b/q8c/q8d.c (case 3 = case 1,
   kept at case 1's position for break/return in either copy), t4_ot.c plain (case 1 tail -> `jmp` into case 0).
R2 Exception: the later copy survives when it FALLS THROUGH into the label that the earlier copy jumps to (the
   compare runs when that label is placed; the code in front of the label is kept). Typical: code after the last
   switch, falling into the function end. q2.c (hit copy -> `jmp` into the after-switch copy), m1.c,
   t5_size.c ot (`jmp 0x1d`: first if-branch jumps into the second, which falls into ret).
R3 Size: a partial tail is merged only if it is longer than a near jmp (5 B): `add esp,4; ret` (4 B) stays
   duplicated (p1.c), `add esp,4; xor eax,eax; ret` (6 B) is merged (t5_size.c g). Whole blocks behind a label
   move regardless of size.
R4 -ot (profile ot/ot-oe): partial-tail merging by R1 is off; only whole-block label moves (R1) and fall-through
   merges (R2) remain. t4_ot.c: plain merges case 1's `add; push 6; call M; ...` tail into case 0, ot keeps both;
   case 3 (= case 0) merged in both.
R5 Labels: the backward compare skips labels in the kept copy but STOPS at a label inside the copy being deleted.
   Under ot the remainder is then kept (oracle ActorPalette 0045a290, STRICT: two 10-instruction blocks at +10b
   and +17b, the second has a jump target inside). Under plain the part after the label is still merged.
R6 Epilogue label: when the `pop..; ret` epilogue is placed directly after a transfer (switch select, jmp), its
   label is moved into an older identical epilogue (also in an earlier function: the most recent real one in the
   queue) and every conditional branch to the function end goes there; unconditional jumps get clones of
   `pop..; ret` at output time. If the epilogue is reached by fall-through, its label stays local.
   t6_epi.c (f2 `ja` -> f0's `pop esi`); v_base.c (`je` -> UpdateStorage epilogue) vs v_m1.c (`je +24a`, local);
   full TU: UpdateBiodermPanel's `ja` goes to UpdateStorage 00403cdc with the in-switch launch and to
   UpdateMainMenuSelection's local epilogue with the after-switch launch.
R7 Clones made at output time (jump to <= jmp-size code ending in ret) are never merged again: oracle
   SimInTrapezoid 00453310 (STRICT) keeps two adjacent `xor eax,eax; pop x4; ret` blocks.
Not levers (measured): break vs return in any case, trailing no-code statements after the switch
(`do{}while(0);`, `{;}`, `label:;`), WCGMEMORY 16 KB..100 MB (object byte-identical, shell.c),
goto (`if(!c)break; goto launch;` compiles to `jne launch; jmp brk`, the reversed sense), inline launch helpers,
`if(result){switch}` wrapping (probes v_*.c, h_tmpl*.c, hcount.py: 40 combinations).
Oracle census (build/tail1/dupscan.py): only 6 functions in the whole program keep two identical whole blocks;
the 3 STRICT ones are explained: SimInTrapezoid (R7), ActorPalette (R5), open_link_connection 0046d0f4 vs
the next function's 0046d1f6 (`mov eax,1; pop x4; ret` at two function ends, both epilogues reached by
fall-through, R6, so only the 5 B `pop..ret` could merge, R3). The other 3 (UpdateHercUpgrade, SendEvent,
ActorQuery) are parked.

How to apply: list the copies in source order. The copy written first survives (R1) unless a later copy is the
last code before the label the others jump to (R2). Under ot only whole blocks and fall-through merges happen.
A shared `pop; ret` target in an earlier function means the epilogue followed a transfer (R6).

## The three functions

- 00403cdf Shell::UpdateHercUpgrade: unreachable from natural source under R1. Cases 54 and 59 are complete
  identical blocks emitted in source order with the same exit label; R1 merges them by moving case 59's label
  (probe h0.c reproduces the candidate; 40 break/return/trailing-statement combinations all merge). Two copies
  survive only if the second differs from the first at the moment it is emitted (different call/argument symbol
  or a label inside it, R5), and no natural spelling produced that. D220 unchanged; no candidate file.
- 00403f80 Shell::UpdateMainMenuSelection: the original keeps the result-switch launch copy (hit case 3 =
  `je +30; jmp +237`) AND its conditional exits go to UpdateStorage's epilogue 00403cdc. R2 gives the first
  (launch code after the switch: candidates/tail1/shell/shell.c, D547 -> D129, the hit copy is byte-exact) but
  then the epilogue is reached by fall-through and stays local (R6), so je/ja are short to +24a, the function is
  11 B short and three later STRICT functions lose their shared-tail displacements. In-switch spellings give R6
  but R1 keeps the hit copy. Both at once require the result-switch copy to be emitted before the hit-switch
  copy; with the hit switch written first (it executes first) no natural spelling does that.
  DO NOT ACCEPT the candidate (LOSS 3); it documents R2 only.
  python tools/promote.py candidates/tail1/shell/shell.c --tu src/shell/shell.c --as 'shell\shell.c' --profile plain-oe
  -> `LOSS (not accepted): ['Shell::Open', 'Shell::PlayTakeoff', 'ShellLoadPalette']` (baseline src: 41 strict, new 0, lost 0)
- 0045e430 FxBeam: no cross-jumping residue left in the current src: both redraws keep their own
  ViewFlip/ShellLimitFrameRate in original and candidate. D559 is the first redraw's actor-loop placement and
  register homes. Not changed.

STRICT gained: none.

## Rows for docs/codegen_rules.md

| Context | Trigger | Effect | Status |
|---|---|---|---|
| cross-jumping, plain/plain-oe | identical tails (> 5 B) or whole blocks in several paths | the copy emitted first survives; later copies become a jmp into it, a whole duplicate block behind a case label loses the label to the first copy. break vs return, trailing no-code statements, case labels do not change it | verified (tail1: p1, q1, q3, q8b-d, h0 sweeps) |
| cross-jumping, all profiles | later copy falls through into the label the earlier copy jumps to (e.g. code after the last switch, falling into the function end) | the LATER copy survives, the earlier one becomes `jcc..; jmp later` | verified (tail1: q2, m1, t5_size ot) |
| cross-jumping size | partial common tail <= 5 B (`add esp,4; ret`) vs 6 B | not merged vs merged | verified (tail1: p1, t5_size) |
| -ot (ot, ot-oe) | identical partial tails | not merged; only whole-block label moves and fall-through merges remain | verified (tail1: t4_ot plain vs ot) |
| cross-jumping, label inside | jump target inside the later copy | backward match stops at that label; under ot the copies stay duplicated | observed (oracle ActorPalette 0045a290 STRICT) |
| epilogue placement | `pop..; ret` epilogue directly after a transfer (switch select) vs reached by fall-through | after a transfer its label moves into the most recent identical epilogue in the queue, also in an earlier function, and jcc's to the end go there (near jcc); by fall-through it stays local (short jcc) | verified (tail1: t6_epi, v_base vs v_m1, UpdateBiodermPanel in both shell.c builds) |
| output-time clones | jmp to <= 5 B code ending in ret | cloned at output, never merged again: adjacent identical `xor eax,eax; pop..; ret` blocks | observed (oracle SimInTrapezoid 00453310 STRICT) |
| WCGMEMORY | 16 KB..100 MB passed to the pinned compiler | object byte-identical (shell.c): memory pressure does not explain unmerged copies | measured negative (tail1, build/tail1/memwrap.py) |
| oracle census | identical whole blocks kept inside one function | only 6 places program-wide (3 in STRICT code, explained by the label/clone/fall-through rows; UpdateHercUpgrade, SendEvent 00436138, ActorQuery 00436dcc parked) | measured (build/tail1/dupscan.py) |

Notes recorded: `python tools/notes.py add --lane tail1` on 00403cdf (invariant), 00403f80 (open), 0045e430 (open).
