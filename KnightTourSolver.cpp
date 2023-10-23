#include "KnightTourSolver.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

KnightTourSolver::KnightTourSolver(Board& board) : board(board), maxMoveCount(0), deadEnds(0), milDeadEnds(0), forthMoveCount(0) {}

bool KnightTourSolver::solveKnightTour(int startX, int startY, char closedTourChoice, char magicTourChoice) {
    computeMaxMoveCount(startX, startY);
    int moveCount = 2;  // Starting with move count 2 since the first position is already set
    return solve(startX, startY, startX, startY, moveCount, closedTourChoice, magicTourChoice);
}

void KnightTourSolver::computeMaxMoveCount(int x, int y) {
    maxMoveCount = board.computeMaxMoveCount(x, y);
}

bool KnightTourSolver::solve(int x, int y, int startX, int startY, int moveCount, char closedTourChoice, char magicTourChoice) {
    if ((moveCount == board.getRows() * board.getColumns() - board.getNumForbidden() + 1) &&
        ((closedTourChoice == 'N' || closedTourChoice == 'n') || board.isClosedTour(x, y, startX, startY)) &&
        ((magicTourChoice == 'N' || magicTourChoice == 'n') || board.isMagicBoard())) {
        return true;
    }

    auto moves = board.getPossibleMoves(x, y, 0);
    std::sort(moves.begin(), moves.end(), [&](const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return board.getPossibleMoves(a.first, a.second, 0).size() < board.getPossibleMoves(b.first, b.second, 0).size();
    });

    if (moveCount == 5) {
        forthMoveCount++;
    }

    for (const auto& move : moves) {
        int nextX = move.first;
        int nextY = move.second;
        board.setMoveCount(nextX, nextY, moveCount);

        if (solve(nextX, nextY, startX, startY, moveCount + 1, closedTourChoice, magicTourChoice)) {
            return true;
        }

        board.resetMoveCount(nextX, nextY);  // Backtrack

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
