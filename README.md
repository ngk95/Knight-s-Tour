Overview:
This program solves the classic problem of the Knight's Tour on a chessboard. It seeks to move a knight around an n x m board such that it visits every square exactly once.

Features:
Allows user-defined board sizes, ranging from 3x3 up to 10x10.
Permits a certain number of squares to be marked as "forbidden", meaning the knight can't step on them.
Provides feedback on the computation progress and paths tested.
Utilizes Warnsdorff's rule to prioritize knight moves based on potential next moves, increasing the efficiency of the search.
Handles input errors gracefully, prompting the user to re-enter invalid inputs.
How to Use:
Run the program.

Follow on-screen prompts to input:

The board's dimensions (rows and columns).
Number of forbidden squares.
Coordinates of each forbidden square.
Type of tour to look for.
Starting position of the knight.
Once the input is complete, the program will begin computing the knight's path.

Upon completion, the program will display the solution (if found) or inform you that no solution was found for the given configuration.
