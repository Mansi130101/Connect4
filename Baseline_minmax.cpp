#include <iostream>
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>
#include <time.h>
#include <algorithm>
using namespace std;

int ROWS = 6;
int COLUMNS = 7;
int INF=10000000;
int PLAYER = 0;
int AI = 1;
int EMPTY = 0;
int PLAYER_PIECE = 1;
int AI_PIECE = 2;
int WINDOW_LENGTH = 4;
int None = 10;

int** draw_board(){
  	int** board = (int **)calloc(ROWS, sizeof(int *));
  	for (int i=0; i<ROWS; i++)
        board[i] = (int *)calloc(COLUMNS, sizeof(int));
	return board;
}

void play_move(int** board, int row, int col, int piece){
	board[row][col] = piece;
}

int is_valid(int** board, int col){
	if (board[ROWS-1][col] == 0)
    	return 1;
  	return 0;
}

int find_empty_row(int** board, int col){
	for (int r = 0; r < ROWS; r++)
		if (board[r][col] == 0)
			return r;
	return -1;
}

void print_board(int** board){
	for (int i = ROWS-1; i > -1; i--){
      	for (int j = 0; j < COLUMNS; j++)
         	cout<<board[i][j]<<" ";
		cout<<"\n";
	}
	cout<<"\n";
}

int winning_move(int** board, int piece){
	// Check horizontal locations for win
	for (int c = 0; c < COLUMNS-3; c++)
		for (int r = 0; r < ROWS; r++)
			if (board[r][c] == piece && board[r][c+1] == piece && board[r][c+2] == piece && board[r][c+3] == piece)
				return 1;

	// Check vertical locations for win
	for (int c = 0; c < COLUMNS; c++)
		for(int r = 0; r < ROWS-3; r++)
			if (board[r][c] == piece && board[r+1][c] == piece && board[r+2][c] == piece && board[r+3][c] == piece)
				return 1;

	// Check positively sloped diaganols
	for (int c = 0; c < COLUMNS-3; c++)
		for (int r = 0; r < ROWS-3; r++)
			if (board[r][c] == piece && board[r+1][c+1] == piece && board[r+2][c+2] == piece && board[r+3][c+3] == piece)
				return 1;

	// Check negatively sloped diaganols
	for (int c = 0; c < COLUMNS-3; c++)
		for (int r = 3; r < ROWS; r++)
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

int evaluate(int* window, int size_window, int piece){
  	int score = 0;
  	int opp_piece = PLAYER_PIECE;
  	if (piece == PLAYER_PIECE)
    	opp_piece = AI_PIECE;

  	if (count(window,size_window,piece) == 4)
    	score += 10000;
  	else{
        if (count(window,size_window,piece) == 3 && count(window,size_window,EMPTY) == 1)
            score += 50;
		else{
			if (count(window,size_window,piece) == 2 && count(window,size_window,EMPTY) == 2)
            	score += 2;
		}
  	}

  	if (count(window,size_window,opp_piece) == 3 && count(window,size_window,EMPTY) == 1)
    	score -= 51;

  	return score;
}

int calculate_score(int** board, int piece){
	int score = 0;
	// Score Horizontal
	int window[WINDOW_LENGTH];
	int row_array[COLUMNS];
	int col_array[ROWS];
	for (int r = 0; r < ROWS; r++){
		for (int j = 0; j < COLUMNS; j++)
			row_array[j] = board[r][j];
		for (int c = 0; c < COLUMNS-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = row_array[c+w];
			score += evaluate(window, WINDOW_LENGTH, piece);
        }
    }
    // Score Vertical
	for (int c = 0; c < COLUMNS; c++){
		for (int j = 0; j < ROWS; j++)
			col_array[j] = board[j][c];
		for (int r = 0; r < ROWS-3; r++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = col_array[r+w];
			score += evaluate(window, WINDOW_LENGTH, piece);
        }
    }
	// Score positive sloped diagonal
	for (int r = 0; r < ROWS-3; r++){
		for (int c = 0; c < COLUMNS-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = board[r+w][c+w];
			score += evaluate(window, WINDOW_LENGTH, piece);
		}
	}
	// Score negative sloped diagonal
	for (int r = 0; r < ROWS-3; r++){
		for (int c = 0; c < COLUMNS-3; c++){
			for (int w = 0; w < WINDOW_LENGTH; w++)
				window[w] = board[r+3-w][c+w];
			score += evaluate(window, WINDOW_LENGTH, piece);
		}
	}
    
	return score;
}

int* get_valid_columns(int** board){
	int* valid_columns;
    valid_columns = (int *)calloc(COLUMNS+2, sizeof(int));
    int k = 0;
	for (int col = 0; col < COLUMNS; col++){
		if (is_valid(board, col)){
			k++;
            valid_columns[k]=col;
        }
    }
    valid_columns[0] = k;
	return valid_columns;
}

int pick_best_move(int** board, int piece){
	int* valid_columns;
    valid_columns = get_valid_columns(board);
	int best_score = -INF, col, row, score;
    int** temp_board;
	temp_board = draw_board();
    int len_valid_loc = valid_columns[0];
    int RandIndex = rand() % len_valid_loc;
	int best_col = valid_columns[RandIndex+1];
	for(int i = 1; i <= len_valid_loc; i++){
        col = valid_columns[i];
		row = find_empty_row(board, col);
		for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLUMNS; c++)
                temp_board[r][c] = board[r][c];
		play_move(temp_board, row, col, piece);
		score = calculate_score(temp_board, piece);
		if (score > best_score){
			best_score = score;
			best_col = col;
        }
    }
	return best_col;
}

int is_terminal_node(int** board){
	int a=0;
	a = winning_move(board, PLAYER_PIECE) + winning_move(board, AI_PIECE);
	int *valid_columns;
	valid_columns=get_valid_columns(board);
  	if (valid_columns[0]== 0)
    	a += 1;
	return (a>0)?1:0;
}

struct col_val { 
    int column, value; 
}; 
typedef struct col_val Struct; 
  
Struct minimax(int** board, int depth, int alpha, int beta, int maximizingPlayer){
	Struct out,new_score;
	int* valid_columns;
    valid_columns = get_valid_columns(board);
	int len_valid_loc = valid_columns[0];
	int is_terminal = is_terminal_node(board);
	if (depth == 0 || is_terminal){
		if (is_terminal){
			if (winning_move(board, AI_PIECE)){
				out.column = None;
				out.value = INF+depth;
				return out;
			}
			else{
				if (winning_move(board, PLAYER_PIECE)){
					out.column = None;
					out.value = -INF+depth;
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
			out.value = calculate_score(board, AI_PIECE);
			return out;
		}
	}
	int col,row;
	if (maximizingPlayer){
		out.value = -1*INF;
		out.column = valid_columns[1];
		for(int i = 1; i <= len_valid_loc; i++){
        	col = valid_columns[i];
			row = find_empty_row(board, col);
			board[row][col]=AI_PIECE;
			new_score = minimax(board, depth-1, alpha, beta, 0);
			board[row][col]=0;
			if (new_score.value > out.value){
				out.value = new_score.value;
				out.column = col;
			}
			alpha =max(alpha, out.value);
			if (alpha >= beta)
				break;
		}
		return out;
	}
	else{ //Minimizing player
		out.value = INF;
		out.column = valid_columns[1];
		for(int i = 1; i <= len_valid_loc; i++){
        	col = valid_columns[i];
			row = find_empty_row(board, col);
			board[row][col]=PLAYER_PIECE;
			new_score = minimax(board, depth-1, alpha, beta, 1);
			board[row][col]=0;
			if (new_score.value < out.value){
				out.value = new_score.value;
				out.column = col;
			}
			beta =min(beta, out.value);
			if (beta<=alpha)
				break;
		}
		return out;
	}
}

int main(){
	srand (time(NULL));
  	int** board;
	int row, col;
	board = draw_board();
	print_board(board);
  	int game_over = 0, turn = rand() % 2, flag=0;
  	while (!game_over){
		flag=0;
		for (int c = 0; c < COLUMNS; c++){
			if (is_valid(board, c)){
				flag=1;
				continue;
			}
		}
		if (!flag){
			cout<<"MATCH DRAW !!\n";
			game_over = 1;
		}
		// Ask for Player 1 Input
		if (turn == PLAYER){
			cout<<"Player 1: Select a column form 0-6:\n";
			cin>>col;

			if (col>=0 && col<7 && is_valid(board, col)){
				row = find_empty_row(board, col);
				play_move(board, row, col, PLAYER_PIECE);
				if (winning_move(board, PLAYER_PIECE)){
					cout<<"Player 1 (PLAYER) wins !!\n";
					game_over = 1;
				}
				print_board(board);
		    	turn += 1;
		    	turn = turn % 2;
            }
            else
            	cout<<"Invalid Move\n";
		}

		// Ask for Player 2 Input
		if (turn == AI && !game_over){
            Struct out;
			out = minimax(board, 7, -INF-10, INF+10, 1);
			col = out.column;
			int minimax_score = out.value;
            if (is_valid(board, col)){
                row = find_empty_row(board, col);
                play_move(board, row, col, AI_PIECE);
                if (winning_move(board, AI_PIECE)){
                    cout<<"Player 2 (AI) wins !!\n";
					game_over = 1;
                }
				print_board(board);
                turn += 1;
                turn = turn % 2;
            }
        }
    }
	cout<<"GAME OVER\n";
  	free(board);
	return 0;
 }
