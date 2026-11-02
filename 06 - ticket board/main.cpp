// 06 — ticket board
// Brief: see BRIEF.md in this folder.
//
// Build in VS Code with Ctrl+Shift+B, or from a terminal:
// g++ -std=c++17 -Wall -Wextra -o main.exe main.cpp

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
struct TICKET {
    int id;
    int priority;       // 1 = most urgent, 3 = least urgent
    TIME raised;
    unsigned int flags;     // reference flag table
    string site;
};

struct BOARD {
    int count;
    TICKET *tickets;
};

// ---- Prototypes: every function's declaration goes here, above main ----
void printRule(char symbol = '-', int width = 50);      //prints widht copies of a symbol, then a newline
inline int toMinutes(TIME t);       //minutes since midnight: 07:05 -> 425
void printTime(int minutes);        //Prints a number of minutes as HH:MM, both parts always two digits. No newline. Used for clock times and for durations.
void printTime(TIME t);     //outputs the same as printTime but with TIME struct input
int minutesOpen(TICKET t, TIME now);        //gives the minutes from the time the ticket was raised to the current time 


int main() {
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

    // Starting values from the brief — the script your program runs, top to bottom.
    COMMAND script[27] = {
        //act ,id  ,val,at      ,site
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
void printRule(char symbol, int width) {        //notice how there's no default values in the definition when it's already in the declaration
    for (int i = 0; i < width; i++)
        cout << symbol;
    cout << endl;
    return;
}

inline int toMinutes(TIME t) {
    int minSinceMidnight = t.hour * 60 + t.minute;
    return minSinceMidnight;
}

void printTime(int minutes) {
    int hr = minutes/60;
    int min = minutes % 60;
    if (hr > 9 && min <= 9)
        cout << hr << ":" << "0" << min;
    else if (hr <= 9 && min > 9)
        cout << "0" << hr << ":" << min;
    else if (hr <= 9 && min <= 9)
        cout << "0" << hr << ":" << "0" << min;
    else
        cout << hr << ":" << min;
    return;
}

void printTime(TIME t) {
    printTime(toMinutes(t));
    return;
}

int minutesOpen(TICKET t, TIME now) {
    int raisedTime = toMinutes(t.raised);
    int currentTime = toMinutes(now);
    return currentTime - raisedTime;
}

