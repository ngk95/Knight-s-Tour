#ifndef KNIGHT_TOUR_SOLVER_H
#define KNIGHT_TOUR_SOLVER_H

#include "Board.h"

class KnightTourSolver {
public:
    // Constructor accepting a Board object
    KnightTourSolver(Board& board);

    // Method to implement the recursive backtracking algorithm
    bool solveKnightTour(int startX, int startY, char closedTourChoice, char magicTourChoice);
    bool solve(int x, int y, int startX, int startY, int moveCount, char closedTourChoice, char magicTourChoice);

private:
    // Reference to the Board object
    Board& board;

    // Variables to store algorithm state
    int maxMoveCount;
    int deadEnds;
    int milDeadEnds;
    int forthMoveCount;

    // Method to compute the maximum move count for progress estimation
    void computeMaxMoveCount(int x, int y);
};

#endif  // KNIGHT_TOUR_SOLVER_H
