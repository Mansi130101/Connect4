#include<iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include<math.h>
using namespace std;

int ROW_COUNT = 6;
int COLUMN_COUNT = 7;
int PLAYER = 0;
int AI = 1;
int EMPTY = 0;
int PLAYER_PIECE = 1;
int AI_PIECE = 2;


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
}

void print_board(int** board){
	for (int i = 0; i <  ROW_COUNT; i++)
      		for (int j = COLUMN_COUNT-1; j > -1; j--)
         		printf("%d ", *(board + i*COLUMN_COUNT + j));
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

int evaluate_window(window, piece){
  int score = 0;
  int opp_piece = PLAYER_PIECE;
  if (piece == PLAYER_PIECE)
    opp_piece = AI_PIECE;

  if (window.count(piece) == 4)
    score += 100;
  elif (window.count(piece) == 3 and window.count(EMPTY) == 1)
    score += 5;
  elif (window.count(piece) == 2 and window.count(EMPTY) == 2)
    score += 2;

  if (window.count(opp_piece) == 3 and window.count(EMPTY) == 1)
    score -= 4;

  return score;
}

def score_position(board, piece){
	int score = 0;
	// Score center column
	center_array = [int(i) for i in list(board[:, COLUMN_COUNT//2])]
	center_count = center_array.count(piece)
	score += center_count * 3

	// Score Horizontal
	for r in range(ROW_COUNT):
		row_array = [int(i) for i in list(board[r,:])]
		for c in range(COLUMN_COUNT-3):
			window = row_array[c:c+WINDOW_LENGTH]
			score += evaluate_window(window, piece)

	// Score Vertical
	for c in range(COLUMN_COUNT):
		col_array = [int(i) for i in list(board[:,c])]
		for r in range(ROW_COUNT-3):
			window = col_array[r:r+WINDOW_LENGTH]
			score += evaluate_window(window, piece)

	// Score posiive sloped diagonal
	for r in range(ROW_COUNT-3):
		for c in range(COLUMN_COUNT-3):
			window = [board[r+i][c+i] for i in range(WINDOW_LENGTH)]
			score += evaluate_window(window, piece)

	for r in range(ROW_COUNT-3):
		for c in range(COLUMN_COUNT-3):
			window = [board[r+3-i][c+i] for i in range(WINDOW_LENGTH)]
			score += evaluate_window(window, piece)

	return score;
}

int is_terminal_node(int** board){
	int a = winning_move(board, PLAYER_PIECE) + winning_move(board, AI_PIECE);
  if (len(get_valid_locations(board)) == 0)
    a +=1;
  return a;
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
			printf("Player 1: Select a column form 0-6");
			scanf("%d",&col);

			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
				drop_piece(board, row, col, 1);
				if (winning_move(board, 1)){
					printf("Player 1 wins!!\n");
					game_over = 1;
				}
			}
		}
		// Ask for Player 2 Input
		else{			
			printf("Player 2: Select a column form 0-6");
			scanf("%d",&col);

			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
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
		return 0;
	}
	printf("GAME OVER\n");
  	free(board);
 }

