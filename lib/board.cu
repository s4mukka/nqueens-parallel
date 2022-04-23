#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

char **initializeBoard(int length) {
  int i, j;
  char **board = (char **) malloc(length * sizeof(char *));

  for (i = 0; i < length; i++)
    board[i] = (char *) malloc(length * sizeof(char));

  for (i = 0; i < length; i++)
    for (j = 0; j < length; j++)
      board[i][j] = ' ';

  return board;
}

void printBoard(char **board, int length) {
  int i, j;

  printf("Board %dx%d\n", length, length);

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++)
      printf("[%c]", board[i][j]);
    printf("\n");
  }
}
