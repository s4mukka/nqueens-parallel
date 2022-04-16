#include <stdio.h>
#include <stdlib.h>

#include "board.h"

Board *createBoard(int n) {
  int i, j;

  Board *board = (Board *) malloc(sizeof(Board));
  board->length = n;
  board->queens = 0;

  board->board = (char **) malloc(board->length * sizeof(char *));

  for (i = 0; i < board->length; i++) {
    board->board[i] = (char *) malloc(board->length * sizeof(char));
  }

  for (i = 0; i < board->length; i++)
    for (j = 0; j < board->length; j++)
      board->board[i][j] = ' ';

  return board;
}

void addQueen(Board *board, int row, int column) {
  board->board[row][column] = 'Q';
  board->queens++;
}

void removeQueen(Board *board, int row, int column) {
  board->board[row][column] = ' ';
  board->queens--;
}

void cleanBoard(Board *board) {
  int i, j;

  for (i = 0; i < board->length; i++) {
    free(board->board[i]);
  }

  free(board->board);
}

void printBoard(Board *board) {
  int i, j;

  printf("Board %dx%d\n", board->length, board->length);

  for (i = 0; i < board->length; i++) {
    for (j = 0; j < board->length; j++)
      printf("[%c]", board->board[i][j]);
    printf("\n");
  }
}