#include "implementation.h"
#include <stdio.h>
#include <stdlib.h>

// int solveNQueens(Board *board, int column, int solved) {
//   if (column >= board->length)
//     return TRUE;

//   int i;

//   for (i = 0; i < board->length; i++) {
//     if (isSafe(board, i, column)) {
//       board->board[i][column] = 'Q';

//       if (solveNQueens(board, column + 1, solved))
//         return TRUE;

//       board->board[i][column] = ' ';
//     }
//   }

//   return FALSE;
// }

// int solveNQueens(Board *board) {
//   int i, column, lastRow[board->length];

//   for(i = 0; i < board->length; i++)
//     lastRow[i] = 0;

//   column = 0;
//   while (column < board->length && column >= 0) {
//     for (i = lastRow[column]; i < board->length; i++) {
//       if (isSafe(board, i, column)) {
//         board->board[i][column] = 'Q';
//         lastRow[column++] = i;
//         break;
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

int solveNQueens(Board *board, int column) {
  int i;

  if (column >= board->length)
    return TRUE;

  for (i = 0; i < board->length; i++) {
    if (isSafe(board, i, column)) {
      addQueen(board, i, column);

      if (solveNQueens(board, column + 1))
        return TRUE;

      removeQueen(board, i, column);
    }
  }

  return FALSE;
}

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