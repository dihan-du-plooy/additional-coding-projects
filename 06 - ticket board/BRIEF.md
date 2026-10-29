---
project: "06 - ticket board"
scope: "Module 3 capstone — lessons 3.0–3.9, plus Modules 1 and 2"
level: 3
features: [pointer, "pointer arithmetic", "pointer subtraction", NULL, "array name as pointer", "function prototype", "function definition", void, return, "passing by value", "passing by reference", "passing a pointer", "default parameter", inline, "function overloading", "ternary operator", "bubble sort", new, "delete []", struct, "nested struct", "array of structs", switch, "for loop", "do loop", bool, "bitwise operators", bitmask, "bit shifting", unsigned, "/", "%", char, string, cout]
set: 2026-10-29
status: open        # open | submitted | marked
---

# 06 — Ticket board

## Goal

A maintenance ticket board for a small O&M team. Your program runs a fixed **script** of
commands: open tickets, set and clear flags on them, escalate them, close them, and print the
board, sorted so the most urgent, longest-waiting work is at the top. The board lives in a
**dynamic array** that grows by one when a ticket is opened and shrinks by one when a
ticket is closed.

This is the **Module 3 capstone**. It's bigger than 04 (roughly 200–260 lines including the
given script) and almost all of it is functions. Build it in the order of the parts below. Get
each part compiling and correct before you start the next.

### How this one is different

- **No `cin`.** The input is the `script` array that's already in `main.cpp`, so every run
  is identical. Debugging is much easier when the input can't change under you.
- **`main` is a dispatcher.** It loops over the script and, inside a `switch`, decides which
  functions to call and what message to print. The real work happens in the functions below.
- **Marking runs a memory checker** (AddressSanitizer). It reports memory used after
  `delete []` and memory you forgot to free. Clean compiles and correct output aren't enough
  on their own here.

---

## Part A — types and time helpers

### Given (already in `main.cpp`, do not change)

```cpp
struct TIME {
    int hour, minute;
};

struct COMMAND {
    char action;
    int id;
    int value;
    TIME at;
    string site;
};
```

How each command uses the fields (`—` means "ignore it"):

| `action` | `id` | `value` | `at` | `site` |
| --- | --- | --- | --- | --- |
| `O` open a ticket | new ticket number | priority, 1–3 | time raised | site name |
| `C` close | ticket | — | time closed | — |
| `E` escalate | ticket | — | — | — |
| `F` set a flag | ticket | flag bit | — | — |
| `U` clear a flag | ticket | flag bit | — | — |
| `B` print the board | — | — | time of the report | — |

### Yours to declare, exactly as written

```cpp
struct TICKET {
    int id;
    int priority;          // 1 = most urgent, 3 = least
    TIME raised;
    unsigned int flags;    // see the flag table
    string site;
};

struct BOARD {
    int count;             // how many tickets are on the board right now
    TICKET *tickets;       // the dynamic array, or NULL when the board is empty
};
```

`BOARD` is the same idea as `Collection` in lab 3.9.6: the pointer and its count travel
together, so one reference parameter carries both.

### The flag bits

| Bit | Weight | Letter | Meaning |
| --- | --- | --- | --- |
| 0 | 1 | `P` | Parts needed. **A ticket can't be closed while this is set.** |
| 1 | 2 | `S` | Subcontractor booked |
| 2 | 4 | `N` | Client notified |

### The functions

These are the program's **interface**. Use these exact prototypes. You may add helper
functions of your own, but these must all exist and do what the table says.

| Prototype | Part | What it does |
| --- | --- | --- |
| `void printRule(char symbol = '-', int width = 50);` | A | Prints `width` copies of `symbol`, then a newline. |
| `inline int toMinutes(TIME t);` | A | Minutes since midnight: 07:05 → `425`. |
| `void printTime(int minutes);` | A | Prints a number of minutes as `HH:MM`, both parts always **two digits**. No newline. Used for clock times **and** for durations. |
| `void printTime(TIME t);` | A | Same format, from a `TIME`. **It must not repeat the formatting work.** Let it hand over to the other `printTime`. |
| `int minutesOpen(TICKET t, TIME now);` | A | Minutes from `t.raised` to `now`. |
| `char flagLetter(int bit);` | B | `0` → `'P'`, `1` → `'S'`, `2` → `'N'`, anything else `'?'`. Use a `switch`. |
| `bool hasFlag(TICKET t, int bit);` | B | `true` if that bit of `t.flags` is set. **The only place in the program that tests a flag bit.** |
| `void printTicket(TICKET t, TIME now);` | B | One board line (format below), including the newline. |
| `void addTicket(BOARD &board, TICKET t);` | B | Grows the board by one and puts `t` at the end. |
| `void tallyPriorities(BOARD board, int *tally);` | B | `tally` points at an `int` array of 3. Fill it with how many tickets are P1, P2, P3. |
| `void printBoard(BOARD &board, TIME now);` | B, E | Prints the whole board block. In Part E it sorts the board first. |
| `TICKET *findTicket(BOARD board, int id);` | C | A pointer to the ticket with that `id` **inside the board's array**, or `NULL` if there isn't one. |
| `void setFlag(TICKET &t, int bit);` | C | Sets that bit and leaves the others alone. |
| `bool clearFlag(TICKET &t, int bit);` | C | Clears **only** that bit. If it wasn't set, change nothing and return `false`. |
| `bool escalate(TICKET &t);` | C | Moves priority up one level (P3 → P2, P2 → P1). If it's already P1, change nothing and return `false`. |
| `void removeTicket(BOARD &board, TICKET *which);` | D | Shrinks the board by one, dropping the ticket `which` points at. |
| `bool comesAfter(TICKET a, TICKET b);` | E | `true` if `a` belongs **lower** on the board than `b`. |
| `void sortBoard(BOARD &board);` | E | Bubble sort (3.8), using `comesAfter` to decide every swap. |

### Rules for all of them

1. All the prototypes go **above** `main`. All the definitions go **below** `main`. This is the
   layout lesson 3.3 is about: `main` can call a function it hasn't seen the body of yet,
   because the prototype came first.
2. **No global variables.** Everything a function needs arrives through its parameters (3.4:
   side effects).
3. Pass by **reference** (`&`) only where the prototype says so. Those are the functions that
   change what they're given.

### Checkpoint A

Before writing anything else, put this test at the top of `main`, temporarily:

```cpp
TIME t = {7, 5};
printTime(t);   cout << endl;
printTime(605); cout << endl;
printTime(0);   cout << endl;
cout << toMinutes(t) << endl;
TICKET test = {1, 2, {6, 40}, 0, "Test"};
TIME now = {9, 30};
cout << minutesOpen(test, now) << endl;
printRule('=');
printRule();
return 0;
```

It must print exactly:

```
07:05
10:05
00:00
425
170
==================================================
--------------------------------------------------
```

Delete the test when it passes.

---

## Part B — opening tickets and printing the board

### The board in `main`

Declare `BOARD board` starting **empty**: count `0`, pointer `NULL`.

### The dispatcher

Inside the given `for` loop, one `switch` on `cmd.action`. For now handle only:

- **`O`:** build a `TICKET` from the command (flags start at `0`), add it with `addTicket`,
  then print `Opened T<id> P<priority> <site>`.
- **`B`:** `printBoard(board, cmd.at)`.
- **anything else** (`default`): `Unknown command '<action>'`. Until Parts C and D exist, `C`,
  `E`, `F` and `U` will land here too. That's expected.

### `addTicket`

- The board grows by **exactly one** element. The new ticket goes **last**. Every ticket
  already on the board keeps its place.
- The old array must be **freed**. You did the same thing in lab 3.9.6.
- It has to work when the board is empty (`tickets` is `NULL`, `count` is `0`).

### The board block

Fields are separated by **exactly two spaces**. The flags are always three characters, bit 0
first: the letter if that bit is set, `-` if it isn't. Build them with a loop over the bits,
`hasFlag`, `flagLetter` and the **ternary operator**.

```

Board at 09:30
==================================================
P1  T103  07:50  age 01:40  [P--]  Delta Cold Store
P2  T104  06:20  age 03:10  [--N]  Riverside Spar
--------------------------------------------------
Open: 2  P1: 1  P2: 1  P3: 0

```

From top to bottom:

1. A blank line.
2. `Board at HH:MM`.
3. `printRule('=')`.
4. One `printTicket` line per ticket, **or** `No open tickets` if the board is empty.
5. `printRule()`.
6. `Open: <count>  P1: <n>  P2: <n>  P3: <n>`. The three numbers come from
   `tallyPriorities`, given an `int` array of 3 that `printBoard` declares.
7. A blank line.

A board line is `P<priority>  T<id>  <raised>  age <age>  [<flags>]  <site>`. The age is
`minutesOpen` at the board's time, printed with `printTime`. The site goes last because it's the
only field whose width changes, so nothing after it can drift.

### Checkpoint B

Run the whole script. The first part of the output must be exactly this. It's not sorted yet, and
there are no flags yet:

```
Opened T101 P2 Karoo Mall
Opened T102 P3 Hilltop Farm
Opened T103 P1 Delta Cold Store
Opened T104 P2 Riverside Spar
Unknown command 'F'
Unknown command 'F'

Board at 09:30
==================================================
P2  T101  06:40  age 02:50  [---]  Karoo Mall
P3  T102  07:15  age 02:15  [---]  Hilltop Farm
P1  T103  07:50  age 01:40  [---]  Delta Cold Store
P2  T104  06:20  age 03:10  [---]  Riverside Spar
--------------------------------------------------
Open: 4  P1: 1  P2: 2  P3: 1

```

---

## Part C — finding tickets: `E`, `F`, `U`

### `findTicket`

Returns **where the ticket lives**, a pointer into `board.tickets`, not a copy of it. If no
ticket has that `id`, it returns `NULL`.

> [!tip] Working with a `TICKET *` without `->`
> `found` is a `TICKET *`. `*found` is the `TICKET` it points at. Any function above that takes
> a `TICKET` or a `TICKET &` can be handed `*found` directly. If you want a field in `main`,
> `TICKET t = *found;` copies the ticket into `t`. You never need `->`. That's a later lesson.

### The three commands

Do the checks **in this order**, and stop at the first one that applies:

**`F` and `U`**

1. If `cmd.value` is outside 0–2: `Invalid flag bit <value>`. Don't even look for the ticket.
2. If the ticket isn't on the board: `T<id> not found`.
3. **`F`:** set the bit, then print `T<id> flag <letter> set`.
4. **`U`:** if the bit was set, clear it and print `T<id> flag <letter> cleared`. If it wasn't set,
   print `T<id> flag <letter> not set - nothing to clear`.

**`E`**

1. If the ticket isn't on the board: `T<id> not found`.
2. If it moved up, print `T<id> escalated to P<new priority>`. If it was already P1, print
   `T<id> already P1`.

---

## Part D — closing tickets: `C`, and the end of the script

### The `C` command

1. If the ticket isn't on the board: `T<id> not found`.
2. If its `P` flag (bit 0) is set: `T<id> waiting on parts - cannot close`. It stays on the
   board. Only the `P` flag blocks a close. The other flags don't.
3. Otherwise remove it and print `Closed T<id> after HH:MM`. The duration runs from the ticket's
   `raised` time to the command's `at` time.

### `removeTicket`

- The board shrinks by **exactly one**. Every other ticket keeps its order.
- Work out **which position** `which` points at by **pointer subtraction**. Don't search the
  array again by `id`. You already know where it is.
- The old array must be freed.
- If the ticket being removed is the **last one on the board**, don't allocate an empty array.
  Free the old one, set `tickets` to `NULL` and `count` to `0`. A later `O` has to be able to
  grow the board again from there.

### After the last command

Print `End of script - <count> ticket still open`. Use `tickets` instead of `ticket` whenever
the count isn't exactly 1, and choose it with the ternary operator. Then free whatever is still
on the board.

---

## Part E — sorting the board

`printBoard` calls `sortBoard` before printing anything. The board order is:

1. **Higher priority first.** P1 at the top, P3 at the bottom.
2. **Within the same priority, the ticket raised earliest goes first.**

`comesAfter` holds that rule in one place. `sortBoard` is the 3.8 bubble sort working on
`TICKET`s instead of `int`s, with a `swapped` flag and an `aux` that's a whole `TICKET`.

The sort changes the board's actual order, which is why `printBoard` takes the board by
reference. Later commands then work on the sorted array, so that's the order you'll find tickets
in from then on.

---

## Sample run

The complete output of the finished program, from the given script. It must match
**character for character**, including the blank lines.

```
Opened T101 P2 Karoo Mall
Opened T102 P3 Hilltop Farm
Opened T103 P1 Delta Cold Store
Opened T104 P2 Riverside Spar
T103 flag P set
T104 flag N set

Board at 09:30
==================================================
P1  T103  07:50  age 01:40  [P--]  Delta Cold Store
P2  T104  06:20  age 03:10  [--N]  Riverside Spar
P2  T101  06:40  age 02:50  [---]  Karoo Mall
P3  T102  07:15  age 02:15  [---]  Hilltop Farm
--------------------------------------------------
Open: 4  P1: 1  P2: 2  P3: 1

T103 waiting on parts - cannot close
T103 flag P cleared
T103 flag P not set - nothing to clear
Closed T103 after 02:15
T102 escalated to P2
T101 escalated to P1
T101 already P1
T999 not found
Invalid flag bit 5
Opened T105 P1 Ridgeview Plant
T102 flag S set
Unknown command 'X'

Board at 11:00
==================================================
P1  T101  06:40  age 04:20  [---]  Karoo Mall
P1  T105  10:20  age 00:40  [---]  Ridgeview Plant
P2  T104  06:20  age 04:40  [--N]  Riverside Spar
P2  T102  07:15  age 03:45  [-S-]  Hilltop Farm
--------------------------------------------------
Open: 4  P1: 2  P2: 2  P3: 0

Closed T104 after 05:10
Closed T101 after 05:05
Closed T102 after 04:55
Closed T105 after 01:55

Board at 12:30
==================================================
No open tickets
--------------------------------------------------
Open: 0  P1: 0  P2: 0  P3: 0

Opened T106 P3 Karoo Mall

Board at 13:05
==================================================
P3  T106  12:40  age 00:25  [---]  Karoo Mall
--------------------------------------------------
Open: 1  P1: 0  P2: 0  P3: 1

End of script - 1 ticket still open
```

The run tells a story. Each step of it is a test, so if your output drifts, the step where it
drifts tells you which rule broke:

- T103 can't close until its parts flag is cleared, and the second clear finds nothing to clear.
- T101 is escalated twice, but only the first one moves it.
- T104 has a flag (`N`) and still closes at 11:30, because only `P` blocks a close.
- The four closes after 11:00 take a ticket from the middle, the front, the end, and finally the
  only one left. Then the board is empty and has to grow again from nothing.

## Constraints

- **Only use:** everything in your vault up to lesson 3.9. That means `<iostream>`, `<string>`,
  `using namespace std;`, `int`, `unsigned`, `bool`, `char`, `string`, `struct` (including a
  nested one), arrays, pointers, `&`, `*`, `NULL`, pointer `+ - == !=`, `new`, `delete []`,
  functions with prototypes, `void`, `return`, by-value, by-reference and pointer parameters,
  default parameters, `inline`, overloading, `?:`, `if`/`else`, `switch`, `for`/`while`/`do`,
  `break`/`continue`, and the arithmetic, comparison, logical and bitwise operators.
- **Do not use:** `->`, `cin`, global variables, `vector`, `sort` or any other library algorithm,
  `setw`/`setfill`/`setprecision`, `sizeof` (every count you need is a variable), or hex
  literals.
- Single file, `main.cpp`.

## What this is testing

Every part has one thing to be careful about. Most of them still compile. They give output that
looks right and isn't, or, in D, a program that works on your machine and fails the memory
checker.

- **A — overloading and reuse.** Two functions with one name, told apart by their parameter
  lists, not by their return type. The `TIME` version shouldn't redo the `/` and `%` work.
  Repeating a block where you could have reused it was a correction on 04.
- **B — what a reference parameter is for.** Ask yourself what would happen to `main`'s
  `board.count` if `addTicket` took a `BOARD` instead of a `BOARD &`. And the tally index comes
  out of the ticket's priority, but P1 doesn't live at index 1.
- **C — pointers that point into the board.** `board.tickets + i` moves `i` whole tickets, not
  `i` bytes. Check for `NULL` before every `*found`. And `TICKET t = *found;` and
  `escalate(*found)` look similar, but only one of them gives the function the real ticket on
  the board. Know which before you write `E`. The clear-a-bit and `&`-vs-`==` traps from 04 are
  back in `clearFlag` and `hasFlag`.
- **D — memory you've given back.** `which - board.tickets` is a count of tickets, not bytes.
  Once `delete []` has run, `found` points into freed memory. Anything you still need from that
  ticket, get it before you remove it.
- **E — a rule with two keys.** Priority decides, and the time only counts on a tie. Also: what
  should `comesAfter` say about two tickets with the same priority **and** the same time? What
  would bubble sort's `swapped` do if it said `true`?
- **Throughout — the spec's exact output.** Labels, spacing, blank lines, two-digit times. The
  "output doesn't match the sample run" weakness from 04 is checked line by line.

## Compiler messages you might meet

These come from how C++ is set up, not from logic mistakes, and the course hasn't shown them
yet:

- **`jump to case label` / `crosses initialization of ...`.** A variable declared inside one
  `case` is still visible in the cases below it. Wrap that case's statements in `{ }`, a block
  (1.5), so the variable only exists inside it.
- **`default argument given for parameter 1 of ...`.** Write `= '-'` and `= 50` in the
  **prototype** only. The definition below `main` repeats the parameter list without the
  defaults.
- **`undefined reference to ...`.** That's the linker, not the compiler. You declared a prototype
  but never wrote its definition, or the definition's parameter list doesn't match the prototype
  exactly.

## Stuck?

Ask for a hint and you'll get a nudge, not the answer. Getting through Part C, with a note
about where you stopped, is a perfectly good outcome and will be marked properly. A blank file
isn't.

Useful rereads: [[3.1 Pointers vs Arrays - Navigating C++ Data Structures]] (pointer `+` and
`-`), [[3.4 Passing Data to and From Functions]] (one-way vs return ticket),
[[3.5 Default Parameters in Functions]], [[3.7 Overloading Functions Explained]] (the ternary
operator is in here too), [[3.8 Sorting Data - Understanding Sorting Algorithms]],
[[3.9 Dynamic Memory Allocation - Managing Memory on Demand]] and your own lab 3.9.6.
