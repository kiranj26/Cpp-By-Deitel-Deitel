// File: cturtle_graphics.cpp
// Purpose: Turtle graphics simulation (Logo-style) using a 20x20 floor grid and a command array
//
// How to use this file later:
// - Search "EX23" to jump to the turtle graphics exercise.
// - Commands are NOT read from cin. They are read from a predefined commands[] array.
// - The turtle starts at (0,0), pen UP, facing RIGHT.
// - Use command 6 to print the floor at any time.
// - Command format:
//     1 -> Pen Up
//     2 -> Pen Down
//     3 -> Turn Right
//     4 -> Turn Left
//     5, n -> Move forward n spaces (two integers: 5 then distance n)
//     6 -> Print floor
//     9 -> Exit

#include <iostream>
#include <iomanip>

using std::cout;
using std::endl;
using std::setw;

/* ============================================================================
   Constants (avoid magic numbers)
   ========================================================================== */
constexpr int kFloorSize = 20;

constexpr int kCmdPenUp     = 1;
constexpr int kCmdPenDown   = 2;
constexpr int kCmdTurnRight = 3;
constexpr int kCmdTurnLeft  = 4;
constexpr int kCmdMove      = 5;   // followed by a distance
constexpr int kCmdPrint     = 6;
constexpr int kCmdExit      = 9;

constexpr int kPenUp   = 0;
constexpr int kPenDown = 1;

// Direction encoding (clockwise):
// 0 = RIGHT, 1 = DOWN, 2 = LEFT, 3 = UP
constexpr int kDirRight = 0;
constexpr int kDirDown  = 1;
constexpr int kDirLeft  = 2;
constexpr int kDirUp    = 3;

/* ============================================================================
   Global state (kept simple for this exercise)
   ========================================================================== */
int floor_[kFloorSize][kFloorSize] = {0};

int turtleRow = 0;          // 0..19
int turtleCol = 0;          // 0..19
int turtlePen = kPenUp;     // default: UP
int turtleDir = kDirRight;  // default: facing RIGHT

/* ============================================================================
   Helper 1: Print the floor
   - Prints '*' where floor cell is 1
   - Prints '-' where floor cell is 0
   ========================================================================== */
void printFloor(const int array[][kFloorSize], int row, int col) {
    cout << "============================== FLOOR ==============================\n";
    for (int r = 0; r < row; r++) {
        for (int c = 0; c < col; c++) {
            if (array[r][c] == 1) {
                cout << setw(2) << '*';
            } else {
                cout << setw(2) << '-';
            }
        }
        cout << "\n";
    }
    cout << "===================================================================\n\n";
}

/* ============================================================================
   Helper 2: Turn right / left
   - Right: 0->1->2->3->0
   - Left:  0->3->2->1->0
   ========================================================================== */
void turnRight() {
    turtleDir = (turtleDir + 1) % 4;
}

void turnLeft() {
    turtleDir = (turtleDir + 3) % 4; // equivalent to -1 mod 4
}

/* ============================================================================
   Helper 3: Move forward N steps with boundary checks
   Behavior:
   - For each step, attempt to advance by 1 cell in the current direction.
   - If the next cell is outside the 20x20 grid, stop moving early.
   - If pen is DOWN, mark the cell we move INTO as 1.
   - If pen is UP, do not modify the floor at all.
   ========================================================================== */
void moveForward(int steps) {
    for (int i = 0; i < steps; i++) {
        int nextRow = turtleRow;
        int nextCol = turtleCol;

        if (turtleDir == kDirRight) {
            nextCol++;
        } else if (turtleDir == kDirDown) {
            nextRow++;
        } else if (turtleDir == kDirLeft) {
            nextCol--;
        } else { // kDirUp
            nextRow--;
        }

        // Boundary check: if next move is outside, stop moving early
        if (nextRow < 0 || nextRow >= kFloorSize || nextCol < 0 || nextCol >= kFloorSize) {
            return;
        }

        // Commit move
        turtleRow = nextRow;
        turtleCol = nextCol;

        // Trace only when pen is DOWN
        if (turtlePen == kPenDown) {
            floor_[turtleRow][turtleCol] = 1;
        }
    }
}

/* ============================================================================
   Helper 4: Process one command from the command stream
   - For command 5 (move), caller must provide the distance (next integer).
   ========================================================================== */
void processCommand(int cmd, int moveDistanceIfAny, bool hasMoveDistance) {
    switch (cmd) {
        case kCmdPenUp:
            turtlePen = kPenUp;
            break;

        case kCmdPenDown:
            turtlePen = kPenDown;
            // Optional: mark current cell immediately when pen goes down
            // This is useful if you want the starting point to be part of the drawing.
            floor_[turtleRow][turtleCol] = 1;
            break;

        case kCmdTurnRight:
            turnRight();
            break;

        case kCmdTurnLeft:
            turnLeft();
            break;

        case kCmdMove:
            if (hasMoveDistance && moveDistanceIfAny > 0) {
                moveForward(moveDistanceIfAny);
            }
            break;

        case kCmdPrint:
            printFloor(floor_, kFloorSize, kFloorSize);
            break;

        case kCmdExit:
            // Handled by main loop
            break;

        default:
            // Unknown command: ignore in this exercise
            break;
    }
}

int main() {

    /* =========================================================================
       EX23: Turtle Graphics (Logo-style simulation)
       Requirements satisfied:
       - 20x20 array floor initialized to 0.
       - Commands are read from an array.
       - Track turtle position + pen position.
       - Default: start at (0,0), pen UP, facing RIGHT.
       - Commands implemented:
           1 Pen Up
           2 Pen Down
           3 Turn Right
           4 Turn Left
           5,n Move forward n
           6 Print Floor
           9 Exit
       - While pen is DOWN, turtle traces by setting floor cells to 1.
       ======================================================================= */

    // Command stream example:
    // - Pen down
    // - Move 12
    // - Turn right (now facing DOWN)
    // - Move 5
    // - Turn right (now facing LEFT)
    // - Move 12
    // - Print
    // - Exit
    //
    // Format note:
    // - Any move uses two ints: 5 then distance
    const int commands[] = {
        kCmdPenDown,
        kCmdMove, 12,
        kCmdTurnRight,
        kCmdMove, 5,
        kCmdTurnRight,
        kCmdMove, 12,
        kCmdPrint,
        kCmdExit
    };

    const int commandsSize = static_cast<int>(sizeof(commands) / sizeof(commands[0]));

    for (int i = 0; i < commandsSize; i++) {
        int cmd = commands[i];

        if (cmd == kCmdExit) {
            break;
        }

        if (cmd == kCmdMove) {
            // Move command must have a distance next
            if (i + 1 < commandsSize) {
                int distance = commands[i + 1];
                processCommand(cmd, distance, true);
                i++; // consume the distance
            } else {
                // Move command missing distance; ignore
                processCommand(cmd, 0, false);
            }
        } else {
            processCommand(cmd, 0, false);
        }
    }

    return 0;
}
