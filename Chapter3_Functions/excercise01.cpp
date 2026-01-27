// File: chapter2_notes.cpp
// Purpose: Compact revision notes + runnable mini-examples (math, loops, rand ranges, recursion, formatting)
//
// How to use this file later:
// - Search "EX" to jump to a topic.
// - Each EX block demonstrates one concept with a minimal example.
// - Functions above main are reusable helpers used by multiple exercises.

#include <iostream>
#include <iomanip>
#include <cmath>      // std::fabs, std::floor, std::ceil
#include <cstdlib>    // rand, srand
#include <ctime>      // time

using std::cin;
using std::cout;
using std::endl;
using std::setw;
using std::fixed;
using std::setprecision;
using std::left;

/* ============================================================================
   Helper 1: Integer power (iterative)
   - Computes base^exponent for exponent >= 0
   - Time: O(exponent)
   ========================================================================== */
int integerPower(int base, int exponent) {
    int result = 1;
    for (int i = 0; i < exponent; i++) {
        result *= base;
    }
    return result;
}

/* ============================================================================
   Helper 2: Integer power (recursive)
   - Same output as integerPower for exponent >= 0
   - Recursion depth = exponent
   ========================================================================== */
int integerPowerRecursive(int base, int exponent) {
    if (exponent == 0) {
        return 1;
    }
    return base * integerPowerRecursive(base, exponent - 1);
}

/* ============================================================================
   Helper 3: Print digits of an integer in normal order using recursion
   - Example: 1234 -> prints 1234
   - Key idea: recursive call prints higher digits first, then prints (n % 10)
   - Constraint: n >= 0 (exercise range 0..32767)
   ========================================================================== */
void printDecimal(int n) {
    if (n < 10) {
        cout << n;
        return;
    }
    printDecimal(n / 10);
    cout << (n % 10);
}

/* ============================================================================
   Helper 4: Print an NxN square of a chosen character
   - Reads the character from input each call
   ========================================================================== */
void printSquareOfCharacter(int side) {
    char ch = '*';
    cout << "Enter character for a " << side << "x" << side << " square: ";
    cin >> ch;

    for (int i = 0; i < side; i++) {
        for (int j = 0; j < side; j++) {
            cout << ch;
        }
        cout << '\n';
    }
    cout << '\n';
}

/* ============================================================================
   Helper 5 : Seconds elapsed since the last time the clock struck 12
   - Input time is in 12-hour format: HH:MM:SS
   - Hours allowed: 1..12 (12 maps to 0 elapsed hours)
   - Minutes/Seconds: 0..59
   ========================================================================== */
int secondsFromTwelve(int h, int m, int s) {
    // Normalize 12 -> 0 (because 12:xx:yy is the start of the cycle)
    if (h == 12) {
        h = 0;
    }
    return (h * 60 * 60) + (m * 60) + s;
}

/* ============================================================================
   Helper 6 : Validate 12-hour clock inputs
   ========================================================================== */
bool isValid12HourTime(int h, int m, int s) {
    if (h < 1 || h > 12) return false;
    if (m < 0 || m > 59) return false;
    if (s < 0 || s > 59) return false;
    return true;
}

int main() {

    /* =========================================================================
       EX1: fabs, floor, ceil (positive and negative behavior)
       - floor(x): greatest integer <= x
       - ceil(x):  smallest integer >= x
       ======================================================================= */
    cout << std::fabs(7.5) << endl;                 // 7.5
    cout << std::floor(7.5) << endl;                // 7
    cout << std::fabs(0.0) << endl;                 // 0
    cout << std::ceil(0.0) << endl;                 // 0
    cout << std::fabs(-6.4) << endl;                // 6.4
    cout << std::ceil(-6.4) << endl;                // -6
    cout << std::ceil(-(std::fabs(-8 + std::floor(-5.5)))) << endl;

    cout << "\n";

    /* =========================================================================
       EX16: Random integer generation in a closed range [a, b]
       Formula:
           n = a + rand() % (b - a + 1)
       Works for positive, negative, and mixed ranges.
       Example:
           -19 <= n <= 15
           count = 15 - (-19) + 1 = 35
       ======================================================================= */
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    {
        constexpr int a = -19;
        constexpr int b = 15;
        constexpr int range = (b - a + 1);

        cout << "EX16 random integers in [" << a << ", " << b << "]:" << endl;
        for (int i = 0; i < 20; i++) {
            int n = a + (std::rand() % range);
            cout << setw(5) << n << " ";
        }
        cout << "\n\n";
    }

    /* =========================================================================
       EX17: Random integer from a fixed set WITHOUT arrays/vectors (switch mapping)
       Set values (14 total):
           2, 4, 6, 8
           3, 5, 7, 9, 11
           6, 10, 14, 18, 22
       Method:
           r = rand() % 14
           switch(r) -> value
       ======================================================================= */
    {
        int r = std::rand() % 14;
        int value = 0;

        switch (r) {
            case 0:  value = 2;  break;
            case 1:  value = 4;  break;
            case 2:  value = 6;  break;
            case 3:  value = 8;  break;

            case 4:  value = 3;  break;
            case 5:  value = 5;  break;
            case 6:  value = 7;  break;
            case 7:  value = 9;  break;
            case 8:  value = 11; break;

            case 9:  value = 6;  break;
            case 10: value = 10; break;
            case 11: value = 14; break;
            case 12: value = 18; break;
            case 13: value = 22; break;
        }

        cout << "EX17 random value from fixed set: " << value << "\n\n";
    }

    /* =========================================================================
       EX18: base^exponent using iterative and recursive functions
       ======================================================================= */
    cout << "EX18 3^3 iterative  = " << integerPower(3, 3) << endl;
    cout << "EX18 3^3 recursive  = " << integerPowerRecursive(3, 3) << "\n\n";

    /* =========================================================================
       EX22: Print NxN square of a chosen character
       ======================================================================= */
    cout << "EX22 squares:\n";
    printSquareOfCharacter(3);
    printSquareOfCharacter(5);

    /* =========================================================================
       EX25: Print digits of a number (0..32767) in normal order using recursion
       Example input: 4562 -> output digits: 4562
       ======================================================================= */
    int num = 0;
    cout << "EX25 Enter number (0..32767): ";
    cin >> num;

    cout << "Digits: ";
    if (num < 0) {
        cout << "-";
        num = -num;
    }
    printDecimal(num);
    cout << endl;

    /* ============================================================================
      EX26: Read two times (HH:MM:SS within a 12-hour cycle) and compute:
      1) seconds since last 12 for each time
      2) elapsed seconds from time1 to time2 (wrapping across 12 if needed)
      ========================================================================== */
    constexpr int kCycleSeconds = 12 * 60 * 60; // 43200

    int h1 = 0, m1 = 0, s1 = 0;
    int h2 = 0, m2 = 0, s2 = 0;

    cout << "EX26 Enter time 1 (HH MM SS in 12-hour format): ";
    if (!(cin >> h1 >> m1 >> s1)) {
        cout << "Invalid input. Exiting.\n";
        return 1;
    }

    cout << "EX26 Enter time 2 (HH MM SS in 12-hour format): ";
    if (!(cin >> h2 >> m2 >> s2)) {
        cout << "Invalid input. Exiting.\n";
        return 1;
    }

    if (!isValid12HourTime(h1, m1, s1) || !isValid12HourTime(h2, m2, s2)) {
        cout << "Invalid time(s). Hours must be 1..12, minutes/seconds 0..59.\n";
        return 1;
    }

    int t1 = secondsFromTwelve(h1, m1, s1);
    int t2 = secondsFromTwelve(h2, m2, s2);

    int diff = t2 - t1;
    if (diff < 0) {
        diff += kCycleSeconds; // wrap within the same 12-hour cycle
    }

    cout << "Seconds since last 12 for time1: " << t1 << "\n";
    cout << "Seconds since last 12 for time2: " << t2 << "\n";
    cout << "Elapsed seconds from time1 -> time2: " << diff << "\n\n";

    return 0;
}

