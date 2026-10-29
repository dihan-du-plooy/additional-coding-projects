// 06 — ticket board
// Brief: see BRIEF.md in this folder.
//
// Build in VS Code with Ctrl+Shift+B, or from a terminal:
//     g++ -std=c++17 -Wall -Wextra -o main.exe main.cpp

#include <iostream>
#include <string>

using namespace std;

// ---- Given: do not change these two structures ----------------------------

struct TIME {
    int hour, minute;
};

struct COMMAND {
    char action;     // O C E F U B - see the brief
    int id;          // ticket number
    int value;       // O: priority 1-3    F/U: flag bit
    TIME at;         // O: time raised     C: time closed    B: time of the report
    string site;     // O only
};

// ---- Part A: declare struct TICKET and struct BOARD here, exactly as in the brief ----


// ---- Prototypes: every function's declaration goes here, above main ----


int main() {

    // Starting values from the brief — the script your program runs, top to bottom.
    COMMAND script[27] = {
        //  action  id   value  at          site
        { 'O', 101, 2, { 6, 40}, "Karoo Mall"       },
        { 'O', 102, 3, { 7, 15}, "Hilltop Farm"     },
        { 'O', 103, 1, { 7, 50}, "Delta Cold Store" },
        { 'O', 104, 2, { 6, 20}, "Riverside Spar"   },
        { 'F', 103, 0, { 0,  0}, ""                 },
        { 'F', 104, 2, { 0,  0}, ""                 },
        { 'B',   0, 0, { 9, 30}, ""                 },
        { 'C', 103, 0, { 9, 45}, ""                 },
        { 'U', 103, 0, { 0,  0}, ""                 },
        { 'U', 103, 0, { 0,  0}, ""                 },
        { 'C', 103, 0, {10,  5}, ""                 },
        { 'E', 102, 0, { 0,  0}, ""                 },
        { 'E', 101, 0, { 0,  0}, ""                 },
        { 'E', 101, 0, { 0,  0}, ""                 },
        { 'C', 999, 0, {10, 30}, ""                 },
        { 'F', 104, 5, { 0,  0}, ""                 },
        { 'O', 105, 1, {10, 20}, "Ridgeview Plant"  },
        { 'F', 102, 1, { 0,  0}, ""                 },
        { 'X',   0, 0, { 0,  0}, ""                 },
        { 'B',   0, 0, {11,  0}, ""                 },
        { 'C', 104, 0, {11, 30}, ""                 },
        { 'C', 101, 0, {11, 45}, ""                 },
        { 'C', 102, 0, {12, 10}, ""                 },
        { 'C', 105, 0, {12, 15}, ""                 },
        { 'B',   0, 0, {12, 30}, ""                 },
        { 'O', 106, 3, {12, 40}, "Karoo Mall"       },
        { 'B',   0, 0, {13,  5}, ""                 }
    };
    int scriptLength = 27;

    // Part B: declare the board here, starting empty (once struct BOARD exists above).

    for (int i = 0; i < scriptLength; i++) {
        COMMAND cmd = script[i];

        // your code here — one switch on cmd.action

    }

    // Part D: the end-of-script line, then free whatever is still on the board.

    return 0;
}

// ---- Definitions: every function's body goes here, below main ----

