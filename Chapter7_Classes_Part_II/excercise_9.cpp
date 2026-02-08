#include <iostream>
using namespace std;

/*
===============================================================================
EX 9: IntegerSet (0..100) using a boolean membership array

Core representation:
  - bool set[101]
  - set[i] == true  => integer i is IN the set
  - set[i] == false => integer i is NOT in the set

Key learnings / rules we proved:
  1) Set equality:
       S == T  <=>  for every i in [0..100], membership matches
       Implementation: scan all indices; first mismatch => not equal.
  2) Union:
       S ∪ T = { i | i in S OR i in T }
       Implementation: result[i] = this[i] || other[i]
  3) Intersection:
       S ∩ T = { i | i in S AND i in T }
       Implementation: result[i] = this[i] && other[i]
  4) Constructor ambiguity pitfall:
       If you have BOTH:
         IntegerSet()
         IntegerSet(int a=-1, ..., int e=-1)
       then calling IntegerSet x; is ambiguous (both match 0 args).
       Fix: keep only ONE constructor (the 5-arg with defaults).
  5) API design:
       insert/delete return bool instead of printing inside the class
       -> caller decides how to handle invalid inputs.
===============================================================================
*/

class IntegerSet {
private:
    bool set[101]; // index represents the integer; value represents membership

    void fillZero() {
        for (int i = 0; i <= 100; ++i) {
            set[i] = false;
        }
    }

public:
    // Single constructor: also serves as the "default" empty-set constructor
    IntegerSet(int a = -1, int b = -1, int c = -1, int d = -1, int e = -1) {
        fillZero();
        if (a >= 0 && a <= 100) set[a] = true;
        if (b >= 0 && b <= 100) set[b] = true;
        if (c >= 0 && c <= 100) set[c] = true;
        if (d >= 0 && d <= 100) set[d] = true;
        if (e >= 0 && e <= 100) set[e] = true;
    }

    bool insertElement(int k) {
        if (k >= 0 && k <= 100) {
            set[k] = true;
            return true;
        }
        return false;
    }

    bool deleteElement(int k) {
        if (k >= 0 && k <= 100) {
            set[k] = false;
            return true;
        }
        return false;
    }

    bool isEqual(const IntegerSet& other) const {
        for (int i = 0; i <= 100; ++i) {
            if (set[i] != other.set[i]) {
                return false;
            }
        }
        return true;
    }

    IntegerSet unionOfIntegerSets(const IntegerSet& other) const {
        IntegerSet result; // empty set via defaults
        for (int i = 0; i <= 100; ++i) {
            result.set[i] = set[i] || other.set[i];
        }
        return result;
    }

    IntegerSet intersectionOfIntegerSets(const IntegerSet& other) const {
        IntegerSet result; // empty set via defaults
        for (int i = 0; i <= 100; ++i) {
            result.set[i] = set[i] && other.set[i];
        }
        return result;
    }

    // Print as required by exercise:
    // - numbers separated by spaces
    // - print --- if empty
    void setPrint() const {
        bool isEmpty = true;
        for (int i = 0; i <= 100; ++i) {
            if (set[i]) {
                cout << i << " ";
                isEmpty = false;
            }
        }
        if (isEmpty) {
            cout << "---";
        }
        cout << "\n";
    }
};

int main() {
    // 1) Empty set test
    IntegerSet set1;
    cout << "Set 1 (empty): ";
    set1.setPrint(); // expect ---

    // 2) Constructor-filled sets
    IntegerSet set2(1, 3, 5, 7, 9);
    cout << "Set 2: ";
    set2.setPrint();

    IntegerSet set3(2, 4, 6, 8, 10);
    cout << "Set 3: ";
    set3.setPrint();

    // 3) Union / Intersection
    IntegerSet unionSet = set2.unionOfIntegerSets(set3);
    cout << "Union(Set 2, Set 3): ";
    unionSet.setPrint(); // expect 1..10

    IntegerSet intersectionSet = set2.intersectionOfIntegerSets(set3);
    cout << "Intersection(Set 2, Set 3): ";
    intersectionSet.setPrint(); // expect ---

    // 4) insert / delete behavior + invalid inputs
    cout << "\nInsert/Delete tests:\n";
    bool ok;

    ok = set1.insertElement(42);
    cout << "Insert 42 into Set 1 (ok=" << (ok ? "true" : "false") << "): ";
    set1.setPrint(); // expect 42

    ok = set1.deleteElement(42);
    cout << "Delete 42 from Set 1 (ok=" << (ok ? "true" : "false") << "): ";
    set1.setPrint(); // expect ---

    ok = set1.insertElement(200);
    cout << "Insert 200 into Set 1 (ok=" << (ok ? "true" : "false") << "): ";
    set1.setPrint(); // expect --- (unchanged)

    ok = set1.deleteElement(-5);
    cout << "Delete -5 from Set 1 (ok=" << (ok ? "true" : "false") << "): ";
    set1.setPrint(); // expect --- (unchanged)

    // 5) Equality tests
    cout << "\nEquality tests:\n";
    IntegerSet set4(1, 3, 5, 7, 9);
    cout << "Set 2: ";
    set2.setPrint();
    cout << "Set 4: ";
    set4.setPrint();
    cout << "Set2 == Set4 ? " << (set2.isEqual(set4) ? "true" : "false") << "\n"; // expect true
    cout << "Set2 == Set3 ? " << (set2.isEqual(set3) ? "true" : "false") << "\n"; // expect false

    return 0;
}
