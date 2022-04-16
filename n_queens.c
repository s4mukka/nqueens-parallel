#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "implementations/implementation.h"

void printTimeDifference(struct timeval t_ini, struct timeval t_fim);

int main(int argc, char **argv) {
  struct timeval t_ini, t_fim;
  gettimeofday(&t_ini, NULL);

  int n = strtol(argv[1], NULL, 10);
  Board *chessboard;

  chessboard = createBoard(n);

  printf(solveNQueens(chessboard, 0) ? "Solved\n" : "Not solved\n");

  printBoard(chessboard);

  cleanBoard(chessboard);
  free(chessboard);

  gettimeofday(&t_fim, NULL);
  printTimeDifference(t_ini, t_fim);

  return 0;
}



void printTimeDifference(struct timeval t_ini, struct timeval t_fim) {
  double d_time = t_fim.tv_sec + t_fim.tv_usec / 1e6
                    - t_ini.tv_sec - t_ini.tv_usec / 1e6;

  printf("Time: %.2fs\n", d_time);
}
