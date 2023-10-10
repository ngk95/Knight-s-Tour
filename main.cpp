#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

int maxMoveCount{0};
int deadEnds{0};
int milDeadEnds{0};
int forthMoveCount{0};

// Checks if a move is valid on the current board
bool isMoveValid(int x, int y, const std::vector<std::vector<int>>& board);

// Returns the valid moves for the current position
std::vector<std::pair<int, int>> getPossibleMoves(int x, int y, const std::vector<std::vector<int>>& board);

// Computes the maxMoveCount (up to 4th move), based on which is determined the progress percentage
int computeMaxMoveCount(int x, int y, const std::vector<std::vector<int>>& board);

// The algorithm
bool solveKnightTour(int x, int y, int moveCount, std::vector<std::vector<int>>& board, const int numForbidden);

// Prints the board if a solution is found
void printBoard(const std::vector<std::vector<int>>& board, int numForbidden);



//Checking if a move is valid on the current board setup
bool isMoveValid(int x, int y, const std::vector<std::vector<int>>& board, int lookupMove) {
    if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size()) {
        return false; // Out of board boundaries
    }
    return board[x][y] == lookupMove; // Return true only if square is the lookup move and not forbidden
}

// Get possible moves for specific square based on the current scenario
std::vector<std::pair<int, int>> getPossibleMoves(int x, int y, const std::vector<std::vector<int>>& board, int lookupMove) {
    static const int moveX[] = {2, 1, -1, -2, -2, -1, 1, 2};
    static const int moveY[] = {1, 2, 2, 1, -1, -2, -2, -1};

    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 8; ++i) {
        int newX = x + moveX[i];
        int newY = y + moveY[i];
        if (isMoveValid(newX, newY, board, lookupMove)) {
            moves.push_back({newX, newY});
        }
    }
    return moves;
}

bool isClosedTour(int currentX, int currentY, int startX, int startY, const std::vector<std::vector<int>>& board) {
    // Get all possible moves from the current position
    auto possibleMoves = getPossibleMoves(currentX, currentY, board, 1);

    // Check if any of the possible moves match the starting position
    for (const auto& move : possibleMoves) {
        if (move.first == startX && move.second == startY) {
            return true; // Found a move that goes back to the starting position
        }
    }
    return false; // No forward move matches the starting position
}

bool isMagicBoard(const std::vector<std::vector<int>>& board) {
    int n = board.size();
    if (n != board[0].size()) {
        return false; // Not a square board
    }

    // Calculate the sum for the first row to compare with others
    int expectedSum = 0;
    for (int value : board[0]) {
        expectedSum += value;
    }

    // Check rows
    for (int i = 1; i < n; i++) {
        int rowSum = 0;
        for (int j = 0; j < n; j++) {
            rowSum += board[i][j];
        }
        if (rowSum != expectedSum) {
            return false;
        }
    }

    // Check columns
    for (int j = 0; j < n; j++) {
        int colSum = 0;
        for (int i = 0; i < n; i++) {
            colSum += board[i][j];
        }
        if (colSum != expectedSum) {
            return false;
        }
    }

    // Check diagonals
    int diag1Sum = 0, diag2Sum = 0;
    for (int i = 0; i < n; i++) {
        diag1Sum += board[i][i];
        diag2Sum += board[i][n - 1 - i];
    }

    if (diag1Sum != expectedSum || diag2Sum != expectedSum) {
        return false;
    }

    return true;
}

int computeMaxMoveCount(int x, int y, const std::vector<std::vector<int>>& board) {
    auto firstMoves = getPossibleMoves(x, y, board, 0);
    for (const auto& firstMove : firstMoves) {
        auto secondMoves = getPossibleMoves(firstMove.first, firstMove.second, board, 0);

        for (const auto& secondMove : secondMoves) {
            // Skip if the second move is the starting position or the first move.
            if ((secondMove.first == x && secondMove.second == y) ||
                (secondMove.first == firstMove.first && secondMove.second == firstMove.second)) continue;

            auto thirdMoves = getPossibleMoves(secondMove.first, secondMove.second, board, 0);

            // Filter out third moves that overlap with starting, first, or second positions.
            thirdMoves.erase(std::remove_if(thirdMoves.begin(), thirdMoves.end(), [&](const std::pair<int, int>& move) {
                return (move.first == x && move.second == y) ||
                       (move.first == firstMove.first && move.second == firstMove.second) ||
                       (move.first == secondMove.first && move.second == secondMove.second);
            }), thirdMoves.end());

            maxMoveCount += thirdMoves.size();
        }
    }
    return maxMoveCount;
}

bool solveKnightTour(int x, int y, int startX, int startY, int moveCount, std::vector<std::vector<int>>& board, const int numForbidden, char closedTourChoice, char magicTourChoice) {
    // If all non-forbidden squares are visited and, if requested, the tour is closed, only then the Tour is complete
    if ((moveCount == board.size() * board[0].size() - numForbidden + 1) &&
        ((closedTourChoice == 'N' || closedTourChoice == 'n') || isClosedTour(x, y, startX, startY, board)) &&
        ((magicTourChoice == 'N' || magicTourChoice == 'n') || isMagicBoard(board))){
        return true;
    }

    auto moves = getPossibleMoves(x, y, board, 0);

    // Sort the possible next moves based on the number of their own possible next moves (Warnsdorff's rule)
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return getPossibleMoves(a.first, a.second, board, 0).size() < getPossibleMoves(b.first, b.second, board, 0).size();
    });

    if (moveCount == 5) {  // moveCount == 1 is the starting square, so not counted as a move
        forthMoveCount++;
    }

    for (const auto& move : moves) {
        int nextX = move.first;
        int nextY = move.second;
        board[nextX][nextY] = moveCount;

        if (solveKnightTour(nextX, nextY, startX, startY, moveCount + 1, board, numForbidden, closedTourChoice, magicTourChoice)) {
            return true;
        }

        board[nextX][nextY] = 0;  // Backtrack

        ++deadEnds;  //Backtrack means dead-end

        if (deadEnds == 10000000) {
            float percentage = (static_cast<float>(forthMoveCount) * 100) / maxMoveCount;
            ++milDeadEnds;
            deadEnds = 0;
            std::cout << milDeadEnds << "0 million paths tested... <" << std::fixed << std::setprecision(1) << percentage << "% complete.\n";
        }
    }

    return false;
}

// Print the board (takes into consideration the number of digits in the biggest number on the board)
void printBoard(const std::vector<std::vector<int>>& board, int numForbidden) {
    int rows = board.size();
    int columns = board[0].size();
    int maxMove = rows * columns - numForbidden;
    int cellWidth = std::to_string(maxMove).length();
    std::string forbiddenString(cellWidth, 'X');

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (board[i][j] == -1) {
                std::cout << "[" << forbiddenString << "]";
            } else {
                std::cout << "[" << std::setw(cellWidth) << board[i][j] << "]";
            }
        }
        std::cout << "\n";
    }
}

int main() {
    std::cout << "--- Welcome to the Knight's Tour Problem Solver! ---\n\n"
              << "The setup includes:\n"
              << "1. The size of the board (Min 3x3; Max 10x10)\n"
              << "2. The number of forbidden squares\n"
              << "3. The positions of the forbidden squares, if there are any\n"
              << "4. Choose whether to look for a closed tour\n"
              << "5. The starting point\n\n"
              << "Please keep in mind that for larger boards with forbidden squares,\n"
              << "in some cases the computation time might be longer.\n\n"
              << "Let's begin!\n\n";

    int rows, columns;

    std::cout << "Enter number of rows (Min. 3; Max. 10): ";
    while (!(std::cin >> rows) || rows < 3 || rows > 10) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please input a number from " << 3 << " to " << 10 << ".\n";
        std::cout << "Enter number of rows (Min. 3; Max. 10): ";
    }

    std::cout << "Enter number of columns (Min. 3; Max. 10): ";
    while (!(std::cin >> columns) || columns < 3 || columns > 10) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please input a number from " << 3 << " to " << 10 << ".\n";
        std::cout << "Enter number of columns (Min. 3; Max. 10): ";
    }

    std::vector<std::vector<int>> board(rows, std::vector<int>(columns, 0));
    int numForbidden;

    std::cout << "Enter number of forbidden squares: ";
    while (!(std::cin >> numForbidden) || numForbidden < 0 || numForbidden > rows * columns - 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please input a number from " << 0 << " to " << rows * columns - 1 << ".\n";
        std::cout << "Enter number of forbidden squares: ";
    }

    for (int i = 0; i < numForbidden; ++i) {
        int x, y;
        std::cout << "Enter coordinates for forbidden square " << i + 1 << " (row column): ";
        while (!(std::cin >> x >> y) || x < 1 || x > rows || y < 1 || y > columns || board[x-1][y-1] == -1) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid coordinates. Please retry.\n";
        }
        board[x-1][y-1] = -1;
    }

    char closedTourChoice;
    std::cout << "Would you like to look for a closed tour? (Y/N): ";
    while (!(std::cin >> closedTourChoice) || (closedTourChoice != 'Y' && closedTourChoice != 'y' && closedTourChoice != 'N' && closedTourChoice != 'n')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter Y or N: ";
    }

    char magicTourChoice;
    /*TO REPLACE magicTourChoice = 'N';
    std::cout << "Would you like to look for a magical tour? (Y/N): ";
    while (!(std::cin >> magicTourChoice) || (magicTourChoice != 'Y' && magicTourChoice != 'y' && magicTourChoice != 'N' && magicTourChoice != 'n')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter Y or N: ";
    }
    */
    magicTourChoice = 'N';

    int startX, startY;
    std::cout << "Enter the starting position (row column): ";
    while (!(std::cin >> startX >> startY) || startX < 1 || startX > rows || startY < 1 || startY > columns || board[startX-1][startY-1] == -1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid starting position. Please retry.\n";
    }
    --startX;
    --startY;
    board[startX][startY] = 1;
    int maxMoveCount = computeMaxMoveCount(startX, startY, board);

    std::cout << "Searching for a solution...\n";
    if (solveKnightTour(startX, startY, startX, startY, 2, board, numForbidden, closedTourChoice, magicTourChoice)) {
        std::cout << deadEnds + 1 << " path(s) tested. Solution found!\n";
        printBoard(board, numForbidden);
    } else {
        std::cout << "All paths (" << milDeadEnds * 10000000 + deadEnds << ") were tested and no solution was found from this starting point.\n";
    }

    return 0;
}
