#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <locale.h>

__device__ int deviceSolutions = 0;

void printTimeDifference(struct timeval t_ini, struct timeval t_fim);
__device__ int isSafe(char *board, int length, int row, int column);
__global__ void nQueens(char *board, int length, int column);
__device__ void nQueensR(char *board, int length, int column);
void solveNQueens(char *board, int length);
char *initializeBoard(int length);
void printBoard(char *board, int length);
__device__ void printBoardD(char *board, int length);

__device__ char * my_strcpy(char *dest, const char *src, int length){
  int i = 0;

  for(i = 0; i < length; i++)
    dest[i] = src[i];

  return dest;
}

int main(int argc, char **argv) {
  struct timeval t_ini, t_fim;
  gettimeofday(&t_ini, NULL);

  int n = strtol(argv[1], NULL, 10);

  char *chessboard = initializeBoard(n);

  // solveNQueens(chessboard, n);

   int solutions = 0;
  char *boardDevice;

  cudaMalloc((void **) &boardDevice, n * n * sizeof(char));
  cudaMemcpy(boardDevice, chessboard, n * n * sizeof(char), cudaMemcpyHostToDevice);

  size_t limit = limit = 1024 * n * n;

  cudaDeviceSetLimit(cudaLimitStackSize, limit);

  nQueens<<<1, n>>>(boardDevice, n, 0);

  cudaMemcpyFromSymbol(&solutions, deviceSolutions, sizeof(int));

  setlocale(LC_NUMERIC, "");

  cudaDeviceSynchronize();

  printf("Number of solutions: %'d\n", solutions);

  gettimeofday(&t_fim, NULL);
  printTimeDifference(t_ini, t_fim);

  cudaFree(boardDevice);

  return 0;
}

void printTimeDifference(struct timeval t_ini, struct timeval t_fim) {
  double d_time = t_fim.tv_sec + t_fim.tv_usec / 1e6
                    - t_ini.tv_sec - t_ini.tv_usec / 1e6;

  printf("Time: %.2fs\n", d_time);
}

#define TRUE 1
#define FALSE 0

__device__ void nQueensR(char *board, int length, int column) {
  int i;

  if (column >= length) {
    atomicAdd(&deviceSolutions, 1);
    return;
  }

  for (i = 0; i < length; i++) {
    if (isSafe(board, length, i, column)) {
      board[i * length + column] = 'Q';
      nQueensR(board, length, column + 1);
      board[i * length + column] = ' ';
    }
  }
}

// __device__ void nQueensR(char *board, int length, int column) {
//   int i, *lastRow;

//   lastRow = (int *) malloc(length * sizeof(int));

//   for(i = 0; i < length; i++)
//     lastRow[i] = 0;

//   lastRow[column]++;

//   printBoardD(board, length);

//   while (1) {
    
//     if (column < 0) {
//       return;
//     }

//     if (column >= length) {
//       // printBoardD(board, length);
//       column--;
//       board[i * length + column] = ' ';
//       if (lastRow[column] < length - 1)
//         lastRow[column]++;
//       else {
//         lastRow[column] = 0;
//         column--;
//         if (column >= 0) {
//           board[lastRow[column] * length + column] = ' ';
//           lastRow[column]++;
//         }
//       }
//       atomicAdd(&deviceSolutions, 1);;
//     }

//     for (i = lastRow[column]; i < length; i++) {
//       if (isSafe(board, length, i, column)) {
//         board[i * length + column] = 'Q';
//         lastRow[column++] = i;
//         break;
//       } else if (i == (length - 1)) {
//         lastRow[column] = 0;
//         column--;

//         if (column >= 0) {
//           board[lastRow[column] * length + column] = ' ';
//           if (lastRow[column] < length - 1)
//             lastRow[column]++;
//           else {
//             lastRow[column] = 0;
//             column--;
//             if (column >= 0) {
//               board[lastRow[column] * length + column] = ' ';
//               lastRow[column]++;
//             }
//           }
//         }
//       }
//     }
//   }
// }

__global__ void nQueens(char *board, int length, int column) {
  int i =  blockIdx.x * blockDim.x + threadIdx.x;

  char *copyBoard = (char *) malloc(length * length * sizeof(char));
  copyBoard = my_strcpy(copyBoard, board, length * length);

  copyBoard[i * length + column] = 'Q';
  // copyBoard[i] = 'Q';
  nQueensR(copyBoard, length, column + 1);
  copyBoard[i * length + column] = ' ';

  free(copyBoard);
}

// void solveNQueens(char *board, int length) {
//   int solutions = 0;
//   char **boardDevice;

//   cudaMalloc(boardDevice, length * length * sizeof(char));
//   cudaMemcpy(boardDevice, board, length * length * sizeof(char), cudaMemcpyHostToDevice);

//   nQueens<<<1, length>>>(boardDevice, length, 0);



//   setlocale(LC_NUMERIC, "");

//   printf("Number of solutions: %'d\n", solutions);
// }

__device__ int isSafe(char *board, int length, int row, int column) {
  int i, j;

  for (i = 0; i < column; i++)
    if (board[row * length + i] == 'Q')
      return FALSE;

  for (i = row, j = column; i >= 0 && j >= 0; i--, j--)
    if (board[i * length + j] == 'Q')
        return FALSE;

  for (i = row, j = column; j >= 0 && i < length; i++, j--)
    if (board[i * length + j] == 'Q')
      return FALSE;

  return TRUE;
}

char *initializeBoard(int length) {
  int i, j;
  char *board = (char *) malloc(length * length * sizeof(char));

  for (i = 0; i < length; i++)
    for (j = 0; j < length; j++)
      board[i * length + j] = ' ';

  return board;
}

void printBoard(char *board, int length) {
  int i, j;

  printf("Board %dx%d\n", length, length);

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++)
      printf("[%c]", board[i * length + j]);
    printf("\n");
  }
}

__device__ void printBoardD(char *board, int length) {
  int i, j;

  printf("Board %dx%d #%d (sum=%d)\n", length, length, blockIdx.x * blockDim.x + threadIdx.x, deviceSolutions);

  for (i = 0; i < length; i++) {
    for (j = 0; j < length; j++)
      printf("[%c]", board[i * length + j]);
    printf("\n");
  }
}
