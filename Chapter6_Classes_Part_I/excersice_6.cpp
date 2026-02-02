/*******************************************************
 * File: exercise_6.cpp
 *
 * Topic:
 *  - Basic C++ class design
 *  - Constructor with member initialization list
 *  - Mutable object behavior (in-place modification)
 *  - const-correctness for read-only member functions
 *
 * Problem Statement:
 *  Create a class Complex that:
 *   - Stores a complex number (real, imaginary)
 *   - Allows addition and subtraction of values
 *   - Prints the complex number in the form (a,b)
 *
 * Design Choice (IMPORTANT):
 *  - add() and sub() MODIFY the object itself
 *  - No return value from add/sub
 *  - print() only reads object state → marked const
 *******************************************************/

#include <iostream>
using std::cout;
using std::endl;

class Complex {

private:
    /*
     * Internal state of the object.
     * A complex number is represented as:
     *   real + imaginary * i
     *
     * NOTE:
     * We DO NOT store sqrt(-1).
     * The object only stores numeric coefficients.
     */
    float real;
    float imaginary;

public:
    /*
     * Constructor
     *
     * Uses MEMBER INITIALIZATION LIST instead of assignment.
     *
     * Why?
     *  - Members are initialized directly (not default + assign)
     *  - Required for const members and references
     *  - Preferred and idiomatic C++
     *
     * Default arguments allow:
     *   Complex c;        → (0,0)
     *   Complex c(1,2);   → (1,2)
     */
    Complex(float r = 0, float i = 0)
        : real(r), imaginary(i) {}

    /*
     * add()
     *
     * Adds values to the EXISTING complex number.
     * This function MUTATES the object.
     *
     * Example:
     *   (1,2).add(3,4) → (4,6)
     *
     * Return type is void because:
     *  - Object is modified in place
     *  - Caller can inspect state via print()
     */
    void add(float a, float b);

    /*
     * sub()
     *
     * Subtracts values from the EXISTING complex number.
     *
     * Example:
     *   (4,6).sub(1,2) → (3,4)
     */
    void sub(float a, float b);

    /*
     * print()
     *
     * Prints the current state of the object.
     *
     * Marked const because:
     *  - Does NOT modify real or imaginary
     *  - Enforces read-only behavior at compile time
     *
     * Format requirement: (a,b)
     */
    void print(void) const;
};

/* ===== Function Definitions ===== */

void Complex::add(float r, float i) {
    real += r;
    imaginary += i;
}

void Complex::sub(float r, float i) {
    real -= r;
    imaginary -= i;
}

void Complex::print(void) const {
    cout << "(" << real << ", " << imaginary << ")" << endl;
}

int main() {

    /*
     * Create a complex number initialized to (1,2)
     */
    Complex c1(1.0, 2.0);
    c1.print();      // (1,2)

    /*
     * Add (3,1.9) to existing object
     * Object state changes permanently
     */
    c1.add(3.0, 1.9);
    c1.print();      // (4,3.9)

    /*
     * Subtract (3,1.9)
     */
    c1.sub(3.0, 1.9);
    c1.print();      // (1,2)

    return 0;
}
