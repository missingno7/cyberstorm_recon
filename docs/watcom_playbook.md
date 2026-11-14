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

