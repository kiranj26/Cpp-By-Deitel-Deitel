// File: chapter2_loops_sentinels_notes.cpp
// Purpose: Revision notes + runnable exercises (sentinels, validation, accumulators, max tracking, patterns)
//
// How to use later:
// - Search "EX" to jump to a topic.
// - Each EX block is independent and prints a clear label.
// - Sentinel pattern used across exercises: enter -1 to stop.
//
// Key patterns in this file:
// 1) Sentinel loop: read -> validate -> if sentinel stop -> process -> accumulate
// 2) Input validation: if (!(cin >> value)) exit with error
// 3) Accumulators: totals, averages, earnings
// 4) Tracking largest values: update max1/max2
// 5) Nested loops for patterns (hollow square)

#include <iostream>
#include <limits>

using std::cin;
using std::cout;
using std::endl;

/* ============================================================================
   Small helper: unified "invalid input" exit
   - Keeps main readable
   ========================================================================== */
int failInputAndExit() {
    cout << "Invalid input. Exiting.\n";
    return 1;
}

int main() {

    constexpr double kSentinel = -1.0;

    /* =========================================================================
       EX16: MPG per tankful + overall MPG
       Inputs:
       - gallons used (-1 to exit)
       - miles driven (-1 to exit)
       Output:
       - miles/gallon for each tank
       - overall milesTotal/gallonsTotal at the end
       Notes:
       - Using a sentinel simplifies loop exit
       - Must accumulate totals for overall MPG
       ======================================================================= */
    double milesTotal = 0.0;
    double gallonsTotal = 0.0;

    while (true) {
        double gallons = 0.0;
        cout << "EX16 Enter gallons used (-1 to exit): ";
        if (!(cin >> gallons)) return failInputAndExit();
        if (gallons == kSentinel) break;

        double miles = 0.0;
        cout << "EX16 Enter miles driven (-1 to exit): ";
        if (!(cin >> miles)) return failInputAndExit();
        if (miles == kSentinel) break;

        cout << "EX16 Tank MPG: " << (miles / gallons) << "\n";

        milesTotal += miles;
        gallonsTotal += gallons;
    }

    if (gallonsTotal > 0.0) {
        cout << "EX16 Overall MPG: " << (milesTotal / gallonsTotal) << "\n\n";
    } else {
        cout << "EX16 Overall MPG: N/A (no valid tankful data)\n\n";
    }

    /* =========================================================================
       EX17: Department store credit limit check
       Inputs per customer:
       - account number (int) (-1 to exit)
       - beginning balance
       - total charges
       - total credits
       - credit limit
       Computation:
       - newBalance = beginningBalance + totalCharges - totalCredits
       Output:
       - if newBalance > creditLimit -> print warning and details
       ======================================================================= */
    while (true) {
        int accountNumber = 0;
        cout << "EX17 Enter account number (-1 to exit): ";
        if (!(cin >> accountNumber)) return failInputAndExit();
        if (accountNumber == static_cast<int>(kSentinel)) break;

        double beginningBalance = 0.0;
        cout << "EX17 Enter beginning balance: ";
        if (!(cin >> beginningBalance)) return failInputAndExit();

        double totalCharges = 0.0;
        cout << "EX17 Enter total charges: ";
        if (!(cin >> totalCharges)) return failInputAndExit();

        double totalCredits = 0.0;
        cout << "EX17 Enter total credits: ";
        if (!(cin >> totalCredits)) return failInputAndExit();

        double creditLimit = 0.0;
        cout << "EX17 Enter credit limit: ";
        if (!(cin >> creditLimit)) return failInputAndExit();

        double newBalance = beginningBalance + totalCharges - totalCredits;

        if (newBalance > creditLimit) {
            cout << "EX17 Account: " << accountNumber << "\n";
            cout << "EX17 Credit limit: " << creditLimit << "\n";
            cout << "EX17 New balance: " << newBalance << "\n";
            cout << "EX17 Credit limit exceeded.\n\n";
        } else {
            cout << "EX17 Within credit limit.\n\n";
        }
    }

    /* =========================================================================
       EX18: Salesperson earnings
       Rule:
       - earnings = 200 + 0.09 * sales
       Sentinel:
       - sales = -1 to exit
       ======================================================================= */
    while (true) {
        double sales = 0.0;
        cout << "EX18 Enter sales in dollars (-1 to exit): ";
        if (!(cin >> sales)) return failInputAndExit();
        if (sales == kSentinel) break;

        double earnings = 200.0 + (0.09 * sales);
        cout << "EX18 Salary is: " << earnings << "\n\n";
    }

    /* =========================================================================
       EX19: Gross pay with overtime
       Inputs:
       - hours worked (-1 to exit)
       - hourly pay rate
       Rule:
       - <= 40 hours: hours * pay
       - > 40 hours: 40*pay + (hours-40)*pay*1.5
       Note:
       - Stored as double for consistency
       ======================================================================= */
    while (true) {
        double hours = 0.0;
        cout << "EX19 Enter hours worked (-1 to exit): ";
        if (!(cin >> hours)) return failInputAndExit();
        if (hours == kSentinel) break;

        double pay = 0.0;
        cout << "EX19 Enter pay rate ($00.00): ";
        if (!(cin >> pay)) return failInputAndExit();

        if (hours > 40.0) {
            double overtimeHours = hours - 40.0;
            double grossPay = (40.0 * pay) + (overtimeHours * pay * 1.5);
            cout << "EX19 Salary is: " << grossPay << "\n\n";
        } else {
            cout << "EX19 Salary is: " << (hours * pay) << "\n\n";
        }
    }

    /* =========================================================================
       EX20: Track the two largest numbers from up to 10 entries
       - Reads 10 numbers (or stops early if -1)
       - Tracks largest and second largest
       Pattern:
       - if (n > max1) { max2 = max1; max1 = n; }
       - else if (n > max2) { max2 = n; }
       ======================================================================= */
    int count = 1;
    double max1 = std::numeric_limits<double>::lowest();
    double max2 = std::numeric_limits<double>::lowest();

    while (count <= 10) {
        double n = 0.0;
        cout << "EX20 Enter a number (-1 to exit): ";
        if (!(cin >> n)) return failInputAndExit();
        if (n == kSentinel) break;

        if (n > max1) {
            max2 = max1;
            max1 = n;
        } else if (n > max2) {
            max2 = n;
        }

        count++;
    }

    if (max1 != std::numeric_limits<double>::lowest()) {
        cout << "EX20 Two largest numbers: " << max1 << " and " << max2 << "\n\n";
    } else {
        cout << "EX20 No valid numbers entered.\n\n";
    }

    /* =========================================================================
       EX28: Print a hollow square of size N using '*'
       Example N = 5:
       *****
       *   *
       *   *
       *   *
       *****
       Rules:
       - First row and last row: all '*'
       - Middle rows: '*' + spaces + '*'
       ======================================================================= */
    int size = 0;
    cout << "EX28 Enter size of square: ";
    if (!(cin >> size)) return failInputAndExit();

    if (size <= 0) {
        cout << "EX28 Invalid size. Must be > 0\n";
    } else {
        cout << "EX28 Hollow square:\n";
        for (int row = 0; row < size; row++) {
            if (row == 0 || row == size - 1) {
                for (int col = 0; col < size; col++) cout << '*';
            } else {
                cout << '*';
                for (int col = 1; col < size - 1; col++) cout << ' ';
                if (size > 1) cout << '*';
            }
            cout << '\n';
        }
    }

    return 0;
}

