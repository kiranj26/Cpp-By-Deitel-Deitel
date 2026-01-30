/*******************************************************
 * File: function_pointers.cpp
 *
 * Topic: Function Pointers in C++ (Up to Step 5)
 *
 * Purpose:
 * --------
 * This file is a revision-friendly walkthrough of
 * function pointers in C++, written from a C background
 * perspective.
 *
 * Covered Concepts:
 * -----------------
 * 1. What a function pointer is
 * 2. Declaring and using function pointers
 * 3. Reassigning function pointers
 * 4. Using typedef / using for readability
 * 5. Passing function pointers as arguments (callbacks)
 * 6. Returning function pointers from functions
 * 7. Arrays of function pointers (dispatch table)
 *
 * Mental Model:
 * -------------
 * - A function pointer stores the ADDRESS of a function
 * - Calling through the pointer results in an INDIRECT CALL
 * - Function names decay to function pointers (similar
 *   to how array names decay to pointers in expressions)
 *******************************************************/

#include <iostream>
using namespace std;

/*-----------------------------------------------------
  Simple arithmetic functions
  These are NORMAL functions living in the code section.
  Their addresses can be stored in function pointers.
-----------------------------------------------------*/

int add(int a, int b) {
    return a + b;
}

int sub(int a, int b) {
    return a - b;
}

int mult(int a, int b) {
    return a * b;
}

int safediv(int a, int b) {
    // defensive divide to avoid crash
    return (b == 0) ? 0 : (a / b);
}

/*-----------------------------------------------------
  Function pointer type alias
-------------------------------------------------------
  Read this as:
  "Op is a pointer to a function that:
   - takes (int, int)
   - returns int"
-----------------------------------------------------*/

using Op = int (*)(int, int);

/*-----------------------------------------------------
  Passing a function pointer to another function
  (Callback pattern)
-------------------------------------------------------
  apply() does NOT know which operation it performs.
  The behavior is injected at runtime using a pointer.
-----------------------------------------------------*/

int apply(int x, int y, Op op) {
    return op(x, y);   // indirect function call
}

/*-----------------------------------------------------
  Function pointer used multiple times
-------------------------------------------------------
  apply3():
  1) op(x, y) is evaluated first
  2) result is then passed again to op()
-----------------------------------------------------*/

int apply3(int x, int y, int z, Op op) {
    return op(op(x, y), z);
}

/*-----------------------------------------------------
  Function returning a function pointer
-------------------------------------------------------
  choose():
  - returns different function ADDRESSES
  - caller decides when/how to invoke the function
-----------------------------------------------------*/

Op choose(char symbol) {
    if (symbol == '+') return add;
    if (symbol == '-') return sub;
    if (symbol == '*') return mult;
    if (symbol == '/') return safediv;
    return nullptr;   // invalid choice safety
}

int main() {

    /*-------------------------------------------------
      STEP 1: Simple function pointer
    ---------------------------------------------------
      fptr stores the ADDRESS of function add().
      add decays to &add in this context.
    -------------------------------------------------*/

    int (*fptr)(int, int) = add;

    // direct function call
    cout << add(2, 3) << endl;

    // indirect function call via pointer
    cout << fptr(2, 3) << endl;

    /*-------------------------------------------------
      Reassigning the function pointer
    ---------------------------------------------------
      Now fptr points to a DIFFERENT function.
      Same pointer, different behavior.
    -------------------------------------------------*/

    fptr = sub;
    cout << fptr(7, 3) << endl;

    /*-------------------------------------------------
      STEP 2: Using type alias (Op)
    ---------------------------------------------------
      Makes function pointer usage readable and safe.
    -------------------------------------------------*/

    Op op = mult;
    cout << op(2, 2) << endl;

    op = safediv;
    cout << op(10, 2) << endl;

    /*-------------------------------------------------
      STEP 3: Passing function pointer as argument
    ---------------------------------------------------
      This is a classic CALLBACK mechanism.
    -------------------------------------------------*/

    cout << apply(12, 2, safediv) << endl;
    cout << apply3(2, 5, 10, add) << endl;

    /*-------------------------------------------------
      STEP 4: Function returning function pointer
    ---------------------------------------------------
      Behavior is selected at runtime.
    -------------------------------------------------*/

    op = choose('+');
    if (op != nullptr) {
        cout << op(2, 19) << endl;
    }

    /*-------------------------------------------------
      STEP 5: Array of function pointers
    ---------------------------------------------------
      This acts like a DISPATCH TABLE.
      Common in:
        - embedded systems
        - protocol handlers
        - command interpreters
    -------------------------------------------------*/

    Op arr[] = { add, sub, mult, safediv };

    int a = 19, b = 11;

    cout << "add: " << arr[0](a, b) << endl;
    cout << "sub: " << arr[1](a, b) << endl;
    cout << "mul: " << arr[2](a, b) << endl;
    cout << "div: " << arr[3](a, b) << endl;

    /*-------------------------------------------------
      Loop-based dispatch (table-driven logic)
    -------------------------------------------------*/

    for (int i = 0; i < 4; i++) {
        cout << "arr[" << i << "](20,4) = "
             << arr[i](20, 4) << endl;
    }

    return 0;
}
