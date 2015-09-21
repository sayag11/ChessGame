#include "Chess.h"

int  cnt_test_minimax = 0;
struct Move* move_to_do;
int is_allocd_move_to_do = 0;

int Score(int color,char some_board[BOARD_SIZE][BOARD_SIZE]){
	int whiteScore = 0;
	int blackScore = 0;
	char piece;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			piece = some_board[i][j];
			switch (piece){
			case W_PAWN:
				whiteScore += 1;
				break;
			case W_BISHOP:
				whiteScore += 3;
				break;
			case W_ROOK:
				whiteScore += 5;
				break;
			case W_KNIGHT:
				whiteScore += 3;
				break;
			case W_QUEEN:
				whiteScore += 9;
				break;
			case W_KING:
				whiteScore += 400;
				break;
			case B_PAWN:
				blackScore += 1;
				break;
			case B_BISHOP:
				blackScore += 3;
				break;
			case B_ROOK:
				blackScore += 5;
				break;
			case B_KNIGHT:
				blackScore += 3;
				break;
			case B_QUEEN:
				blackScore += 9;
				break;
			case B_KING:
				blackScore += 400;
				break;
			}
		}
	}
	if (color == WHITE)
		return whiteScore - blackScore;
	else
		return blackScore - whiteScore;
}

int bestDepth(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){ // recieves a board and returns the best minimax depth
	int moves_upper_bound;
	int pieces_count = 0;
	if (color == WHITE){
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (isWhite(some_board[i][j]))
					pieces_count++;
			}
		}
	}
	else{ // color==BLACK
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (isBlack(some_board[i][j]))
					pieces_count++;
			}
		}
	}


	moves_upper_bound = MOVES_BOUND * pieces_count; // upper bound of moves per piece is 28
	int d = 1; // d is the "best depth" that will be returned after calculated
	while ((pow((double)moves_upper_bound, (double)d)) < 1000000){ // number of boards in minimax tree is bounded by  10^6
		d++;
	}
	return d;

}

int alphaBeta(int color, char some_board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int depthleft, int maxi){
	if (depthleft == 0){
		if (maxi)
			return Score(color, some_board);
		else
			return Score(!color, some_board);
	}
	struct Move* alpha_move = NULL;
	struct Moves* moves = getAllMoves(color, some_board);
	filterMoves(color, moves);
	struct Move* temp = moves->firstMove;
	while (temp != NULL)
	{
		if (isMate(!color, temp->board_after_move)){
			temp->score = WIN_SCORE;
		}
		else if (isTie(color, temp->board_after_move) || isTie(!color, temp->board_after_move)){
			temp->score = TIE_SCORE;
		}
		else
			temp->score = alphaBeta(!color, temp->board_after_move, alpha, beta, depthleft - 1, !maxi);
		if (maxi){
			if (temp->score >= beta){
				if (depthleft != difficulty){
					freeMoves(moves);
					return beta;// fail hard beta-cutoff
				}
				else{
					if (depthleft == difficulty){
						if (alpha_move != NULL){
							if (!is_allocd_move_to_do){
								move_to_do = calloc(1, sizeof(struct Move));
								assert(move_to_do != NULL);
								is_allocd_move_to_do = 1;
							}
							CopyMove(move_to_do, alpha_move);
						}
					}
					freeMoves(moves);
					return beta;
				}
			}
			if (temp->score > alpha){
				alpha = temp->score; // alpha acts like max in MiniMax
				if (depthleft == difficulty)
					alpha_move = temp;

			}
		}
		else{ // minimizer
			if (temp->score <= alpha){
				freeMoves(moves);
				return alpha; // fail hard alpha-cutoff
			}
			if (temp->score < beta){
				beta = temp->score; // beta acts like min in MiniMax
			}
		}
		temp = temp->next;

	}
	if (depthleft == difficulty){ // only happens on Maximizer
		if (alpha_move != NULL){
			if (!is_allocd_move_to_do){
				move_to_do = calloc(1, sizeof(struct Move));
				assert(move_to_do != NULL);
				is_allocd_move_to_do = 1;
			}
			CopyMove(move_to_do, alpha_move);
		}
	}
	freeMoves(moves);
	if (maxi)
		return alpha;
	else // minimizer
		return beta;
}