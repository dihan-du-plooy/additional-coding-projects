// 06 — ticket board
// Brief: see BRIEF.md in this folder.
//
// Build in VS Code with Ctrl+Shift+B, or from a terminal:
// g++ -std=c++17 -Wall -Wextra -o main.exe main.cpp

#include <iostream>
#include <string>

using namespace std;

//------------------------------------------------------------STRUCT DEC--------------------------------------------------------------------------
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

//------------------------------------------------------------FUNC DEC--------------------------------------------------------------------------
void printRule(char symbol = '-', int width = 50);      //prints widht copies of a symbol, then a newline
inline int toMinutes(TIME t);       //minutes since midnight: 07:05 -> 425
void printTime(int minutes);        //Prints a number of minutes as HH:MM, both parts always two digits. No newline. Used for clock times and for durations.
void printTime(TIME t);     //outputs the same as printTime but with TIME struct input
int minutesOpen(TICKET t, TIME now);        //gives the minutes from the time the ticket was raised to the current time
char flagLetter(int bit);       //responds with P,S or N based on flag bit input with ? for fale safe
bool hasFlag(TICKET t, int bit);        //returns true if a ticket has a flag
void printTicket(TICKET t, TIME now);       //for the report, print convention = P1  T103  07:50  age 01:40  [P--]  Delta Cold Store
void addTicket(BOARD &board, TICKET t);     //Grows the board by one and puts t at the end.
void tallyPriorities(BOARD board, int *tally);      //tally points at an int array of 3. Fill it with how many tickets are P1, P2, P3.
void printBoard(BOARD &board, TIME now);        //Prints the whole board block. In Part E it sorts the board first.


//-----------------------------------------------------------------------------DISPATCHER---------------------------------------------------------------------------------
int main() {

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

    //----------------------------------------------------------------------------PART B----------------------------------------------------------------------------------
    BOARD board = {0, NULL};


    for (int i = 0; i < scriptLength; i++) {
        COMMAND cmd = script[i];
        if (cmd.action == 'O') {
            TICKET currentTicket = {cmd.id, cmd.value, cmd.at, 0, cmd.site};      //Contemplate ticket global existance
            addTicket(board, currentTicket);
            cout << "Opened T" << currentTicket.id << " P" << currentTicket.priority << " " << currentTicket.site << endl;
        }
        else {
            switch (cmd.action) {
                 case 'B': printBoard(board, cmd.at); break;
                 default: cout << "Unknown command " << "'" << cmd.action << "'" << endl; break;      //default behaviour: print any command not yet compensated for as unknown
            }
        }
    }

    // Part D: the end-of-script line, then free whatever is still on the board.

    return 0;
}

//------------------------------------------------------------------------------FUNC DEFINITIONS---------------------------------------------------------------------------
void printRule(char symbol, int width) {        //keep note of how there's no default values in the definition when it's already in the declaration
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

void printTime(TIME t) {        //good use of function overloading, same functions accomodating for 2 data types.
    printTime(toMinutes(t));
    return;
}

int minutesOpen(TICKET t, TIME now) {
    int raisedTime = toMinutes(t.raised);    //TIME t.raised to min
    int currentTime = toMinutes(now);       //TIME now to min
    return currentTime - raisedTime;        //return min difference
}

char flagLetter(int bit) {              //might need to change switch values to the bit values and not their weight
    switch (bit) {
        case 1: return 'P';     //after a return you don't need a break
        case 2: return 'S';
        case 4: return 'N';
        default: return '?';
    }   
}

bool hasFlag(TICKET t, int bit) {
    if(t.flags)
        return true;
    return false;
}

void printTicket(TICKET t, TIME now) {      //print convention = P1  T103  07:50  age 01:40  [P--]  Delta Cold Store
    cout << "P" << t.priority << "  T" << t.id << "  ";
    printTime(t.raised);        //you can't add a function that's already printing something to cout.
    cout << "  age ";
    printTime(minutesOpen(t, now)); cout << " ";
    switch (t.flags) {
        case 1: cout << "[P--]"; break;
        case 2: cout << "[-S-]"; break;
        case 4: cout << "[--N]"; break;
        default: cout << "[---]";
    }
    cout << " " << t.site << endl;
}

void addTicket(BOARD &board, TICKET t) {
    TICKET *tempTickArr = new TICKET[board.count + 1];      //creates block in memory one size bigger than board tickets array (locally)
    for (int i = 0; i < board.count; i++)       //copies board tickets over to block
        tempTickArr[i] = board.tickets[i];      
    tempTickArr[board.count] = t;       //adds new ticket to block
    delete[] board.tickets;     //deletes board tickets memory
    board.tickets = tempTickArr;        //assign board tickets to new tempTickArray
    board.count++;      //adds ticket count to board's tracker
}

void tallyPriorities(BOARD board, int *tally) {
    for (int i = 0; i < board.count; i++) {
        switch (board.tickets[i].priority) {
            case 1: tally[0] += 1; break;       //a pointer is dereferenced with []
            case 2: tally[1] += 1; break;
            case 3: tally[2] += 1; break; 
        }  
    }
}

void printBoard(BOARD &board, TIME now) {
    int tally[3] = {0, 0, 0};       //array for tallyPriorities
    tallyPriorities(board, tally);      //populating tally array
    //----------------------Board Print---------------------------
    cout << endl;
    cout << "Board at "; printTime(now); cout << endl;      //because printTime doesn't return anything you can't put it straight in the cout stream
    printRule('=');
    for (int i = 0; i < board.count; i++) {
        printTicket(board.tickets[i], now);  
    }
    printRule();
    cout << "Open: " << board.count << "  P1: " << tally[0] << "  P2: " << tally[1] << "  P3: " << tally[2] << endl;
    cout << endl;
}

