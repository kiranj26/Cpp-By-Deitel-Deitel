#include <iostream>
using namespace std;

/*
===============================================================================
Topic : const keyword in C++

Core idea:
const -> "this thing cannot be changed THROUGH THIS NAME / ACCESS PATH"

Key principles:
- const does NOT own the object
- const does NOT make memory globally read-only
- const only restricts how THIS identifier can be used
- the underlying object may still be modified through a non-const access path
===============================================================================
*/

/*
-------------------------------------------------------------------------------
1) const variable

Promise to the compiler:
- Through this identifier, no modification will be performed
- Violating this promise is NOT a runtime bug
- It is a contract violation -> compile-time error

Important:
- A const variable MUST be initialized
- Since future assignment is forbidden, the value must be valid at birth
- The value may still change indirectly via any non-const alias
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
2) const with pointers

Always separate the two things in a pointer declaration:
- p   -> the pointer itself (where it points)
- *p  -> the data being pointed to

const can apply to either one (or both)
-------------------------------------------------------------------------------

Pointer patterns (conceptual):
- const T* p        -> data is const, pointer can move
- T* const p        -> pointer is const, data can change
- const T* const p  -> both pointer and data are const
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
3) const references

Reference property (always true):
- A reference cannot be reseated once bound
  (once it refers to an object, it cannot refer to a different one later)

So what does const do here?
- Since reseating is already impossible, const on a reference only restricts
  modification of the referred object THROUGH THIS REFERENCE PATH.

Examples (conceptual):
- int& r        -> can modify the referred object through r
- const int& r  -> cannot modify the referred object through r (read-only view)

Why const T& is extremely common in APIs:
- Avoid copying large objects (performance)
- Express intent: "this function will not modify the object"
- Let the compiler enforce that promise
- Can accept both const and non-const arguments (more flexible API)
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
4) const member functions in classes

Core idea:
- Every non-static member function has an implicit pointer: `this`
- In a non-const member function, `this` behaves like: ClassName*
- In a const member function, `this` behaves like: const ClassName*

What const on a member function promises:
- The function will not modify the object's observable state
- It can only call other const member functions
- The compiler enforces this rule

Why this matters:
- Enables read-only APIs
- Allows const objects to safely expose getters
- Prevents accidental state modification

Rule:
- A const object can ONLY call const member functions
- Calling a non-const member function on a const object is a compile-time error
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
const object vs member function compatibility table

Legend:
- OK     : allowed by the compiler
- ERROR  : compile-time error

+-------------------+--------------------------+-----------------------------+
| Object type       | Non-const member function| Const member function       |
+-------------------+--------------------------+-----------------------------+
| Non-const object  | OK                       | OK                          |
| Const object      | ERROR                    | OK                          |
+-------------------+--------------------------+-----------------------------+

Explanation:

1) Non-const object -> non-const member function
   - Allowed because the object is mutable
   - Function may modify internal state

2) Non-const object -> const member function
   - Allowed
   - Const member function promises NOT to modify the object
   - Reading state is always safe

3) Const object -> non-const member function
   - NOT allowed
   - Non-const member function may modify state
   - Violates the const promise of the object

4) Const object -> const member function
   - Allowed
   - Both sides agree: read-only access

Key mental model:
- Constness is checked from the OBJECT'S point of view
- A const object can ONLY see const-qualified member functions
-------------------------------------------------------------------------------
*/

/*
-------------------------------------------------------------------------------
5) const in function parameters & return types

A) const in function parameters
- If a function only needs to READ an object, prefer: const T&
  Why?
  - Avoids copying large objects (performance)
  - Expresses intent: "this function will not modify your object"
  - Compiler enforces the promise
  - Accepts const objects, non-const objects, and temporaries (flexible APIs)

Rule of thumb:
- Read-only parameter -> const T&
- Writable parameter  -> T& (or pointer, depending on design)

B) const on return-by-value (usually useless)
- If a function returns T by value, the caller gets a copy.
- That returned object belongs to the caller.
- Returning "const T" by value usually doesn't protect anything meaningful and
  is generally avoided in modern C++.

C) const on returned references/pointers (matters a lot)
- If a function returns a reference/pointer, it is returning access to an
  existing object.
- Returning const T& or const T* prevents the caller from mutating that object
  through the returned access path.
- Useful for encapsulation and protecting internal state/invariants.
-------------------------------------------------------------------------------
*/

class Counter
{
private:
    int value;

public:
    Counter(int v) : value(v) {}

    // Const member function: read-only access
    int getValue() const
    {
        return value;
    }

    // Non-const member function: modifies state
    void setValue(int v)
    {
        value = v;
    }

    // Const member function attempting to modify state (illegal)
    void tryModify() const
    {
        // value = 10;
        // ERROR: cannot assign to non-static data member inside a const member function
    }
};

int main()
{
    /*
    =========================
    Section 1: const variable
    =========================
    */

    // const int x;
    // ERROR: const variables must be initialized at declaration

    const int x = 5;
    // x = 10;  // ERROR: cannot modify a const variable

    cout << "Const variable x: " << x << endl;

    /*
    ==============================
    Section 2: const with pointers
    ==============================
    */

    int a = 10;
    const int b = 20;
    int z = 29;

    // Case 1: pointer to non-const data
    int* ptr1 = &a;
    *ptr1 = 20;      // OK
    ptr1 = &z;       // OK

    // Case 2: pointer to const data
    const int* ptr2 = &b;
    // *ptr2 = 100;  // ERROR: cannot modify through ptr2
    ptr2 = &z;       // OK
    cout << "Value through ptr2 after re-pointing: " << *ptr2 << endl;

    // Case 3: const pointer to non-const data
    int* const ptr3 = &a;
    *ptr3 = 40;      // OK
    // ptr3 = &z;    // ERROR: cannot re-point ptr3
    cout << "Value through ptr3: " << *ptr3 << endl;

    // Case 4: const pointer to const data
    const int* const ptr4 = &b;
    // *ptr4 = 3000; // ERROR
    // ptr4 = &z;    // ERROR

    /*
    Core mental model:
    const never changes the object itself.
    It only restricts access through a specific name/path.
    */

    /*
    ===============================
    Section 3: const with references
    ===============================
    */

    int m = 199;

    int& ref = m;
    ref = 39;
    cout << "m after modifying through ref: " << m << endl;

    const int& cref = m;
    // cref = 198;   // ERROR: cannot modify through const reference

    ref = 198;
    cout << "Value through cref after modifying m via non-const path: " << cref << endl;

    /*
    ===============================
    Section 4: const member functions
    ===============================
    */

    Counter obj(50);
    cout << "Value from non-const object: " << obj.getValue() << endl;

    obj.setValue(100);
    cout << "Value after setValue: " << obj.getValue() << endl;

    const Counter constObj(200);
    cout << "Value from const object: " << constObj.getValue() << endl;

    // constObj.setValue(300);
    // ERROR: const object cannot call non-const member function

    return 0;
}
