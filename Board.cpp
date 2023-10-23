#include "Board.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

Board::Board(int rows, int columns, int numForbidden, const std::vector<std::pair<int, int>>& forbiddenPositions, std::pair<int, int> startingPosition)
    : rows(rows), columns(columns), numForbidden(numForbidden), board(rows, std::vector<int>(columns, 0)) {
    initializeBoard(forbiddenPositions, startingPosition);
}

void Board::initializeBoard(const std::vector<std::pair<int, int>>& forbiddenPositions, std::pair<int, int> startingPosition) {
    for (const auto& position : forbiddenPositions) {
        board[position.first - 1][position.second - 1] = -1;
    }
    board[startingPosition.first - 1][startingPosition.second - 1] = 1;
}

void Board::printBoard() const {
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

bool Board::isMagicBoard() const {
    if (rows != columns) {
        return false;  // Not a square board
    }

    int expectedSum = 0;
    for (int value : board[0]) {
        expectedSum += value;
    }

    for (int i = 1; i < rows; i++) {
        int rowSum = 0;
        for (int j = 0; j < columns; j++) {
            rowSum += board[i][j];
        }
        if (rowSum != expectedSum) {
            return false;
        }
    }

    for (int j = 0; j < columns; j++) {
        int colSum = 0;
        for (int i = 0; i < rows; i++) {
            colSum += board[i][j];
        }
        if (colSum != expectedSum) {
            return false;
        }
    }

    int diag1Sum = 0, diag2Sum = 0;
    for (int i = 0; i < rows; i++) {
        diag1Sum += board[i][i];
        diag2Sum += board[i][rows - 1 - i];
    }

    return diag1Sum == expectedSum && diag2Sum == expectedSum;
}

bool Board::isClosedTour(int currentX, int currentY, int startX, int startY) const {
    auto possibleMoves = getPossibleMoves(currentX, currentY, 1);
    for (const auto& move : possibleMoves) {
        if (move.first == startX && move.second == startY) {
            return true;
        }
    }
    return false;
}

bool Board::isMoveValid(int x, int y, int lookupMove) const {
    if (x < 0 || x >= rows || y < 0 || y >= columns) {
        return false;  // Out of board boundaries
    }
    return board[x][y] == lookupMove;
}

std::vector<std::pair<int, int>> Board::getPossibleMoves(int x, int y, int lookupMove) const {
    static const int moveX[] = {2, 1, -1, -2, -2, -1, 1, 2};
    static const int moveY[] = {1, 2, 2, 1, -1, -2, -2, -1};

    std::vector<std::pair<int, int>> moves;
    for (int i = 0; i < 8; ++i) {
        int newX = x + moveX[i];
        int newY = y + moveY[i];
        if (isMoveValid(newX, newY, lookupMove)) {
            moves.push_back({newX, newY});
        }
    }
    return moves;
}

int Board::computeMaxMoveCount(int x, int y) {
    auto firstMoves = getPossibleMoves(x, y, 0);
    int maxMoveCount = 0;
    for (const auto& firstMove : firstMoves) {
        auto secondMoves = getPossibleMoves(firstMove.first, firstMove.second, 0);
        for (const auto& secondMove : secondMoves) {
            if ((secondMove.first == x && secondMove.second == y) ||
                (secondMove.first == firstMove.first && secondMove.second == firstMove.second)) continue;

            auto thirdMoves = getPossibleMoves(secondMove.first, secondMove.second, 0);
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

void Board::setMoveCount(int x, int y, int moveCount) {
    board[x][y] = moveCount;
}

void Board::resetMoveCount(int x, int y) {
    board[x][y] = 0;
}
