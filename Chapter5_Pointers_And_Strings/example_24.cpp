// File: example_24.cpp
// Purpose: Shuffle a standard 52-card deck and deal (print) the cards in shuffled order
//
// How to use this file later:
// - Search "EX" to jump to the demo in main.
// - Search "Notes" to review what the program is practicing.
// - This example uses a 2D deck[4][13] array where each slot stores a unique shuffle order number.

#include <iostream>
#include <iomanip>
#include <stdint.h>
#include <cstdlib>
#include <ctime>

using namespace std;

constexpr int DECKSIZE = 52;

/* ============================================================================
   Notes: What we studied here (key learnings)
   ----------------------------------------------------------------------------
   1) How we represent a deck (2D array)
      - deck[4][13] represents:
          4 suits (rows) and 13 faces (columns)
      - Each slot corresponds to one real card:
          suit index = row, face index = col
      - Instead of storing a "card struct", we store an integer "order number"
        that tells us when that card appears in the shuffle.

   2) Shuffle idea used here (unique random placement)
      - We want to assign numbers 1..52 to the 52 card slots randomly.
      - Each number represents the position in the shuffled order.
         Example: if deck[2][5] == 1 then the first dealt card is:
                  suit[2] with face[5]
      - To avoid duplicates:
          keep choosing random (row, col) until we find an empty slot (0)

   3) Dealing idea used here (print in shuffle order)
      - After shuffling, we "deal" by printing cards in order:
          i = 1 to 52
      - For each i, we scan the deck to find where deck[row][col] == i
      - Then we print:
          face[col] of suit[row]

   4) Why the loops start at 1 not 0 (important!)
      - The deck array uses 0 to mean "empty slot"
      - So shuffle fills with numbers 1..52
      - Dealing must also search for 1..52
      - If you search 0..51 you miss 52 and waste time searching for 0

   5) Common pitfalls we avoided
      - Off-by-one errors (printing range must match stored numbers)
      - Typos in suit/face strings (output correctness)
      - Forgetting to clear deck before shuffling (re-shuffle would break)

   6) Complexity (just to understand what happens)
      - Shuffle uses random trial until empty slot found.
      - Deal printing scans 52 slots for each of 52 cards:
          52 * 52 checks (small, fine for learning)
   ========================================================================== */

/* ============================================================================
   Helper 1: shuffleDeck
   Input:
     - deck: 4x13 array
   Behavior:
     - Clears deck to 0 (empty)
     - Randomly assigns unique values 1..52 to the slots
   ========================================================================== */
void shuffleDeck(int deck[4][13]) {

    // Initialise deck (clear to 0)
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 13; c++) {
            deck[r][c] = 0;
        }
    }

    // For each of the 52 Cards
    for (int i = 1; i <= DECKSIZE; i++) {
        int row, col;

        // while the card with face and suit not already drawn
        do {
            // choose slot of deck randomly
            row = rand() % 4;
            col = rand() % 13;
        } while (deck[row][col] != 0);

        // if not previously choosen store 1
        deck[row][col] = i;
    }
}

/* ============================================================================
   Helper 2: dealDeck
   Input:
     - deck: shuffled 4x13 deck storing order numbers
     - suit: suit names (size 4)
     - face: face names (size 13)
   Behavior:
     - Prints cards in shuffled order from 1..52
   ========================================================================== */
void dealDeck(const int deck[4][13], const char* suit[], const char* face[]) {

    // For each of the 52 Cards
    for (int i = 1; i <= DECKSIZE; i++) {

        // for each of the elements in adeck array
        for (int p = 0; p < 4; p++) {
            for (int q = 0; q < 13; q++) {

                // if the slot contains desired card number
                if (deck[p][q] == i) {

                    // print
                    cout << setw(4) << right << face[q] << " of "
                         << setw(8) << left << suit[p] << endl;
                }
            }
        }
    }
}

int main() {

    /* =========================================================================
       EX: Shuffle first, then deal (print) the whole deck
       - We seed rand() one time at the start of the program
       - Then we shuffle the deck and deal it out
       ======================================================================= */

    srand(time(0));

    // Initialise deck
    int deck[4][13];

    // Initialise suit
    const char* suit[] = {"Hearts", "Diamonds", "Clubs", "Spades"};

    // Initialise Face
    const char* face[] = {"Ace","Two","Three","Four","Five","Six","Seven",
                          "Eight","Nine","Ten","Jack","Queen","King"};

    // shufflineg firt nd dealing
    shuffleDeck(deck);
    dealDeck(deck, suit, face);

    return 0;
}
