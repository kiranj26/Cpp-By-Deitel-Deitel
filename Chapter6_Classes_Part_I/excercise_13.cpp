/************************************************************
 * File: exercise_14.cpp
 *
 * Topic: Rectangle / Square detection from 4 Cartesian points
 *
 * What we learned (big picture):
 *  1) Invariants matter:
 *     - Each point must be in the 1st quadrant and within [0..20].
 *     - All 4 points must be distinct (no duplicates).
 *     - Only then we try to classify the polygon.
 *
 *  2) Do NOT assume input order:
 *     - Points can come in any order.
 *     - So we can't assume (P1->P2) is a side, (P2->P3) is a side, etc.
 *
 *  3) Use squared distances (avoid sqrt early):
 *     - For two points (x1,y1), (x2,y2):
 *         d^2 = (x2-x1)^2 + (y2-y1)^2
 *     - Squared distances avoid floating errors and still preserve comparisons.
 *
 *  4) Rectangle "distance fingerprint" (order independent):
 *     - With 4 points there are 6 pairwise distances:
 *         AB, AC, AD, BC, BD, CD  -> 6 distances
 *     - For a rectangle (non-square), squared distances form:
 *         { s1^2, s1^2, s2^2, s2^2, d^2,  d^2 }
 *       i.e. 3 unique values, each repeated twice.
 *
 *  5) Pythagorean check (right angle guarantee):
 *     - For a rectangle:
 *         diagonal^2 = side1^2 + side2^2
 *     - After sorting distances:
 *         d4 == d5 (equal diagonals)
 *         d0 == d1 (side1^2 pair)
 *         d2 == d3 (side2^2 pair)
 *         d4 == d0 + d2  (pythagoras in squared form)
 *
 *  6) Square is a special rectangle:
 *     - side1^2 == side2^2 (all sides equal)
 *     - diagonal^2 = 2 * side^2
 *
 *  7) Length / breadth / perimeter / area:
 *     - side lengths might not be integers if the rectangle is rotated.
 *     - So we compute sqrt only at the end using double.
 *
 ************************************************************/

#include <iostream>
#include <algorithm>
#include <cmath>
#include <iomanip>

using std::cout;
using std::endl;

class Rectangle {
private:
    // Store 4 points (x,y). Using int because constraints are integer-like [0..20].
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int x4, y4;

    // 6 pairwise squared distances between 4 points
    int dist2[6];

    // Derived geometry (computed after validation)
    double length;   // larger side
    double breadth;  // smaller side

private:
    // ---------- Utility / invariants helpers ----------

    bool inBounds(int x, int y) const {
        // First quadrant and within [0..20]
        return (x >= 0 && x <= 20) && (y >= 0 && y <= 20);
    }

    int squaredDistance(int ax, int ay, int bx, int by) const {
        // Use integer math for stability; no pow(), no sqrt().
        int dx = bx - ax;
        int dy = by - ay;
        return dx * dx + dy * dy;
    }

    void computeAllPairwiseDistances() {
        // There are exactly 6 unique pairs among 4 points.
        // We do NOT assume any order of points.

        dist2[0] = squaredDistance(x1, y1, x2, y2); // 12
        dist2[1] = squaredDistance(x1, y1, x3, y3); // 13
        dist2[2] = squaredDistance(x1, y1, x4, y4); // 14
        dist2[3] = squaredDistance(x2, y2, x3, y3); // 23
        dist2[4] = squaredDistance(x2, y2, x4, y4); // 24
        dist2[5] = squaredDistance(x3, y3, x4, y4); // 34
    }

    bool hasDuplicatePoints() const {
        // If any pairwise distance^2 == 0, then two points are identical.
        // A polygon with duplicate vertices is invalid.
        for (int i = 0; i < 6; ++i) {
            if (dist2[i] == 0) {
                return true;
            }
        }
        return false;
    }

    bool isRectangleByDistanceSignature() const {
        // dist2[] must already be sorted.
        //
        // For rectangle:
        //  - smallest two equal (side1^2)
        //  - next two equal (side2^2)
        //  - largest two equal (diagonal^2)
        //  - diagonal^2 = side1^2 + side2^2 (Pythagorean theorem converse)
        //
        // Sorted indices: 0 1 2 3 4 5

        bool pairs =
            (dist2[0] == dist2[1]) &&
            (dist2[2] == dist2[3]) &&
            (dist2[4] == dist2[5]);

        // Note: side1^2 is dist2[0], side2^2 is dist2[2], diagonal^2 is dist2[4]
        bool pythagoras = (dist2[4] == dist2[0] + dist2[2]);

        return pairs && pythagoras;
    }

    bool isSquareByDistanceSignature() const {
        // Square is a special rectangle:
        // - It's a rectangle, and
        // - All sides equal => side1^2 == side2^2
        // - diagonal^2 = 2 * side^2
        //
        // Using sorted list:
        // - dist2[0..3] are sides (all equal)
        // - dist2[4..5] are diagonals (equal)
        bool sidesAllEqual = (dist2[0] == dist2[1]) &&
                             (dist2[1] == dist2[2]) &&
                             (dist2[2] == dist2[3]);

        bool diagonalsEqual = (dist2[4] == dist2[5]);

        bool diagRelation = (dist2[4] == 2 * dist2[0]);

        // Also ensure it's a rectangle (right angles) using our rectangle check.
        return sidesAllEqual && diagonalsEqual && diagRelation && isRectangleByDistanceSignature();
    }

    void computeLengthBreadthFromSortedDistances() {
        // After sorting:
        // side1^2 = dist2[0]
        // side2^2 = dist2[2]
        // length = sqrt(max(side1^2, side2^2))
        // breadth = sqrt(min(side1^2, side2^2))
        int s1 = dist2[0];
        int s2 = dist2[2];

        int maxS = (s1 > s2) ? s1 : s2;
        int minS = (s1 > s2) ? s2 : s1;

        length = std::sqrt(static_cast<double>(maxS));
        breadth = std::sqrt(static_cast<double>(minS));
    }

public:
    // ---------- Construction / setting ----------

    Rectangle(int a1 = 1, int b1 = 1,
              int a2 = 2, int b2 = 1,
              int a3 = 2, int b3 = 2,
              int a4 = 1, int b4 = 2)
        : x1(1), y1(1), x2(2), y2(1), x3(2), y3(2), x4(1), y4(2),
          length(0.0), breadth(0.0)
    {
        // Keep the invariant idea:
        // - We start with a safe default rectangle.
        // - Then we attempt to set user-provided coordinates.
        setCoordinates(a1,b1, a2,b2, a3,b3, a4,b4);
    }

    void setCoordinates(int a1, int b1,
                        int a2, int b2,
                        int a3, int b3,
                        int a4, int b4)
    {
        // Validate each point. If any point is invalid, we fallback to a safe default set.
        // (You can choose a different policy: reject and keep old, etc.)

        if (!inBounds(a1,b1) || !inBounds(a2,b2) || !inBounds(a3,b3) || !inBounds(a4,b4)) {
            cout << "Error: One or more points out of bounds (must be 0..20 in first quadrant). Using defaults.\n";
            // Keep defaults already present.
            return;
        }

        x1 = a1; y1 = b1;
        x2 = a2; y2 = b2;
        x3 = a3; y3 = b3;
        x4 = a4; y4 = b4;
    }

    // ---------- Public query / output ----------

    void printCoordinates() const {
        cout << "( " << x1 << "," << y1 << " )\t"
             << "( " << x2 << "," << y2 << " )\t"
             << "( " << x3 << "," << y3 << " )\t"
             << "( " << x4 << "," << y4 << " )\n";
    }

    // Analyze shape: compute distances, sort, validate, classify, and compute metrics if valid rectangle.
    void analyzeAndPrintReport() {
        computeAllPairwiseDistances();

        // Reject duplicate points early
        if (hasDuplicatePoints()) {
            cout << "Invalid: duplicate points detected (distance^2 == 0). Not a polygon.\n";
            return;
        }

        // Sort distances for signature checks
        std::sort(dist2, dist2 + 6);

        cout << "Sorted squared distances:\n";
        for (int i = 0; i < 6; ++i) {
            cout << "  d" << i << "^2 = " << dist2[i] << "\n";
        }

        bool rect = isRectangleByDistanceSignature();
        bool sq   = isSquareByDistanceSignature();

        if (!rect) {
            cout << "Classification: NOT a rectangle (and therefore not a square).\n";
            return;
        }

        // It's a rectangle. Compute length/breadth/perimeter/area.
        computeLengthBreadthFromSortedDistances();

        cout << "Classification: RECTANGLE\n";
        if (sq) {
            cout << "Classification: SQUARE (special rectangle)\n";
        }

        // Print measures
        cout << std::fixed << std::setprecision(3);
        cout << "Length  = " << length  << "\n";
        cout << "Breadth = " << breadth << "\n";
        cout << "Perimeter = " << (2.0 * (length + breadth)) << "\n";
        cout << "Area      = " << (length * breadth) << "\n";
    }
};

int main() {

    auto runTest = [](const char* name,
                      int x1, int y1,
                      int x2, int y2,
                      int x3, int y3,
                      int x4, int y4)
    {
        cout << "\n====================================================\n";
        cout << "TEST: " << name << "\n";
        cout << "====================================================\n";

        Rectangle r(x1,y1, x2,y2, x3,y3, x4,y4);
        r.printCoordinates();
        r.analyzeAndPrintReport();
    };

    // A) Axis-aligned rectangle (not square): width=6, height=2
    runTest("Axis-aligned rectangle (expect rectangle, not square)",
            1,1, 7,1, 7,3, 1,3);

    // B) Axis-aligned square: side=5
    runTest("Axis-aligned square (expect square + rectangle)",
            3,3, 8,3, 8,8, 3,8);

    // C) Rotated square / diamond (expect square)
    runTest("Rotated square / diamond (expect square + rectangle)",
            5,2, 8,5, 5,8, 2,5);

    // D) Rhombus (not rectangle)
    runTest("Rhombus (expect NOT rectangle, NOT square)",
            1,1, 4,2, 7,1, 4,0);

    // E) Duplicate point
    runTest("Duplicate point (expect invalid)",
            1,1, 1,1, 4,1, 1,4);

    // F) Out of bounds
    runTest("Out of bounds coordinate (expect fallback defaults)",
            0,0, 25,0, 0,5, 25,5);

    return 0;
}
