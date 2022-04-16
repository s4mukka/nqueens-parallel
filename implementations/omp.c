#include <stdio.h>
#include <omp.h>

#include "implementation.h"

int solveNQueens(Board *board, int column) {
  int i;

  if (column >= board->length)
    return TRUE;

  #pragma omp parallel
  #pragma omp for private(board, column)
  for (i = 0; i < board->length; i++) {
    if (isSafe(board, i, column)) {
      #pragma omp task firstprivate(board, i, column)
      {
        addQueen(board, i, column);
        if (!solveNQueens(board, column + 1)) {
          removeQueen(board, i, column);
        }
      }
    }
    // #pragma omp taskwait
  }


  return board->queens == board->length;
}

// int solveNQueens(Board *board, int column, int solved) {
//   if (column >= board->length)
//     return TRUE;

//   if (solved)
//     return TRUE;

//   int i;


//   for (i = 0; i < board->length; i++) {
//     if (isSafe(board, i, column) && !solved) {
//       board->board[i][column] = 'Q';

//       #pragma omp task firstprivate(board, column)
//       {
//         if (solveNQueens(board, column + 1, solved))
//           solved = TRUE;

//         if (!solved)
//           board->board[i][column] = ' ';
//       }
//     }
//   }

//   printf("===COLUMN #%d===SOLVED #%s===\n", column, solved ? "True" : "False");
//   printBoard(board);
//   // #pragma omp taskwait
//   return solved;
// }

// int solveNQueens(Board *board) {
//   int i, column, lastRow[board->length];

//   for(i = 0; i < board->length; i++)
//     lastRow[i] = 0;

//   column = 0;
//   // #pragma omp parallel for
//   while (column < board->length) {
//     for (i = lastRow[column]; i < board->length; i++) {
//       if (isSafe(board, i, column)) {
//         board->board[i][column] = 'Q';
//         lastRow[column++] = i;
//         i = board->length;
//       } else if (i == (board->length - 1)) {
//         lastRow[column] = 0;
//         column--;
//         if (column >= 0) {
//           board->board[lastRow[column]][column] = ' ';
//           if (lastRow[column] < board->length - 1)
//             lastRow[column]++;
//           else {
//             lastRow[column] = 0;
//             column--;
//             if (column >= 0) {
//               board->board[lastRow[column]][column] = ' ';
//               lastRow[column]++;
//             }
//           }
//         }
//       }
//     }
//   }

//   if (column < 0) {
//     return FALSE;
//   }

//   return TRUE;
// }

// int solveNQueens(Board *board, int column) {
//   if (column >= board->length)
//     return TRUE;

//   int i, solved = 0;

//   #pragma omp parallel shared(solved)
//   {
//     #pragma omp for
//     for (i = 0; i < board->length; i++) {
//       if (isSafe(board, i, column) && !solved) {
//         board->board[i][column] = 'Q';

//         #pragma omp task
//         {
//           if (solveNQueens(board, column + 1)) {
//             solved = 1;
//           }

//           if (!solved) {
//             board->board[i][column] = ' ';
//           }
//         }
//       }
//     }
//   }

//   if (solved) {
//     return TRUE;
//   }

//   return FALSE;
// }

int isSafe(Board *board, int row, int column) {
  int i, j;

  for (i = 0; i < column; i++)
    if (board->board[row][i] == 'Q')
      return FALSE;

  for (i = row, j = column; i >= 0 && j >= 0; i--, j--)
    if (board->board[i][j] == 'Q')
        return FALSE;

  for (i = row, j = column; j >= 0 && i < board->length; i++, j--)
    if (board->board[i][j] == 'Q')
      return FALSE;

  return TRUE;
}