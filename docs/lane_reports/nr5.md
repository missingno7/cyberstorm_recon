# Lane nr5 near-miss round

Two functions pass fresh STRICT dry runs against 1.10A, covering 339 bytes. The full-TU comparisons for the two changed files show exactly two status changes. Every other code extent retained its baseline status, size and differing-byte count. Both dry runs report zero losses.

## Promotion dry runs

```text
python tools/promote.py candidates/nr5/gui/menu.c --tu src/gui/menu.c --as 'gui\menu.c' --profile plain
src/gui/menu.c: 62 strict functions (4934 bytes), new 1, lost 0

python tools/promote.py candidates/nr5/io/file.c --tu src/io/file.c --as 'io\file.c' --profile ot
src/io/file.c: 18 strict functions (1705 bytes), new 1, lost 0
```

## STRICT candidates

- **00427e96 `MenuLabel::Draw` · 243 B · D6 → STRICT.** Earlier work captured the background color to reach D4. The winning expression keeps the chained color assignment and adds `align > 0 ? extra : (extra >> 1)` directly to `left`. This reproduces the three earlier clip-coordinate loads and pushes in EDX, ESI and ECX. Exactly one of the 36 combinations of color capture, text-state store order and alignment join was STRICT.
- **0047e610 `IoClose` · 96 B · D4 → STRICT.** Keep the `active` snapshot for the line-170 assertion and pass literal zero as its message. The captured value is zero on that path. Both predicate spellings reproduced the original current-position save in ESI and end-position save in EDI. Reading the field directly with the same null message gave D3. The seek calls and their arguments are unchanged.

## Open functions

| VA and function | Final state | Prior work and result of this round |
|---|---|---|
| 0042827d `MenuEdit::Event` | 600 B, D3 | Earlier typed character bounds and index-first addresses restored the extent. Crossing the backspace source and destination address spellings retained D3 at best. Capturing the previous index, current cursor or text base and trying all six orders of the post-copy updates reached D17 at best. At +114 the original computes `cursor-1` in EAX and reloads the base into EDI. The candidate uses EDX and EAX. |
| 00426b8c `Flic::DecodePalette` | 185 B, D2 | Prior pointer and counter changes left the last clear open. Coupling count width and zero handling with loop updates retained D2. Staging RGB bytes independently and changing the type of the clear destination also retained D2 at best. Only the final `memset` pointer load and push differ, EBP in the original and ESI here. |
| 004261e5 `Flic::$dt` | 203 B, D5 | Prior reports identified a scheduling difference. Sound-loop and resource capture combinations increased the distance. Factoring the storage prefix into a base class gave D6 in all four variants. The original pushes zero at +a7 before adding 0x20 to EDI for member destruction. The candidate adds first. |
| 00426c72 `Flic::DecodeRegion` | 61 B, D4 | Earlier scalar getters, coordinate captures and a shared frame operation left the register swap. An advancing reader reached D70 at best. Coordinate snapshots crossed with direct or member blits and unpack linkage reached D4. The assembly public is `UnpackFlicPixels`. Both linkage probes retained the same best distance. The original preserves left/top in EDI/EBP across unpack. The candidate preserves them in EBP/EDI. |
| 0046c461 `SoundPlay` | 1083 B, D3 | Earlier format/buffer scope and named attenuation recovered the stack layout. Coupling the existing clamp and pan helpers with attenuation staging retained D3 at best. A free-voice loop with an internal break also retained D3 with the original looping and attenuation locals. The original loads the signed table entry at +390, multiplies it at +398 and pushes it at +3a1 using EBP throughout. The candidate uses EDX. |

The seven baseline listings account for every differing byte locally. None of the seven depends on a neighboring function's call or branch displacement. The retained Flic and noise copies are byte-identical to their source files. Their dry runs reported `new 0, lost 0` each.

[Structured results for all 14 families](attempts.jsonl) · [Baseline listings, variant scores and promotion logs](../../build/nr5/)
