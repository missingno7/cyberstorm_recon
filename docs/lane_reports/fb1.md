# Lane fb1: the two blocked roots of shell/shell.c

python tools/promote.py candidates/fb1/shell/shell.c --tu src/shell/shell.c --as 'shell\shell.c' --profile plain-oe
-> `src/shell/shell.c: 47 strict functions (15990 bytes), new 4, lost 0` / `dry run OK`

Candidate: candidates/fb1/shell/shell.c (diff against src: 4 lines in Shell::UpdateHercUpgrade, line count unchanged,
so every ASSERT line number is kept).

## Root 2: Shell::UpdateHercUpgrade (00403cdf) - FIXED, STRICT 579 B

New STRICT: Shell::UpdateHercUpgrade, Shell::UpdateMainMenuSelection (00403f80), Shell::UpdateBiodermPanel (00404258),
Shell::UpdateMultiplayerMenu (004043a8).

Source change:

    switch(result){
    ...
    case 85:HercUpgradeAction85(&g4b700c);return;     (was break)
    case 54:HercUpgradeRefresh(&g4b700c);break;
    case 59:HercUpgradeRefresh(&g4b700c);return;      (was break)
    ...
    }
    result=0;                                          (new; produces no code)

Why it works (model from Open Watcom optcom.c/optins.c; the outcome is measured on the pinned 10.6, the mechanism
is the reading of that source):
- Every simple case enters the peephole queue as `push g; call X; add esp,4; jmp L`. ComTail first shares
  `add esp,4; jmp L` with the first block that jumps to the same label (case 85) and rewrites the block to
  `push; call; jmp Lnew`. OptPush then retries the NEW jump against the other jumps to Lnew: this retry is what
  finds the identical 54 block and merges 59 into it (tail1's R1).
- The retry only happens for a jump processed at input time. If 54's `jmp` targets a label that is not yet the
  same label (the switch end) while 59 and 85 jump to the function end, 54 keeps its `add esp,4; jmp` until the
  switch-end label arrives, is aliased to the function end, and TraceCommon then shares only the `add esp,4` tail
  with block 85. TraceCommon does not retry the rewritten jump, so 54 and 59 stay two blocks: the original bytes.
- `break` and `return` normally reach the same label because the empty block after the switch is threaded before
  the queue (this is why all break/return sweeps of m1/r1/tail1 merged). A statement after the switch that is only
  removed later as dead (`result=0;`) keeps that block, and with it the second label, alive until the queue.

Measured (build/fb1/v4.txt, v5.txt, v6.txt; runner build/fb1/hv.py):
- STRICT: {85,59 return, rest break}; {85,50,49,57,59 return}; every simple case return except 54; every simple
  case return except 59; the same with the long cases 47/48/53 returning. All need the statement after the switch.
- Not STRICT: dead store with all break (563), 54 or 59 the only return (576, tail kept in the wrong block),
  85 the only return (563), returns only on 57..58 (576).
- Statement after the switch: `result=0;`, `result++;`, a store to another existing local work. `;`, `{}`, a
  label, `return;`, `(void)expr;`, `if(result==54){}`, a new unused local, an empty inline call do not.

Review point for the integrator: the dead assignment is ordinary C, but it is the only form found, and it is in
the candidate only because of its effect on the bytes; which cases return (minimal set 85+59, or "all return,
54 breaks") is not decided by the bytes. The candidate uses the minimal set.
Possibly reusable for the other two parked "identical whole blocks" functions of tail1's census (SendEvent
00436138, ActorQuery 00436dcc): look for one copy that leaves through the switch end while the other returns.
Not tested there.

## Root 1: Shell::Update (00402230) - still DIFF 1,308 B / D801 (UpdateSelection, WithinBudget still wait)

What the original does that no natural source found reproduces:
- The candidate has two extra 2-byte instructions, `mov esi,eax` after the inlined ResumeMenus mode load (+2a1)
  and `mov esi,ebx` in front of +1ed. New: in the candidate BOTH `mov esi,ebx` at +c3 (also in the original) and
  `mov esi,eax` are dead copies (esi is not read before it is redefined on any path). In the original the +c3 copy
  is live: it is the `this` of the RestoreMenu bodies at +187/+1ba, 300 bytes later, and the selector of the
  ResumeMenus tree switch never gets a second register. Reading: the original keeps ESI for the inlined `this`
  chain over +c3..+1ed; the candidate gives ESI to a copy of the switch selector in between, which cuts the chain
  (it is re-created from EBX at +1ed). This is an interpretation of the listings, not a measured allocator fact.
- j3's "reversed briefing cases give 1,304 B" is not the original code: `mov esi,ebx` at +1ed is still there
  (build/fb1/rev.txt); only the length coincides.
- Byte-neutral respellings do not move it (always identical 1308/D801, 47 strict): 128 break/return combinations
  in BeginComms/ResumeMenus/RestoreMenu, `result=0;` after the three inlined switches with break/return mixes,
  unused or copy locals in Update and all inlined functions, `Request()` vs `&input`, key type/source in
  UpdateStorage, a named `&input` local, `long s=screen`, U32/long/int/unsigned for resumeReason and the
  RestoreMenu parameter, member-call form for UpdateComms/UpdateHercBuy, LogicMode() in BeginComms,
  SetMissionChoice(unknown1d=...).
- Only changes that alter the temporaries of Update flip the copy, and each brings other damage: one extra live
  temporary in case 38 (diagnostic, adds code) removes `mov esi,eax` but changes BeginComms' kind byte handling
  and the default restore's push order; replacing the inlined UpdateStorage by a call changes the whole briefing
  region; direct field read in ResumeMenus removes both copies but gives 1,307 B and shrinks UpdateMainMenu;
  `active`/`gIn7c` direct reads swap ESI/EDI in the default restore; `kind=0;` after BeginComms' switch
  (standalone BeginComms stays STRICT) gives 1,311 B. ResumeMenus with the third restore after the switch,
  default first, or `if(!Has..)` breaks the standalone ResumeMenus/UpdateBriefing.
- Shell::UpdateBioderm's 9 bytes look like the same family (reason and &input homes swapped in the default
  restore); not worked on.
Open question: which source difference inside the inlined UpdateBriefing/ResumeMenus chain makes the `this` copy
of +c3 the value the restore bodies use while leaving the standalone UpdateBriefing (534 B) and ResumeMenus
(302 B) unchanged. No candidate change was kept for this root.

## Notes recorded
`python tools/notes.py add --lane fb1` on 00403cdf (fixed) and 00402230 (open).

## Rows proposed for docs/codegen_rules.md

| Context | Trigger | Effect | Status |
|---|---|---|---|
| cross-jumping, plain/plain-oe | two identical whole case blocks, one leaves by `break` to a switch end that is kept as its own label until the queue (a dead statement such as `result=0;` after the switch), the other by `return` | both blocks survive; only the `add esp,N` tail is shared with the first block | verified (fb1: UpdateHercUpgrade STRICT, build/fb1/v4..v6) |
| same | nothing after the switch | break and return reach one label before the queue; identical blocks always merge | verified (fb1 v3/v4, agrees with tail1) |
| plain-oe, dead register copies | inlined `this` or switch selector copied to a second register that is never read | the copy instruction stays in the output (Update +c3, +2a1 in the candidate) | observed (fb1) |
