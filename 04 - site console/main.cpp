// 04 — site console
// Brief: see BRIEF.md in this folder. Build it part by part (A to F).
//
// Build in VS Code with Ctrl+Shift+B, or from a terminal:
//     g++ -std=c++17 -Wall -Wextra -o main.exe main.cpp

#include <iostream>
#include <string>

using namespace std;

// Part A: declare struct INVERTER here, exactly as in the brief
struct INVERTER {
    string name;
    double kwh[6];          // energy per 2-hour slot, 06:00-18:00
    unsigned int status;    // fault bits, see the table below
};

int main() {

    // Part A: starting data, each filled with ONE initializer list
    INVERTER site[3] = {
    {"INV 1", 4.5, 11.0, 16.5, 17.0, 12.0, 5.0, 0},
    {"INV 2", 4.0, 10.5, 15.0, 9.5, 11.5, 4.5, 2},
    {"INV 3", {3.5, 9.0, 14.0, 13.5,}, 12},
    };

    int period[6] = { 2, 0, 1, 1, 1, 0 };

    double tariff[2][3] = {{5.00, 1.5, 0.8}, {2.0, 1.25, 0.75}};

    // Part B: the menu loop, then Parts C to F inside it
    // your code here

    return 0;
}
