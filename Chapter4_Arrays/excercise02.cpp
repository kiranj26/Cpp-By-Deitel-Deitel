// File: chapter4_notes.cpp
// Purpose: Arrays + counters + dice simulation + recursion + simple seat reservation system
//
// How to use this file later:
// - Search "EX" to jump to a topic.
// - Each EX block demonstrates one concept with a minimal example.
// - Functions above main are reusable helpers used by multiple exercises.

#include <iostream>
#include <iomanip>
#include <cstdlib>   // std::rand, std::srand
#include <ctime>     // std::time

using std::cin;
using std::cout;
using std::endl;
using std::setw;

/* ============================================================================
   Constants (avoid magic numbers)
   ========================================================================== */
constexpr int kDiceSides        = 6;
constexpr int kDiceRolls        = 3600;
constexpr int kMinDiceValue     = 1;
constexpr int kMaxDiceValue     = kDiceSides;
constexpr int kMinSum           = 2;                 // 1 + 1
constexpr int kMaxSum           = 2 * kDiceSides;    // 6 + 6
constexpr int kSumArraySize     = kMaxSum + 1;       // allow direct indexing by sum

constexpr int kPlaneSeats       = 10;
constexpr int kSmokingStartIdx  = 0;  // seats 1..5  => indices 0..4
constexpr int kSmokingEndIdx    = 4;
constexpr int kNonSmokeStartIdx = 5;  // seats 6..10 => indices 5..9
constexpr int kNonSmokeEndIdx   = 9;

constexpr int kMenuSmoking      = 1;
constexpr int kMenuNonSmoking   = 2;
constexpr int kMenuExit         = -2;

/* ============================================================================
   Helper 1: Print an int array in one line
   - Prints all elements (0..arraySize-1)
   - setw aligns output nicely for reading
   ========================================================================== */
void printArray(const int array[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        cout << setw(4) << array[i];
    }
    cout << "\n";
}

/* ============================================================================
   Helper 2: Print a 2D array (rows x cols)
   - Used for printing the dice pair-frequency table
   - Each cell [r][c] corresponds to (die1 = r+1, die2 = c+1)
   ========================================================================== */
void print2DArray(const int array[][kDiceSides], int row, int col) {
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            cout << setw(4) << array[i][j];
        }
        cout << "\n";
    }
    cout << "\n";
}

/* ============================================================================
   Helper 3: Roll a fair kDiceSides-sided die using rand()
   - Returns an integer in [1, kDiceSides]
   - Formula:
       1 + (rand() % kDiceSides)
   ========================================================================== */
int rollDiceUsingRand(void) {
    return (kMinDiceValue + (std::rand() % kDiceSides));
}

/* ============================================================================
   Helper 4: Recursive sum of array elements
   - Returns sum of the first "size" elements of arr
   - Edge cases:
       size <= 0 -> 0
       size == 1 -> arr[0]
   ========================================================================== */
int recursiveSum(const int arr[], int size) {
    if (size <= 0) {
        return 0;
    }
    if (size == 1) {
        return arr[0];
    }
    return arr[size - 1] + recursiveSum(arr, size - 1);
}

/* ============================================================================
   Helper 5: Find the first available seat in a range [startIdx, endIdx]
   Seat model:
   - seats[i] == 0 -> free
   - seats[i] == 1 -> occupied
   Returns:
   - index of a free seat, or -1 if none available in the range
   ========================================================================== */
int findFirstFreeSeat(const int seats[], int startIdx, int endIdx) {
    for (int i = startIdx; i <= endIdx; i++) {
        if (seats[i] == 0) {
            return i;
        }
    }
    return -1;
}

/* ============================================================================
   Helper 6: Print a simple boarding pass
   - Seat number displayed to user is 1..10 (index + 1)
   ========================================================================== */
void printBoardingPass(int seatIndex, bool isSmoking) {
    cout << "------- Your Boarding Pass --------\n";
    if (isSmoking) {
        cout << "---------  Smoking Zone  ----------\n";
    } else {
        cout << "-------- Non-Smoking Zone ---------\n";
    }
    cout << "---------- Seat Number: " << (seatIndex + 1) << " ----------\n\n";
}

int main() {

    // Seed rand once so results change each run.
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    /* =========================================================================
       EX17: Rolling of two dice (frequency counting)
       Goals:
       - Roll two dice kDiceRolls times using rand().
       - Count:
         (A) pair outcomes (die1, die2) in a 6x6 table
         (B) sum outcomes (2..12) in a 1D array indexed by sum
       Notes:
       - Expected average per pair cell ≈ kDiceRolls / 36.
       - Sum 7 should be most frequent; 2 and 12 should be least frequent.
       ======================================================================= */
    {
        int pairCounter[kDiceSides][kDiceSides] = {0};
        int sumCounter[kSumArraySize] = {0}; // indices 0..12; valid sums 2..12

        for (int count = 0; count < kDiceRolls; count++) {
            int roll1 = rollDiceUsingRand();
            int roll2 = rollDiceUsingRand();

            pairCounter[roll1 - kMinDiceValue][roll2 - kMinDiceValue]++;
            sumCounter[roll1 + roll2]++;
        }

        cout << "EX17 Pair frequency table (die1 rows 1..6, die2 cols 1..6):\n";
        print2DArray(pairCounter, kDiceSides, kDiceSides);

        cout << "EX17 Sum frequency table (valid sums are 2..12):\n";
        cout << "Sum:   ";
        for (int s = kMinSum; s <= kMaxSum; s++) {
            cout << setw(4) << s;
        }
        cout << "\nCount: ";
        for (int s = kMinSum; s <= kMaxSum; s++) {
            cout << setw(4) << sumCounter[s];
        }
        cout << "\n\n";
    }

    /* =========================================================================
       EX18: Recursive sum of an array
       ======================================================================= */
    {
        const int arr[] = {1,2,3,4,5,6,7,8,9,10,11};
        const int n = static_cast<int>(sizeof(arr) / sizeof(arr[0]));
        cout << "EX18 Recursive sum of array: " << recursiveSum(arr, n) << "\n\n";
    }

    /* =========================================================================
       EX20: Airline Reservation System (1D array version)
       Problem:
       - Plane has 10 seats:
           Smoking section: seats 1..5
           Non-smoking:     seats 6..10
       - Menu:
           1 -> book smoking
           2 -> book non-smoking
           -2 -> exit
       - Seat storage:
           seats[10] initialized to 0
           0 = free, 1 = occupied
       - Never assign a seat that is already assigned.
       - If selected section is full, prompt user to accept the other section.
         If no, print: "Next flight in 3 hours."
       ======================================================================= */
    {
        int seats[kPlaneSeats] = {0};

        while (true) {
            cout << "EX20 Seat Booking Menu:\n";
            cout << "  1 -> Smoking (seats 1..5)\n";
            cout << "  2 -> Non-Smoking (seats 6..10)\n";
            cout << " -2 -> Exit\n";
            cout << "Enter choice: ";

            int choice = 0;
            if (!(cin >> choice)) {
                cout << "Invalid input. Exiting.\n";
                return 1;
            }

            if (choice == kMenuExit) {
                cout << "Exiting booking system.\n";
                break;
            }

            bool wantSmoking = false;
            if (choice == kMenuSmoking) {
                wantSmoking = true;
            } else if (choice == kMenuNonSmoking) {
                wantSmoking = false;
            } else {
                cout << "Invalid selection. Try again.\n\n";
                continue;
            }

            // Attempt booking in the chosen section first.
            int firstStart = wantSmoking ? kSmokingStartIdx : kNonSmokeStartIdx;
            int firstEnd   = wantSmoking ? kSmokingEndIdx   : kNonSmokeEndIdx;

            int seatIndex = findFirstFreeSeat(seats, firstStart, firstEnd);

            if (seatIndex != -1) {
                seats[seatIndex] = 1;
                printBoardingPass(seatIndex, wantSmoking);
                continue;
            }

            // Chosen section is full; offer the other section.
            cout << (wantSmoking ? "Smoking" : "Non-smoking") << " section is full.\n";
            cout << "Would you like a seat in the other section? (y/n): ";

            char answer = 'n';
            cin >> answer;

            if (answer != 'y' && answer != 'Y') {
                cout << "Next flight in 3 hours.\n\n";
                break;
            }

            int otherStart = wantSmoking ? kNonSmokeStartIdx : kSmokingStartIdx;
            int otherEnd   = wantSmoking ? kNonSmokeEndIdx   : kSmokingEndIdx;

            seatIndex = findFirstFreeSeat(seats, otherStart, otherEnd);

            if (seatIndex == -1) {
                // Both sections are full.
                cout << "Both sections are full. Next flight in 3 hours.\n\n";
                break;
            }

            seats[seatIndex] = 1;
            printBoardingPass(seatIndex, !wantSmoking);
        }
    }

    return 0;
}
