# Watcom 10.6 playbook (imported from rayman_recon)

Source: `rayman_recon@35f82bc:docs/COMPILER.md` and `docs/CAUSES.md`, the user's other Watcom 10.6 reconstruction
(Rayman, DOS). Imported 2026-11-14 so workers here do not rediscover it. Read it as calibrated experience from the
SAME code generator, not as proof for this target; rows proven here live in `docs/codegen_rules.md`.

Differences to keep in mind:
- Rayman is C (`wcc386 -oaxt -s -5r -fpc`, register calling convention). CyberStorm is C++ (`wpp386 -4s -s -bm`,
  stack convention, profiles plain / ot / +oe). `-oa` effects (globals cached in registers) do not apply here;
  `inline` is legal here; `-oe` IS used here in many files.
- Rayman's W-BIND and W-PORT rules (port leftovers, Android sizes) are project specific and were left out.
- Rayman's `#pragma aux` inline helpers were a user-approved exception there. Here `#pragma aux` with code bytes is
  forbidden (AGENTS.md); nothing so far suggests this game used such helpers.

## What Rayman already established about register allocation (do not repeat)
- Unrelated declaration context does NOT steer allocation (H1, controlled tests). Same result here (IoClose).
- A pure register/home tie needs a different SET of backend records (created by a different expression or statement
  shape), not a different order: declaration permutations cannot reach it. Exhaustive declaration-order sweeps
  (24-720 orders, about 20 functions) succeeded about twice. From about 95 small-diff candidates the dedicated
  inverse-allocation method closed 2.
- Most near-miss functions have several independent causes; over 295 functions only 2 had one isolated cause.
  "The bulk differs in IR shape (size/structure) before allocation": fix shape first.
- Tools built there: `tools/cgtrace.py` (debugger observation of the pinned wcc386 allocator), `tools/explain.py`
  (per-function mechanism report: home sort, rover, colour tie, Score), `tools/permute.py` (pycparser source
  permuter). They are tied to the C compiler binary and C syntax: not usable on wpp386 / C++ without a port, and
  their yield there was low. Porting them is not recommended before the SHAPE class here is worked off.

## Symptom -> what to try (Rayman W-rules)

| ID | Symptom in the diff | Try | Not when / note |
|---|---|---|---|
| W-RETW | prologue push set differs; `cwde`/`movsx`/`and eax,0ffh` vs `xor ah,ah` | narrower **own return** (s16/u8/u16) | ~25 units (get_proj_x, but*pressed, MURDUR) |
| W-WIDTH | `cmp ax` vs `cmp eax`, movzx vs movsx, frame size | narrower/wider locals, loop index (s16), params, signed vs unsigned | ~25 units |
| W-UNUSED | extra EDX/EBX save, `ret 4` | add/remove an unused formal parameter as the DOS callers pass it | 7 units |
| W-OPORD | load order, `jbe`/`jae` swap, ESI vs EDI | write operands in the binary's load order (`a<b` vs `b>a`, `x+y` vs `y+x`) | ~25 units |
| W-THRESH | `cmp 2/jbe` vs `cmp 3/jb`, `test ax` vs `cmp ax,1` | other spelling of the same bound: `>9`/`>=10`, `>0`/`!=0`/`>=1`, `-x+c`/`c-x`, `/2` vs `>>1`, `~m` vs `0xff-m` | ~11 units |
| W-BRANCH | inverted jcc / swapped blocks | reorder if/else (often "death/zero-HP case first"); independent `if`s vs `else if`; keep DOS guards | ~22 units |
| W-STORE | store/mov scheduling | DOS statement order of field/global stores | ~20 units |
| W-TEMP | saved-register set, extra spill, reloads | usually *remove* a cached local/pointer (re-read `obj->x`, globals); sometimes a block-scoped s16/s32 snapshot. -oa caches and writes back globals itself - never add a manual writeback | ~28 units |
| W-DECL | stack slot / register colouring only | local declaration order or block scope; prefer block scoping - exhaustive permutation sweeps (24-720 orders, ~20 functions) succeeded ~2 times | ~15 units |
| W-BITF | and/shift vs `test byte`, clear/store sequences | bitfield view (union with `raw`) instead of mask arithmetic; `!flip` not `^` | ~13 units; shared views in obj.h/map.h/game.h |
| W-BOOL | `setne`/`and`, byte-register choice | natural boolean forms: `b=1; if(x) b=0;`, `!(a&&b)` vs `!a\|\|!b`, s32 flag local | casts that only force bytes (`?(s16)1:(s16)0`, `(u8)(a!=b)`) are rejected (OPEN) |
| W-LOOP | counter register, saved regs, `__STOSB` call | pointer walk with cached count; `for(;c;)` vs `while(c)` vs do/while; advance at bottom | ~16 units |
| W-SWITCH | extra tables, reloads | grouped labels vs duplicated bodies; remove DOS-absent empty cases (each emits a table entry); `case 0:` vs `case 0: default:`; a one-case switch is not an if | ~16 units |
| W-ABS | `cdq/xor/sub` vs `test/jge/neg` | Abs macro or ternary on a promoted `(s32)` operand; never `<stdlib.h>` abs/labs | ~11 units |
| W-COPY | `rep movsd` forms, `memset_` call | `memcpy` with `<string.h>` (intrinsic, F2 prefix) vs struct assignment per site; memset is a call; C zero loop -> `__STOSB`; `strcpy(x,"")`/`strcmp(x,"")` add CONST slots | F3 STOSD has no known source (OPEN) |
| W-ABI | wrong argument registers, caller saves around asm calls | ABI-only `#pragma aux parm [...] modify exact [...]` (src/include/lowlevel.h; docs/reference/lowlevel_abi.h) | 8 units |
| W-INTR | port/interrupt code as calls | `<i86.h>` `_disable`/`_enable` (adds CLD), `inp`/`outp`, `_dos_getvect`/`_dos_setvect`, `__interrupt __far`; else ORIGINAL_INLINE pragma (AGENTS.md) | 5 units |
| W-CONST | CONST segment mismatch | string literal order = first appearance in source; duplicates merged | input, worldmap |

**Do not try (measured dead ends):**
- Compiler options: `-on/-oi/-ol/-ol+/-om/-or/-oc/-ou` were probed in ~40 units / 100+ functions with **zero STRICT
  gain**; `-oe`/`-oe=N` (~12 units) always inlines and regresses; `-oh/-ok/-oi+` are rejected. Releases 10.0/10.5:
  identical to 10.6; 10.0a worse (~15 units). Measured over all 121 units / 1,497 functions (ai_decomp review
  2026-10-21): `-d1` no change; `-d2` changes every function, 0 gains; `-j` 1 loss (load GetStringTxt), 0 gains;
  `-ei` no change. The profile is settled: `-zq -oaxt -s -5r -fpc`.
- `inline`/`__inline`/`static __inline` (E1022) and C99 declarations: rejected by Watcom 10.6 (~11 units retried).
- `register` keyword: no effect in most of ~8 units.
- Exhaustive declaration-order or width grids on a function whose residue is a helper idiom (ashr16 toward zero,
  sgn, CL-count shifts, inclusive-range booleans, fill/copy): plain C never produces these; they are original
  inline helpers - use src/include/inline_helpers.h (AGENTS.md exception, section "Original inline helpers").
- Byte-forcing constructs (dead redundant init, empty cases, scratch clobbers, type-forcing block-scope externs)

## Watcom 10.6 allocator internals (PROVEN from the WCC386 10.6 binary, build/workers/cg_re, 2026-10-22)
- Stack homes: an UNSTABLE Shell exchange sort over ALL backend names of the function, then filtering to the names that
  need homes; no declaration-order key; same-ID aliases sort by descending offset (168/168 real sorts; 19/19 homes in
  four NEAR functions). Hence tie order depends on the complete set/order of backend names (incl. temporaries and
  locals later optimised away) - this is the "hidden backend records" effect.
- Global conflicts are sorted by unsigned savings only, through the same unstable sorter (218/218 sorts).
- Colouring (GiveBestReg): maximise full/half-size move affinity, then prefer a fully contained GivenRegisters choice,
  then the first eligible list entry (3,575/3,575 decisions). 10.6's dword list has EBX before ECX (OW1.4: ECX before
  EBX; table at RVA 7f674); byte/word lists equal OW1.4 (AH=1, AL=2 order index).
- LdStAlloc::FindRegister uses per-width rovers that advance BEFORE testing: with no exclusions its first byte choice is
  AH, while an unconstrained global temp gets AL - explains direct-predicate AH vs explicit-snapshot AL (ra_byte).
- Loop cost = 20^min(depth,5) (SetLoopCost table 1,20,400,8000,160000,3200000).

## C constructs and backend records (inv_shape, Watcom 10.6 game profile, 2026-10-22)

**PROVEN, scoped measurements:** 58 synthetic functions in `build/workers/inv_shape/probes/constructs.c`, observed through the cg_re/inv_alloc read-only debugger harness. Compiler = pinned wc106 `-zq -oaxt -s -5r -fpc`. The complete observed OMF equals the normal same-path OMF. This table is a set of calibrated examples, not a C-to-IR predictor or a matching recipe.

Counts are **R/H**, the numbers of ALL backend names entering the first rough sort and final home sort. H is NOT the number of stack homes. The following partition refers to the first rough input: C = a surviving user master identified through AllocUserTemp, anon = anonymous name, alias = flag-2 alias, opt = optimizer symbol. A nonzero symbol field alone does not establish a C local; loops also synthesize symbol-tagged constants. Numeric IDs are function-local. The raw records, savings/conflicts, rover choices and five IR phases are in `probes/summary.json` and the adjacent traces/listings.

| ID / construct | Probe functions and R/H; first-input partition | Record consequence and limits |
|---|---|---|
| S1: Name a sum: `return x+w-bx` -> `t=x+w; return t-bx` | `add_expr`: 8/8; 3 C, 5 anon, 0 alias, 0 opt; `add_named`: 8/8; 4 C, 4 anon, 0 alias, 0 opt | 8 records remain 8; the ADD result becomes a C master. Naming is not an extra-record operation. |
| S2: Copy then compound-add: `t=x; t+=w; return t-bx` | `add_expr`: 8/8; 3 C, 5 anon, 0 alias, 0 opt; `add_compound`: 7/7; 4 C, 3 anon, 0 alias, 0 opt | 8 -> 7: the promoted left operand is the named destructive ADD destination; the separate ADD-result name disappears. Short a=a+b versus a+=b is instead identical in this probe (short_assign/short_compound, 2/2). |
| S3: Name the left or right promotion | `add_expr`: 8/8; 3 C, 5 anon, 0 alias, 0 opt; `add_named_left`: 8/8; 4 C, 4 anon, 0 alias, 0 opt; `add_named_right`: 8/8; 4 C, 4 anon, 0 alias, 0 opt | Same total, different owner and creation order. Naming the right operand creates its CONVERT before the left CONVERT, without commuting ADD operands. Full-unit ECLAIR shows that this does NOT guarantee the required conflict order. |
| S4: Reuse an existing result for the second calculation | `two_sums`: 9/9; 5 C, 4 anon, 0 alias, 0 opt; `second_reuse`: 8/8; 5 C, 3 anon, 0 alias, 0 opt | 9 -> 8: one second-expression name disappears. Reusing the variable is distinct from adding a new local. Two ETINCELLES trials confirm count reduction, but neither closes. |
| S5: ?: versus if/else assignment | `choose_ternary`: 4/4; 3 C, 1 anon, 0 alias, 0 opt; `choose_init`: 4/4; 3 C, 1 anon, 0 alias, 0 opt; `choose_if`: 4/4; 4 C, 0 anon, 0 alias, 0 opt | All have 4 surviving records. The initialized ternary discards the C result local and retains an anonymous merge; if/else retains the C master instead. AllocTemp calls differ (5,6,5), despite equal surviving totals. choose_short_init/choose_short_if likewise 4/4 with anonymous versus named result. |
| S6: Ternary absolute value versus in-place update | `abs_ternary`: 2/2; 1 C, 1 anon, 0 alias, 0 opt; `abs_inplace`: 1/1; 1 C, 0 anon, 0 alias, 0 opt | 2 -> 1 surviving records: the ternary merge disappears. This is already a documented failed source direction for the constrained moskito case; it was NOT recompiled here. Not an ABS-helper justification. |
| S7: Repeated expression versus snapshot | `repeated_sum`: 9/9; 3 C, 6 anon, 0 alias, 0 opt; `snapshot_sum`: 6/6; 4 C, 2 anon, 0 alias, 0 opt | 9 -> 6, including a named shared value. This probe uses value parameters, so the call cannot change those operands. repeat_word/snapshot_word separately produce 4 -> 5, but are NOT interchangeable when consume can change the global. |
| S8: Direct byte predicate versus byte snapshot | `predicate_byte`: 1/1; 0 C, 1 anon, 0 alias, 0 opt; `snapshot_byte`: 2/2; 1 C, 1 anon, 0 alias, 0 opt | 1 -> 2, with a useful C byte master. Allocation/rover history changes. This is the established stackRay/ra_byte direction, not a new closure here. |
| S9: Byte offset update: direct / byte / promoted short / wide snapshot | `adjust_direct`: 3/3; 2 C, 1 anon, 0 alias, 0 opt; `adjust_snapshot`: 4/4; 3 C, 1 anon, 0 alias, 0 opt; `adjust_promoted`: 3/3; 3 C, 0 anon, 0 alias, 0 opt; `adjust_wide`: 4/4; 3 C, 0 anon, 1 alias, 0 opt | 3 / 4 / 3 / 4. The byte snapshot adds a C byte record; the short snapshot replaces the anonymous conversion; the wide snapshot creates a 2-byte alias of the named 4-byte master. adjust_wide has only 3 AllocTemp events but 4 surviving records: alias creation is not covered by AllocTemp counts. |
| S10: `sum+=*p++` versus `sum+=*p; ++p` | `ptr_post`: 7/7; 3 C, 2 anon, 0 alias, 2 opt; `ptr_pre`: 6/6; 3 C, 1 anon, 0 alias, 2 opt; `ptr_bottom`: 6/6; 3 C, 1 anon, 0 alias, 2 opt | 7 / 6 / 6: *p++ retains the old-value pointer name (flag 8); reading *p then ++p or p=p+1 does not. Two optimizer-symbol records are also present in all three. Unused prefix/postfix scalar results are a separate question. |
| S11: Equivalent element-address spellings | `addr_index`: 5/5; 2 C, 3 anon, 0 alias, 0 opt; `addr_plus`: 5/5; 2 C, 3 anon, 0 alias, 0 opt; `addr_bytes`: 5/5; 2 C, 3 anon, 0 alias, 0 opt | All 5/5. p[i], *(p+i), and byte-address arithmetic with the element scale do not change the set in this context. No reason to repeat those spellings on a constrained target without another discriminator. |
| S12: `a+b<c`, `c>a+b`, or `t=a+b; return t<c` | `cmp_sum_left`: 10/10; 3 C, 7 anon, 0 alias, 0 opt; `cmp_sum_right`: 10/10; 3 C, 7 anon, 0 alias, 0 opt; `cmp_named`: 10/10; 4 C, 6 anon, 0 alias, 0 opt | All 10/10. Naming the sum changes ownership; reversing operands changes comparison source order, not the count. compare_if/compare_ternary/compare_assign also all have 5/5 after frontend temporary removal. |
| S13: Redundant promotion cast versus unsigned cast at a call | `argument_byte`: 2/2; 1 C, 1 anon, 0 alias, 0 opt; `argument_short_cast`: 2/2; 1 C, 1 anon, 0 alias, 0 opt; `argument_unsigned_cast`: 3/3; 1 C, 2 anon, 0 alias, 0 opt | 2 / 2 / 3 surviving records. The intermediate s16 cast creates an extra transient AllocTemp but disappears before sorting. The unsigned cast prevents reuse of the signed-int promotion across the call, retaining a distinct conversion for the return; values still fit int for the u8 domain. |
| S14: Narrowing a sum versus promoting operands | `cast_sum`: 4/4; 2 C, 2 anon, 0 alias, 0 opt; `cast_operands`: 5/5; 2 C, 3 anon, 0 alias, 0 opt | 4 / 5. This changes arithmetic semantics outside the narrow range; count reduction alone is not evidence to narrow a target. add_cast/add_expr both 8/8: redundant s32 casts do nothing there. |
| S15: Name a scaled product | `product_direct`: 8/8; 2 C, 6 anon, 0 alias, 0 opt; `product_snapshot`: 8/8; 3 C, 5 anon, 0 alias, 0 opt | Both 8/8, with anonymous versus C-owned product. The full-unit UFO trial likewise keeps its counts and its typed operation/dependency graph after owner/ID normalization, but changes allocation and loses the previously STRICT table owner. Cardinality and ownership alone do not determine the desired priority. |
| S16: Short versus wide cross-call copy / difference | `narrow_copy`: 2/2; 1 C, 1 anon, 0 alias, 0 opt; `widen_copy`: 2/2; 2 C, 0 anon, 0 alias, 0 opt; `narrow_delta`: 5/5; 3 C, 2 anon, 0 alias, 0 opt; `wide_delta`: 6/6; 3 C, 3 anon, 0 alias, 0 opt | Copies both 2/2, with differing ownership; difference/Abs cases 5/5 versus 6/6. Width, owner and number of names are separate axes, and width changes require independent semantic evidence. |

**STRONG interpretation, supported by these probes and six full-unit trials:** ownership replacement, name elimination, alias creation, and global/local conflict membership must be tracked separately. Equal counts do not mean equal records; a new C spelling can preserve the local conflict prefix. Deleting one name in a frozen final list is not a prediction of a C edit that also changes earlier savings, aliases, reductions or scheduling.

**PROVEN full-unit negatives (4 functions / 3 units, six source trials):** ECLAIR named-left remains 352 B/4 differences; named-right 352 B/2 differences but retains width-before-X conflict priority. ETINCELLES result reuse (both sites / second site only) gives 556 B/83 or 33 differences. UFO named scaled offset gives 844 B/363 differences and loses test_block_chdir STRICT. FonduPixel X snapshot gives 212 B/24 differences. None gains STRICT. ECLAIR 4 -> 2 is only a byte-diagnostic improvement. No closing source is proposed.

**PROVEN conditional inverse negatives:** on the frozen slope graph, neither conversion-owner replacement at the five actual sprite-declaration positions (10 cases) nor one added byte snapshot there (5 cases) yields all five DOS homes. On the observed new pixel-X graph, seven declaration positions retain Y-before-X. These are calculations on JSON under the stated record assumptions, not C permutation compiles or impossibility proofs.

**OPEN:** original record graphs and combined colour/home/scheduling constraints. The table gives no calibrated way to suppress the late Score TEST substitution, change a call-live eligibility set, or select an original fill/helper implementation. Prior negatives and helper gates remain in force.

## Watcom 10.6 Score pass: register-equivalence substitution (PROVEN from the binary, build/workers/cg_score)
- TryRegOp (RVA 7375a) replaces an operand by the FIRST accepted older member of its register-equivalence ring (preferred
  next-live search, then an unconditional-liveness fallback); ChangeIns (73515) checks operand size, a valid generation
  entry and CC compatibility. Replayed 1,482/1,482 captured decisions. There is no per-expression/no-score flag.
- Ordinary copies (ScoreMove/RegAdd/RegInsert) create the rings; a physical (also partial) redefinition of the source
  register severs the equivalence; rings propagate through single-input blocks and reset at multi-input joins.
- B5: the four DOS `TEST EAX` sites survive only if EAX's ring has no older member at the predicate (a join or a source
  redefinition before it). C edits that create that state flip the TEST but change other instructions (stoneman 1 ->
  470 diffs, luciole 2 -> 87) - the original differs in more than this decision. OPEN which original IR fact held.

## Byte-register rover and case source order (PROVEN 2026-10-22, fee DO_FEE_ETAPE)
- LdStAlloc's byte rover list is AL, AH, DL, DH, BL, BH, CL, CH (Watcom hw bits AL=2, AH=1, DL=0x80, DH=0x40, BL=8,
  BH=4, CL=0x20, CH=0x10); it advances one entry per request BEFORE testing and skips excluded registers. Requests are
  made in IR block order (source order), including requests whose instruction LdStCompress later folds back into a
  memory operand - those still consume a rover step.
- Consequence: one extra/missing early request shifts EVERY later byte register by one list position (DL<->DH,
  BL<->BH, AL<->AH). DO_FEE_ETAPE: the original has `case 0:` as a separate FIRST case with the same body as `default:`
  (Watcom merges the identical blocks, so the final code and jump table look like one shared block at the end); its
  store is the first request. Moving it closed DO_FEE_ETAPE and, through its jump table, wait_for_dialogue_fee.
- Tool: build/workers/inv_alloc/debug_compile.py now records the IR instruction served by each rover request
  (`instruction` field), so a register mismatch can be traced to the exact request order.
- Profile difference in the home sort (hm_inverse, PROVEN): under the HMI profile (-od, unoptimised) unused scalar locals
  KEEP stack homes and take part in the unstable home sort, unlike the game profile (-oaxt), where they create no backend
  record (h6_locals). Local declaration order derived from the traced sort closed hmi_system sosDIGIUnInitDriver and
  sosFreeVDSPage; the sample functions' homes are recovered by the same inverse but pointer-lowering differences remain.

## Compare with zero: TEST is selected, CMP reg,0 is never generated (PROVEN from the 10.6 binary, build/workers/cg_cmp)
- Comparison opcodes (all widths) select generation tables whose register-vs-constant entry with verify V_OP2ZERO
  (constant kind ABSOLUTE, fits I16, value 0) comes BEFORE the immediate CMP rows and emits G_TEST (TEST r,r). This is
  instruction selection during expansion (not a peephole); no flag-user or block-context condition exists. Hence
  Watcom 10.6 C never emits `CMP reg,0` for a register compared with an absolute zero.
- Audit of all 296 game NEAR: 150 DOS `CMP reg,0` sites - 40 inside the sgn ORIGINAL_INLINE helper, 104 in Watcom's
  strcpy intrinsic loop, 6 in protection/interrupt sequences next to DR/INT instructions (INIT_OPTIONS_CHOICE x4,
  FIN_MAP_LOOP x1, LOAD_CONTINUE_SCREEN x1). Those 6 are STRONG evidence of whole-sequence original inline assembly
  (also opaque control flow: a near JE over 14 bytes, an explicit success JMP). Not covered by the current pragma rule
  (only DR/INT themselves may be inline) - user decision pending.

## Double constants: the 10.x front end snaps to 16 significant digits (PROVEN 2026-10-23, build/workers/fpu_cw)
- Every double constant emitted by stock Watcom 10.0/10.0a/10.5/10.6 (wcc386 and wpp386, native NT loader and the
  DOS4GW/W32RUN-hosted BINW compiler under DOSBox-X) equals the correctly rounded value of its decimal rounded to 16
  significant digits: `0.10526315789473684` -> 3fbaf286bca1af25 (= 0.1052631578947368), `...685`..`...694` -> ...af2d,
  `...695`.. -> ...af34. Folded expressions (`1.0/9.5`, `2.0/19.0`, `1/(9.0+0.5)`, `L`/`f` suffixes, casts; 26 forms in
  initialisers and code) land on the same grid. Independent of the x87 control word (0x27F vs 0x37F forced in the
  compiler process: identical output). Watcom 11.0 and OW 1.4 convert exactly (...af28).
- DOS anomaly: box CalcTab (1:13f70, also v1.12 1:13b55) multiplies by 3fbaf286bca1af28 = the correctly rounded
  1/9.5, which is OFF this grid - no 10.x literal or fold can produce it. Every other double constant in DOS game code
  (6 sites: init_calcbloc_func, collision_rayman, DoneMusic, SpriteWorldBlocksFree) is on the grid. box compiled by
  11.0 matches 0/14 functions (10.6: 9/14), so the object's code generator is 10.x. OPEN: the original front end
  (a patched 10.x with exact conversion?) or a non-literal origin. CalcTab stays NEAR (1 byte); never patch the
  constant by other means.
- Also on the grid (2026-10-23): Watcom C 9.5a and 9.5b (ev_toolchain vendor-patch reconstruction, DOS host under
  DOSBox-X). No patched 10.x C compiler binary was found (ev_toolchain: 10.0b = FDIV update, 10.5a C used by POV-Ray
  but not obtained, "10.6a" = linker update, QNX 10.6 A/B = separate product stream).

## Watcom 10.6 instruction scheduler (PROVEN from the binary, build/workers/sched, 2026-10-23)
- Runs in PostOptimize after register allocation, **inside each basic block** (blocks of <=2 IR instructions are
  skipped); it never moves code across a join or branch. Model build/workers/sched/model.py replays 118/118 block DAGs
  and 867/867 selections (11 real functions in 9 units + 12 synthetic alias probes + 3 trials).
- Dependences: RAW/WAR/WAW by name identity, not addresses or C pointer types (no TBAA). Different direct globals never
  alias, however adjacent; same global aliases iff byte ranges overlap; a store through a free pointer aliases a global
  read only if that global is address-taken (USE_ADDRESS) - otherwise -oa separates them; identical-base indexed pairs
  are separated if the index is not redefined in between. HiddenDependancy (6d1e5) also links instructions that share
  a register READ (result + operand 0 of two-operand forms; full register family, so AL/AH serialize) - validated by 23
  pair edges in 7 functions.
- Selection builds the block bottom-up: lowest StallCost against the built suffix, then highest DAG height, then the
  index-adjust preference, then stallability (+3 per indexed operand, +2 register, +1 direct memory, +3 indexed result),
  then the greatest backend instruction ID. Pentium FU table at 782e3 (CPU >= 5). The generation-table row, not the
  emitted bytes, decides FU class, so identical machine instructions need not mean identical scheduler input.
- Consequence: source statement order is not a general scheduler control. Of 306 NEAR functions, 7 differ only in
  order (allocate_splash, AllocateBonus, InitPcSoundCard, AFFICHE_KEY_SCREEN, SaveScreen,
  dos_allocate_conventional_block, effect_helper_50738). 2 are ILLEGAL schedules of our DAG (dosmem: alias/name +
  HiddenDependancy edge - an address-taken global's load cannot sink below a pointer store; effect_helper_50738:
  HiddenDependancy), so the IR differs before scheduling. 5 are LEGAL orders our build ranks differently (stall cost /
  DAG height decide), so the priority inputs differ (generation rows, edges, instruction IDs). `tools/explain.py` now
  reports this per reorder window (`schedule` mechanism, tools/scheduler.py, t_sched).
  y_floor and CalculVitesseJumelle are not scheduler cases (copy missing / placed in another block). 0 closures.

# Part 2: unoptimised stack-convention code (imported from kegg_recon)

Source: `kegg_recon@40d1a4d:docs/compiler-notes.md`, the user's Watcom 10.0 reconstruction built with
`-3s -d2 -s` (stack calling convention, no optimisation). CyberStorm's `plain` / `plain-oe` profiles are the same
regime (`-4s -s`, no `-o` letters; no `-d2` here, so the `-d2` push forms do not apply). These sections describe how
statements, conditions, loops and switches are laid out there: use them when a plain-profile function is class SHAPE
(`tools/parked.py`). Read as calibrated experience; verify on this target before relying on a row.

## Frame layout rule (lifter agent, fitted to 16 probes, reproduces 297/309 matched functions)
- Watcom shell-sorts the list [parameters, autos in declaration order, return temp] by size (gap sequence n//2,
  then (g+1)//2) and assigns slots in the sorted order. So parameter types and the return type move the locals:
  wrong slot order usually means a wrong prototype, a wrong narrow/wide local or return type, or a missing
  nested-block auto. `tools/lift.py` inverts this rule; use it to get a consistent declaration set.

## Operand evaluation order and expression temps (worker `hard`, probes `build/workers/hard/p/t3..t10.c`, `dis.py`)
- Binary operators (`+`, `<`, ...) evaluate the *deeper* operand tree first; on a tie the left one goes first.
  No-op conversion nodes count as a level: int<->unsigned (explicit cast or usual arithmetic conversion),
  short->int (`movsx`), integer->pointer cast; pointer->pointer casts and constant struct/array offsets
  folded into the address do not always (2-D row offset does). Writing the operands in the other order does
  not help (commutative ops are not swapped). So register order tells you where the hidden conversions are:
  - `g_e326[a1] + g_e336[a1]` -> left index in EAX first; the original's right-index-in-EDX-first is
    `int g_e326[3][4]; g_e326[0][a1] + g_e326[1][a1] + g_e326[2][a1]` (f_a810 EXACT; `g_e336[(unsigned)a1]`
    also works) (`t3.c`).
  - `p->e + *p->f` (int, int*) -> right chain first (`mov eax,[p]; mov eax,[eax+f]; mov edx,[p]; mov edx,[edx+e]`);
    with `unsigned *f` (int+unsigned converts the left) or `(int)unsigned_e + ...` the left base is loaded first
    (`mov edx,[p]; mov eax,[p]; mov eax,[eax+f]; mov edx,[edx+e]; add edx,[eax]`). `*(unsigned *)int_field`
    does not (int->ptr cast deepens the right again); `*(unsigned *)charptr_field` does (`t6.c`-`t8.c`).
    f_7bf4 EXACT with `struct Frame { unsigned img; int time; } *f;` and `g_8db4 = p->e + p->f->img;`.
  - `if ((xs = bx + b.ox) + b.w < (ax = x + a.ox))`: the left (assignment + movsx) is deeper -> computed first
    into EDX, compare `cmp edx,[ax]; jl` (f_b6e4). With `... + mx` (int) on the right side the original's
    `cmp eax,edx; jg` comes from writing `(ax = x + a.ox) + mx > (xs = bx + b.ox) + b.w` (f_b76c) (`t10.c`).
- Assignment inside a condition keeps the `mov [slot],r; cmp r2,[slot]` reload (as `asgcond.c`).
- A call result copied *before* the caller's `add esp,N` (`call f; mov ebx,eax` or `mov [ebp-8],eax; add esp,8`)
  is a temporary inside a larger expression; a named-local assignment statement stores *after* `add esp`.
  FPU conversion temps get ordinary 4-byte stack slots and are reused: `(float)(int expr)` spilled across a
  later call is `mov [t],ebx; fild [t]; fstp [t]`, and `(int)(f() * g * fl)` gives `mov [t],eax; fild [t];
  ...; call __CHP; fistp [t]`. So a frame smaller than the declared locals + a temp means "no locals, one big
  expression": f_b03c EXACT as `g_e1f8 = f_dd53(..) + ((f_13324(..) + f_13324(..)) >> 1) +
  (int)(f_dd53(-g_740c >> 1, g_740c >> 1) * g_e1fc * (float)(abs(x1-x2) + abs(y1-y2))) / (g_740c >> 1);`
  (heaviest operand -- the division -- evaluated first, partial sums in EBX/ESI).
- Struct-by-value params: f_b6e4/f_b76c take `(Frame a, int x, int y, Frame b, int x, int y, ...)` with an
  18-byte packed `Frame` of 9 shorts (arguments are dword-padded: 20 bytes on the stack). Their return type is
  `short` (no `int` return + local order reproduces the frame); the matched caller f_7551 compares `cmp eax,-1`,
  i.e. its TU saw an `int` (probably implicit) declaration.
- `and eax,1; and eax,1; mov edx,eax; and byte ptr [p],~m; shl edx,k; or dword ptr [p],edx` = a 1-bit bitfield
  store of an already-masked value: `((Flags *)g_de5c)->b2 = (x >> 3) & 1;` (f_4cf2 EXACT; manual
  `(*p & ~4) | (v << 2)` gives load/or/store).


## Control-flow layouts under `-3s -d2 -s` (worker `struct`; probes in tests/probes/struct/)

Probes: `tests/probes/struct/*.c`, compiled with the pinned wcc386 (`tools/dosrun.py`) and printed by
`tests/probes/struct/pr.py FILE.c` (Open Watcom 2.0 wdis used only to read the object). Labels `L$n` below are wdis labels.
Everything marked PROVEN was seen in a probe; the tool `tools/structure.py` encodes exactly these rules and every
rewrite it keeps was additionally gated by `tools/check.py` (whole unit EXACT) and the sandbox image link.

## 1. Code generation is template + three clean-ups (PROVEN)

-d2 does no jump threading, no tail merging, no loop rotation. Every statement is emitted from a fixed template,
then only these clean-ups run (to a fixed point):

| clean-up | probe | example |
|---|---|---|
| unreachable code after an unconditional `jmp` (or a switch dispatch) is dropped, up to the next *referenced* label | `peep.c` q3/q5/q6, `sw2.c` w1 | `goto X; g(); X:` -> no `call g`; `if (a) { f(); return; } else g();` -> no `jmp Lend` after `f` |
| a jump to the next instruction is dropped; a dropped `jcc` leaves its `cmp` | `peep.c` q1/q2, `ret.c` r14 | `if (a == 1) {}` -> bare `cmp a,1` |
| `jcc L1; jmp L2; L1:` -> `j!cc L2` when no *referenced* label sits on the `jmp` (unreferenced labels do not block) | `peep.c` q8, `pp2.c` v1-v4 | `if (c) goto L;` is `FJ(c,M) jmp L M:` -> one `jcc L`; `for (..) { f(); if (a == 1) break; }` ends in `jne Lstep` (no back `jmp`) |

Consequences: `if (c) goto/break/continue/return;` is a single jcc; a jump-to-jump chain (`L1: jmp L2`) is the end
of a nested `if/else` (or an `&&` trampoline), never an optimisation artefact; unreferenced labels are free.

## 2. Conditions (PROVEN, `cond.c`, `loops.c`)

FJ(c,F) = code that jumps to F when c is false and falls through when true; TJ(c,T) the converse; L1/L2 fresh:

    atom:  FJ(a,F) = cmp; j!cc F          TJ(a,T) = cmp; jcc T        (`if (x)` = `cmp x,0`)
    FJ(a && b, F) = FJ(a,L1) TJ(b,L2) L1: jmp F L2:        <- the `&&` trampoline `L1: jmp F`
    FJ(a || b, F) = TJ(a,L1) FJ(b,F) L1:
    TJ(a && b, T) = FJ(a,L1) TJ(b,T) L1:
    TJ(a || b, T) = TJ(a,L1) FJ(b,L2) L1: jmp T L2:        <- the `||` trampoline
    FJ(!x, F) = TJ(x, F)                  TJ(!x, T) = FJ(x, T)

Grouping is visible: `a&&b&&c` = `(a&&b)&&c` (two trampolines), `a||(b&&c)` differs from `(a||b)&&c`, all 12
shapes in `cond.c` match. `if (A) { if (B) S }` (two direct j!cc to the same end) is NOT `if (A && B) S`.
`!(a && b)` and `!a || !b` give identical code (De Morgan). `x != 0` / `x == 0` and `x` / `!x` give identical
code for int globals, struct bytes, params, pointers and call results (`cz.c`).

## 3. Statements (PROVEN, `loops.c`, `ret.c`, `lab.c`, `peep.c`)

    if (c) S                 FJ(c,E) S E:
    if (c) S1 else S2        FJ(c,Le) S1 jmp E Le: S2 E:          (jmp E dropped if S1 ends in a jump)
    else-if chain            inner end label == outer end label: every branch jumps straight to the common end
    nested if/else           inner `jmp Ein` where Ein: `jmp Eout`  (the lifter's `L_x:; goto L_y;` chains)
    while (c) S              T: FJ(c,E) S C: jmp T E:             continue -> C (the back jmp), break -> E
    while (1) S              T: S C: jmp T E:                     continue -> C
    do S while (c);          T: S C: TJ(c,T) E:                   continue -> C (the test), break -> E
    for (I; c; N) S          I T: TJ(c,B) jmp E N': N jmp T B: S jmp N' E:     continue -> N', break -> E
    for (I; ; N) S           I jmp B N': N B: S jmp N' E:          (for (;;): `jmp B` is a jump-to-next, dropped;
                                                                    continue -> the loop top, unlike while (1))
    return e; / return;      mov [ret],e; jmp R / jmp R            (R = epilogue label; last statement: no jmp)
    infinite loop at end     epilogue is unreachable and dropped   (`lab.c` u6/u8: no `ret`)

The `for` test is at the top with a jump over the step (no rotation). A trailing `if (x) break;` in a loop body
merges with the back jump (`jcc E; jmp T; E:` -> `j!cc T`).

## 4. switch (PROVEN `sw.c`, `sw2.c`, `sw3.c`, `sw4.c`; threshold HYPOTHESIS)

- The value is stored to a temp slot (`mov [ebp-x],eax`, byte temp for `unsigned char`), then dispatched. The
  temp behaves like a block-scope auto; the lifter shows it as a local (`v_10`, `format`, `shot_kind_byte`).
- Compare tree: case values merged into ranges of consecutive values with the same target; `node(ranges)` splits
  at index `(n-1)//2` = range [lo,hi]: `cmp lo; jb Llow; cmp hi; jbe T; node(upper); Llow: node(lower)`; the `jb`
  is omitted when lo is already known; a one-value leaf is `cmp v; je T; jmp D`; an empty side is `jmp D`
  (D = default, or the switch end). Unsigned `jb/jbe`, signed `jl/jle` when a case value is negative.
  Reproduces `sw3.c` x1 (8 values, 7 ranges), x2, x3 (byte temp), x4, x6, x7 exactly.
- Jump table (`jmp L; nop-pad; table; L: sub base; cmp n; ja D; jmp cs:[eax*4+T]`): seen for 4 cases spanning
  5..7 values (`sw4.c` y2/y3, `sw.c` s2); trees for 3 cases, for 4 cases spanning 13 and 7 spanning 20.
  HYPOTHESIS: table when cases >= 4 and span <= ~2*cases.
- Code between `switch (e) {` and the first `case` is unreachable and dropped (`sw2.c` w1); `default: break;` ==
  no default; case bodies follow in source order, `break` = `jmp E`.
- A lifted `v = e;` + compare tree on v is exactly `switch (e)` (tool rule; gate-verified on U04CF2, U0608A,
  U0B804: the temp local disappears, frame unchanged).

## 5. Block-scope autos (PROVEN by gate, U04CF2 update_racket_state)

Hoisting a nested-block declaration (`{ unsigned char v_10; ...}`) to the function block moves the frame slots
(check.py: `[ebp-4]` -> `[ebp-8]` for the other autos). Block-scope autos are allocated after the function-level
ones, so the tool keeps such declarations in a `{ }` block around the statements that use them.

## 6. What still needs a goto (tool output, 51 left in 8 units + T06/T08 untouched)

- jumps into the middle of another branch or a loop from outside (shared tails): U08585 update_game_balls wall
  bounce (`if (x <= 16) { if (vx >= 0) goto L_880f; ...} else { L_880f: ...}`), U0608A `goto L_6975` into the
  else branch, U05966 `goto L_5d63/L_5d86`, U0B1DF `goto newline_control` (into a switch case);
- multi-level exits / re-entry: U00708 `goto L_9e0` out of two nested do-whiles, U00010 run_gameplay_session
  (`goto L_16d` / `goto L_d6` back into the middle of the level loop from after it);
- `goto` to a label that is not the loop's continue point (U04CF2 `goto L_5153` before the loop tail).
HYPOTHESIS: no C construct compiled by these rules produces those layouts without a goto (templates only jump to a
construct's own entry/exit/continue positions), so these are most likely real gotos in the 1994 source.

