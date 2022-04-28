#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h> 
#include <string.h>
#include <omp.h>

#define N 14
bool SOLUTION_EXISTS = false;

bool can_be_placed(int board[N][N], int row, int col);
void print_solution(int board[N][N]);
bool solve_NQueens(int board[N][N], int col);


bool can_be_placed(int board[N][N], int row, int col) 
{ 
    int i, j; 
  
    /* Check this row on left side */
    for (i = 0; i < col; i++) 
        if (board[row][i]) 
            return false; 
  
    /* Check upper diagonal on left side */
    for (i=row, j=col; i>=0 && j>=0; i--, j--) 
        if (board[i][j]) 
            return false; 
  
    /* Check lower diagonal on left side */
    for (i=row, j=col; j>=0 && i<N; i++, j--) 
        if (board[i][j]) 
            return false; 
  
    return true; 
} 

void print_solution(int board[N][N]) 
{ 
    static int k = 1; 
    printf("Solution #%d-\n",k++); 
    int i,j;
    for (i = 0; i < N; i++) 
    { 
        for (j = 0; j < N; j++) 
            printf(" %d ", board[i][j]); 
        printf("\n"); 
    } 
    printf("\n"); 
}


bool solve_NQueens(int board[N][N], int col) 
{ 

    int new_board[N][N];
    memcpy(new_board, board, sizeof(new_board));


    if (col == N) 
    { 
        //#pragma omp critical
           // print_solution(board);
        #pragma omp critical
            SOLUTION_EXISTS = true;
        return true; 
    } 
    int i;
    for (i = 0; i < N; i++) 
    { 
        if (can_be_placed(board, i, col)) 
        { 
            new_board[i][col] = 1; 

            #pragma omp task firstprivate(col)
                solve_NQueens(new_board, col + 1); 

            new_board[i][col] = 0;
        }
    }

    return SOLUTION_EXISTS; 
}

void printTimeDifference(struct timeval t_ini, struct timeval t_fim) {
  double d_time = t_fim.tv_sec + t_fim.tv_usec / 1e6
                    - t_ini.tv_sec - t_ini.tv_usec / 1e6;

  printf("Time: %.2fs\n", d_time);
}


int main() 
{ 
    int board[N][N]; 
    memset(board, 0, sizeof(board)); 
    
    struct timeval t_ini, t_fim;
    gettimeofday(&t_ini, NULL);
    #pragma omp parallel
    {   
        #pragma omp single
        {
            #pragma omp taskgroup
            {
                solve_NQueens(board, 0);
            }
        }
    }
    

    if (SOLUTION_EXISTS == false) 
    { 
        printf("No Solution Exits! \n"); 
    } 
    gettimeofday(&t_fim, NULL);
    printTimeDifference(t_ini, t_fim);
    return 0;
} 