/*
===============================================================================
file_name : topic_new_delete.cpp
topic     : new and delete operators in C++

Mental Model (C analogy):
- malloc/free => allocate/deallocate raw memory
- new/delete  => allocate/deallocate raw memory + ALSO run ctor/dtor (lifetime)

Core rules (what we proved):
1) new T(...) / new T{...}
   - Allocates raw memory for a T
   - Constructs the T in that memory (constructor runs)
   - Returns a T* to the constructed object

2) delete ptr
   - Calls destructor of *ptr (if ptr != nullptr)
   - Deallocates the memory previously allocated by new

3) new[] must pair with delete[]
   - new T[ N ]  => constructs N objects
   - delete[] p  => destructs N objects (reverse order), then deallocates

4) Mismatching delete forms is UNDEFINED BEHAVIOR (UB)
   - new[] with delete (no []) is UB
   - new with delete[] is UB
   UB can:
     - call only 1 destructor
     - call the wrong destructors
     - crash
     - appear to work (most dangerous)

5) delete does NOT null your pointer; deleting nullptr is safe
   - After delete p;  p still holds the old address (dangling if not reset)
   - delete nullptr; is a no-op and safe

6) Why RAII exists (concept):
   - If new succeeds but you return/throw before delete, you leak.
   - Manual new/delete becomes fragile with many exit paths.
   - RAII ties cleanup to scope lifetime (destructor runs automatically).
   - Smart pointers are RAII wrappers for heap memory (later topic).

Checklist (done):
[X] new allocates + constructs
[X] delete destructs + deallocates
[X] delete doesn’t set pointer to null automatically
[X] nullptr and deleting null
[X] new[] / delete[] pairing rules
[X] What happens on mismatched delete (new[] with delete, etc.)
[X] Accessing freed memory (dangling pointer) demo
[X] Memory leak demo
[X] Basic exception-safety intuition (what if something throws after new)
[X] Why RAII / smart pointers exist (only concept, later topic)
===============================================================================
*/

#include <iostream>
using std::cout;
using std::endl;

class Dumb {
private:
    static int counter;       // shared across all Dumb objects (used to create unique IDs)
    static int aliveCounter;  // tracks how many Dumb objects are currently alive
    int x = 0;                // for this learning file: either an ID (default ctor) or payload (int ctor)

public:
    // Default constructor: assigns a unique ID using the shared counter
    Dumb() {
        x = ++counter;
        ++aliveCounter;
        cout << "Dumb default constructed with x = " << x << endl;
    }

    // Value constructor: uses x as a payload value (note: different meaning than default ctor)
    Dumb(int val) : x(val) {
        ++aliveCounter;
        cout << "Dumb constructed with value: " << x << endl;
    }

    ~Dumb() {
        cout << "Dumb destructed with value: " << x << endl;
        --aliveCounter;
    }

    static int getAliveCounter() { return aliveCounter; }

    int getValue() const { return x; }
};

// Static member definitions (required)
int Dumb::counter = 0;
int Dumb::aliveCounter = 0;

int main() {
    /* -----------------------------------------------------------------------
       Phase 1: Proving “new constructs, delete destructs”
       ---------------------------------------------------------------------*/
    Dumb* ptr = new Dumb(200);
    delete ptr;

    /*
      Observed:
        Dumb constructed with value: 200
        Dumb destructed with value: 200
      Conclusion:
        new => constructor runs
        delete => destructor runs
    */

    /* -----------------------------------------------------------------------
       Phase 2: delete doesn’t null your pointer; deleting nullptr is safe
       ---------------------------------------------------------------------*/
    if (ptr == nullptr) {
        cout << "ptr is null after delete\n";
    } else {
        cout << "ptr is NOT null after delete (dangling pointer)\n";
    }

    // Nulling doesn’t “fix” freed memory; it prevents accidental use via this pointer.
    ptr = nullptr;

    if (ptr == nullptr) {
        cout << "ptr is null after nulling\n";
    } else {
        cout << "ptr is NOT null after nulling\n";
    }

    cout << "Attempting to delete a nullptr...\n";
    delete ptr; // safe no-op
    cout << "Deleting nullptr did NOT cause a crash (safe in C++)\n";

    /* -----------------------------------------------------------------------
       Phase 3: new[] vs delete[] (pairing rules)
       ---------------------------------------------------------------------*/
    Dumb* arr = new Dumb[3];  // default ctor runs 3 times
    delete[] arr;             // dtor runs 3 times in reverse order

    /* -----------------------------------------------------------------------
       Phase 4: Mismatched delete (UB) -- kept SAFE by default
       ---------------------------------------------------------------------*/
    Dumb* arr2 = new Dumb[2];

    // MISMATCH DEMO (UNDEFINED BEHAVIOR):
    // If you change the next line to: delete arr2;  (no [])
    // behavior is UNDEFINED and may destroy only one element, crash, etc.
    // delete arr2; // INCORRECT (UB): mismatched delete form

    delete[] arr2; // CORRECT pairing

    /* -----------------------------------------------------------------------
       Phase 5: Memory leak demo (safe + deterministic)
       ---------------------------------------------------------------------*/
    Dumb* leakPtr = new Dumb(300);
    // We allocated and kept the pointer, but we intentionally never called delete,
    // so the object remains alive at program exit => leak.
    cout << "Current alive Dumb objects: " << Dumb::getAliveCounter() << endl;

    /* -----------------------------------------------------------------------
       Phase 6: Dangling pointer access demo (UB, but educational)
       ---------------------------------------------------------------------*/
    Dumb* danglingPtr = new Dumb(400);
    delete danglingPtr; // danglingPtr now points to freed storage (dangling)

    cout << "Attempting to access dangling pointer...\n";
    // UNDEFINED BEHAVIOR: may print old value, garbage (like 0), crash,
    // or differ by build/optimizer/run. Do NOT rely on this.
    cout << "Dangling pointer value: " << danglingPtr->getValue() << endl;

    // Mitigation pattern: set to nullptr after delete and guard before use
    danglingPtr = nullptr;
    cout << "Dangling pointer is now null, safe to check:\n";
    if (danglingPtr == nullptr) {
        cout << "danglingPtr is null after nulling\n";
    } else {
        cout << "danglingPtr is NOT null after nulling\n";
    }

    return 0;
}

/*
===============================================================================
Exception-safety intuition (comments only):

- If new succeeds but an early return / exception happens before delete,
  the allocated object is leaked.

- Manual new/delete is fragile when there are multiple exit paths.

RAII intuition (comments only):

- RAII = Resource Acquisition Is Initialization
- Wrap ownership of resources (heap memory, file handles, locks, sockets) inside
  objects whose destructors release them automatically.
- Smart pointers are RAII wrappers for heap memory (we’ll learn later).
===============================================================================
*/
