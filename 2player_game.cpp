#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
using namespace std;

int ROW_COUNT = 6;
int COLUMN_COUNT = 7;

int** create_board(){
  	int** board = (int **)calloc(ROW_COUNT, sizeof(int *));
  	for (int i=0; i<ROW_COUNT; i++)
        board[i] = (int *)calloc(COLUMN_COUNT, sizeof(int));
	return board;
}

void drop_piece(int** board, int row, int col, int piece){
	board[row][col] = piece;
}

int is_valid_location(int** board, int col){
	if (board[ROW_COUNT-1][col] == 0)
    	return 1;
  	return 0;
}

int get_next_open_row(int** board, int col){
	for (int r = 0; r < ROW_COUNT; r++)
		if (board[r][col] == 0)
			return r;
	return -1;
}

void print_board(int** board){
	for (int i = ROW_COUNT-1; i > -1; i--){
      	for (int j = COLUMN_COUNT-1; j > -1; j--)
         	printf("%d ", board[i][j]);
		printf("\n");
	}
	printf("\n");
}

int winning_move(int** board, int piece){
	// Check horizontal locations for win
	for (int c = 0; c < COLUMN_COUNT-3; c++)
		for (int r = 0; r < ROW_COUNT; r++)
			if (board[r][c] == piece && board[r][c+1] == piece && board[r][c+2] == piece && board[r][c+3] == piece)
				return 1;

	// Check vertical locations for win
	for (int c = 0; c < COLUMN_COUNT; c++)
		for(int r = 0; r < ROW_COUNT-3; r++)
			if (board[r][c] == piece && board[r+1][c] == piece && board[r+2][c] == piece && board[r+3][c] == piece)
				return 1;

	// Check positively sloped diaganols
	for (int c = 0; c < COLUMN_COUNT-3; c++)
		for (int r = 0; r < ROW_COUNT-3; r++)
			if (board[r][c] == piece && board[r+1][c+1] == piece && board[r+2][c+2] == piece && board[r+3][c+3] == piece)
				return 1;

	// Check negatively sloped diaganols
	for (int c = 0; c < COLUMN_COUNT-3; c++)
		for (int r = 3; r < ROW_COUNT; r++)
			if (board[r][c] == piece && board[r-1][c+1] == piece && board[r-2][c+2] == piece && board[r-3][c+3] == piece)
				return 1;
  
  	return 0;
}

int main(){
  	int** board;
	int row, col;
	board = create_board();
	print_board(board);
	int game_over = 0, turn = 0;
  	while (!game_over){
		// Ask for Player 1 Input
		if (turn == 0){
			printf("Player 1: Select a column form 0-6:\n");
			scanf("%d",&col);

			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
				if (row == -1)
					continue;
				drop_piece(board, row, col, 1);
				if (winning_move(board, 1)){
					printf("Player 1 wins!!\n");
					game_over = 1;
				}
			}
		}
		// Ask for Player 2 Input
		else{			
			printf("Player 2: Select a column form 0-6:\n");
			scanf("%d",&col);

			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
				if (row == -1)
					continue;
				drop_piece(board, row, col, 2);
				if (winning_move(board, 1)){
					printf("Player 2 wins!!\n");
					game_over = 1;
				}
			}
		}
		print_board(board);
		turn += 1;
		turn = turn % 2;
	}
	printf("GAME OVER\n");
  	free(board);
	return 0;
 }
