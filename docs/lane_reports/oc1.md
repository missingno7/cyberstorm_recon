# Lane oc1 - last function of an object

Result: 1 of 5 functions STRICT, 1 object COMPLETE (tu_00480390). Two more brought to within a few register
choices (BuildTerrainPath D23, SimFire with every instruction shape matching). Every hypothesis family is
recorded in candidates/oc1/attempts.jsonl (`python tools/notes.py show VA`).

## Changed files (promote dry runs)

### candidates/oc1/_unknown/tu_00480390.c  -> object COMPLETE

```text
python tools/promote.py candidates/oc1/_unknown/tu_00480390.c --tu src/_unknown/tu_00480390.c --as '_unknown\tu_00480390.c' --flags '-4s -s -bm -oxt -oe=200'
src/_unknown/tu_00480390.c: 11 strict functions (1235 bytes), new 1, lost 0
dry run OK (add --publish to accept)
```

```text
python tools/tucheck.py --tu src/_unknown/tu_00480390.c --src candidates/oc1/_unknown/tu_00480390.c
COMPLETE src/_unknown/tu_00480390.c (1235 code bytes)
TU complete: 1 of 185 accepted TUs, 1235 code bytes
```

Change (InitializeWinG 004806a0): the GDI fallback goes in an explicit `else`, and the Win32s branch has no
`return` of its own. There is one final `return wingApi.initialized;`:

```c++
if (!wingApi.initialized) {
    if (IsWin32s()) {
        if (!LoadWinG()) return 0;
        wingApi.initialized = 1;
    } else {
        wingApi.initialized = UseGdiFallback();
    }
}
return wingApi.initialized;
```

### candidates/oc1/sim/simfire.c  (no new STRICT; SimFire D1645 -> D958, every instruction shape now equal)

```text
python tools/promote.py candidates/oc1/sim/simfire.c --tu src/sim/simfire.c --as 'sim\simfire.c' --profile ot-oe
src/sim/simfire.c: 1 strict functions (48 bytes), new 0, lost 0
dry run OK (add --publish to accept)
```

Recommended for acceptance as a better baseline. It contains semantic fixes that the listing proves:
- `PacketList::Fire` and `ApplyDamage` take `target.target.id` (TargetSide +0, the push of [ebp-0xfe]), not
  `target.target.actorId`.
- SimGame+0x9a6 is a real `Context weapons;` member. The chance read is `[slot*0x6e + ebx + 0xa91]`, not a
  cast pointer to an array.
- Hit jitter: `RandomBelow(game->zoom) - game->zoom/2`. RandomBelow is the same inline helper that pherc.c
  uses. With it, half is computed before the call and zoom is reloaded as the divisor.
- Error clamp: the `ClampValue` if/return helper (same as pherc.c). The ternary FireClamp cost an extra
  4-byte local, so the frame was 0x1c4 instead of 0x1c0.

## Diagnostic files (not promotable), in candidates/oc1/wip/

- `wip/tu_004324d0.c`: BuildTerrainPath at 928 B, D23. It would lose GridSetSelection (STRICT today). That
  function jumps into BuildTerrainPath's epilogue, and the jump is 1 byte off until the path function is exact.
- `wip/simglob.c`: SimSetupWorld. It reproduces the original's rectangle store grouping, but more bytes differ
  (D326 against the D318 baseline) because the width and height registers are swapped. Kept for the next lane.

## Per function: final state, and what the original still does differently

- **004806a0 InitializeWinG**: STRICT. The object is complete.
- **004324d0 BuildTerrainPath**: D579 -> D23 (wip). What was found:
  - Frame layout: `Point next,start; int counts[2]; Point destination;` at function scope, and
    `Point current;` inside the while body. This is the only exact frame among 1080 combinations.
  - `next.Set(&current)` is a member-wise copy. A struct copy (movsd) would need ESI/EDI; avoiding it keeps
    the direction pointer in EDI.
  - The visibility test is written in the loop body. The `PathActor(index, excluded)` inline copied its
    parameter into a new stack slot.
  - `if(counts[group]>0){int n=counts[group]; ...}` gives the original's compare against a zero register
    (`cmp edi,ecx`) and the copy `mov esi,ecx` before the call.
  - `ActorHidden(int)` is called as `ActorHidden(a->index)`.

  Still different: (1) the PathRawActor table base is loaded into EDX (original: ESI). (2) The address for the
  final `path[count]=0` is built in EBP (original: EAX), which makes the code 1 byte longer. Neither changed
  under table, pointer, return, declaration-position or declaration-order spellings.
- **0044b9c0 SimSetupWorld**: D318 (baseline kept).
  - Found: setter order portrait, view, full, status, bottom panel, with `width--` after SetFastView. This
    reproduces the original's store grouping (all rights, then lefts and tops, then bottoms) and the `dec`
    before the call.
  - Found: the callees are the named ClipInitialize(ClipState*, const Rect*) and
    DisplaySetRect(void*, const DisplayRect*).
  - Still different: width and height homes are swapped (original ESI/EBX). The constant 147 is in EBP
    instead of EDI. The original reloads g4d381c.bottom for two stores, and at the end loads s into EBP.
    Neither shared-edge reads nor any of the 120 setter orders × width forms move these homes.
- **00452980 SimFire**: D958, frame now 0x1c0.
  - Still different: the order of the stack slots for scalars. The original's ascending order is targetCell,
    angle, other, scatter, hit, seed, weapon.
  - Still different: the GridPoint slots are interleaved with the by-value FxFire temporaries in another
    order.
  - Register homes follow from these slots: hit is tested in ECX, `other` is kept in ESI across
    WorldDrawTerrain, divisors 100 and 5 are in ECX, and the second miss SendEvent pushes `push 0`
    immediates.
  - A hill-climb over the declaration order of 11 locals, 25 GridPoint orders and scopes, and
    angle/scatter scoping barely moves the slots.
- **004275dc PlyMatch**: not reworked (D941). I re-read the listing and recorded what I saw in the ledger.

## Codegen facts these lanes established (verified, decisive A/B)

| Context | Trigger | Effect | Where |
|---|---|---|---|
| ot, flags -oe=200 | `if(c){...; flag=1;} else flag=f();` then one `return flag;`, vs `return flag;` inside the if-branch | The single final return makes Watcom copy the shared `mov eax,[g]; epilogue` block into the branch: you get a store of immediate 1 plus a reload. A return inside the branch gives `mov ecx,1; mov [g],ecx; mov eax,ecx` (D116). Dropping the else also gives D116 | InitializeWinG |
| ot | `p = q` struct copy inside a loop that also walks a pointer | A struct copy (movsd) occupies ESI/EDI, so the walking pointer spills. Member-wise `p.Set(&q)` frees EDI for the pointer, and the loop counter spills instead | BuildTerrainPath |
| ot | inline helper `f(int x, int excluded)` called with a local | The parameter is copied into its own 4-byte stack slot per call (frame +4). Writing the body in place removes it | BuildTerrainPath; SimFire's ternary FireClamp did the same (0x1c4 vs 0x1c0) |
| ot | `int n=a[i]; if(n>0) ... % n` vs `if(a[i]>0){int n=a[i]; ... % n}` | The first gives `mov esi,[a+i]; test esi,esi`. The second gives `xor edi,edi` before the loop, `mov ecx,[a+i]; cmp edi,ecx; jge` and `mov esi,ecx` before the call | BuildTerrainPath |
| ot | aggregate locals (8-byte Points / int[2]) | The slot order depends on scope (function vs loop body) and on the code that uses them, not only on declaration order. Re-run the layout search after every code change | BuildTerrainPath (layout_t3f / t23 / t26 tables) |
| ot-oe | `next_link_seed(&s)%z - z/2` vs `RandomBelow(z) - z/2` (inline helper) | The helper computes z/2 first and keeps it in ESI across the call, and reloads z as the divisor (the original). The plain expression does the call first and reuses one z load | SimFire |

## Tools (lane-local, build/oc1/)

- `ivar.py`: variants runner that counts aligned differing instructions (accepts --flags).
- `slotvar.py` and `permclimb.py`: score candidates by stack-slot sequence; hill-climb over declaration orders.
- `leas.py` and `pathlayout.py`: extract frame offsets of address-taken locals.
- `var.py`: variants.py with --flags.
