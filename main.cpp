#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <iomanip>

bool isMoveValid(int x, int y, const std::vector<std::vector<int>>& board);
std::vector<std::pair<int, int>> getPossibleMoves(int x, int y, const std::vector<std::vector<int>>& board);
bool solveKnightTour(int x, int y, int moveCount, std::vector<std::vector<int>>& board, const int numForbidden);
void printBoard(const std::vector<std::vector<int>>& board, int numForbidden);

int main() {
    std::cout << "--- Welcome to the Knight's Tour Problem Solver! ---\n\n"
              << "The setup includes:\n"
              << "1.The size of the board (Min 3x3; Max 10x10)\n"
              << "2.The number of forbidden squares\n"
              << "3.The positions of the forbidden squares, if there are any\n"
              << "4.The starting point\n\n"
              << "Please keep in mind that for larger boards with forbidden squares,\n"
              << "in some cases the computation time might be longer.\n\n"
              << "Let's begin!\n\n";

    int rows, columns;

    std::cout << "Enter number of rows (Min. 3; Max. 10): ";
    while (!(std::cin >> rows) || rows < 3 || rows > 100) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please input a number from " << 3 << " to " << 10 << ".\n";
        std::cout << "Enter number of rows (Min. 3; Max. 10): ";
    }

    std::cout << "Enter number of columns (Min. 3; Max. 10): ";
    while (!(std::cin >> columns) || columns < 3 || columns > 100) {
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

    int startX, startY;
    std::cout << "Enter the starting position (row column): ";
    while (!(std::cin >> startX >> startY) || startX < 1 || startX > rows || startY < 1 || startY > columns || board[startX-1][startY-1] == -1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid starting position. Please retry.\n";
    }
    board[startX-1][startY-1] = 1;

    std::cout << "Searching for a solution...\n";
    if (solveKnightTour(startX-1, startY-1, 2, board, numForbidden)) {
        std::cout << "Solution found!\n";
        printBoard(board, numForbidden);
    } else {
        std::cout << "No solution available from this starting point.\n";
    }

    return 0;
}

//Checking if I move is valid on the current board setup
bool isMoveValid(int x, int y, const std::vector<std::vector<int>>& board) {
    if (x < 0 || x >= board.size() || y < 0 || y >= board[0].size()) {
        return false;  // Out of board boundaries
    }
    return board[x][y] == 0;  // Return true only if square is unvisited and not forbidden
}

// Get possible moves for specific square based on the current scenario
std::vector<std::pair<int, int>> getPossibleMoves(int x, int y, const std::vector<std::vector<int>>& board) {
    static const int moveX[] = {2, 1, -1, -2, -2, -1, 1, 2};
    static const int moveY[] = {1, 2, 2, 1, -1, -2, -2, -1};

    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 8; ++i) {
        int newX = x + moveX[i];
        int newY = y + moveY[i];
        if (isMoveValid(newX, newY, board)) {
            moves.push_back({newX, newY});
        }
    }
    return moves;
}

// Recursion using Warnsdorff's rule and backtracking.
bool solveKnightTour(int x, int y, int moveCount, std::vector<std::vector<int>>& board, const int numForbidden) {

    // If all non-forbidden squares are visited, the Tour is complete
    if (moveCount == board.size() * board[0].size() - numForbidden + 1) {
        return true;
    }

    auto moves = getPossibleMoves(x, y, board);
    // Sort the possible next moves based on the number of their own possible next moves (Warnsdorff's rule)
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return getPossibleMoves(a.first, a.second, board).size() < getPossibleMoves(b.first, b.second, board).size();
    });

    for (const auto& move : moves) {
        int nextX = move.first;
        int nextY = move.second;
        board[nextX][nextY] = moveCount;
        if (solveKnightTour(nextX, nextY, moveCount + 1, board, numForbidden)) {
            return true;
        }
        board[nextX][nextY] = 0;  // Backtrack
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
