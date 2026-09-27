---
project: "03 - alarm queue"
scope: "Lessons 2.8–2.9 — Structures, declaring and initializing"
level: 3
features: [struct, "nested struct", "array of structs", "structure initializer", "selection operator", "for loop", if, "logical operators", bool, string, cout]
set: 2026-09-21
status: open
---

# 03 — Alarm queue

## Goal

An EMS site has raised six alarms during the day. Some have been cleared, some are still
open. Print the open ones, and work out which open alarm was raised **earliest**.

Do project **02** first — this one assumes you've already got the initializer right.

## The structures

```cpp
struct TIME {
    int hour, minute;
};

struct ALARM {
    std::string device;
    int code;
    TIME raised;
    bool cleared;
};
```

## Starting values

`ALARM alarms[6]`, filled with one initializer list:

| # | Device | Code | Raised | Cleared |
| - | ------ | ---- | ------ | ------- |
| 0 | PCS1   | 3012 | 08:45  | yes     |
| 1 | BMS2   | 1104 | 14:05  | no      |
| 2 | PCS1   | 3018 | 09:50  | yes     |
| 3 | MET1   | 2201 | 14:02  | no      |
| 4 | BMS2   | 1109 | 11:30  | yes     |
| 5 | PCS1   | 3020 | 19:01  | no      |

All six times are fully recorded here, so every inner list is complete.

## Requirements

1. Declare the two structures as given.
2. Declare and initialize `alarms[6]` from the table, in one initializer list.
3. Loop over the array and print one line for each alarm that is **not** cleared, in array
   order, as: `OPEN  <device>  <code>  <HH:MM>` — two digits for hour and minute.
4. Print a blank line, then:
   - `Open alarms: <count>`
   - `Earliest open: <device> code <code> at <HH:MM>`
5. "Earliest" means earliest **time of day**, considering only alarms that are not cleared.
   Work it out by looping over the array — do not type the answer in.
6. If two open alarms shared the same hour and minute, the one earlier in the array wins.
   (None do here, but your comparison should not break if they did.)

## Sample run

```
OPEN  BMS2  1104  14:05
OPEN  MET1  2201  14:02
OPEN  PCS1  3020  19:01

Open alarms: 3
Earliest open: MET1 code 2201 at 14:02
```

## Constraints

- Only use: `struct`, nested structures, arrays of structures, structure initializers, the
  `.` selection operator, `for` / `while`, `if` / `else`, `&&` / `||` / `!`, `bool`, `int`,
  `std::string`, `cout`, `<<`, `endl`.
- Do **not** use: functions of your own, `setw`, `setprecision`, `fixed`, vectors, pointers,
  sorting library calls, or anything else not yet in your vault notes.
- Single file, `main.cpp`.

## What this is testing

Comparing a nested structure against another nested structure. `raised` is two numbers, not
one, so "is this alarm earlier?" is not a single comparison — hour decides it unless the
hours are equal, and only then does minute matter.

The data is built so that three plausible shortcuts each give a *different* wrong answer:
comparing only the hour, comparing only the minute, and forgetting to skip the cleared
alarms. If your program prints anything other than the sample run, one of those three is
what you did.

## Stretch

Once it works: add a second summary line naming the **most recent** open alarm, using the
same comparison logic in reverse. Resist copy-pasting the block and flipping the sign
without reading it — that's where the off-by-one thinking hides.

## Stuck?

Ask for a hint — you'll get a nudge, not the answer. Start by writing the comparison for two
`TIME` values on paper, in words, before you write any C++.
