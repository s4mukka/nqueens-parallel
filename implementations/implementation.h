#include "../lib/board.h"

#define TRUE 1
#define FALSE 0

int solveNQueens(Board *board, int column);
int isSafe(Board *board, int row, int column);