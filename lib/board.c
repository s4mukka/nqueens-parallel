#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "board.h"

void initializeBoard(char board[n][n], int length) {
  int i, j;

  #pragma omp parallel for
  for (i = 0; i < length; i++)
    for (j = 0; j < length; j++)
      board[i][j] = ' ';
}

void printBoard(char board[n][n], int length) {
  int i, j;

  printf("Board %dx%d\n", n, n);

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++)
      printf("[%c]", board[i][j]);
    printf("\n");
  }
}
