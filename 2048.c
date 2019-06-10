//
//  2048.c
//  C-playground
//
//  Created by Jing Wang (Jane)
//  NUID: 001636175
//

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <termios.h>
#include <unistd.h>


#define MAX 16
#define GOAL 2048 // You cound change it if you want a different goal
#define FIRST 239
#define UP 128
#define DOWN 129
#define LEFT 130
#define RIGHT 131

// the matrix label for mapping four directions
int right[4][4] = {{3, 7, 11, 15},
                       {2, 6, 10, 14},
                       {1, 5, 9, 13},
                       {0, 4, 8, 12}};

int left[4][4] = {{0, 4, 8, 12},
                     {1, 5, 9, 13},
                     {2, 6, 10, 14},
                     {3, 7, 11, 15}};

int up[4][4] = {{0, 1, 2, 3},
                    {4, 5, 6, 7},
                    {8, 9, 10, 11},
                    {12, 13, 14, 15}};

int down[4][4] = {{12, 13, 14, 15},
                      {8, 9, 10, 11},
                      {4, 5, 6, 7},
                      {0, 1, 2, 3}};

//new number array to give 4:20%, 2:80%
int newnumber[5] ={4, 2, 2, 2, 2};

//get a random empty cell in the board
int get_empty_cell(int board[]){
    int r = rand()% MAX;
    if (board[r] == 0) return r;
    return get_empty_cell(board);
}

// put a random 2/4 in the chosen cell
void put_number_in_cell (int board[]){
    int newcell = get_empty_cell(board);
    int r = rand()%5;
    assert(board[newcell] == 0);
    board[newcell] = newnumber[r];
    return;
}

//print the current board
void emitBoard(int board[])
{
    printf("\n");
    
    char s[MAX][5] = {0};
    for(int i=0; i< MAX; i++) {
            if (board[i] != 0){
                sprintf(s[i], "%4d", board[i]);}
    }
    
    printf(" %4s | %4s | %4s | %4s | \n",
           s[0],
           s[1],
           s[2],
           s[3]);
    
    printf("---------------------------\n");
    
    printf(" %4s | %4s | %4s | %4s | \n",
           s[4],
           s[5],
           s[6],
           s[7]);
    
    printf("----------------------------\n");
    
    printf(" %4s | %4s | %4s | %4s | \n",
           s[8],
           s[9],
           s[10],
           s[11]);
    
    printf("----------------------------\n");
    
    printf(" %4s | %4s | %4s | %4s | \n",
           s[12],
           s[13],
           s[14],
           s[15]);
    
    return;
}

//  for shifting the cells towards direction
int shift_cell(int board[],int dir[4][4]){
    int canshift = 0;
    for(int i = 0; i<4; i++){
        int count = 0;
        for(int j = 0; j<4; j++){
            if (board[dir[j][i]] != 0) {
                board[dir[count][i]] = board[dir[j][i]];
                if (count != j) {
                    board[dir[j][i]] = 0;
                    canshift = 1;
                }
                count = count+1;
            }
        }
    }
    return canshift;
}

//  for combining the cells towards direction
int combine_cell(int board[],int dir[4][4]){
    int cancombine =0;
    for(int i = 0; i<4; i++){
        for(int j = 1; j<4; j++){
            if ((board[dir[j-1][i]] == board[dir[j][i]]) && (board[dir[j][i]] != 0)){
                board[dir[j-1][i]] = board[dir[j-1][i]]*2;
                for(int a = j+1; a<4; a++){
                    board[dir[a-1][i]] = board[dir[a][i]];}
                board[dir[3][i]] = 0;
                cancombine = 1;
            }
        }
    }
    return cancombine;
}

// for judege if the board cound be moved towards giving direction
int moveboard(int board[],int dir[4][4]){
    int shift = shift_cell(board,dir);
    int combine = combine_cell(board,dir);
    return shift||combine;
}

// for including getch()
int getch(){
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

// for dealing with four arrow keys and accordingly moving the board
int get_dir(int board[]){
    if (getch() == FIRST) {
        getch();
        switch(getch()) {
            case UP:
                printf("up");
                if (!moveboard(board,up)){
                    printf(" no move!");
                    get_dir(board);
                }
                break;
                
            case DOWN:
                printf("down");
                if (!moveboard(board,down)){
                    printf(" no move!");
                    get_dir(board);
                }
                break;
                
            case LEFT:
                printf("left");
                if (!moveboard(board,left)){
                    printf(" no move!");
                    get_dir(board);
                }
                break;
    
            case RIGHT:
                printf("right");
                if (!moveboard(board,right)){
                    printf(" no move!");
                    get_dir(board);
                }
                break;
        }
        return 1;
    }
    return 0;
}

// for judging if the player lose
int lose(int board[]){
    int test[MAX];
    for(int i=0;i<MAX;i++){test[i]=board[i];};
    if ((moveboard(test,up))||
        (moveboard(test,down))||
        (moveboard(test,left))||
        (moveboard(test,right))){
        return 0;
    }
    printf("\nYou Lose!\n\n");
    return 1;
}

// for judging if the player win
int win(int board[]){
    for(int i = 0; i<MAX; i++){
        if (board[i] == GOAL) {printf("\nYou Win!\n\n"); return 1;}
    }
    return 0;
}

int main(){
    srand((unsigned int)time(NULL)); // seed the ramdom number
    int cur_board[MAX] = {0}; // initiate and clear the board
    
    put_number_in_cell(cur_board); // put two random numbers in the board as the start
    put_number_in_cell(cur_board);
    emitBoard(cur_board);
    
    while ((!win(cur_board)) && (!lose(cur_board))){ // while the game continues, print select line and moving the board
        printf("\nSelect a direction (↑ ← ↓ →)");
        if (get_dir(cur_board)) {
            put_number_in_cell(cur_board);
            emitBoard(cur_board);
        }
    }
    return 0;
}
