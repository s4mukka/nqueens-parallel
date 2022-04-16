typedef struct board {
  char **board;
  int queens;
  int length;
} Board;

Board *createBoard(int n);
void addQueen(Board *board, int row, int column);
void removeQueen(Board *board, int row, int column);
void cleanBoard(Board *board);
void printBoard(Board *board);
