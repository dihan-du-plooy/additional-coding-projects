---
project: "02 - inverter reading log"
scope: "Lessons 2.8–2.9 — Structures, declaring and initializing"
level: 2
features: [struct, "nested struct", "array of structs", "structure initializer", "zero extension", "selection operator", "for loop", if, bool, double, string, cout]
set: 2026-09-21
status: open
---

# 02 — Inverter reading log

## Goal

Hold six inverter energy readings in one array of structures, then print each reading and a
small summary: total energy, the peak reading, and how many readings were flagged as faults.

## The structures

Declare these exactly as written — field names and order matter, because the marking checks
your initializers against them.

```cpp
struct TIME {
    int hour, minute;
};

struct READING {
    std::string inverter;
    TIME taken;
    double kwh;
    bool fault;
};
```

## Starting values

Build a single array `READING logs[6]` and fill it **with one initializer list at the point
of declaration**. No `logs[0].kwh = ...;` assignments afterwards — the whole point of this
project is the initializer.

| # | Inverter | Hour | Minute | kWh  | Fault |
| - | -------- | ---- | ------ | ---- | ----- |
| 0 | INV1     | 6    | 30     | 4.2  | no    |
| 1 | INV2     | 6    | 30     | 3.9  | no    |
| 2 | INV1     | 12   | *(not recorded)* | 18.7 | no |
| 3 | INV2     | 12   | 15     | 17.4 | yes   |
| 4 | INV1     | 17   | 45     | 6.1  | no    |
| 5 | INV2     | 17   | *(not recorded)* | 5.8  | yes   |

> Where the minute was **not recorded** it must end up as `0` — but do **not** type the `0`
> yourself. Let the initializer produce it. Getting this right is most of the marks.

## Requirements

1. Declare the two structures as given above.
2. Declare `logs[6]` and fill it with one initializer list, honouring the note above.
3. Loop over the array with a `for` loop and print one line per reading, in array order:
   - the inverter name, then a space
   - the time as `HH:MM` — **two digits each**, so hour 6 prints as `06`, and a not-recorded
     minute prints as `00`
   - two spaces, then the kWh value
   - two spaces, then `OK` if `fault` is false or `FAULT` if it is true
4. Print a blank line, then three summary lines:
   - `Total: <sum of all kwh> kWh`
   - `Peak: <inverter> at <HH:MM> (<kwh> kWh)` — the single reading with the highest kWh
   - `Faults: <count>`
5. The summary values must be **calculated by looping over the array**, not typed in as
   literals you worked out yourself.

## Sample run

```
INV1 06:30  4.2  OK
INV2 06:30  3.9  OK
INV1 12:00  18.7  OK
INV2 12:15  17.4  FAULT
INV1 17:45  6.1  OK
INV2 17:00  5.8  FAULT

Total: 56.1 kWh
Peak: INV1 at 12:00 (18.7 kWh)
Faults: 2
```

## Constraints

- Only use: `struct`, nested structures, arrays of structures, structure initializers, the
  `.` selection operator, `for` / `while`, `if` / `else`, `bool`, `int`, `double`,
  `std::string`, `cout`, `<<`, `endl`.
- Do **not** use: functions of your own, `setw`, `setprecision`, `fixed`, vectors, pointers,
  or anything else not yet in your vault notes.
- Single file, `main.cpp`.

> [!note] One expected warning
> When the initializer is **correct**, `-Wextra` will print
> `warning: missing initializer for member 'TIME::minute'`. That warning is the compiler
> noticing the deliberate zero extension. It is not a fault and you should not "fix" it by
> typing the `0` in — doing that defeats the whole exercise. Any *other* warning is worth
> reading properly.

## What this is testing

Whether you can get a short inner initializer to land in the right fields. Two of these
readings have an incomplete `TIME`, and the initializer list fills fields in flat declaration
order — so if you get the braces wrong, a kWh value will quietly become a minute and every
field after it will shift. The program will still compile.

Second, smaller thing: tracking the *peak* needs you to remember which element it was, not
just the biggest number you saw.

## Stuck?

Ask for a hint — you'll get a nudge, not the answer. Re-read the "Zero extension" and
"Nested structures and arrays" sections of [[2.9 Declaring and Initializing Structures - Fundamentals and Techniques]]
before you ask; the answer to the brace question is in there.
