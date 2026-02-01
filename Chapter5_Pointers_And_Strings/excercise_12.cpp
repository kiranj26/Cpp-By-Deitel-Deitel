// File: example_24.cpp
// Purpose: Shuffle a standard 52-card deck and deal (print) cards in shuffled order,
//          then deal TWO 5-card poker hands and evaluate each.
//
// How to revise later:
// - Search "NOTES" for the big picture.
// - Search "DEAL HAND" for how we extract a hand from the 2D shuffled deck.
// - Search "EVAL" for hand-evaluation logic.
// - Search "COMMON BUGS" for pitfalls we fixed.
//
// IMPORTANT: We intentionally keep the SAME structure you built so you can revisit.
//            (Global arrays + count arrays + small helper functions.)

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

constexpr int DECKSIZE = 52;
constexpr int HANDSIZE = 5;

// ============================ GLOBAL HAND STATE ============================
// We keep these as globals because that’s how your program evolved.
// Later (if you want) we can refactor into a struct, but NOT now.

// Hand A (first 5 cards)
int handRankA[5] = {0};
int handSuiteA[5] = {0};
int handRankCountA[15] = {0};   // indices 2..14 used (Ace=14)
int handSuiteCountA[4] = {0};   // 0..3 suits

// Hand B (next 5 cards)
int handRankB[5] = {0};
int handSuiteB[5] = {0};
int handRankCountB[15] = {0};
int handSuiteCountB[4] = {0};

// ==========================================================================

/* =============================================================================
   NOTES (Big picture learnings)
   -----------------------------------------------------------------------------
   1) Deck representation
      - deck[4][13] is 4 suits x 13 faces.
      - Each slot stores an "order number" 1..52.
      - 0 means empty while building the shuffle.

   2) Shuffle approach (unique random placement)
      - Assign numbers 1..52 to random empty slots.
      - No duplicates because we only place into a slot when it is 0.

   3) Dealing approach
      - If we want the card number i in the shuffle:
          scan deck to find deck[suit][face] == i
      - Then suit index = suit, face index = face.

   4) Poker evaluation approach (counts)
      - Convert face index into poker rank 2..14.
      - Build:
          rankCount[15] -> how many of each rank you have
          suitCount[4]  -> how many of each suit you have
      - Use these arrays to detect hand categories:
          flush, straight, pairs, trips, quads, full house

   COMMON BUGS we avoid:
      - Passing rankCount into isFlush by mistake (flush must use suitCount)
      - Straight logic: must handle A-2-3-4-5
      - Straight logic: must ensure exactly 5 distinct ranks (no duplicates)
      - Resetting counts BEFORE dealing each hand
   ============================================================================= */

/* ============================================================================
   Helper 1: shuffleDeck
   Behavior:
     - Clears deck to 0
     - Assigns unique shuffle order numbers 1..52 randomly
   ========================================================================== */
void shuffleDeck(int deck[4][13]) {

    // Clear deck: 0 means "empty"
    for (int r = 0; r < 4; r++) {
        for (int c = 0; c < 13; c++) {
            deck[r][c] = 0;
        }
    }

    // Place 1..52 uniquely
    for (int i = 1; i <= DECKSIZE; i++) {
        int row, col;
        do {
            row = rand() % 4;
            col = rand() % 13;
        } while (deck[row][col] != 0);

        deck[row][col] = i;
    }
}

/* ============================================================================
   Rank conversion
   face index q:
     0 -> Ace (rank 14)
     1 -> Two (rank 2)
     ...
     12 -> King (rank 13)
   ========================================================================== */
int rankValueFromFaceIndex(int faceIdx)
{
    if (faceIdx == 0) return 14;     // Ace
    return (faceIdx + 1);            // 1->2, 2->3, ..., 12->13
}

/* ============================================================================
   Utility: clearCountArrays
   Why:
     - Every time we "build" a new hand, counts must start from 0.
   ========================================================================== */
void clearCountArrays(int *rankCount, int *suitCount)
{
    for (int i = 0; i < 15; i++) rankCount[i] = 0;
    for (int i = 0; i < 4; i++)  suitCount[i] = 0;
}

/* ============================================================================
   DEAL HAND (core learning)
   Deal exactly 5 cards starting from a given shuffle position.

   Example:
     startOrder = 1  => picks cards #1..#5   (Hand A)
     startOrder = 6  => picks cards #6..#10  (Hand B)

   We keep your "scan the 2D deck for order number" logic (learning-friendly).
   ========================================================================== */
void dealFiveCardHandFromOrderRange(const int deck[4][13],
                                    const char * suit[],
                                    const char * face[],
                                    int startOrder,
                                    int *outHandSuite,
                                    int *outHandRank,
                                    int *outRankCount,
                                    int *outSuitCount)
{
    cout << "============= " << HANDSIZE << " card Poker Hand (order "
         << startOrder << " to " << (startOrder + HANDSIZE - 1) << ") =============\n";

    // For each dealt card number in this hand...
    for (int dealIdx = 0; dealIdx < HANDSIZE; dealIdx++) {

        int orderWanted = startOrder + dealIdx;

        // scan the deck to find the card whose shuffle order is "orderWanted"
        for (int p = 0; p < 4; p++) {
            for (int q = 0; q < 13; q++) {

                if (deck[p][q] == orderWanted) {

                    // Print the actual card
                    cout << setw(4) << right << face[q] << " of "
                         << setw(8) << left << suit[p] << "\n";

                    // Store suit and rank in the hand arrays
                    outHandSuite[dealIdx] = p;

                    int rank = rankValueFromFaceIndex(q);
                    outHandRank[dealIdx] = rank;

                    // Update count arrays (this is what powers evaluation)
                    outRankCount[rank]++;
                    outSuitCount[p]++;
                }
            }
        }
    }
}

/* ============================================================================
   Count helper:
   "How many ranks appear exactly N times?"
   - N=4 => quads count
   - N=3 => trips count
   - N=2 => pairs count
   - N=1 => distinct cards count
   ========================================================================== */
int countRanksWithFrequency(const int * rankCount, int N)
{
    int count = 0;
    for (int r = 2; r <= 14; r++) {
        if (rankCount[r] == N) count++;
    }
    return count;
}

/* ============================================================================
   EVAL: Flush
   - A flush means all 5 cards share the same suit
   - suitCount array is the ONLY correct input for flush
   ========================================================================== */
bool isFlush(const int * suitCount)
{
    for (int s = 0; s < 4; s++) {
        if (suitCount[s] == 5) return true;
    }
    return false;
}

/* ============================================================================
   EVAL: Straight
   Logic:
     1) Must have 5 distinct ranks (no duplicates), i.e. exactly five ranks with count 1
     2) Special case: A-2-3-4-5
     3) Otherwise, detect any run of 5 consecutive ranks

   NOTE: This function returns ONLY true/false for now.
   Later if you want "highest card of straight" for tie-break, we can add it.
   ========================================================================== */
bool isStraight(const int * rankCount)
{
    // If we don't have 5 distinct ranks, we can't be a straight
    if (countRanksWithFrequency(rankCount, 1) != 5) {
        return false;
    }

    // Special straight: A-2-3-4-5
    if (rankCount[14] == 1 &&
        rankCount[2]  == 1 &&
        rankCount[3]  == 1 &&
        rankCount[4]  == 1 &&
        rankCount[5]  == 1)
    {
        return true;
    }

    // General run detection
    int runLength = 0;
    for (int r = 2; r <= 14; r++) {
        if (rankCount[r] == 1) {
            runLength++;
            if (runLength == 5) return true;
        } else {
            runLength = 0;
        }
    }

    return false;
}

/* ============================================================================
   EVAL: Pair/Trips/Quads/FullHouse
   We keep your style: very direct logic based on frequency counts.
   ========================================================================== */
bool isOnePair(const int * rankCount)
{
    return (countRanksWithFrequency(rankCount, 2) == 1);
}

bool isTwoPair(const int * rankCount)
{
    return (countRanksWithFrequency(rankCount, 2) == 2);
}

bool isThreeOfAKind(const int * rankCount)
{
    // IMPORTANT: exclude the full house case (3+2) by requiring no pairs
    return (countRanksWithFrequency(rankCount, 3) == 1) &&
           (countRanksWithFrequency(rankCount, 2) == 0);
}

bool isFourOfAKind(const int * rankCount)
{
    return (countRanksWithFrequency(rankCount, 4) == 1);
}

bool isFullHouse(const int * rankCount)
{
    return (countRanksWithFrequency(rankCount, 3) == 1) &&
           (countRanksWithFrequency(rankCount, 2) == 1);
}

/* ============================================================================
   EVAL: handleHand
   Returns a "priority" number (lower is stronger).
   This is a super common pattern in card evaluators.

   Priority:
     1 Straight Flush
     2 Four of a Kind
     3 Full House
     4 Flush
     5 Straight
     6 Three of a Kind
     7 Two Pair
     8 One Pair
     9 High Card
   ========================================================================== */
int handleHand(const int * rankCount, const int * suitCount)
{
    int winPriority = 9;

    // We print once so your output stays consistent and easy to revise
    cout << "================== Hand Result ==================\n";

    if (isStraight(rankCount) && isFlush(suitCount)) {
        cout << "Straight Flush\n";
        winPriority = 1;
    } else if (isFourOfAKind(rankCount)) {
        cout << "Four of a Kind\n";
        winPriority = 2;
    } else if (isFullHouse(rankCount)) {
        cout << "Full House\n";
        winPriority = 3;
    } else if (isFlush(suitCount)) {
        cout << "Flush\n";
        winPriority = 4;
    } else if (isStraight(rankCount)) {
        cout << "Straight\n";
        winPriority = 5;
    } else if (isThreeOfAKind(rankCount)) {
        cout << "Three of a Kind\n";
        winPriority = 6;
    } else if (isTwoPair(rankCount)) {
        cout << "Two Pair\n";
        winPriority = 7;
    } else if (isOnePair(rankCount)) {
        cout << "One Pair\n";
        winPriority = 8;
    } else {
        cout << "High Card\n";
        winPriority = 9;
    }

    return winPriority;
}

/* ============================================================================
   Debug print helpers (for your learning)
   - This matches your "print arrays" style so you can reason quickly.
   ========================================================================== */
void printHandArrays(const char* name, const int *handSuite, const int *handRank)
{
    cout << name << " SUIT INDEXES: ";
    for (int i = 0; i < 5; i++) cout << handSuite[i] << "\t";
    cout << "\n";

    cout << name << " RANK VALUES : ";
    for (int i = 0; i < 5; i++) cout << handRank[i] << "\t";
    cout << "\n";
}

void printCountArrays(const char* name, const int *suitCount, const int *rankCount)
{
    cout << name << " SUIT COUNT [H D C S]: ";
    for (int i = 0; i < 4; i++) cout << suitCount[i] << "\t";
    cout << "\n";

    cout << name << " RANK COUNT (2..14): ";
    for (int r = 2; r <= 14; r++) cout << rankCount[r] << "\t";
    cout << "\n";
}

/* ============================================================================
   MAIN
   ========================================================================== */
int main()
{
    srand(static_cast<unsigned>(time(0)));

    int deck[4][13];

    const char* suit[] = {"Hearts", "Diamonds", "Clubs", "Spades"};
    const char* face[] = {"Ace","Two","Three","Four","Five","Six","Seven",
                          "Eight","Nine","Ten","Jack","Queen","King"};

    // Shuffle once
    shuffleDeck(deck);

    // ---------------------- Hand A: cards 1..5 ----------------------
    clearCountArrays(handRankCountA, handSuiteCountA);

    dealFiveCardHandFromOrderRange(deck, suit, face,
                                   1,                 // start order
                                   handSuiteA,
                                   handRankA,
                                   handRankCountA,
                                   handSuiteCountA);

    printHandArrays("HAND A", handSuiteA, handRankA);
    printCountArrays("HAND A", handSuiteCountA, handRankCountA);
    int priorityA = handleHand(handRankCountA, handSuiteCountA);

    cout << "\n";

    // ---------------------- Hand B: cards 6..10 ----------------------
    clearCountArrays(handRankCountB, handSuiteCountB);

    dealFiveCardHandFromOrderRange(deck, suit, face,
                                   6,                 // start order
                                   handSuiteB,
                                   handRankB,
                                   handRankCountB,
                                   handSuiteCountB);

    printHandArrays("HAND B", handSuiteB, handRankB);
    printCountArrays("HAND B", handSuiteCountB, handRankCountB);
    int priorityB = handleHand(handRankCountB, handSuiteCountB);

    cout << "\n";

    // ---------------------- Compare winner ----------------------
    // Smaller priority number = stronger hand
    cout << "================== WINNER ==================\n";
    if (priorityA < priorityB) {
        cout << "HAND A wins\n";
    } else if (priorityB < priorityA) {
        cout << "HAND B wins\n";
    } else {
        cout << "Tie (same category). Next step: implement tie-breakers.\n";
    }

    return 0;
}
