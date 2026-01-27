// File: chapter4_notes.cpp
// Purpose: Arrays + counters (salary buckets) + bubble sort (basic + early-exit optimization)
//
// How to use this file later:
// - Search "EX" to jump to a topic.
// - Each EX block demonstrates one concept with a minimal example.
// - Functions above main are reusable helpers used by multiple exercises.

#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::setw;

/* ============================================================================
   Helper 1: Print an int array in one line
   - Prints all elements (0..arraySize-1)
   - setw just aligns output nicely for reading
   ========================================================================== */
void printArray(const int array[], int arraySize) {
    for (int i = 0; i < arraySize; i++) {
        cout << setw(4) << array[i];
    }
    cout << "\n";
}

/* ============================================================================
   Helper 2: Salary calculation
   - Salary = 200 + 9% of sales
   - Returns int (truncation) to match the exercise style
   ========================================================================== */
int calculateSalary(int sales) {
    return (200.0 + (0.09 * sales));
}

/* ============================================================================
   Helper 3: Bucket salary into a counter array (no long if/else chain)
   What we learned:
   - If ranges are uniform (width 100 here), we can compute the index directly:
       index = (earnings - 200) / 100

   Bucket mapping (counter[0..8]):
     0 -> 200-299
     1 -> 300-399
     2 -> 400-499
     3 -> 500-599
     4 -> 600-699
     5 -> 700-799
     6 -> 800-899
     7 -> 900-999
     8 -> 1000+

   Safety rules:
   - If earnings < 200 -> index becomes negative -> DO NOT index the array.
     Early return prevents undefined behavior.
   - If index > 8 -> clamp to 8 (1000+ bucket).
   ========================================================================== */
void checkEarningsAgainstCounter(int counter[], int earnings) {
    int index = (earnings - 200) / 100;

    if (index < 0) {
        // earnings < 200: invalid range for this exercise; avoid negative indexing
        return;
    }

    if (index > 8) {
        // anything above 1000 always maps to the last bucket
        index = 8;
    }

    counter[index]++;
}

/* ============================================================================
   Helper 4: Bubble sort (basic version)
   Key idea:
   - Compare neighbors (arr[j-1], arr[j]) and swap if out of order.
   - After each full pass, the largest element "bubbles" to the end.
   - After i passes, i elements at the end are already sorted.
   ========================================================================== */
void bubbleSortBasic(int arr[], int arrSize) {
    for (int i = 0; i < arrSize - 1; i++) {
        for (int j = 1; j < arrSize - i; j++) {
            if (arr[j - 1] > arr[j]) {
                int temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
            }
        }
    }
}

/* ============================================================================
   Helper 5: Bubble sort (modified / early-exit version)
   What we learned (skip if no swaps):
   - If we do a full pass and no swaps happen, the array is already sorted.
   - So we can stop early (avoid wasted passes).

   Benefit:
   - Best case (already sorted) becomes much faster.
   - Worst case remains the same big-O, but real runs often improve.
   ========================================================================== */
void bubbleSortEarlyExit(int arr[], int arrSize) {
    for (int i = 0; i < arrSize - 1; i++) {
        bool didSwap = false;

        for (int j = 1; j < arrSize - i; j++) {
            if (arr[j - 1] > arr[j]) {
                int temp = arr[j];
                arr[j] = arr[j - 1];
                arr[j - 1] = temp;
                didSwap = true;
            }
        }

        if (!didSwap) {
            // No swaps in this pass => array already sorted
            break;
        }
    }
}

int main() {

    /* =========================================================================
       EX10: Salary ranges counter using a single array (bucket indexing)
       - Read weekly sales until sentinel (-1)
       - Convert sales -> salary
       - Convert salary -> bucket index and increment counter
       ======================================================================= */
    constexpr int kSentinel = -1;

    const int ranges = 9;
    int counter[ranges] = {0};

    while (true) {
        int sales = 0;
        cout << "EX10 Enter weekly sales in $ (Enter -1 to exit): ";
        cin >> sales;

        if (sales == kSentinel) {
            break;
        }

        int salary = calculateSalary(sales);
        checkEarningsAgainstCounter(counter, salary);
    }

    cout << "EX10 Salary buckets (200-299 ... 1000+):\n";
    printArray(counter, ranges);
    cout << "\n";

    /* =========================================================================
       EX11: Bubble sort demo (basic)
       - Demonstrates neighbor compare + swap
       ======================================================================= */
    {
        int arr[10] = {11, 21, 31, 14, 15, 61, 17, 18, 118, 5};

        cout << "EX11 Before bubbleSortBasic:\n";
        printArray(arr, 10);

        bubbleSortBasic(arr, 10);

        cout << "EX11 After bubbleSortBasic:\n";
        printArray(arr, 10);
        cout << "\n";
    }

    /* =========================================================================
       EX12: Modified Bubble sort demo (early exit)
       - Same sorting result as basic bubble sort
       - Stops early if a full pass makes zero swaps
       ======================================================================= */
    {
        int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        cout << "EX12 Before bubbleSortEarlyExit (already sorted input):\n";
        printArray(arr, 10);

        bubbleSortEarlyExit(arr, 10);

        cout << "EX12 After bubbleSortEarlyExit:\n";
        printArray(arr, 10);
        cout << "\n";
    }

    /* ============================================================================
    EX15: Print only unique numbers from user input (range-limited input)
    Problem statement:
    - Ask the user for 20 integers.
    - Each number is guaranteed to be in the range [10, 100] inclusive.
    - As each number is read, print it ONLY if it has not appeared before.
    - Worst case: all 20 numbers are unique → all 20 are printed.
    - Constraint: Use the SMALLEST possible array.

    Interview-style reasoning:
    1) Even though we read 20 numbers, the VALUE RANGE is fixed and small: 10..100.
        Total possible distinct values = 100 - 10 + 1 = 91.

    2) Because the range is known in advance, we do NOT need:
        - nested loops
        - searching previously entered numbers
        - storing all 20 values

    3) Instead, we track whether a VALUE has appeared before.

    Mapping idea:
    - Value range: 10..100
    - Array indices: 0..90
    - Mapping formula:
            index = value - 10

    Presence logic:
    - If presence[index] == false:
            → first occurrence
            → print value
            → mark presence[index] = true
    - Else:
            → duplicate
            → do nothing

    Why this uses the smallest possible array:
    - One boolean per possible value → 91 elements total.
    - Smaller and faster than storing inputs and searching.
    - Absolute minimum could be bits, but boolean array is ideal here.

    Time complexity:
    - O(20) → one constant-time check per input.

    Space complexity:
    - O(91) → fixed, independent of number of inputs.
    ========================================================================== */
    {
        const int kMinValue = 10;
        const int kMaxValue = 100;
        const int kRangeSize = (kMaxValue - kMinValue + 1); // 91
        const int kInputCount = 20;

        bool seen[kRangeSize] = {false};

        cout << "EX15 Enter 20 numbers (each between 10 and 100):\n";

        for (int i = 0; i < kInputCount; i++) {
            int value = 0;
            cin >> value;

            // Map value (10..100) to index (0..90)
            int index = value - kMinValue;

            // Since problem guarantees valid range, no extra validation needed
            if (!seen[index]) {
                cout << value << " ";
                seen[index] = true;
            }
        }

        cout << "\n\n";
    }

    return 0;
}
