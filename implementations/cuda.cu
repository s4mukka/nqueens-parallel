// #include "implementation.h"
#include <stdio.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0

__device__ int isSafe(char **board, int length, int row, int column) {
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

__device__ void nQueens(char **board, int length, int column, int *solutions) {
  int i;

  if (column >= length) {
    (*solutions)++;
    return;
  }

  for (i = 0; i < length; i++)
    if (isSafe(board, length, i, column)) {
      board[i][column] = 'Q';
      nQueens(board, length, column + 1, solutions);
      board[i][column] = ' ';
    }
}

__global__ void solve(char **board, int length, int column, int *solutions) {
  nQueens(board, length, column, solutions);
}

void solveNQueens(char **board, int length) {
  int solutions = 0;
  int *deviceSolutions;
  char *deviceBoard;

  cudaMallocManaged((void **) &deviceSolutions, sizeof(int));
  cudaMemcpy(deviceSolutions, &solutions, sizeof(int), cudaMemcpyHostToDevice);

  cudaMallocManaged((void **) &deviceBoard, length * length * sizeof(char));
  cudaMemcpy(deviceBoard, board, length * length * sizeof(char), cudaMemcpyHostToDevice);

  dim3 threadsPerBlock(length, length);
  
  solve<<<1, threadsPerBlock>>>(&deviceBoard, length, 0, &solutions);

  printf("Number of solutions: %d\n", solutions);
}
