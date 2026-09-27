---
project: "01 - alarm clock"
scope: "Module 1 — all seven lessons (1.0–1.6)"
level: 2
features: [int, cin, ">>", cout, "<<", endl, "*", "+", "/", "%", if, "comparison operators"]
set: 2026-08-14
status: open        # open | submitted | marked
---

# 01 — Alarm clock

## Goal

Ask the user what time it is now and how many minutes until their alarm should go off, then
work out and print the time the alarm will ring, in 24-hour `HH:MM` form — including when the
alarm lands after midnight.

## Requirements

1. Read three whole numbers with `cin`, each after a prompt containing these words:

   ```
   Hour now (0-23):
   Minute now (0-59):
   Minutes until alarm:
   ```

   Spacing after the colon is not graded; the wording is.

2. Assume the user behaves — the hour is 0–23, the minute is 0–59, and the wait is **0 to 1439
   minutes** (less than a full day). You do **not** need to check or handle bad input.

3. Work out the alarm time and print it as one final line, in exactly this form:

   ```
   Alarm at HH:MM
   ```

4. **Both the hour and the minute must always be two digits.** `1:15` is wrong; `01:15` is right.
   `10:5` is wrong; `10:05` is right.

5. If the alarm rings on the **next day**, append ` (next day)` to that same line — one space
   before the bracket:

   ```
   Alarm at 01:15 (next day)
   ```

   If it rings on the same day, that part must not appear at all.

## Starting values

None — this program reads its input with `cin`.

## Sample runs

```
Hour now (0-23): 14
Minute now (0-59): 30
Minutes until alarm: 200
Alarm at 17:50
```

```
Hour now (0-23): 23
Minute now (0-59): 45
Minutes until alarm: 90
Alarm at 01:15 (next day)
```

```
Hour now (0-23): 9
Minute now (0-59): 55
Minutes until alarm: 10
Alarm at 10:05
```

## Constraints

- Only use: `#include <iostream>`, `main`, `int`, variables and assignment, `cin` and `>>`,
  `cout` and `<<`, `endl` and `\n`, the arithmetic operators `+ - * / %`, the shortcut forms
  (`+=` etc.) if you want them, the comparison operators, and `if`.
- **Do not use `else`** — you haven't been taught it yet. Lesson 1.5 stops at plain `if`.
  Everything here can be done with `if` on its own.
- Do not use: loops, functions of your own, arrays, `&&` / `||`, `setw` / `setfill`,
  or anything from a library the course hasn't covered.
- Single file, `main.cpp`.

## What this is testing

Whether you can turn "now plus a wait" into a clock time — which means seeing that the day
wraps around at 24 hours, and that `/` and `%` are a pair working on the same total, not on
each other's leftovers by accident. The two-digit rule is testing whether you actually read
the spec, which is a real part of the job.

## Stuck?

Ask for a hint — you'll get a nudge, not the answer. Submitting something broken with a note
about where you got stuck is a perfectly good outcome; a blank file isn't.
