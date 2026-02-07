/*
    file_name : topic_composition.cpp
    topic     : Composition in C++ Classes (HAS-A)

    Definition:
        Composition means one class *owns* other objects as its data members.
        Those member objects have lifetimes tied to the owning object:
            - Members are constructed as part of constructing the owner.
            - Members are destroyed automatically when the owner is destroyed.

    Key intuition:
        "Car has an Engine" (HAS-A)  !=  "Car is an Engine" (IS-A / inheritance)

    Non-negotiable C++ rules we proved:
        1) Member objects are constructed BEFORE the constructor body runs.
        2) Member construction order is based on MEMBER DECLARATION ORDER
           inside the class (top-to-bottom), NOT on:
              - the order of parameters passed to the constructor, or
              - the order written in the constructor initializer list.
        3) Destruction happens automatically when the owner dies (scope end),
           and member destruction order is the reverse of construction order.

    What I will build & prove (checklist):
        [x] Phase 1: Create Engine + Car (Engine is a direct member of Car).
        [x] Phase 2: Add another member (Transmission) and prove construction order.
        [x] Phase 3: Prove destruction order using scope blocks.
        [x] Phase 4: Composition vs aggregation (pointer/reference) — who owns lifetime?
        [ ] Phase 5: Copy/assign Car — what happens to members?
        [ ] Phase 6: Tricky members: const member, reference member — what rules change?

    Notes to future self:
        - "Constructing the Car object" includes constructing its members.
          The Car constructor body runs only after all members exist.
        - If initializer list order differs from member declaration order,
          the declaration order still wins (compilers often warn).
        - Aggregation (raw pointer/reference) is NOT ownership.
          It is your job to guarantee the pointee outlives the user.
*/

#include <iostream>
using std::cout;
using std::endl;

/* =========================
   Engine
   =========================
   - Used to demonstrate composition + aggregation.
   - Contains an ID so we can print which instance is alive/dying.
*/
class Engine {
  private:
    int engine_id;

  public:
    Engine(int id) : engine_id(id) {
        cout << "Engine Id [" << engine_id << "] Constructed\n";
    }

    Engine() : engine_id(0) {
        cout << "Engine Default Constructed\n";
    }

    ~Engine() {
        cout << "Engine Id [" << engine_id << "] Destroyed\n";
    }

    int getId() const {
        return engine_id;
    }
};

/* =========================
   Transmission
   ========================= */
class Transmission {
  private:
    int transmission_id;

  public:
    Transmission(int id) : transmission_id(id) {
        cout << "Transmission Id [" << transmission_id << "] Constructed\n";
    }

    Transmission() : transmission_id(0) {
        cout << "Transmission Default Constructed\n";
    }

    ~Transmission() {
        cout << "Transmission Id [" << transmission_id << "] Destroyed\n";
    }
};

/* =========================
   Car (COMPOSITION)
   =========================
   - Car OWNS Engine + Transmission as direct members (by value).
   - Their lifetimes are bound to Car's lifetime.
*/
class Car {
  private:
    Engine eng;           // composition (HAS-A): owned, lifetime bound
    Transmission trans;   // composition (HAS-A): owned, lifetime bound

  public:
    /* Phase 2 experiment:
       Initializer list is intentionally reversed (trans, then eng),
       but construction still follows member declaration order:
         eng first, then trans.
    */
    Car(int engine_id, int transmission_id)
        : trans(transmission_id), eng(engine_id) {
        cout << "Car constructed\n";
    }

    Car() {
        cout << "Car Default Constructed\n";
    }

    ~Car() {
        cout << "Car Destroyed\n";
    }
};

/* =========================
   CarBorrowed (AGGREGATION)
   =========================
   - CarBorrowed DOES NOT own the Engine.
   - It only "uses" an Engine via pointer.
   - Lifetime is NOT bound: engine must outlive this object, or pointer dangles.
*/
class CarBorrowed {
  private:
    Engine* engPtr;   // aggregation: borrowed pointer (non-owning)

  public:
    CarBorrowed(Engine* enginePtr) : engPtr(enginePtr) {
        cout << "CarBorrowed constructed\n";
    }

    ~CarBorrowed() {
        cout << "CarBorrowed Destroyed\n";
    }

    void printEngineId() const {
        if (engPtr == nullptr) {
            cout << "Engine Id through CarBorrowed: (nullptr)\n";
            return;
        }
        cout << "Engine Id through CarBorrowed: " << engPtr->getId() << "\n";
    }
};

int main() {

    /* ========= Phase 1 =========
       Prove: member constructed before Car constructor body.
       Prove: member destroyed after Car destructor body.
    */
    {
        cout << "\n--- Phase 1: Basic composition + scope ---\n";
        cout << "Before scope\n";
        {
            cout << "Entered scope\n";
            Car c;
            cout << "Leaving scope\n";
        }
        cout << "After scope\n";
    }

    /* ========= Phase 2 =========
       Prove: declaration order wins over initializer list order.
    */
    {
        cout << "\n--- Phase 2: Declaration order beats initializer-list order ---\n";
        Car c2(1, 2);
    }

    /* ========= Phase 3 =========
       Prove: innermost scope dies first.
    */
    {
        cout << "\n--- Phase 3: Nested scope destruction timing ---\n";
        cout << "Before OUTER scope\n";
        {
            cout << "Entered OUTER scope\n";
            Car outer(5, 6);

            {
                cout << "Entered INNER scope\n";
                Car inner(3, 4);
                cout << "Leaving INNER scope\n";
            }

            cout << "Leaving OUTER scope\n";
        }
        cout << "After OUTER scope\n";
    }

    /* ========= Phase 4 (safe) =========
       Aggregation demo: CarBorrowed uses Engine, does NOT own it.
       Prove: Engine outlives CarBorrowed if Engine is created in outer scope.
    */
    {
        cout << "\n--- Phase 4: Aggregation (borrowed pointer, non-owning) ---\n";
        cout << "Entered OUTER scope\n";
        Engine eng1(101);

        {
            cout << "Entered INNER scope\n";
            CarBorrowed cb(&eng1);
            cb.printEngineId();
            cout << "Leaving INNER scope\n";
        }

        cout << "Leaving OUTER scope (Engine will be destroyed after this)\n";
    }

    /* Phase 4b : a borrowed pointer can outlive the object it points to → dangling pointer risk.*/
    {
        Engine* danglingEngPtr = nullptr;
        {
            Engine eng2(102);
            danglingEngPtr = &eng2;

            cout << "Dangling Engine Id: " << danglingEngPtr->getId() << "\n";

            CarBorrowed cbDangling(danglingEngPtr);
            cbDangling.printEngineId();
        }
        // after leaving inner scope the eng2 is destroyed
        cout << "After Engine eng2 destroyed\n";
        // danglingEngPtr now dangles!
        cout << "Dangling Engine Id (undefined behavior if accessed): ";
        // cout << danglingEngPtr->getId() << "\n"; // DON'T DO THIS
    }
    return 0;
}
