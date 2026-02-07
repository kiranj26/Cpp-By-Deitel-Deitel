/*
===============================================================================
file name : topic_friend.cpp
topic     : Friend functions and friend classes in C++

Goal of this file (revisable notes + runnable proofs):
    You can compile & run this file as-is.
    For each "proof" that is supposed to FAIL, the failing line is kept commented.
    To see the compiler error, uncomment ONLY the marked line(s).

Definitions (core):
    1) friend function
        - A NON-member function that is granted access to a class's private/protected
          members, because the class declares it as a friend.

    2) friend class
        - A whole class whose member functions are granted access to another class's
          private/protected members, because that class declares it as a friend.

Key intuition:
    Friendship is an access-control exception. It does NOT mean:
        - not ownership
        - not scope change
        - not lifetime change
        - not "membership" (friend functions are still non-members)

We will prove:
    [Phase 1] Friend function can access private members.
    [Phase 2] Friend class can access private members.
    [Phase 3.1] Friendship is NOT symmetric.
    [Phase 3.2] Friendship is NOT transitive.
    [Phase 3.3] Friendship is NOT inherited.

Important mental model:
    - Friendship is granted by the class being accessed (the "owner" of private data).
    - Only member functions have an implicit `this` pointer.
    - A friend function does NOT get `this` because it is NOT a member function.

Forward declaration note:
    - A forward declaration (e.g., `class Auditor;`) lets us:
        * declare functions that take Auditor& / Auditor* as parameters
      but it does NOT let us:
        * access Auditor members (needs full class definition)
===============================================================================
*/

#include <iostream>
using std::cout;
using std::endl;

/* Forward declarations (names only) */
class Auditor;
class ThirdParty;
class DerivedAuditor;

/*==============================================================================
  ClassA: owns a private integer x.
  We'll demonstrate friend function + friend class access to x.
==============================================================================*/
class ClassA {
private:
    int x;

public:
    explicit ClassA(int val) : x(val) {}

    void print() const {
        cout << "ClassA x = " << x << endl;
    }

    /* Phase 1: friend function
       - Declared inside the class ONLY to grant access.
       - The function itself remains a NON-member.
    */
    friend void setX(ClassA& a, int val);

    /* Phase 2: friend class
       - All member functions of Auditor can access ClassA's private/protected.
    */
    friend class Auditor;

    /* Phase 3.1: "not symmetric"
       We want ClassA to try to access Auditor's private member.
       Important: define this function AFTER Auditor is fully defined
       so that any error is about "private access", not "incomplete type".
    */
    void tryReadAuditorSecret(const Auditor& aud) const;
};

/*==============================================================================
  Phase 1 helper: friend function definition (NON-member)
==============================================================================*/
void setX(ClassA& a, int val) {
    /* Allowed because ClassA declared setX as a friend */
    a.x = val;
}

/* Non-friend function trying to access ClassA private member -> should fail.
   We keep the illegal line commented so the file compiles by default.
*/
void failedSetX(ClassA& a, int val) {
    (void)val; // silence unused warning if you don't use val
    // Uncomment next line to see error:
    // a.x = val;   // ERROR: 'x' is a private member of 'ClassA'
}

/*==============================================================================
  Auditor: has its own private data (sharedSecret).
  Because ClassA friends Auditor, Auditor can access ClassA::x.
==============================================================================*/
class Auditor {
    /* Phase 3.2 setup: Auditor friends ThirdParty (but that does NOT grant
       ThirdParty access to ClassA, proving NOT transitive).
    */
    friend class ThirdParty;

private:
    int sharedSecret;

public:
    Auditor() : sharedSecret(0) {}
    explicit Auditor(int secret) : sharedSecret(secret) {}

    void readClassA(const ClassA& a) const {
        /* Allowed because ClassA declared `friend class Auditor;` */
        cout << "Auditor reads ClassA x = " << a.x << endl;
    }

    void writeClassA(ClassA& a, int val) const {
        /* Allowed because ClassA declared `friend class Auditor;` */
        a.x = val;
        cout << "Auditor writes ClassA x -> " << val << endl;
    }
};

/*==============================================================================
  Phase 3.1 proof: NOT symmetric
  - Even though Auditor can access ClassA private members,
    ClassA cannot access Auditor private members (unless Auditor also friends ClassA).
==============================================================================*/
void ClassA::tryReadAuditorSecret(const Auditor& aud) const {
    cout << "ClassA attempting to read Auditor's private secret..." << endl;

    /* Uncomment next line to prove NOT symmetric:
       - This should FAIL because ClassA is NOT a friend of Auditor.
    */
    // cout << "Auditor sharedSecret = " << aud.sharedSecret << endl; // ERROR: private member of Auditor

    cout << "(By default we keep the illegal access commented.)" << endl;
}

/*==============================================================================
  Phase 3.2 proof: NOT transitive
  Setup:
    - ClassA friends Auditor  -> Auditor can access ClassA::x
    - Auditor friends ThirdParty
  Claim:
    - ThirdParty does NOT automatically gain access to ClassA::x
==============================================================================*/
class ThirdParty {
public:
    void tryReadClassA(const ClassA& a) const {
        cout << "ThirdParty attempting to read ClassA's private x..." << endl;

        /* Uncomment next line to prove NOT transitive:
           - This should FAIL because ClassA did NOT friend ThirdParty.
           - Even though Auditor friended ThirdParty, that friendship does not propagate.
        */
        // cout << "ClassA x = " << a.x << endl; // ERROR: 'x' is a private member of 'ClassA'

        cout << "(By default we keep the illegal access commented.)" << endl;
    }

    void tryReadAuditorSecret(const Auditor& aud) const {
        /* This is allowed because Auditor explicitly friends ThirdParty. */
        cout << "ThirdParty reads Auditor secret (allowed) ..." << endl;
        // This is private, but ThirdParty is a friend of Auditor:
        // We'll show it safely by actually printing it (no need to hide).
        // Because this file is about proving access, it's useful to show one "works" too.
        // (This is not about transitivity; it's about verifying the friend declaration.)
        // NOTE: We can access sharedSecret here because Auditor has `friend class ThirdParty;`
        cout << "Auditor sharedSecret = " << aud.sharedSecret << endl;
    }
};

/*==============================================================================
  Phase 3.3 proof: NOT inherited
  Setup:
    - ClassA friends Auditor
    - DerivedAuditor inherits from Auditor
  Claim:
    - DerivedAuditor does NOT automatically become a friend of ClassA
==============================================================================*/
class DerivedAuditor : public Auditor {
public:
    using Auditor::Auditor; // inherit constructors for convenience

    void tryReadClassA(const ClassA& a) const {
        cout << "DerivedAuditor attempting to read ClassA's private x..." << endl;

        /* Uncomment next line to prove NOT inherited:
           - This should FAIL because friendship is NOT inherited.
           - ClassA friended Auditor, not DerivedAuditor.
        */
        // cout << "ClassA x = " << a.x << endl; // ERROR: 'x' is a private member of 'ClassA'

        cout << "(By default we keep the illegal access commented.)" << endl;
    }
};

/*==============================================================================
  main: Runs the "working" parts and includes clear toggles for the compile errors.
==============================================================================*/
int main() {
    cout << "====================================================\n";
    cout << "topic_friend.cpp : Friend functions + friend classes\n";
    cout << "====================================================\n";

    /* Phase 0: Concept checks (captured in comments above)
       - friend function is NOT a member
       - friend function has NO implicit this pointer
       - friendship granted by the class being accessed
    */

    cout << "\n=== Phase 1: friend function ===\n";
    ClassA a(10);
    a.print();

    cout << "Calling friend function setX(a, 20)\n";
    setX(a, 20);
    a.print();

    cout << "\n(Non-friend attempt exists in failedSetX)\n";
    cout << "To see compiler error: uncomment a.x assignment inside failedSetX()\n";
    // failedSetX(a, 30); // This call itself compiles; the illegal line is inside failedSetX

    cout << "\n=== Phase 2: friend class ===\n";
    Auditor auditor(123);
    auditor.readClassA(a);
    auditor.writeClassA(a, 50);
    a.print();

    cout << "\n=== Phase 3.1: NOT symmetric ===\n";
    /* Auditor can read/write ClassA::x because ClassA friends Auditor.
       But ClassA cannot read Auditor::sharedSecret unless Auditor also friends ClassA.
    */
    a.tryReadAuditorSecret(auditor);
    cout << "To see compiler error: uncomment the private access line inside ClassA::tryReadAuditorSecret()\n";

    cout << "\n=== Phase 3.2: NOT transitive ===\n";
    /* ThirdParty is a friend of Auditor, so it can read Auditor::sharedSecret.
       But ThirdParty cannot read ClassA::x, because ClassA never friended ThirdParty.
    */
    ThirdParty tp;
    tp.tryReadAuditorSecret(auditor); // this one works (Auditor -> ThirdParty friendship)
    tp.tryReadClassA(a);              // this compiles because illegal line is commented
    cout << "To see compiler error: uncomment the private access line inside ThirdParty::tryReadClassA()\n";

    cout << "\n=== Phase 3.3: NOT inherited ===\n";
    /* DerivedAuditor inherits from Auditor, but does NOT inherit friendship to ClassA. */
    DerivedAuditor da(777);
    da.tryReadClassA(a); // compiles because illegal line is commented
    cout << "To see compiler error: uncomment the private access line inside DerivedAuditor::tryReadClassA()\n";

    cout << "\nDone.\n";
    return 0;
}
