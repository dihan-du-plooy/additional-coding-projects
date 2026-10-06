---
project: "05 - relay board"
scope: "Lesson 2.3 — bitmasks, with 2.2 loops, 2.4 switch and 2.8–2.9 structures"
level: 2
features: [unsigned, struct, "array of structs", "structure initializer", "for loop", switch, case, default, break, continue, "bitwise operators", bitmask, "bit shifting", "logical operators", if, char, cout]
set: 2026-10-05
status: open        # open | submitted | marked
---

# 05 — Relay board

## Goal

An EMS output card has 8 relays, one per bit of an `unsigned int`. Your program starts from a
known relay state, runs a fixed script of 8 commands against it — set a relay, clear a relay,
toggle a relay — and prints the state as 8 binary digits after every step. No menu, no `cin`:
everything is hardcoded, so the only thing that can go wrong is the bit work.

Roughly 35–50 lines.

## Starting values

Put these at the top of `main` exactly as written.

```cpp
unsigned int relays = 37;   // 00100101 — relays 0, 2 and 5 on

COMMAND script[8] = {
    {'S', 1}, {'C', 2}, {'C', 2}, {'T', 7},
    {'S', 0}, {'T', 5}, {'C', 9}, {'X', 3}
};
```

…using this structure, declared above `main`:

```cpp
struct COMMAND {
    char op;    // 'S' = set (turn on), 'C' = clear (turn off), 'T' = toggle (flip)
    int bit;    // which relay, 0-7
};
```

## Requirements

1. Print the starting state first: the word `Start`, padded so the `->` lines up with the
   step lines, then the 8 bits, then the decimal value in brackets (see the sample run).
2. Then, with a **`for` loop** over `script`, for each command print
   `Step <n>  <op> <bit>  -> ` (steps numbered from 1) and then:
   - If `bit` is outside 0–7, print `bad bit, skipped` and go to the next command — use
     **one** `if` condition and **`continue`**. Check this **before** looking at `op`.
   - Otherwise pick the operation with a **`switch`** on `op`:
     - `S` turns that relay **on**. If it's already on, nothing changes.
     - `C` turns that relay **off**. If it's already off, nothing changes. Every other relay
       must stay exactly as it was.
     - `T` flips that relay.
     - Anything else prints `unknown command, skipped` and goes to the next command —
       use **`continue`** from inside the `switch`.
   - After a successful command, print the 8 bits and the decimal value in brackets.
3. **Printing the 8 bits:** a loop from bit 7 down to bit 0, printing `1` or `0` for each.
   Test each bit with `&` and a mask built with `<<`. No string tricks, no division by 2.
4. Build masks with `1 << bit` — don't hardcode `1, 2, 4, 8 …` anywhere.
5. Finally, count the relays that are on **with a loop and a mask** and print
   `Relays on: <n>`.

## Sample run

```
Start        -> 00100101  (37)
Step 1  S 1  -> 00100111  (39)
Step 2  C 2  -> 00100011  (35)
Step 3  C 2  -> 00100011  (35)
Step 4  T 7  -> 10100011  (163)
Step 5  S 0  -> 10100011  (163)
Step 6  T 5  -> 10000011  (131)
Step 7  C 9  -> bad bit, skipped
Step 8  X 3  -> unknown command, skipped
Relays on: 3
```

Spacing is part of the spec: two spaces after the step number, two before `->`, two before
the `(`. Put your output next to this and check it character by character before you say done.

## Constraints

- Only use: `#include <iostream>`, `using namespace std;`, `unsigned`, `int`, `char`, `struct`,
  arrays and their initializers, `for`, `if` / `else`, `switch` / `case` / `default`, `break`,
  `continue`, `cout` / `<<` / `endl`, and the bitwise (`& | ^ ~ << >>`), logical and comparison
  operators with their shortcut forms (`&=`, `|=`, `^=`).
- Do **not** use: `cin`, functions of your own, `pow` or anything from `<cmath>`, `string`
  tricks for the binary digits, hex literals, `setw` or anything else not yet in your notes.
- Single file, `main.cpp`.

## What this is testing

- Whether you pick the right one of the four bitmask operations from 2.3 for each command —
  steps 3 and 5 exist to catch the wrong choice.
- Whether your bit tests are bracketed correctly — look at where `&` and the comparison
  operators sit in the 2.3 priority table before you write `==` or `!=` next to a `&`.
- What `continue` does when it's inside a `switch` inside a loop — compare it with what `break`
  does in the same spot.

## Stuck?

Ask for a hint — you'll get a nudge, not the answer. Submitting something broken with a note
about where you got stuck is a perfectly good outcome; a blank file isn't.

Useful reread: [[2.3 Algebra and Computer Logic]] — the bitmask table (check / reset / set /
negate) and the updated priority table.
