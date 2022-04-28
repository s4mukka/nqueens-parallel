#include "implementation.h"
#include <stdio.h>
#include <stdlib.h>

// void nQueens(char board[n][n], int length, int column, int *solutions) {
//   int i;

//   if (column >= length) {
//     (*solutions)++;
//     return;
//   }

//   for (i = 0; i < length; i++)
//     if (isSafe(board, length, i, column)) {
//       board[i][column] = 'Q';
//       nQueens(board, length, column + 1, solutions);
//       board[i][column] = ' ';
//     }
// }

void nQueens(char board[n][n], int length, int column, int *solutions) {
  int i, lastRow[length];

  for(i = 0; i < length; i++)
    lastRow[i] = 0;

  while (1) {
    
    if (column < 0) {
      return;
    }

    if (column >= length) {
      board[i][--column] = ' ';
      if (lastRow[column] < length - 1)
        lastRow[column]++;
      else {
        lastRow[column] = 0;
        column--;
        if (column >= 0) {
          board[lastRow[column]][column] = ' ';
          lastRow[column]++;
        }
      }
      (*solutions)++;
    }

    for (i = lastRow[column]; i < length; i++) {
      if (isSafe(board, length, i, column)) {
        board[i][column] = 'Q';
        lastRow[column++] = i;
        break;
      } else if (i == (length - 1)) {
        lastRow[column] = 0;
        column--;

        if (column >= 0) {
          board[lastRow[column]][column] = ' ';
          if (lastRow[column] < length - 1)
            lastRow[column]++;
          else {
            lastRow[column] = 0;
            column--;
            if (column >= 0) {
              board[lastRow[column]][column] = ' ';
              lastRow[column]++;
            }
          }
        }
      }
    }
  }
}

void solveNQueens(char board[n][n], int length) {
  int solutions = 0;

  nQueens(board, length, 0, &solutions);

  printf("Number of solutions: %d\n", solutions);
}

int isSafe(char board[n][n], int length, int row, int column) {
  int i, j;

  for (i = 0; i < column; i++)
    if (board[row][i] == 'Q')
      return FALSE;

  for (i = row, j = column; i >= 0 && j >= 0; i--, j--)
    if (board[i][j] == 'Q')
        return FALSE;

  for (i = row, j = column; j >= 0 && i < length; i++, j--)
    if (board[i][j] == 'Q')
      return FALSE;

  return TRUE;
}