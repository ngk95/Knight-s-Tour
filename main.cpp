#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include "Board.h"
#include "KnightTourSolver.h"

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

    int numForbidden;
    std::cout << "Enter number of forbidden squares: ";
    while (!(std::cin >> numForbidden) || numForbidden < 0 || numForbidden > rows * columns - 1) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please input a number from " << 0 << " to " << rows * columns - 1 << ".\n";
        std::cout << "Enter number of forbidden squares: ";
    }

    std::vector<std::pair<int, int>> forbiddenPositions(numForbidden);
    for (int i = 0; i < numForbidden; ++i) {
        int x, y;
        std::cout << "Enter coordinates for forbidden square " << i + 1 << " (row column): ";
        while (!(std::cin >> x >> y) || x < 1 || x > rows || y < 1 || y > columns) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid coordinates. Please retry.\n";
        }
        forbiddenPositions[i] = {x, y};
    }

    char closedTourChoice;
    std::cout << "Would you like to look for a closed tour? (Y/N): ";
    while (!(std::cin >> closedTourChoice) || (closedTourChoice != 'Y' && closedTourChoice != 'y' && closedTourChoice != 'N' && closedTourChoice != 'n')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter Y or N: ";
    }

    char magicTourChoice;
    std::cout << "Would you like to look for a magical tour? (Y/N): ";
    while (!(std::cin >> magicTourChoice) || (magicTourChoice != 'Y' && magicTourChoice != 'y' && magicTourChoice != 'N' && magicTourChoice != 'n')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid choice. Please enter Y or N: ";
    }

    int startX, startY;
    std::cout << "Enter the starting position (row column): ";
    while (true) {
        if (!(std::cin >> startX >> startY) || startX < 1 || startX > rows || startY < 1 || startY > columns) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Starting position out of boundaries. Please retry.\n";
        } else if (std::find(forbiddenPositions.begin(), forbiddenPositions.end(), std::make_pair(startX, startY)) != forbiddenPositions.end()) {
            std::cout << "Starting position overlaps with a forbidden square. Please retry.\n";
        } else {
            break;  // Exit the loop if the input is valid
        }
    }

    Board board(rows, columns, numForbidden, forbiddenPositions, {startX, startY});
    KnightTourSolver solver(board);

    std::cout << "Searching for a solution...\n";
    if (solver.solveKnightTour(startX - 1, startY - 1, closedTourChoice, magicTourChoice)) {
        std::cout << "Solution found!\n";
        board.printBoard();
    } else {
        std::cout << "No solution was found from this starting point.\n";
    }

    return 0;
}
