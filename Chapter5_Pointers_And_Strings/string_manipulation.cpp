/*
    Author: Kiran Jojare
    File  : string_learning.cpp

    Purpose:
    --------
    This file is a "learning notebook" for classic C-string manipulation functions.
    The goal is to understand:
      - Exact semantics (what each function promises)
      - Common pitfalls (off-by-one, missing '\0', misuse of strncpy)
      - Embedded/production concerns (NULL policy, bounded operations, undefined behavior)

    Covered so far:
    ---------------
    1) strlen-style functions:
        - Array subscript version (clear + review-friendly)
        - Pointer iteration version (idiomatic C/C++)
        - Production-style (size_t + pointer difference)
        - Bounded length version (strnlen-style; critical for embedded buffers)

    2) strcpy-style function:
        - Pointer-based
        - Copy-then-test (ensures '\0' is copied exactly once)
        - Explicit NULL policy using assert
        - Notes on overlap/size dangers

    3) strncpy-style function:
        - Correct clone semantics: writes EXACTLY n bytes
        - May NOT null-terminate if src length >= n
        - Pads '\0' bytes if src ends before n
        - Explicit NULL policy using assert
        - Demonstration notes: printing result as a C-string can be unsafe if not terminated

    IMPORTANT CONCEPTS (carry these everywhere):
    -------------------------------------------
    A) C strings terminate with '\0' (NOT '\n').
    B) "Valid C string" means: somewhere reachable there is a terminating '\0'.
    C) strlen reads only; strcpy/strncpy write.
    D) Destination content is irrelevant for copy operations (you overwrite it).
    E) strcpy stops after copying '\0' (it MUST copy it).
    F) strncpy writes exactly n bytes; it does NOT guarantee '\0' termination.
    G) If buffers overlap, strcpy/strncpy are undefined behavior.
    H) If destination is too small, strcpy overflows (unsafe by design).
    I) For embedded buffers that might not be '\0'-terminated, prefer bounded functions
       (like strnlen) and eventually safe copy patterns (strlcpy-like) for production.

    NULL policy in this file:
    -------------------------
    We use assert() to catch programmer errors early in debug builds. In many embedded
    production builds NDEBUG may remove asserts; in that case, you can adopt a
    "return early" policy instead, but pick one policy and be consistent.

    NOTE:
    -----
    This file uses C++ (iostream, nullptr, bool). The concepts apply 1:1 to C.
*/

#include <iostream>
#include <cstddef>   // size_t
#include <cassert>   // assert

using namespace std;

/* ===========================================================
   STRLEN FAMILY
   =========================================================== */

/*
    strlen semantics:
    ----------------
    - Input: pointer to a null-terminated C string
    - Output: number of characters BEFORE '\0'
    - Does NOT count the '\0'
    - Does NOT modify memory

    Key pitfalls we learned:
    ------------------------
    1) Stop condition is '\0', NOT '\n'.
    2) Don't mix pointer increments and indexing in the same loop.
       - Either use s[i] and i++
       - Or use *p and p++
    3) Use size_t for lengths in production (avoid signed issues).
*/

/* -----------------------------------------------------------
   1) Learning version: Array subscript (review-friendly)
   ----------------------------------------------------------- */
size_t my_strlen_subscript(const char s[])
{
    if (s == nullptr) {
        return 0U;   // embedded-safe default policy for notes
    }

    size_t i = 0U;
    for (i = 0U; s[i] != '\0'; ++i) {
        /* intentionally empty */
    }
    return i;
}

/* -----------------------------------------------------------
   2) Learning version: Pointer iteration (idiomatic)
   ----------------------------------------------------------- */
size_t my_strlen_pointer(const char *s)
{
    if (s == nullptr) {
        return 0U;
    }

    size_t len = 0U;
    while (*s != '\0') {
        ++s;
        ++len;
    }
    return len;
}

/* -----------------------------------------------------------
   3) Production-style: size_t + pointer difference
   ----------------------------------------------------------- */
size_t my_strlen_prod(const char *s)
{
    if (s == nullptr) {
        return 0U;
    }

    const char *start = s;
    while (*s != '\0') {
        ++s;
    }

    return static_cast<size_t>(s - start);
}

/* -----------------------------------------------------------
   4) Embedded-critical: bounded length (strnlen-style)
   ----------------------------------------------------------- */
/*
    Why bounded length matters in embedded:
    --------------------------------------
    Not all buffers are guaranteed to have '\0' termination:
      - UART RX buffers
      - CAN payloads or diagnostic buffers
      - Fixed-length fields from network/protocol packets

    my_strnlen_prod:
    - Looks for '\0' but stops after max_len
    - Prevents runaway reads / memory over-read
*/
size_t my_strnlen_prod(const char s[], size_t max_len)
{
    if (s == nullptr) {
        return 0U;
    }

    size_t i = 0U;
    for (i = 0U; (i < max_len) && (s[i] != '\0'); ++i) {
        /* intentionally empty */
    }
    return i;
}

/* ===========================================================
   STRCPY
   =========================================================== */

/*
    strcpy semantics:
    ----------------
    - Copies src into dest, INCLUDING the terminating '\0'
    - Returns dest (the original start pointer)
    - Undefined behavior if buffers overlap
    - Undefined behavior if dest is too small (overflow)
    - Unsafe by design (no length parameter)

    Key learnings we discovered:
    ----------------------------
    1) Loop must be driven by SOURCE ONLY.
       Destination content is irrelevant because you're overwriting it.
    2) Must copy '\0' exactly once.
       If you stop when you *see* '\0' without copying it, dest is not a valid string.
    3) The easiest correct mental model is "copy-then-test".
       - Copy the byte
       - If the copied byte was '\0', stop

    About *(src++):
    --------------
    - *(src++) reads *src and then increments src
    - Side effects inside conditions can be correct but harder to audit in some standards.
*/

/* -----------------------------------------------------------
   my_strcpy: pointer-based, copy-then-test
   ----------------------------------------------------------- */
char * my_strcpy(char *dest, const char *src)
{
    assert(dest != nullptr && src != nullptr);

    char *start = dest;

    /*
        Copy-then-test pattern:
        - Copy current character from src to dest
        - Stop only after copying '\0'
    */
    do {
        *dest = *src;
        ++dest;
    } while (*(src++) != '\0');

    return start;
}

/* ===========================================================
   STRNCPY
   =========================================================== */

/*
    strncpy is NOT "safe strcpy":
    -----------------------------
    strncpy(dest, src, n) writes EXACTLY n bytes to dest.

    Two behaviors depending on src length:
    --------------------------------------
    Case A: src length < n
        - Copies src INCLUDING '\0'
        - Then PADS dest with additional '\0' bytes until total written == n

    Case B: src length >= n
        - Copies exactly n bytes from src
        - DOES NOT append '\0'
        - Result may NOT be a valid C string
          -> Using printf("%s") / strlen() / logging as string can read past buffer

    Why it existed historically:
    ----------------------------
    - Useful for fixed-size struct fields where you want deterministic full writes.

    Why it is dangerous today (especially embedded):
    ------------------------------------------------
    - Developers assume it guarantees null termination (it doesn't).
    - Non-terminated buffers passed to string functions can cause:
        - memory over-read
        - data corruption
        - hard faults
        - logs printing garbage / leaking memory contents

    Learning design approach we used:
    ---------------------------------
    - Think in terms of "write exactly n bytes"
    - Maintain a "copy mode" until src ends, then "pad mode"
*/

/* -----------------------------------------------------------
   my_strncpy: correct clone semantics (writes exactly n bytes)
   ----------------------------------------------------------- */
char * my_strncpy(char *dest, const char *src, size_t n)
{
    assert(dest != nullptr && src != nullptr);

    char *start = dest;

    /*
        'source_valid' tracks whether we are still copying actual src bytes
        (copy mode) or padding zeros (pad mode).
    */
    bool source_valid = true;

    while (n--) {
        if (source_valid) {
            *dest = *src;

            if (*src == '\0') {
                source_valid = false;  // switch to pad mode for remaining bytes
            } else {
                ++src;                  // advance src only while still copying
            }

            ++dest;                     // always advance dest
        } else {
            *dest = '\0';               // pad
            ++dest;
        }
    }

    return start;
}

/* ===========================================================
   MAIN (TESTS / DEMOS)
   =========================================================== */

/*
    NOTE ON TESTING strncpy:
    ------------------------
    If you copy only 2 bytes using strncpy-like behavior, the destination might not
    be null-terminated. Printing it as a C string (cout << dest) can read past the
    buffer until a '\0' happens to be found somewhere in memory.

    Therefore:
    - Use bounded printing or inspect bytes explicitly when demonstrating strncpy.
    - We'll keep a demonstration that is safe:
        - After strncpy with small n, we will manually set a terminator ONLY for printing.
        - That manual terminator is not part of strncpy semantics; it's to keep the demo safe.
*/

static void print_bytes(const char *buf, size_t len)
{
    // Helper for demonstration: prints raw bytes safely without assuming '\0'
    cout << "[";
    for (size_t i = 0U; i < len; ++i) {
        const unsigned char c = static_cast<unsigned char>(buf[i]);
        if (c >= 32U && c <= 126U) {
            cout << "'" << static_cast<char>(c) << "'";
        } else if (c == 0U) {
            cout << "'\\0'";
        } else {
            cout << "0x" << hex << static_cast<unsigned int>(c) << dec;
        }

        if (i + 1U < len) {
            cout << ", ";
        }
    }
    cout << "]";
}

int main()
{
    const char string1[] = "AAAAAAAAAAAAA";
    const char string2[] = "ZZZZZ";

    cout << "string1: " << string1 << "\n";
    cout << "subscript len : " << my_strlen_subscript(string1) << "\n";
    cout << "pointer len   : " << my_strlen_pointer(string1) << "\n";
    cout << "prod len      : " << my_strlen_prod(string1) << "\n";
    cout << "strnlen(5)    : " << my_strnlen_prod(string1, 5U) << "\n\n";

    cout << "string2: " << string2 << "\n";
    cout << "subscript len : " << my_strlen_subscript(string2) << "\n";
    cout << "pointer len   : " << my_strlen_pointer(string2) << "\n";
    cout << "prod len      : " << my_strlen_prod(string2) << "\n";
    cout << "strnlen(10)   : " << my_strnlen_prod(string2, 10U) << "\n\n";

    // Non-null-terminated buffer example (bounded function saves you)
    char rawBuf[5] = { 'H', 'E', 'L', 'L', 'O' }; // no '\0'
    cout << "rawBuf strnlen(5): " << my_strnlen_prod(rawBuf, sizeof(rawBuf)) << "\n\n";

    // strcpy demo (safe only if dest is large enough)
    char name1[] = "XXXXXXXXXX";    // 10 X's + '\0' => size 11
    const char name2[] = "HELLO";   // fits into name1

    cout << "Before strcpy: " << name1 << "\n";
    cout << "After  strcpy: " << my_strcpy(name1, name2) << "\n\n";

    // strncpy demo (show both behaviors)
    char field[11] = "XXXXXXXXXX";  // size 11

    // Case A: src shorter than n -> pads with '\0'
    my_strncpy(field, "HI", 8U);
    cout << "strncpy Case A (src=\"HI\", n=8): bytes=";
    print_bytes(field, 10U);
    cout << "\n";
    // Safe to print as string because it copied '\0' and padded:
    cout << "As string: \"" << field << "\"\n\n";

    // Case B: src length >= n -> may NOT terminate
    char field2[11] = "XXXXXXXXXX";
    my_strncpy(field2, "HELLO000000000000000000", 2U); // writes exactly 2 bytes: 'H','E'
    cout << "strncpy Case B (src=\"HELLO...\", n=2): bytes=";
    print_bytes(field2, 10U);
    cout << "\n";

    // DO NOT print field2 directly as a C string here; it may not be terminated.
    // If you must print for demo, add a terminator manually (not strncpy behavior):
    field2[2] = '\0';
    cout << "As string (manual terminator for demo only): \"" << field2 << "\"\n";

    return 0;
}
