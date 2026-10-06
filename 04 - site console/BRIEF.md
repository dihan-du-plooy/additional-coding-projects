---
project: "04 - site console"
scope: "Module 2 capstone — lessons 2.0–2.9, plus all of Module 1"
level: 3
features: [struct, "array field in a struct", "array of structs", "structure initializer", "zero extension", "2D array", "array initializer", "do loop", "for loop", "nested loop", switch, case, default, break, continue, "else-if", "logical operators", "bitwise operators", bitmask, "bit shifting", unsigned, double, bool, char, cin, hex, dec, string, cout]
set: 2026-09-24
status: marked      # open | submitted | marked
---

# 04 — Site console

## Goal

A menu-driven console for a small three-inverter PV site. The user picks options from a menu
over and over until they quit: see a site report, decode an inverter's fault bits, clear a
fault, and work out what the day's energy was worth on a time-of-use tariff.

This is a **capstone**. It touches almost every lesson in Module 2 and is bigger than the
earlier projects (roughly 110–150 lines). Build it in the order of the parts below and get each
part compiling and correct before starting the next.

---

## Part A — the data

### The structure

Declare this exactly as written. The field order matters.

```cpp
struct INVERTER {
    string name;
    double kwh[6];          // energy per 2-hour slot, 06:00-18:00
    unsigned int status;    // fault bits, see the table below
};
```

### The three inverters

Declare `INVERTER site[3]` and fill it with **one initializer list** at the point of
declaration. No `site[0].kwh[2] = ...;` assignments afterwards.

| Inverter | 06–08 | 08–10 | 10–12 | 12–14 | 14–16 | 16–18 | status |
| -------- | ----- | ----- | ----- | ----- | ----- | ----- | ------ |
| INV1     | 4.5   | 11.0  | 16.5  | 17.0  | 12.0  | 5.0   | 0      |
| INV2     | 4.0   | 10.5  | 15.0  | 9.5   | 11.5  | 4.5   | 2      |
| INV3     | 3.5   | 9.0   | 14.0  | 13.5  | *(no data)* | *(no data)* | 12 |

> INV3 lost comms at 14:00, so its last two slots were never recorded. They must end up as
> `0` — but **do not type the zeros yourself**. Let the initializer produce them.

### The fault bits

`status` is a bit field. Each bit is one fault:

| Bit | Weight | Fault            |
| --- | ------ | ---------------- |
| 0   | 1      | Grid fault       |
| 1   | 2      | Over-temperature |
| 2   | 4      | Comms lost       |
| 3   | 8      | Isolation fault  |

So INV2's `2` means over-temperature, and INV3's `12` means comms lost **and** isolation fault.

### The tariff

Two more arrays, both filled with initializer lists:

```cpp
int period[6] = { 2, 0, 1, 1, 1, 0 };   // TOU period of each slot: 0 = peak, 1 = standard, 2 = off-peak
```

and a **two-dimensional** `double tariff[2][3]` holding rand per kWh — row is the season,
column is the TOU period:

| Season (row)          | Peak (0) | Standard (1) | Off-peak (2) |
| --------------------- | -------- | ------------ | ------------ |
| 0 — High-demand       | 5.00     | 1.50         | 0.80         |
| 1 — Low-demand        | 2.00     | 1.25         | 0.75         |

**Checkpoint A:** before writing the menu, temporarily print INV3's six `kwh` values and its
`status`. You should see `3.5 9 14 13.5 0 0` and `12`. If you don't, fix the initializer now —
nothing after this will be right until it is. Delete the test print when it passes.

---

## Part B — the menu loop

1. Print this menu (a blank line first), then read **one `char`** with `cin`:

   ```

   === Site console ===
   R  Site report
   F  Fault detail
   C  Clear a fault
   V  Energy value
   Q  Quit
   Choice: 
   ```

2. Handle the choice with a **`switch`**. Upper- and lowercase letters must both work
   (`R` and `r` do the same thing) — without duplicating the code for each.
3. Any other character prints `Unknown option` and the menu shows again.
4. `Q` / `q` prints `Bye` and the program ends. Nothing else ends it.
5. The menu must repeat using a **loop**, and the menu must be shown **at least once** —
   choose the loop kind that guarantees that.

**Checkpoint B:** with every option just printing its own name, check that `R`, `r`, `x` and
`q` all behave correctly, and that `q` actually stops the program.

---

## Part C — `R`: site report

For each inverter, one line:

```
<name>  <total kWh> kWh  status 0x<status in hex>  <OK or FAULT>
```

- The total is the sum of that inverter's six slots, calculated with a loop.
- Print `status` in **hexadecimal** using the manipulator from 1.6. Print the `0x` yourself as
  text.
- `OK` if no fault bits are set at all, `FAULT` otherwise.

Then two summary lines:

```
Site total: <sum of all three totals> kWh
Peak slot: HH:00-HH:00 (<kWh> kWh)
```

- The **peak slot** is the 2-hour slot where the **three inverters together** produced the most.
  Slot 0 is `06:00-08:00`, slot 1 is `08:00-10:00`, and so on. Work the hours out from the slot
  number — don't store them as text.
- Hours are **two digits** (`06`, not `6`).

---

## Part D — `F`: fault detail

1. Prompt `Inverter (1-3): ` and read a number. The user types 1–3; your array is indexed 0–2.
2. Anything outside 1–3 prints `No such inverter` and goes back to the menu.
3. Otherwise print `<name> faults:` then, for each bit that is set, one line
   `  bit <n>  <fault name>` (two spaces before `bit`, two after the number), lowest bit first.
4. Check the bits **with a loop and a bitmask** — not four separate hard-coded `if`s.
   Pick the fault name with a `switch`.
5. If no bits are set, print `  No active faults` instead.

---

## Part E — `C`: clear a fault

1. Prompt `Inverter (1-3): ` then `Bit (0-3): ` and read both.
2. If **either** is out of range, print `Invalid input` — use **one** `if` condition for this.
3. If that bit is **not** currently set, print
   `Bit <n> is not set on <name> - nothing to clear` and change nothing.
4. Otherwise clear **only that bit** — every other bit must stay exactly as it was — and print
   `Cleared bit <n> on <name>. Status now 0x<status in hex>`.

---

## Part F — `V`: energy value

1. Prompt `Month (1-12): ` and read a number.
2. Months 6, 7 and 8 are the **high-demand** season (row 0). Every other valid month is
   **low-demand** (row 1). Anything outside 1–12 prints `Invalid month` and goes back to the menu.
   Pick the season with a `switch`.
3. Print `High-demand season` or `Low-demand season`.
4. For each inverter:
   - If its **Comms lost** bit is set, print `<name>  skipped (comms lost)` and move straight
     on to the next inverter — use `continue`.
   - Otherwise its value is the sum, over the six slots, of that slot's kWh × the tariff for
     this season and that slot's TOU period. Print `<name>  R<value>`.
5. Finally print `Site value: R<sum of the inverters that weren't skipped>`.
6. Print money with plain `cout` — no `fixed`, no `setprecision`. `R78` rather than `R78.00` is
   fine.

---

## Sample run

User input is shown after each prompt. This is one continuous run.

```

=== Site console ===
R  Site report
F  Fault detail
C  Clear a fault
V  Energy value
Q  Quit
Choice: R
INV1  66 kWh  status 0x0  OK
INV2  55 kWh  status 0x2  FAULT
INV3  40 kWh  status 0xc  FAULT
Site total: 161 kWh
Peak slot: 10:00-12:00 (45.5 kWh)

=== Site console ===
...
Choice: F
Inverter (1-3): 3
INV3 faults:
  bit 2  Comms lost
  bit 3  Isolation fault

=== Site console ===
...
Choice: V
Month (1-12): 9
Low-demand season
INV1  R92.25
INV2  R78
INV3  skipped (comms lost)
Site value: R170.25

=== Site console ===
...
Choice: C
Inverter (1-3): 3
Bit (0-3): 2
Cleared bit 2 on INV3. Status now 0x8

=== Site console ===
...
Choice: c
Inverter (1-3): 3
Bit (0-3): 2
Bit 2 is not set on INV3 - nothing to clear

=== Site console ===
...
Choice: C
Inverter (1-3): 7
Bit (0-3): 1
Invalid input

=== Site console ===
...
Choice: v
Month (1-12): 7
High-demand season
INV1  R151.85
INV2  R132.2
INV3  R89.05
Site value: R373.1

=== Site console ===
...
Choice: r
INV1  66 kWh  status 0x0  OK
INV2  55 kWh  status 0x2  FAULT
INV3  40 kWh  status 0x8  FAULT
Site total: 161 kWh
Peak slot: 10:00-12:00 (45.5 kWh)

=== Site console ===
...
Choice: x
Unknown option

=== Site console ===
...
Choice: q
Bye
```

(`...` stands for the rest of the menu, which prints in full every time.)

Notice how the run tells a story: after comms-lost is cleared on INV3, the energy value
**includes** INV3, and its two unrecorded slots contribute nothing.

## Constraints

- Only use: `#include <iostream>` and `<string>`, `using namespace std;`, `int`, `unsigned`,
  `double`, `bool`, `char`, `string`, `struct`, arrays (1D and 2D) and their initializers,
  `cin` / `>>`, `cout` / `<<`, `endl`, `hex` / `dec`, `if` / `else` / `else if`, `switch` /
  `case` / `default`, `while` / `do` / `for`, `break` / `continue`, arithmetic, comparison,
  logical (`&& || !`) and bitwise (`& | ^ ~ << >>`) operators and their shortcut forms.
- Do **not** use: functions of your own (everything lives in `main`), `setw`, `setfill`,
  `fixed`, `setprecision`, `vector`, pointers, `getline`, hex literals like `0xC` (write the
  values in decimal as given), or anything else not yet in your vault notes.
- Single file, `main.cpp`.

## What this is testing

Each part has one thing to be careful about. None of them stop the program compiling — they
all give output that *looks* reasonable and is wrong.

- **A** — the field order in `INVERTER` is not an accident. Look at where `status` sits relative
  to the array, and think about what happens to INV3's `12` if the braces aren't right. This is
  the exact thing you missed on 2026-09-21.
- **B** — what `break` actually breaks out of when it's inside a `switch` that's inside a loop,
  and writing a "keep going unless it's Q or q" condition that is actually right. De Morgan is
  relevant.
- **C** — the peak slot is a sum *down a column* (one slot, all inverters), not across a row.
  And remember what 1.6 said about how long a manipulator stays in effect once you've sent it.
- **D / E** — operator priority. Look at where `&` and `==` sit in the 2.3 priority table
  before you write a bit test. And "clear one bit, leave the rest alone" is a specific one of
  the four bitmask operations — not all of them do that.
- **F** — indexing a 2D array with a value you read *out of another array*.

## Stuck?

Ask for a hint — you'll get a nudge, not the answer. Submitting something that only gets
through Part C, with a note about where you stopped, is a perfectly good outcome and will be
marked properly. A blank file isn't.

Useful rereads: [[2.9 Declaring and Initializing Structures - Fundamentals and Techniques]]
(nested initializers), [[2.3 Algebra and Computer Logic]] (bitmasks and the priority table),
[[2.4 Switch Statements - Another Perspective on Conditional Logic]] (sharing one branch
between cases), [[2.2 Looping constructs - Iterating Through Code Blocks]] (`do` vs `while`,
`break` and `continue`).
