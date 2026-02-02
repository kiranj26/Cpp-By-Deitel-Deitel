/************************************************************
 * File: exercise_7.cpp
 *
 * Goal:
 *  - Implement Rational (fraction) arithmetic using a class.
 *
 * Requirements:
 *  1) Constructor should always store fraction in reduced form
 *     Example: 2/4 -> 1/2
 *  2) Provide member functions for:
 *       add, sub, mul, div
 *  3) Print:
 *       - rational form
 *       - floating-point form
 *
 * Key C++ Learnings:
 *  - Using int (NOT float) for exact fraction math
 *  - Enforcing invariants via a private normalize() function
 *  - Euclid's algorithm for gcd
 *  - const member functions for printing (read-only functions)
 *
 * Design Choice:
 *  - Mutable style (like your Complex exercise):
 *      add/sub/mul/div modify the current object in-place
 ************************************************************/

#include <iostream>
#include <iomanip>
#include <cmath>

using std::cout;
using std::endl;

class Rational {
private:
    /*
     * Internal representation:
     *    numerator / denominator
     *
     * Invariants (ALWAYS true after construction / operations):
     *  1) denominator != 0
     *  2) denominator > 0 (sign kept in numerator)
     *  3) fraction is reduced (gcd(|num|,|den|) == 1)
     *  4) if numerator == 0, store as 0/1
     */
    int numerator;
    int denominator;

    /*
     * normalize():
     *  - Enforces invariants after any state change.
     *  - Called in constructor + after every arithmetic op.
     */
    void normalise(void);

    /*
     * gcd(a,b):
     *  - Euclid algorithm
     *  - returns greatest common divisor
     *  - inputs expected non-negative
     */
    int GCD(int a, int b);

public:
    /*
     * Constructor:
     *  - initializes raw values
     *  - then normalise() forces the invariants
     */
    Rational(int num = 1, int den = 1)
        : numerator(num), denominator(den)
    {
        normalise();
    }

    /*
     * Mutable arithmetic:
     *  - These modify numerator/denominator directly
     *  - Then normalise() to restore reduced canonical form
     */
    void add(int num, int den);
    void sub(int num, int den);
    void mul(int num, int den);
    void div(int num, int den);

    /*
     * Printing:
     *  - const because they do not modify object state
     */
    void printRationalForm(void) const;
    void printFloatingPointForm(void) const;
};

void Rational::add(int a, int b) {
    /*
     * a/b is the incoming rational number.
     * current object is numerator/denominator.
     *
     * Formula:
     *   n/d + a/b = (n*b + d*a) / (d*b)
     */
    numerator   = (numerator * b) + (denominator * a);
    denominator = (denominator * b);
    normalise();
}

void Rational::sub(int a, int b) {
    /*
     * Formula:
     *   n/d - a/b = (n*b - d*a) / (d*b)
     */
    numerator   = (numerator * b) - (denominator * a);
    denominator = (denominator * b);
    normalise();
}

void Rational::mul(int a, int b) {
    /*
     * Formula:
     *   n/d * a/b = (n*a) / (d*b)
     */
    numerator   = numerator * a;
    denominator = denominator * b;
    normalise();
}

void Rational::div(int a, int b) {
    /*
     * Division:
     *   (n/d) ÷ (a/b) = (n/d) * (b/a)
     *
     * Trap:
     *   dividing by 0 is invalid -> if a == 0, reject
     */
    if (a == 0) {
        cout << "Division by zero fraction not allowed (a == 0). Ignoring.\n";
        return;
    }

    numerator   = numerator * b;
    denominator = denominator * a;
    normalise();
}

void Rational::normalise(void) {

    // Rule 1: denominator must never be 0
    if (denominator == 0) {
        cout << "Error: denominator became 0. Forcing denominator to 1.\n";
        denominator = 1;
    }

    // Rule 4: canonical zero: 0/x -> 0/1
    if (numerator == 0) {
        denominator = 1;
        return;
    }

    // Rule 2: denominator must be positive (move sign to numerator)
    if (denominator < 0) {
        numerator   = -numerator;
        denominator = -denominator;
    }

    // Rule 3: reduce using gcd
    int div_by = GCD(std::abs(numerator), std::abs(denominator));
    numerator   /= div_by;
    denominator /= div_by;
}

int Rational::GCD(int a, int b) {
    /*
     * Euclid's algorithm:
     * gcd(a,b):
     * while b != 0:
     *   (a,b) = (b, a%b)
     * return a
     */
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void Rational::printRationalForm(void) const {
    // Simple fraction display
    cout << "Rational: (" << numerator << "/" << denominator << ")\n";
}

void Rational::printFloatingPointForm(void) const {
    // Must cast to avoid integer division
    cout << "Float: " << std::fixed
         << static_cast<double>(numerator) / denominator
         << "\n";
}

int main() {

    auto show = [](const char* label, const Rational& r) {
        cout << "\n=== " << label << " ===\n";
        r.printRationalForm();
        r.printFloatingPointForm();
    };

    Rational r1(2, 4);
    show("Construct 2/4 (expect 1/2)", r1);

    Rational r2(1, -2);
    show("Construct 1/-2 (expect -1/2)", r2);

    Rational r3(-1, -2);
    show("Construct -1/-2 (expect 1/2)", r3);

    Rational r4(0, 5);
    show("Construct 0/5 (expect 0/1)", r4);

    Rational r5(1, 2);
    r5.add(1, 2);
    show("1/2 + 1/2 (expect 1/1)", r5);

    Rational r6(3, 4);
    r6.sub(1, 2);
    show("3/4 - 1/2 (expect 1/4)", r6);

    Rational r7(2, 3);
    r7.mul(9, 4);
    show("2/3 * 9/4 (expect 3/2)", r7);

    Rational r8(3, 5);
    r8.div(9, 10);
    show("3/5 ÷ 9/10 (expect 2/3)", r8);

    Rational r9(1, 2);
    r9.div(0, 7);
    show("1/2 ÷ 0/7 (expect unchanged + error)", r9);

    Rational r10(5, 0);
    show("Construct 5/0 (expect error + safe)", r10);

    return 0;
}
