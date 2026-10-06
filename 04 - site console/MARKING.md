---
project: "04 - site console"
marked: 2026-10-05
verdict: Not yet    # Pass | Pass with corrections | Not yet
---

# Marking — 04 site console

**Verdict:** Not yet · compiles, with 6 warnings · Parts A, B and F work; Part C's peak slot and Part E are wrong

This is a big program and most of it is genuinely good — the verdict is about two specific bugs,
both of which the brief's *What this is testing* section warned about, and one of which makes
the console do the opposite of what it says (pressing **Clear** can *set* a fault). On a real
site console that's the bug that gets someone sent out to an inverter that's fine. Fix the two
bugs and the output format and this is a pass.

Run against the brief's sample sequence, the output diverges in four places: the peak slot, the
second `c 3 2` (re-sets the bit instead of refusing), then — because of that — `v 7` skips INV3
and the final `r` shows `0xc` instead of `0x8`.

## What worked

- **Part A is exactly right, including the trap.** `{"INV 3", {3.5, 9.0, 14.0, 13.5,}, 12}` —
  you braced the array so `12` lands in `status` and the last two slots zero-extend. That is the
  precise thing you missed on 2026-09-21, now done correctly in real code.
- **The menu loop is correct.** `do … while` so the menu shows at least once, a `bool` flag so
  `break` only has to leave the `switch`, and `case 'r': case 'R':` stacked so both cases share
  one branch. All three of the Part B checks pass.
- **Part F's numbers are right** — `R92.25`, `R78`, `R170.25`, `R151.85`, `R132.2` all match, and
  after a correct clear of bit 2 INV3 comes out at `R89.05` with its missing slots contributing
  nothing. You also used `hex` … `dec` correctly in Part C and put the `dec` back straight away —
  that was a Part C trap and you avoided it.
- Section banners and the per-part comments make a 245-line `main` easy to navigate.

## Corrections

### 1. Clearing a bit with `^` toggles it — it doesn't clear it

```cpp
site[invChoice - 1].status = site[invChoice - 1].status ^ 4;   // yours — flips bit 2
site[invChoice - 1].status &= ~(1 << faultBit);                // reset: AND with the negated mask
```

Why it matters: 2.3 lists four bitmask operations — check (`&`), reset (`& ~mask`), set (`|`),
negate (`^`). `^` flips the bit whichever way it currently is. It *looked* right on the first
`C 3 2` because bit 2 was on, but the second time it turned comms-lost back **on** and printed
"Cleared". The brief said "not all of the four do that" for exactly this reason.

Notice that one line replaces your whole four-case `switch` — the mask comes from `1 << faultBit`,
so there's nothing to switch on.

### 2. `&` binds looser than `==`, so the "not set" check never fires

```cpp
while (site[invChoice - 1].status & (1 << faultBit) == 0)      // yours
//     is read as:  status & ((1 << faultBit) == 0)  →  status & 0  →  always 0  →  never loops
if ((site[invChoice - 1].status & (1 << faultBit)) == 0)       // parentheses round the & first
```

Why it matters: in the 2.3 priority table the comparison operators sit **above** `&`, `^` and `|`.
Any bit test that compares the result of `&` needs the `&` in its own brackets. The compiler
told you about this one — `warning: suggest parentheses around comparison in operand of '&'`.
Correction 1 and this one together are why `Bit 2 is not set on INV3` never appears.

You got it right in Part F — `(site[i].status & 4) == 4` — so this is a slip under pressure
rather than not knowing; but it's the kind that has to become automatic.

### 3. The peak slot is one of six 2-hour slots, not three pairs

```cpp
if (j == 2) hrSlot2 += site[i].kwh[j] + site[i].kwh[j+1];   // yours — adds 10–12 AND 12–14
```

Each `kwh[j]` is already a 2-hour slot (the struct comment says so), so there are six slots, and
the peak is the biggest **column** sum: slot `j` added across all three inverters.
Your version produced `08:00-10:00 85.5` — the label is wrong *and* the number is two slots
added together. The answer is `10:00-12:00 (45.5 kWh)`.

The shape the brief wanted: one loop over `j` (slots), an inner loop over `i` (inverters)
building that slot's total, and "is this bigger than the best so far?" — then the hours come
from the slot number: start hour = `6 + 2 * j`. No `hrSlot1/2/3` variables at all, which also
means it would still work with 12 slots.

Separately, `else if (hrSlot2 > hrSlot1 || hrSlot3)` is always true — `|| hrSlot3` asks "is
`hrSlot3` non-zero?", not "is `hrSlot2` bigger than `hrSlot3`?". Each side of `&&` / `||` needs
to be a full comparison.

### 4. Bad input should go back to the menu, not trap the user in a re-prompt loop

Parts D, E and F all say "prints `No such inverter` / `Invalid input` / `Invalid month` and goes
back to the menu". Yours loops until a valid value is typed. That's a defensible design in real
life, but it isn't what the spec asked for, and it pulled in `cin.ignore` and
`numeric_limits<streamsize>` from `<limits>`, which aren't in your notes yet. For Part E the
brief also asked for **one** `if` that checks both numbers — your bit number is never range
checked at all, so `C 3 7` silently does nothing.

### 5. The output doesn't match the spec

Compare yours to the sample, line for line:

| Brief | Yours |
| --- | --- |
| `=== Site console ===` / `R  Site report` | `=== Site Console ===` / `R Site Report`, plus an extra blank line before `Choice:` |
| `INV1  66 kWh  status 0x0  OK` | `INV 1 66 kWh status: 0x0 OK.` |
| `Inverter (1-3): ` | `Which inverter would you like to check: ` |
| `  bit 2  Comms lost` | `bit 2 Coms lost.` (and a stray `12` after `faults:`) |
| `Site value: R170.25` | `Site value: R170.25 for month 9 (Low Season)` |
| `Unknown option` | `Unknown Option. Pick again: ` |

None of these are hard — but a spec's output format is a contract. Anything that reads this
program's output (a script, a log parser, a marking script) breaks on the first difference.
Read the sample run with your output next to it before you call a project done.

## Suggestions

- **Part F: index the 2D array with the value you read out of `period`.** You wrote the same
  20-line block twice (one per season) with a `switch` on `period[j]` inside each. The point of
  the 2D array is that this is one line:
  `value += site[i].kwh[j] * tariff[season][period[j]];` — use the month `switch` only to set
  `int season = 0;` or `1;`, then do one loop. The brief also asked for `continue` on comms-lost;
  that removes the `else` and a level of indentation.
- **Part D:** you reached for `pow(2, i)` from `<cmath>` to build the mask, but you already used
  the right tool in Part E: `1 << i`. It's exact, it's an integer, and it's what the bitmask
  notes use. `pow` works with `double`s and needed three `(int)` casts.
- **Totals in Part C** — the brief asked for a loop; the long `kwh[0] + … + kwh[5]` line gives
  the right answer but you then wrote the loop anyway two blocks later. Do the per-inverter loop
  once and add each inverter's total into `siteTotal` as you go.

## Stretch

Once it passes: add an `S` option that **sets** a fault bit (inverter and bit, same validation
as `C`). With reset done properly, set is one operator away — and having both side by side is
the best way to stop mixing them up.

> [!note] Beyond the lesson
> - **Compiler warnings.** Five of the six warnings are `comparison of integer expressions of
>   different signedness` — `sizeof(...)/sizeof(...)` gives an *unsigned* number and `i` is a
>   signed `int`. Harmless here, but the sixth warning was a real bug (correction 2). If you
>   build with `-Wall -Wextra` (the `.vscode/tasks.json` in new project folders does), read the
>   warnings every time — they're the compiler marking your work before I do.
> - **`sizeof` for counting array elements** works, and it's good that you tried it. Later in
>   the course you'll meet ways to avoid it; for now, a plain `3` and `6` matched the brief and
>   avoids the warnings.
