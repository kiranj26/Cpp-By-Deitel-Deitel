/*
    EX31 : Selection Sort (Iterative + Recursive Thinking)

    What we studied in this chat (key learnings):
    ---------------------------------------------------------------------------
    1) Selection Sort core idea
       - We build a sorted prefix from left to right.
       - For each position i, we find the smallest element in the subarray [i..n-1]
         and place it at position i by swapping.
       - After pass i, the element at index i is final and never changes again.

    2) Off-by-one discipline (common bug source)
       - Array indices are 0..n-1.
       - Most loops are safer as:
             for (int j = i; j < n; j++)
         instead of:
             j <= n - 1
       - Printing arrays: print all elements 0..size-1 (no "size-1" loop bound).

    3) Swapping rule
       - Only swap if minIndex != i to avoid unnecessary self-swaps.

    4) Recursive pattern identification for selection sort
       - Define the subproblem: "Sort the subarray starting at index start."
       - One step of work:
           a) Find min in [start..end-1]
           b) Swap into index start
       - Recurse on a smaller problem: sort [start+1..end-1]
       - Base case: stop when there are 0 or 1 elements left to sort.
         In index terms:
           - if start >= end - 1, stop (subarray size is 0 or 1)

    5) Consistent meaning of 'end'
       - In this file, end is treated as "one past the last valid index"
         (same style as array sizes in C/C++).
         Example:
           - For an array of size 10, valid indices are 0..9
           - We call selectionSortRecursive(arr, 0, 10)

    6) Small style improvements we practiced
       - Avoid magic numbers by passing arrSize around.
       - Keep prints/debug optional, but printing during learning is fine.
       - Keep code readable: i = current position being filled.

    ---------------------------------------------------------------------------
*/

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;

/* ============================================================================
   Helper 1: Print an int array in one line
   - Prints all elements (0..size-1)
   ========================================================================== */
void printArray(const int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << setw(4) << arr[i];
    }
    cout << endl;
}

/* ============================================================================
   Helper 2: Selection sort (iterative)
   - For each i in [0..n-2], find min in [i..n-1], swap into i
   - Time: O(n^2)
   - Space: O(1)
   ========================================================================== */
void selectionSort(int arr[], int arrSize) {

    for (int i = 0; i < arrSize - 1; i++) {

        // Assume the minimum is at i, then search the rest of the suffix
        int minIndex = i;

        for (int j = i + 1; j < arrSize; j++) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }

        // Swap only if needed
        if (minIndex != i) {
            int hold = arr[minIndex];
            arr[minIndex] = arr[i];
            arr[i] = hold;
        }
    }
}

/* ============================================================================
   Helper 3: Selection sort (recursive thinking)
   Recursive mental model we practiced:
   - Subproblem: sort arr[start..end-1]
   - Step:
       1) find min in arr[start..end-1]
       2) swap it into arr[start]
   - Recurse:
       sort arr[start+1..end-1]
   - Base case:
       stop when subarray size is 0 or 1 => start >= end - 1

   IMPORTANT: Here, 'end' is "one past last index" (like size).
   Example call for size 10:
       selectionSortRecursive(arr, 0, 10)
   ========================================================================== */
void selectionSortRecursive(int arr[], int start, int end) {

    // Base case: 0 or 1 element left
    if (start >= end - 1) {
        return;
    }

    // Find the minimum element in [start..end-1]
    int minIndex = start;
    for (int j = start + 1; j < end; j++) {
        if (arr[j] < arr[minIndex]) {
            minIndex = j;
        }
    }

    // Swap minimum into position start (if needed)
    if (minIndex != start) {
        int hold = arr[minIndex];
        arr[minIndex] = arr[start];
        arr[start] = hold;
    }

    // Recurse on the remaining suffix
    selectionSortRecursive(arr, start + 1, end);
}

int main() {

    /* =========================================================================
       EX31: Selection Sort demo
       - We print the array before and after sorting.
       - You can swap between iterative and recursive versions to compare.
       ======================================================================= */

    int arr[10] = {3, 2, 1, 5, 6, 0, 3, 8, 5, 4};

    cout << "Array before selection sort:\n";
    printArray(arr, 10);

    // Option A: iterative
    // selectionSort(arr, 10);

    // Option B: recursive (end = size)
    selectionSortRecursive(arr, 0, 10);

    cout << "Array after selection sort:\n";
    printArray(arr, 10);

    return 0;
}
