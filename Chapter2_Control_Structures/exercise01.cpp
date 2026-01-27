// File: chapter2_control_structures_notes.cpp
// Purpose: Revision notes + runnable examples (increments, loops, powers, sums, i/o formatting)
//
// How to use later:
// - Search "EX" to jump to the exercise.
// - Each block is independent and prints output with a clear label.
// - Stream formatting notes included where relevant.

#include <iostream>
#include <iomanip>   // setw, setprecision, fixed, left, showpoint
#include <cmath>     // pow

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::setprecision;
using std::fixed;
using std::left;
using std::showpoint;

/* ============================================================================
   Helper: Integer power using a loop (x^y) for y >= 0
   - Avoids floating math in exercises that want loop logic
   ========================================================================== */
int intPower(int x, int y) {
    int power = 1;
    int i = 1;
    while (i <= y) {
        power *= x;
        ++i;
    }
    return power;
}

int main() {

    /* =========================================================================
       EX 1 & EX 2: Four different statements that add 1 to variable var
       - All below change var (not just compute var+1)
       ======================================================================= */
    int var;

    var = 1;
    var = var + 1;        // 1) assignment form
    cout << "EX1/2 var = var + 1 : " << var << endl;

    var = 1;
    var++;                // 2) post-increment
    cout << "EX1/2 var++        : " << var << endl;

    var = 1;
    ++var;                // 3) pre-increment
    cout << "EX1/2 ++var        : " << var << endl;

    var = 1;
    var += 1;             // 4) compound assignment
    cout << "EX1/2 var += 1     : " << var << "\n\n";

    /* =========================================================================
       EX 3: Create sum and x, add x to sum, print
       ======================================================================= */
    int sum = 0;
    int x = 1;

    sum = sum + x;
    cout << "EX3 New Sum = " << sum << "\n\n";

    /* =========================================================================
       EX 4: Sum of numbers from 1 to 10 using a while loop
       ======================================================================= */
    sum = 0;
    x = 1;

    while (x <= 10) {
        sum = sum + x;
        x++;
    }

    cout << "EX4 Sum from 1 to 10 is: " << sum << "\n\n";

    /* =========================================================================
       EX 6: Compound assignment with pre/post increment
       - x++ uses x first, then increments
       - ++x increments first, then uses x
       ======================================================================= */
    x = 5;
    int product = 5;
    int question = 5;

    product *= x++;     // product = product * (old x), then x = x + 1
    cout << "EX6 product *= x++  -> product = " << product << ", x = " << x << endl;

    question /= ++x;    // x = x + 1 first, then question = question / x
    cout << "EX6 question /= ++x -> question = " << question << ", x = " << x << "\n\n";

    /* =========================================================================
       EX 7 & EX 8: x to the power of y using a while loop
       - User input
       - Uses integer multiplication (no pow) to practice loops
       ======================================================================= */
    int y = 0;

    cout << "EX7/8 Input X: ";
    cin >> x;
    cout << "EX7/8 Input Y: ";
    cin >> y;

    int power = intPower(x, y);
    cout << "EX7/8 " << x << " to the power of " << y << " is: " << power << "\n\n";

    /* =========================================================================
       EX 12.a: Sum of odd numbers between 1 and 99 (inclusive) using for loop
       ======================================================================= */
    sum = 0;
    for (int i = 1; i <= 99; i++) {
        if ((i % 2) != 0) {
            sum += i;
        }
    }
    cout << "EX12.a Sum of odd numbers between 1 and 99: " << sum << "\n\n";

    /* =========================================================================
       EX 12.b: I/O formatting
       Requirement:
       - Print 333.546371 in field width 15
       - Print with precision 0, 1, 2, 3, 4 on the same line
       Notes:
       - fixed is persistent once set
       - setprecision is persistent until changed
       - setw applies ONLY to the next value, so repeat it
       ======================================================================= */
    double value = 333.546371;

    cout << "EX12.b formatted: " << endl;
    cout << fixed << left << showpoint;
    cout << setw(15) << setprecision(0) << value
         << setw(15) << setprecision(1) << value
         << setw(15) << setprecision(2) << value
         << setw(15) << setprecision(3) << value
         << setw(15) << setprecision(4) << value << "\n\n";

    /* =========================================================================
       EX 12.c: pow examples + formatting
       ======================================================================= */
    cout << "EX12.c pow(2.5, 3) raw: " << std::pow(2.5, 3) << endl;
    cout << "EX12.c pow formatted  : " << setw(10) << fixed << setprecision(2)
         << std::pow(2.5, 3) << "\n\n";

    /* =========================================================================
       EX 12.d: Print 1 to 20 using while loop, 5 numbers per line
       ======================================================================= */
    cout << "EX12.d while (1..20, 5 per line):" << endl;
    int p = 1;
    while (p <= 20) {
        if (p % 5 == 0) {
            cout << p << "\n";
        } else {
            cout << p << "\t";
        }
        p++;
    }
    cout << "\n";

    /* =========================================================================
       EX 12.e: Print 1 to 20 using for loop, 5 numbers per line
       ======================================================================= */
    cout << "EX12.e for (1..20, 5 per line):" << endl;
    for (int q = 1; q <= 20; q++) {
        if (q % 5 == 0) {
            cout << q << "\n";
        } else {
            cout << q << "\t";
        }
    }

    return 0;
}
