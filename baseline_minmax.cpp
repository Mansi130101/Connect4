#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace std;

int ROW_COUNT = 6;
int COLUMN_COUNT = 7;
int PLAYER = 0;
int AI = 1;
int EMPTY = 0;
int PLAYER_PIECE = 1;
int AI_PIECE = 2;
int WINDOW_LENGTH = 4;
int None = 10;

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
      		for (int j = 0; j < COLUMN_COUNT; j++)
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

int count(int* window, int size_window, int piece){
	int window_count = 0;
	for (int i = 0; i < size_window; i++)
		if (window[i]==piece)
			window_count += 1;
	return window_count;
}

int evaluate_window(int* window, int size_window, int piece){
  int score = 0;
  int opp_piece = PLAYER_PIECE;
  if (piece == PLAYER_PIECE)
  	opp_piece = AI_PIECE;

  if (count(window,size_window,piece) == 4)
  	score += 100;
  else{
        if (count(window,size_window,piece) == 3 && count(window,size_window,EMPTY) == 1)
        	score += 5;
		else{
			if (count(window,size_window,piece) == 2 && count(window,size_window,EMPTY) == 2)
            			score += 2;
		}
  }

  if (count(window,size_window,opp_piece) == 3 && count(window,size_window,EMPTY) == 1)
  	score -= 10;

  return score;
}

int score_position(int** board, int piece){
	int score = 0, center_count = 0;
	// Score center column
	int center_array[ROW_COUNT];
	for (int i = 0; i < ROW_COUNT; i++){
		center_array[i] = board[i][(COLUMN_COUNT-1)/2];
		center_count = count(center_array,ROW_COUNT,piece);
	}
	score += center_count * 3;
	// Score Horizontal
	int window[WINDOW_LENGTH];
	int row_array[COLUMN_COUNT];
	int col_array[ROW_COUNT];
	for (int r = 0; r < ROW_COUNT; r++){
		for (int j = 0; j < COLUMN_COUNT; j++)
			row_array[j] = board[r][j];
		for (int c = 0; c < COLUMN_COUNT-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = row_array[c+w];
			score += evaluate_window(window, WINDOW_LENGTH, piece);
        }
    }
    // Score Vertical
	for (int c = 0; c < COLUMN_COUNT; c++){
		for (int j = 0; j < ROW_COUNT; j++)
			col_array[j] = board[j][c];
		for (int r = 0; r < ROW_COUNT-3; r++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = col_array[r+w];
			score += evaluate_window(window, WINDOW_LENGTH, piece);
        }
    }
	// Score positive sloped diagonal
	for (int r = 0; r < ROW_COUNT-3; r++){
		for (int c = 0; c < COLUMN_COUNT-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = board[r+w][c+w];
			score += evaluate_window(window, WINDOW_LENGTH, piece);
		}
	}
	// Score negative sloped diagonal
	for (int r = 0; r < ROW_COUNT-3; r++){
		for (int c = 0; c < COLUMN_COUNT-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = board[r+3-w][c+w];
			score += evaluate_window(window, WINDOW_LENGTH, piece);
		}
	}
    
	return score;
}

int* get_valid_locations(int** board){
	int* valid_locations;
    	valid_locations = (int *)calloc(50, sizeof(int));
    	int k = 0;
	for (int col = 0; col < COLUMN_COUNT; col++){
		if (is_valid_location(board, col)){
			k++;
            	valid_locations[k]=col;
        	}
    	}
    	valid_locations[0] = k+1;
	return valid_locations;
}

int pick_best_move(int** board, int piece){
	int* valid_locations;
    	valid_locations = get_valid_locations(board);
	int best_score = -100000000, col, row, score;
    	int** temp_board;
	temp_board = create_board();
    	int len_valid_loc = valid_locations[0];
    	int RandIndex = rand() % len_valid_loc;
	int best_col = valid_locations[RandIndex+1];
	for(int i = 1; i < len_valid_loc+1; i++){
        	col = valid_locations[i];
		row = get_next_open_row(board, col);
		for (int r = 0; r < ROW_COUNT; r++)
            		for (int c = 0; c < COLUMN_COUNT; c++)
                		temp_board[r][c] = board[r][c];
		drop_piece(temp_board, row, col, piece);
		score = score_position(temp_board, piece);
		if (score > best_score){
			best_score = score;
			best_col = col;
        	}
    	}
	return best_col;
}

int is_terminal_node(int** board){
	int a = winning_move(board, PLAYER_PIECE) + winning_move(board, AI_PIECE);
  	if (get_valid_locations(board)[0] == 1)
    	a += 1;
	if (a>0)
  		return 1;
	return 0;
}

struct col_val { 
    	int column, value; 
}; 
typedef struct col_val Struct; 
  
Struct minimax(int** board, int depth, int alpha, int beta, int maximizingPlayer){
	Struct out,new_score;
	int* valid_locations;
    	valid_locations = get_valid_locations(board);
	int len_valid_loc = valid_locations[0];
	int is_terminal = is_terminal_node(board);
	if (depth == 0 || is_terminal){
		if (is_terminal){
			if (winning_move(board, AI_PIECE)){
				out.column = None;
				out.value = 100000000;
				return out;
			}
			else{
				if (winning_move(board, PLAYER_PIECE)){
					out.column = None;
					out.value = -100000000;
					return out;
				}
				else{ // Game is over, no more valid moves
					out.column = None;
					out.value = 0;
					return out;
				}
			}
		}
		else{ // Depth is zero
			out.column = None;
			out.value = score_position(board, AI_PIECE);
			return out;
		}
	}
	int col,row;
	int** b_copy;
	b_copy = create_board();
	if (maximizingPlayer){
		out.value = -100000000;
		out.column = valid_locations[rand() % len_valid_loc +1];
		for(int i = 0; i < len_valid_loc; i++){
        		col = valid_locations[i];
			row = get_next_open_row(board, col);
			for (int r = 0; r < ROW_COUNT; r++)
            			for (int c = 0; c < COLUMN_COUNT; c++)
                			b_copy[r][c] = board[r][c];
			drop_piece(b_copy, row, col, AI_PIECE);
			new_score = minimax(b_copy, depth-1, alpha, beta, 0);
			if (new_score.value > out.value){
				out.value = new_score.value;
				out.column = col;
			}
			alpha = std::max(alpha, out.value);
			if (alpha >= beta)
				break;
		}
		return out;
	}
	else{ //Minimizing player
		out.value = -100000000;
		out.column = valid_locations[(rand() % len_valid_loc) +1];
		for(int i = 0; i < len_valid_loc; i++){
        		col = valid_locations[i];
			row = get_next_open_row(board, col);
			for (int r = 0; r < ROW_COUNT; r++)
            			for (int c = 0; c < COLUMN_COUNT; c++)
                			b_copy[r][c] = board[r][c];
			drop_piece(b_copy, row, col, PLAYER_PIECE);
			new_score = minimax(b_copy, depth-1, alpha, beta, 1);
			if (new_score.value < out.value){
				out.value = new_score.value;
				out.column = col;
			}
			alpha = std::min(alpha, out.value);
			if (alpha >= beta)
				break;
		}
		return out;
	}
}

int main(){
	srand (time(NULL));
  	int** board;
	int row, col;
	board = create_board();
	print_board(board);
	int game_over = 0, turn;
	turn =  rand() % 2;
  	while (!game_over){
		// Ask for Player 1 Input
		if (turn == PLAYER){
			printf("Player 1: Select a column form 0-6:\n");
			scanf("%d",&col);

			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
				if (row == -1){
					printf("Error\n Whole column is filled\n");
					break;
                		}
				drop_piece(board, row, col, PLAYER_PIECE);
				if (winning_move(board, PLAYER_PIECE)){
					printf("Player 1 wins!!\n");
					game_over = 1;
				}
			print_board(board);
		    	turn += 1;
		    	turn = turn % 2;
            		}
		}

		// Ask for Player 2 Input
		if (turn == AI && !game_over){
			//col = rand() % COLUMN_COUNT;
			//col = pick_best_move(board, AI_PIECE);
			Struct out;
			out = minimax(board, 3, -100000000, 100000000, 1);
			col = out.column;
			int minimax_score = out.value;
			if (is_valid_location(board, col)){
				row = get_next_open_row(board, col);
				if (row == -1){
						printf("Error\n Whole column is filled\n");
						break;
				}
				drop_piece(board, row, col, AI_PIECE);
				if (winning_move(board, AI_PIECE)){
			    		printf("Player 2 (AI) wins!!\n");
					game_over = 1;
				}
				print_board(board);
				turn += 1;
				turn = turn % 2;
		    	}
		}
	    }
	printf("GAME OVER\n");
  	free(board);
	return 0;
 }
