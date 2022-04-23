#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <locale.h>

#include "implementation.h"

void printBoard2(char board[n][n], int length) {
  int i, j;

  printf("Board %dx%d\n", length, length);

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++)
      printf("[%c]", board[i][j]);
    printf("\n");
  }
}

void nQueens(char board[n][n], int length, int column, int *solutions) {

  char copy[n][n];

  int i;

  if (column >= length) {
    #pragma omp atomic
    (*solutions)++;
    return;
  }

  for (i = 0; i < length; i++) {
    if (isSafe(board, length, i, column)) {
      memcpy(copy, board, length * length * sizeof(char));

      #pragma omp task firstprivate(column)
      {
        copy[i][column] = 'Q';
        nQueens(copy, length, column + 1, solutions);
        copy[i][column] = ' ';
      }
    }
  }
}

void solveNQueens(char board[n][n], int length) {
  int i, solutions = 0;

  #pragma omp parallel
  {
    #pragma omp single
    {
      #pragma omp taskgroup
      {
        nQueens(board, length, 0, &solutions);
      }
    }
  }

  setlocale(LC_NUMERIC, "");

  printf("Number of solutions: %'d\n", solutions);
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
