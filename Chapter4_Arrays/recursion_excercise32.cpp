// File: palindrome_notes.cpp
// Purpose: Palindrome checking using (1) iterative two-pointer method and (2) recursion
//
// How to use this file later:
// - Search "EX" to jump to the topic.
// - This file focuses on building recursion thinking from an existing loop solution.
// - Works with C-style strings stored in a char array (null-terminated).

#include <iostream>

using std::cout;
using std::endl;

/* ============================================================================
   Notes: What we studied here (key learnings)
   ----------------------------------------------------------------------------
   1) Palindrome definition
      - A string is a palindrome if it reads the same forward and backward.
      - Example: "radar", "level", "abba"

   2) Two-pointer pattern (iterative)
      - Use two indices:
          start at the beginning, end at the last real character
      - Compare arr[start] and arr[end]
      - If mismatch -> not a palindrome
      - Move inward: start++, end--
      - Stop when start >= end (checked all pairs successfully)

   3) Recursion pattern identification
      - Define the subproblem:
          "Is arr[start..end] a palindrome?"
      - One step of work:
          Compare outer characters arr[start] and arr[end]
          If mismatch -> false immediately
      - Recursive step:
          If match, reduce the problem to the inside:
              checkPalindromeRecursive(arr, start+1, end-1)
      - Base case:
          When 0 or 1 character remains, it is always a palindrome:
              start >= end  -> true

   4) Indexing for C-style strings (important!)
      - If we have:
          char s[] = "radar";
        then s includes a null terminator '\0' at the end.
      - Array size from sizeof includes '\0'.
      - Last real character index is:
          end = (size - 2)
        because:
          size - 1 is '\0'
          size - 2 is last real character

   5) Common pitfalls we avoided
      - Wrong base case (start > end returning false) is incorrect:
          crossing means we finished all checks successfully -> true
      - Hardcoding end index (like 4) works only for a specific string length.
        Better to compute it from the array size.
   ========================================================================== */

/* ============================================================================
   Helper 1: Palindrome check (iterative two-pointer)
   Input:
     - arr: C-style string in a char array (null-terminated)
     - arrSize: full array size INCLUDING '\0' (usually from sizeof)
   Behavior:
     - end starts at arrSize - 2 to skip the null terminator.
   ========================================================================== */
bool checkPalindrome(const char arr[], int arrSize) {
    // start = 0
    // end = last real character index = arrSize - 2
    for (int start = 0, end = arrSize - 2; start < end; start++, end--) {
        if (arr[start] != arr[end]) {
            return false;
        }
    }
    return true;
}

/* ============================================================================
   Helper 2: Palindrome check (recursive)
   Subproblem:
     - Is arr[start..end] a palindrome?
   Base case:
     - If start >= end -> 0 or 1 character left -> true
   Recursive step:
     - If outer chars match, check the inside:
         checkPalindromeRecursive(arr, start+1, end-1)
     - If mismatch -> false
   ========================================================================== */
bool checkPalindromeRecursive(const char arr[], int start, int end) {
    // Base case: 0 or 1 character left
    if (start >= end) {
        return true;
    }

    // If outer characters differ, not a palindrome
    if (arr[start] != arr[end]) {
        return false;
    }

    // Reduce problem size: move inward
    return checkPalindromeRecursive(arr, start + 1, end - 1);
}

int main() {

    /* =========================================================================
       EX: Palindrome checking demo (iterative + recursive)
       - We compute end index using sizeof to avoid hardcoding.
       ======================================================================= */

    char str1[] = "kiran";
    int size1 = static_cast<int>(sizeof(str1) / sizeof(str1[0])); // includes '\0'
    int end1 = size1 - 2;                                         // last real char index

    bool result = checkPalindromeRecursive(str1, 0, end1);
    cout << "String - " << str1 << " is " << (result ? "palindrome" : "not a palindrome") << endl;

    char str2[] = "radar";
    int size2 = static_cast<int>(sizeof(str2) / sizeof(str2[0])); // includes '\0'
    int end2 = size2 - 2;                                         // last real char index

    result = checkPalindromeRecursive(str2, 0, end2);
    cout << "String - " << str2 << " is " << (result ? "palindrome" : "not a palindrome") << endl;

    // Optional: compare with iterative version
    result = checkPalindrome(str2, size2);
    cout << "Iterative check for " << str2 << " -> " << (result ? "palindrome" : "not a palindrome") << endl;

    return 0;
}
