#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <utility>

class Board {
public:
    // Constructor to initialize the board dimensions
    Board(int rows, int columns, int numForbidden, const std::vector<std::pair<int, int>>& forbiddenPositions, std::pair<int, int> startingPosition);

    // Method to print the current state of the board
    void printBoard() const;

    // Method to check if the board is magical
    bool isMagicBoard() const;

    // Method to check if the current board forms a closed tour
    bool isClosedTour(int currentX, int currentY, int startX, int startY) const;

    // Method to check the validity of a move
    bool isMoveValid(int x, int y, int lookupMove) const;

    // Method to get the list of possible moves from a position
    std::vector<std::pair<int, int>> getPossibleMoves(int x, int y, int lookupMove) const;

    // Method to compute the maximum move count for progress estimation
    int computeMaxMoveCount(int x, int y);

    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    int getNumForbidden() const { return numForbidden; }

    void setMoveCount(int x, int y, int moveCount);
    void resetMoveCount(int x, int y);

private:
    // The board matrix
    std::vector<std::vector<int>> board;

    // Rows and columns to store the dimensions of the board
    int rows;
    int columns;

    // Number of forbidden squares
    int numForbidden;

    // Helper method to initialize the board based on user input for forbidden squares and starting position
    void initializeBoard(const std::vector<std::pair<int, int>>& forbiddenPositions, std::pair<int, int> startingPosition);
};

#endif  // BOARD_H
