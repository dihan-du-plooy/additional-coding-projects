// 04 — site console
// Brief: see BRIEF.md in this folder.

#include <iostream>
#include <string>
#include <limits>
#include <cmath>

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

    int period[6] = { 2, 0, 1, 1, 1, 0 };   //TOU period of each slot: 0 = peak, 1 = standard, 2 = off-peak

    double tariff[2][3] = {{5.00, 1.5, 0.8}, {2.0, 1.25, 0.75}};

    // Part B: the menu loop, then Parts C to F inside it
    char choice;
    bool loopState = true;
    do {
        cout << "" << endl 
        << "=== Site Console ===\n" << "R Site Report\n" 
        << "F Fault Detail\n" << "C Clear a Fault\n" 
        << "V Energy Value\n" << "Q Quit\n" << endl << "Choice: ";
        cin >> choice;
        switch (choice) {

//==========================================================Report Block============================================================
            //Part C - Individual Inverter Report:
            case 'r':
            case 'R': {
                for (int i = 0; i < 3; i++) {
                    cout << site[i].name << " "
                        << site[i].kwh[0] + site[i].kwh[1] + site[i].kwh[2] + site[i].kwh[3] + site[i].kwh[4] + site[i].kwh[5] << " " << "kWh "
                        << "status: " << "0x" << hex << site[i].status << dec;     //<< hex to switch output from dec to hex and << dec to switch back
                    if (site[i].status == 0)
                        cout << " OK." << endl;
                    else
                        cout << " FAULT." << endl;
                };

                //Part C - Report Summary:
                double siteTotal = 0.0;
                int inverterCount = sizeof(site)/sizeof(site[0]);       //assigned to int variable as it will be compared to int variable i

                //Site Total kWh
                for (int i = 0; i < inverterCount ; i++) { 
                    int kwhHours = sizeof(site[i].kwh)/sizeof(site[i].kwh[0]);      //dynamic counting using sizeof() operator
                    for (int j = 0; j < kwhHours; j++) {
                        siteTotal += site[i].kwh[j];
                    }
                }
                cout << "Site Total: " << siteTotal << "kwh." << endl;

                //Peak Hour
                double hrSlot1 = 0.0, hrSlot2 = 0.0, hrSlot3 = 0.0;
                for (int i = 0; i < inverterCount; i++) {
                    int kwhHours = sizeof(site[i].kwh)/sizeof(site[i].kwh[0]);
                    for (int j = 0; j < kwhHours; j += 2) {
                        if (j == 0) hrSlot1 += site[i].kwh[j] + site[i].kwh[j+1];
                        if (j == 2) hrSlot2 += site[i].kwh[j] + site[i].kwh[j+1];
                        if (j == 4) hrSlot3 += site[i].kwh[j] + site[i].kwh[j+1];
                    }
                }  
                if (hrSlot1 > hrSlot2 && hrSlot1 > hrSlot3)
                    cout << "Peak Slot: 06:00-08:00 " << hrSlot1 << "kWh. ";
                else if (hrSlot2 > hrSlot1 || hrSlot3)
                    cout << "Peak Slot: 08:00-10:00 " << hrSlot2 << "kWh. ";
                else
                    cout << "Peak Slot: 10:00-12:00" << hrSlot3 << "kWh. ";
            }
            break;
            
//==========================================================Fault Report Block============================================================
            //Part D - Individual Inverter Faut Report
            case 'f':
            case 'F': {
                //Inverter selection and validation
                cout << "Which inverter would you like to check: ";
                int invChoice;
                cin >> invChoice;
                while (invChoice > 3 || invChoice < 1) {
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');     //clear entire input stream incase of unbounded reply
                    cout << "No such inverter. There's only " << sizeof(site)/sizeof(site[0]) << " inverters. Please pick again: ";
                    cin >> invChoice;
                }
                //Identify fault codes
                if (site[invChoice -1].status != 0) {
                    cout << site[invChoice - 1].name << " faults: " << site[invChoice - 1].status << endl;
                    for (int i = 0; (int)pow(2, i) <= 8; i++) {
                        if((site[invChoice - 1].status & (int)pow(2, i)) == ((int)pow(2, i)))
                            switch((int)pow(2, i)) {
                                case 1: cout << "bit " << 0 << " Grid Fault." << endl; break;
                                case 2: cout << "bit " << 1 << " Over-Temp." << endl; break;
                                case 4: cout << "bit " << 2 << " Coms lost." << endl; break;
                                case 8: cout << "bit " << 3 << " Isolation Fault." << endl; break;
                            }
                    }
                }
                else
                    cout << "No active faults: ";
            }
            break;

//==========================================================Fault Clearing Block============================================================
            //Part E - Individual inverter fault selection and clearing
            case 'C':
            case 'c': {
                //Selection and validation phase
                cout << "Which inverter would you like to clear a fault from: ";
                int invChoice;
                int faultBit;
                cin >> invChoice;
                while (invChoice > 3 || invChoice < 1) {
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');     //clear entire input stream incase of unbounded reply
                    cout << "No such inverter. There's only " << sizeof(site)/sizeof(site[0]) << " inverters. Please pick again: ";
                    cin >> invChoice;
                }
                if (site[invChoice - 1].status != 0) {
                    cout << "Which fault bit would you like to clear: ";
                    cin >> faultBit;
                    while (site[invChoice - 1].status & (1 << faultBit) == 0) {      //use bit shifting to identify if inv status represents chosen fault bit
                        cout << "Bit " << faultBit << " is not set on " << site[invChoice - 1].name << " Try again: ";        //1100 
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        cin >> faultBit;
                    }
                    //Fault finding and clearing phase
                    switch (faultBit) {
                        case 0: site[invChoice - 1].status = site[invChoice - 1].status ^ 1; 
                                cout << "Cleared bit " << 0 << " on " << site[invChoice - 1].name 
                                << ". " << "Status 0x" << hex << site[invChoice - 1].status << dec;
                                break;
                        case 1: site[invChoice - 1].status = site[invChoice - 1].status ^ 2; 
                                cout << "Cleared bit " << 1 << " on " << site[invChoice - 1].name 
                                << ". " << "Status 0x" << hex << site[invChoice - 1].status << dec;
                                break;
                        case 2: site[invChoice - 1].status = site[invChoice - 1].status ^ 4; 
                                cout << "Cleared bit " << 2 << " on " << site[invChoice - 1].name 
                                << ". " << "Status 0x" << hex << site[invChoice - 1].status << dec;
                                break;
                        case 3: site[invChoice - 1].status = site[invChoice - 1].status ^ 8; 
                                cout << "Cleared bit " << 3 << " on " << site[invChoice - 1].name 
                                << ". " << "Status 0x" << hex << site[invChoice - 1].status << dec;
                                break;
                    }
                }
                else
                    cout << "This inverter has no fault." << endl;
            }
            break;
            
//==========================================================Energy Value Block============================================================
            //Part F - Indv & Sum Energy Value Calculation:
            case 'V':
            case 'v': {
                cout << "For which month do you want to calculate the energy value: ";
                int month;
                cin >> month;
                //Month/Season reference selection
                while (month < 1 || month > 12) {
                    cout << month << " Is not a valid month. Please pass a valid month: ";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin >> month;
                }
                //Value calculation
                double sumEnergyValue = 0.0;
                switch (month) {
                    //Low Demand Season
                    //Individual Inverter Energy Value
                    case 1: case 2: case 3: case 4: case 5: case 9: case 10: case 11: case 12:
                    for (int i = 0; i < sizeof(site)/sizeof(site[0]); i++) {
                        double indvEnergyValue = 0.0;
                        if ((site[i].status & 4) == 4)
                            cout << site[i].name << " skipped (coms lost)." << endl;
                        else {
                            for (int j = 0; j < sizeof(site[i].kwh)/sizeof(site[i].kwh[0]); j++) {
                                switch (period[j]) {
                                    case 0: indvEnergyValue += (site[i].kwh[j] * tariff[1][0]); break;
                                    case 1: indvEnergyValue += (site[i].kwh[j] * tariff[1][1]); break;
                                    case 2: indvEnergyValue += (site[i].kwh[j] * tariff[1][2]); break;
                                } 
                            }
                            cout << site[i].name << " R" << indvEnergyValue << endl;
                        }
                    sumEnergyValue += indvEnergyValue;
                    }
                    //Summarised Energy Value
                    cout << "Site value: R" << sumEnergyValue << " for month " << month << " (Low Season)"; 
                    break;
                    //High Demand Season
                    //Individual Inverter Energy Value
                    case 6: case 7: case 8:
                    for (int i = 0; i < sizeof(site)/sizeof(site[0]); i++) {
                        double indvEnergyValue = 0.0;
                        if ((site[i].status & 4) == 4)
                            cout << site[i].name << " skipped (coms lost)." << endl;
                        else {
                            for (int j = 0; j < sizeof(site[i].kwh)/sizeof(site[i].kwh[0]); j++) {
                                switch (period[j]) {
                                    case 0: indvEnergyValue += (site[i].kwh[j] * tariff[0][0]); break;
                                    case 1: indvEnergyValue += (site[i].kwh[j] * tariff[0][1]); break;
                                    case 2: indvEnergyValue += (site[i].kwh[j] * tariff[0][2]); break;
                                }
                            }
                            cout << site[i].name << " R" << indvEnergyValue << endl;
                        }
                        sumEnergyValue += indvEnergyValue; 
                    }
                    //Summarised Energy Value
                    cout << "Site value: R" << sumEnergyValue << " for month " << month << " (High Season)";
                    break;
                }
            } 
            break;

//==========================================================Quit Block============================================================

            case 'Q':
            case 'q':
            cout << "Bye";
            loopState = false; 
            break;

            default: 
            cout << "Unknown Option. Pick again: " << endl;
            break;
        }
    }
    while (loopState);
    return 0;
}