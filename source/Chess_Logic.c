#include "Chess.h"

char board[BOARD_SIZE][BOARD_SIZE];


// ***Chess Logic ****
// moves and eats for each player ( user and computer ), possible moves, eats, scoring function, changes in the board.

//  All the checks are done in Chess_CommandLine.c so (x,y) is a valid non-empty position on board
struct Moves* getMovesForPosition(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	char pieceToMove = some_board[x][y];
	switch (pieceToMove){ 
	case W_PAWN:
		return PawnMoves(x, y, some_board);
	case W_BISHOP:
		return BishopMoves(x, y, some_board);
	case W_ROOK:
		return RookMoves(x, y, some_board);
	case W_KNIGHT:
		return KnightMoves(x, y, some_board);
	case W_QUEEN:
		return QueenMoves(x, y, some_board);
	case W_KING:
		return KingMoves(x, y, some_board);
	case B_PAWN:
		return PawnMoves(x, y, some_board);
	case B_BISHOP:
		return BishopMoves(x, y, some_board);
	case B_ROOK:
		return RookMoves(x, y, some_board);
	case B_KNIGHT:
		return KnightMoves(x, y, some_board);
	case B_QUEEN:
		return QueenMoves(x, y, some_board);
	case B_KING:
		return KingMoves(x, y, some_board);
	default:
		return NULL;
		;
	}
}


struct Move* createMove(int x, int y, int i, int j, char some_board[BOARD_SIZE][BOARD_SIZE], char promotion){
	struct Move* newMove = calloc(1,sizeof(struct Move));
	assert(newMove != NULL);
	newMove->dst.x = i;
	newMove->dst.y = j;
	newMove->src.x = x;
	newMove->src.y = y;
	CopyBoard(newMove->board_after_move, some_board);
	newMove->board_after_move[i][j] = newMove->board_after_move[x][y];
	newMove->board_after_move[x][y] = EMPTY;
	newMove->next = NULL;
	newMove->prev = NULL;
	if (promotion != EMPTY){
		newMove->board_after_move[i][j] = promotion;
		newMove->promotion = promotion;
	}
	return newMove;
}

/////////////////////// The following functions are a "get moves" functions and they return a list of moves for a pieces


struct Moves* PawnMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1,sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Move* move;
	int isPromoted = 0;
	if (isWhite(some_board[x][y])){
		if (y + 1 == 7) isPromoted = 1; // promote the pawn 
		if (y + 1 <= 7 && some_board[x][y + 1] == EMPTY){ // if you can move to an empty spot that is in the board
			if (isPromoted){
				move = createMove(x, y, x, y + 1, some_board, W_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y + 1, some_board, W_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y + 1, some_board, W_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y + 1, some_board, W_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x, y + 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}
			

		if (y + 1 <= 7 && x - 1 >= 0 && isBlack(some_board[x-1][y+1])){ // can move  diagonal only if can eat a black piece
			if (isPromoted){

				move = createMove(x, y, x - 1, y + 1, some_board, W_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y + 1, some_board, W_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y + 1, some_board, W_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y + 1, some_board, W_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x - 1, y + 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}

		if (y + 1 <= 7 && x + 1 <= 7 && isBlack(some_board[x+1][y+1])){
			if (isPromoted){

				move = createMove(x, y, x + 1, y + 1, some_board, W_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y + 1, some_board, W_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y + 1, some_board, W_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y + 1, some_board, W_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x + 1, y + 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}
	}
	else{ // color is black
		if (y - 1 == 0) isPromoted = 1; // promote the pawn 
		if (y - 1 >= 0 && some_board[x][y - 1] == EMPTY) {// if you can move to an empty spot that is in the board
			if (isPromoted){

				move = createMove(x, y, x, y - 1, some_board, B_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y - 1, some_board, B_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y - 1, some_board, B_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x, y - 1, some_board, B_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x, y - 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}

		if (y - 1 >=0 && x - 1 >= 0 && isWhite(some_board[x - 1][y - 1])){ // can move  diagonal only if can eat a black piece
			if (isPromoted){

				move = createMove(x, y, x - 1, y - 1, some_board, B_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y - 1, some_board, B_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y - 1, some_board, B_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x - 1, y - 1, some_board, B_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x - 1, y - 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}

		if (y - 1 >= 0 && x + 1 <= 7 && isWhite(some_board[x + 1][y - 1])){
			if (isPromoted){

				move = createMove(x, y, x + 1, y - 1, some_board, B_ROOK);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y - 1, some_board, B_KNIGHT);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y - 1, some_board, B_QUEEN);
				addMoveToMovesList(move, moves);
				move = createMove(x, y, x + 1, y - 1, some_board, B_BISHOP);
				addMoveToMovesList(move, moves);
			}
			else{
				move = createMove(x, y, x + 1, y - 1, some_board, EMPTY); // Move with no promotion
				addMoveToMovesList(move, moves);
			}
		}
	}
	return moves;
}

struct Moves* BishopMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1, sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Move* move;
	int i, j;
	// check if can eat up-right
	i = x + 1;
	j = y + 1;
	while (i <= 7 && j <= 7){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i++;
		j++;
	}
	if (isWhite(some_board[x][y]) && i<=7 && j<=7 && isBlack(some_board[i][j])){ // white Bishop eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i <= 7 && j <= 7 && isWhite(some_board[i][j])){ // black bishop eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}
	// check if can eat up-left
	i = x - 1;
	j = y + 1;
	while (i >=0 && j <= 7){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i--;
		j++;
	}
	if (isWhite(some_board[x][y]) && i >=0 && j <= 7 && isBlack(some_board[i][j])){ // white Bishop eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i >= 0 && j <= 7 && isWhite(some_board[i][j])){ // black bishop eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}

	//check if can eat down-left
	i = x - 1;
	j = y - 1;
	while (i >=0 && j >=0){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i--;
		j--;
	}
	if (isWhite(some_board[x][y]) && i >= 0 && j >= 0 && isBlack(some_board[i][j])){ // white Bishop eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i >= 0 && j >= 0 && isWhite(some_board[i][j])){ // black bishop eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}

	// check if can eat down-right
	i = x + 1;
	j = y - 1;
	while (i <= 7 && j >=0){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i++;
		j--;
	}
	if (isWhite(some_board[x][y]) && i <= 7 && j >= 0 && isBlack(some_board[i][j])){ // white Bishop eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i <= 7 && j >= 0 && isWhite(some_board[i][j])){ // black bishop eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}
	return moves;

}

struct Moves* RookMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1, sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Move* move;
	int i, j;
	// check if can eat up
	i = x;
	j = y + 1;
	while (j <= 7){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		j++;
	}
	if (isWhite(some_board[x][y]) &&  j <= 7 && isBlack(some_board[i][j])){ // white rook eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && j <= 7 && isWhite(some_board[i][j])){ // black rook eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}
	// check if can eat right
	i = x +1;
	j = y;
	while (i<=7){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i++;
	}
	if (isWhite(some_board[x][y]) && i <= 7&& isBlack(some_board[i][j])){ // white rook eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i <= 7 && isWhite(some_board[i][j])){ // black rook eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}

	//check if can eat down
	i = x;
	j = y - 1;
	while (j >= 0){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		j--;
	}
	if (isWhite(some_board[x][y]) && j >= 0 && isBlack(some_board[i][j])){ // white rook eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && j >= 0 && isWhite(some_board[i][j])){ // black rook eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}

	// check if can eat left
	i = x - 1;
	j = y;
	while (i >=0 ){
		if (some_board[i][j] != EMPTY)
			break;
		else{
			move = createMove(x, y, i, j, some_board, EMPTY);  // add step to an empty spot on board
			addMoveToMovesList(move, moves);
		}
		i--;
	}
	if (isWhite(some_board[x][y]) && i >= 0 && isBlack(some_board[i][j])){ // white Bishop eats black piece
		move = createMove(x, y, i, j, some_board, EMPTY);
		addMoveToMovesList(move, moves);
	}
	else{
		if (isBlack(some_board[x][y]) && i >= 0 && isWhite(some_board[i][j])){ // black bishop eats white piece
			move = createMove(x, y, i, j, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
	}
	return moves;
}

struct Moves* KnightMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1, sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Move* move;
	if (isWhite(some_board[x][y])){ // knight piece is white
			if (x - 2 >= 0){
				if (y - 1 >= 0 && (isBlack(some_board[x - 2][y - 1]) || some_board[x - 2][y - 1] == EMPTY)){
					move = createMove(x, y, x - 2, y - 1, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
				if (y + 1 <= 7 && (isBlack(some_board[x - 2][y + 1]) || some_board[x - 2][y + 1] == EMPTY)){
					move = createMove(x, y, x - 2, y + 1, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
			}
			if (x +2 <= 7){
				if (y - 1 >= 0 && (isBlack(some_board[x + 2][y - 1]) || some_board[x + 2][y - 1] == EMPTY)){
					move = createMove(x, y, x + 2, y - 1, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
				if (y + 1 <= 7 && (isBlack(some_board[x + 2][y + 1]) || some_board[x + 2][y + 1] == EMPTY)){
					move = createMove(x, y, x + 2, y + 1, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
			}
			if (y + 2 <= 7){
				if (x - 1 >= 0 && (isBlack(some_board[x - 1][y + 2]) || some_board[x - 1][y + 2] == EMPTY)){
					move = createMove(x, y, x - 1, y + 2, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
				if (x + 1 <= 7 && (isBlack(some_board[x + 1][y + 2]) || some_board[x + 1][y + 2] == EMPTY)){
					move = createMove(x, y, x + 1, y + 2, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
			}
			if (y - 2 >=0){
				if (x - 1 >= 0 && (isBlack(some_board[x - 1][y - 2]) || some_board[x - 1][y - 2] == EMPTY)){
					move = createMove(x, y, x - 1, y - 2, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
				if (x + 1 <= 7 && (isBlack(some_board[x + 1][y - 2]) || some_board[x + 1][y - 2] == EMPTY)){
					move = createMove(x, y, x + 1, y - 2, some_board, EMPTY);
					addMoveToMovesList(move, moves);
				}
			}
	}
	else{ // knight piece is black
		if (x - 2 >= 0){
			if (y - 1 >= 0 && (isWhite(some_board[x - 2][y - 1]) || some_board[x - 2][y - 1] == EMPTY)){
				move = createMove(x, y, x - 2, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y + 1 <= 7 && (isWhite(some_board[x - 2][y + 1]) || some_board[x - 2][y + 1] == EMPTY)){
				move = createMove(x, y, x - 2, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
		if (x + 2 <= 7){
			if (y - 1 >= 0 && (isWhite(some_board[x + 2][y - 1]) || some_board[x + 2][y - 1] == EMPTY)){
				move = createMove(x, y, x + 2, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y + 1 <= 7 && (isWhite(some_board[x + 2][y + 1]) || some_board[x + 2][y + 1] == EMPTY)){
				move = createMove(x, y, x + 2, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
		if (y + 2 <= 7){
			if (x - 1 >= 0 && (isWhite(some_board[x - 1][y + 2]) || some_board[x - 1][y + 2] == EMPTY)){
				move = createMove(x, y, x - 1, y + 2, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (x + 1 <= 7 && (isWhite(some_board[x + 1][y + 2]) || some_board[x + 1][y + 2] == EMPTY)){
				move = createMove(x, y, x + 1, y + 2, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
		if (y - 2 >= 0){
			if (x - 1 >= 0 && (isWhite(some_board[x - 1][y - 2]) || some_board[x - 1][y - 2] == EMPTY)){
				move = createMove(x, y, x - 1, y - 2, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (x + 1 <= 7 && (isWhite(some_board[x + 1][y - 2]) || some_board[x + 1][y - 2] == EMPTY)){
				move = createMove(x, y, x + 1, y - 2, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
	}
	return moves;
}

struct Moves* QueenMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* rookmoves = RookMoves(x, y, some_board);
	struct Moves* bishopmoves = BishopMoves(x, y, some_board);
	if (bishopmoves!=NULL)
		concatMovesLists(rookmoves, bishopmoves);
	return rookmoves;
}

struct Moves* KingMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1, sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Move* move;
	if (isWhite(some_board[x][y])){ // white king eats piece ( or moves to an empty spot )
		if (x + 1 <= 7){
			if (y + 1 <= 7 && (isBlack(some_board[x + 1][y + 1]) || some_board[x + 1][y + 1] == EMPTY)){
				move = createMove(x, y, x + 1, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (isBlack(some_board[x + 1][y]) || some_board[x + 1][y] == EMPTY){
				move = createMove(x, y, x + 1, y, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y - 1 >= 0 && (isBlack(some_board[x + 1][y - 1]) || some_board[x + 1][y - 1] == EMPTY)){
				move = createMove(x, y, x + 1, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
		if (y - 1 >= 0 && (isBlack(some_board[x][y - 1]) || some_board[x][y - 1] == EMPTY)){
			move = createMove(x, y, x, y - 1, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
		if (y + 1 <= 7 && (isBlack(some_board[x][y + 1]) || some_board[x][y + 1] == EMPTY)){
			move = createMove(x, y, x, y + 1, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
		if (x - 1 >= 0){
			if (y + 1 <= 7 && (isBlack(some_board[x - 1][y + 1]) || some_board[x - 1][y + 1] == EMPTY)){
				move = createMove(x, y, x - 1, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (isBlack(some_board[x - 1][y]) || some_board[x - 1][y] == EMPTY){
				move = createMove(x, y, x - 1, y, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y - 1 >= 0 && (isBlack(some_board[x - 1][y - 1]) || some_board[x - 1][y - 1] == EMPTY)){
				move = createMove(x, y, x - 1, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
	}
	else{ // black king eats piece ( or moves to an empty spot )
		if (x + 1 <= 7){
			if (y + 1 <= 7 && (isWhite(some_board[x + 1][y + 1]) || some_board[x + 1][y + 1] == EMPTY)){
				move = createMove(x, y, x + 1, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (isWhite(some_board[x + 1][y]) || some_board[x + 1][y] == EMPTY){
				move = createMove(x, y, x + 1, y, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y - 1 >= 0 && (isWhite(some_board[x + 1][y - 1]) || some_board[x + 1][y - 1] == EMPTY)){
				move = createMove(x, y, x + 1, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
		if (y - 1 >= 0 && (isWhite(some_board[x][y - 1]) || some_board[x][y - 1] == EMPTY)){
			move = createMove(x, y, x, y - 1, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
		if (y + 1 <= 7 && (isWhite(some_board[x][y + 1]) || some_board[x][y + 1] == EMPTY)){
			move = createMove(x, y, x, y + 1, some_board, EMPTY);
			addMoveToMovesList(move, moves);
		}
		if (x - 1 >= 0){
			if (y + 1 <= 7 && (isWhite(some_board[x - 1][y + 1]) || some_board[x - 1][y + 1] == EMPTY)){
				move = createMove(x, y, x - 1, y + 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (isWhite(some_board[x - 1][y]) || some_board[x - 1][y] == EMPTY){
				move = createMove(x, y, x - 1, y, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
			if (y - 1 >= 0 && (isWhite(some_board[x - 1][y - 1]) || some_board[x - 1][y - 1] == EMPTY)){
				move = createMove(x, y, x - 1, y - 1, some_board, EMPTY);
				addMoveToMovesList(move, moves);
			}
		}
	}
	return moves;
}


//////////////// end of "get moves" functions for pieces

void CopyBoard(char dest[BOARD_SIZE][BOARD_SIZE], char src[BOARD_SIZE][BOARD_SIZE])
{
	//memcpy(dest, src, sizeof(src));
	for (int i = 0; i < 8; i++){
		memcpy(dest[i], src[i], sizeof(src[i]));
		//for (int j = 0; j < 8; j++){
		//	dest[i][j] = src[i][j];
		//}
	}
}

void MakeMove(struct Move* move, char some_board[BOARD_SIZE][BOARD_SIZE]){
	if (some_board[move->src.x][move->src.y] == W_PAWN && move->dst.y == 7){
		some_board[move->dst.x][move->dst.y] = move->promotion;
		some_board[move->src.x][move->src.y] = EMPTY;
	}
	else if (some_board[move->src.x][move->src.y] == B_PAWN && move->dst.y == 0){
		some_board[move->dst.x][move->dst.y] = move->promotion;
		some_board[move->src.x][move->src.y] = EMPTY;
	}
	else{ // computer made a regular move
		some_board[move->dst.x][move->dst.y] = some_board[move->src.x][move->src.y];
		some_board[move->src.x][move->src.y] = EMPTY;
	}
}

int isBlack(char p){
	return isupper(p);
	//return (p == B_PAWN || p == B_KNIGHT || p == B_ROOK || p == B_BISHOP || p == B_QUEEN || p == B_KING);
}
int isWhite(char p){
	return islower(p);
//	return (p == W_PAWN || p == W_KNIGHT || p == W_ROOK || p == W_BISHOP || p == W_QUEEN || p == W_KING);
}

int isPlayingColor(char p){
	return ((isWhite(p) && next_player==WHITE) || (isBlack(p) && next_player==BLACK));
}

//check if king of color 'color' is threatened and
int isCheck(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Position kingpos = findKing(color, some_board);
//	if (kingpos.x == -1)
	//	printf("Error, no king on the board"); // should never get to this line
	int i = kingpos.x;
	int j = kingpos.y;
	if (i < 0 || j < 0 || i>7 || j>7)
		return 0;
// pawn_checks_king
	if (color == BLACK){
		if (i - 1 >= 0 && j - 1 >= 0 && some_board[i - 1][j - 1] == W_PAWN)
			return 1;
		if (i + 1 <= 7 && j - 1 >= 0 && some_board[i + 1][j - 1] == W_PAWN)
			return 1;
	}
	else{ // color == WHITE
		if (i - 1 >= 0 && j + 1 <= 7 && some_board[i - 1][j + 1] == B_PAWN)
			return 1;
		if (i + 1 <= 7 && j + 1 <= 7 && some_board[i + 1][j + 1] == B_PAWN)
			return 1;
	}

// king_checks_king
	i = kingpos.x;
	j = kingpos.y;
	if (color == WHITE){
		if (i + 1 <= 7){
			if (some_board[i + 1][j] == B_KING)
				return 1;
		}
		if (i - 1 >= 0){
			if (some_board[i - 1][j] == B_KING)
				return 1;
			if (j - 1 >= 0){
				if (some_board[i - 1][j - 1] == B_KING)
					return 1;
			}
			if (j + 1 >= 0){
				if (some_board[i - 1][j + 1] == B_KING)
					return 1;
			}
		}
		if (j + 1 <= 7){
			if (some_board[i][j + 1] == B_KING)
				return 1;
			if (i + 1 <= 7){
				if (some_board[i + 1][j + 1] == B_KING)
					return 1;
			}
		}
		if (j - 1 >= 0){
			if (some_board[i][j - 1] == B_KING)
				return 1;
			if (i + 1 <= 7){
				if (some_board[i + 1][j - 1] == B_KING)
					return 1;
			}
		}
	}
	else{ //color==BLACK
		if (i + 1 <= 7){
			if (some_board[i + 1][j] == W_KING)
				return 1;
		}
		if (i - 1 >= 0){
			if (some_board[i - 1][j] == W_KING)
				return 1;
			if (j - 1 >= 0){
				if (some_board[i - 1][j - 1] == W_KING)
					return 1;
			}
			if (j + 1 <=7){
				if (some_board[i - 1][j + 1] == W_KING)
					return 1;
			}
		}
		if (j + 1 <= 7){
			if (some_board[i][j + 1] == W_KING)
				return 1;
			if (i + 1 <= 7){
				if (some_board[i + 1][j + 1] == W_KING)
					return 1;
			}
		}
		if (j - 1 >= 0){
			if (some_board[i][j - 1] == W_KING)
				return 1;
			if (i + 1 <= 7){
				if (some_board[i + 1][j - 1] == W_KING)
					return 1;
			}
		}
	}

// qrb_checks_king
	if (rookCheckKing(color, kingpos, some_board) || bishopCheckKing(color, kingpos, some_board))
		return 1;

// knight_checks_king

	if (color == WHITE){
		if (kingpos.x - 2 >= 0){
			if (kingpos.y - 1 >= 0 && some_board[kingpos.x - 2][kingpos.y - 1] == B_KNIGHT)
				return 1;
			if (kingpos.y + 1 <= 7 && some_board[kingpos.x - 2][kingpos.y + 1] == B_KNIGHT)
				return 1;
		}
		if (kingpos.x + 2 <= 7){
			if (kingpos.y - 1 >= 0 && some_board[kingpos.x + 2][kingpos.y - 1] == B_KNIGHT)
				return 1;
			if (kingpos.y + 1 <= 7 && some_board[kingpos.x + 2][kingpos.y + 1] == B_KNIGHT)
				return 1;
		}
		if (kingpos.y + 2 <= 7){
			if (kingpos.x - 1 >= 0 && some_board[kingpos.x - 1][kingpos.y + 2] == B_KNIGHT)
				return 1;
			if (kingpos.x + 1 <= 7 && some_board[kingpos.x + 1][kingpos.y + 2] == B_KNIGHT)
				return 1;
		}
		if (kingpos.y - 2 >= 0){
			if (kingpos.x - 1 >= 0 && some_board[kingpos.x - 1][kingpos.y - 2] == B_KNIGHT)
				return 1;
			if (kingpos.x + 1 <= 7 && some_board[kingpos.x + 1][kingpos.y - 2] == B_KNIGHT)
				return 1;
		}
	}
	else{ //color==BLACK
		if (kingpos.x - 2 >= 0){
			if (kingpos.y - 1 >= 0 && some_board[kingpos.x - 2][kingpos.y - 1] == W_KNIGHT)
				return 1;
			if (kingpos.y + 1 <= 7 && some_board[kingpos.x - 2][kingpos.y + 1] == W_KNIGHT)
				return 1;
		}
		if (kingpos.x + 2 <= 7){
			if (kingpos.y - 1 >= 0 && some_board[kingpos.x + 2][kingpos.y - 1] == W_KNIGHT)
				return 1;
			if (kingpos.y + 1 <= 7 && some_board[kingpos.x + 2][kingpos.y + 1] == W_KNIGHT)
				return 1;
		}
		if (kingpos.y + 2 <= 7){
			if (kingpos.x - 1 >= 0 && some_board[kingpos.x - 1][kingpos.y + 2] == W_KNIGHT)
				return 1;
			if (kingpos.x + 1 <= 7 && some_board[kingpos.x + 1][kingpos.y + 2] == W_KNIGHT)
				return 1;
		}
		if (kingpos.y - 2 >= 0){
			if (kingpos.x - 1 >= 0 && some_board[kingpos.x - 1][kingpos.y - 2] == W_KNIGHT)
				return 1;
			if (kingpos.x + 1 <= 7 && some_board[kingpos.x + 1][kingpos.y - 2] == W_KNIGHT)
				return 1;
		}
	}
	return 0;
}

// king of color 'color' is threatened and can't escape
int isMate(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){
	int is_check = isCheck(color, some_board);
	int is_check_after_opp_moves = 1;
	struct Moves* oppMoves;
	if (color == WHITE)
		oppMoves = getAllMoves(WHITE, some_board);
	else // color == black
		oppMoves = getAllMoves(BLACK, some_board);
	if (oppMoves != NULL){
		struct Move* temp = oppMoves->firstMove;
		while (temp != NULL)
		{
			if (!isCheck(color, temp->board_after_move))
				is_check_after_opp_moves = 0;
			struct Move* moveToFree;
			moveToFree = temp;
			temp = temp->next;
			free(moveToFree);
		}
		free(oppMoves);
	}
	return is_check_after_opp_moves && is_check;

}

int isTie(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){
	int is_check = isCheck(color, some_board);
	int is_tie = 0;
	struct Moves* moves = getAllMoves(color, some_board);
	filterMoves(color,moves);
	if (moves == NULL || moves->firstMove==NULL)
		is_tie = 1;
	freeMoves(moves);
	return (!is_check && is_tie);

}

void addMoveToMovesList(struct Move* move, struct Moves* moves){
	struct Move* moveToAdd = calloc(1,sizeof(struct Move));
	assert(moveToAdd != NULL);
	CopyMove(moveToAdd,move);
	if (moves->firstMove == NULL){ // list is empty
		moves->firstMove = moveToAdd;
		moves->lastMove = moveToAdd;
	}
	else{
		moves->lastMove->next = moveToAdd;
		moveToAdd->prev = moves->lastMove;
		moves->lastMove = moveToAdd;
	}
	free(move);
	move = NULL;

}

// we extend moves1 by adding moves2 to it's last node
void concatMovesLists(struct Moves* moves1, struct Moves* moves2){
	if (moves1 == NULL && moves2 != NULL){
		moves1->firstMove = moves2->firstMove;
		moves1->lastMove = moves2->lastMove;
		free(moves2);
		return;
	}
	else if (moves2 == NULL && moves1 != NULL)
		return;
	if (moves1->firstMove == NULL){
		moves1->firstMove = moves2->firstMove;
		moves1->lastMove = moves2->lastMove;
	}
	else if (moves2->firstMove == NULL){
		free(moves2);
		return;
	}
	else{
		moves1->lastMove->next = moves2->firstMove;
		moves2->firstMove->prev = moves1->lastMove;
		moves1->lastMove = moves2->lastMove;
	}
	free(moves2);
}

struct Position findKing(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Position kingpos;
	if (color == WHITE){
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (some_board[i][j] == W_KING){
					kingpos.x = i;
					kingpos.y = j;
					return kingpos;
				}
			}
		}
	}
	else{ // (color == BLACK)
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (some_board[i][j] == B_KING){
					kingpos.x = i;
					kingpos.y = j;
					return kingpos;
				}
			}
		}
	}
	kingpos.x = -1;
	kingpos.y = -1;
	return kingpos;
}

void CopyMove(struct Move* moveDst,struct Move* moveSrc){
	CopyBoard(moveDst->board_after_move, moveSrc->board_after_move);
	moveDst->dst = moveSrc->dst;
	moveDst->src = moveSrc->src;
	moveDst->score = moveSrc->score;
	moveDst->prev = moveSrc->prev;
	moveDst->next = moveSrc->next;
	moveDst->promotion = moveSrc->promotion;
}

int CheckValidMove(char some_board[BOARD_SIZE][BOARD_SIZE], int x, int y, int i, int j){
	int move_is_valid = 0;
	struct Moves* moves = getMovesForPosition(x, y, some_board);
	if (moves != NULL){
		struct Move* temp = moves->firstMove;
		while (temp != NULL)
		{
			struct Move* moveToFree;
			if (temp->dst.x == i && temp->dst.y == j)
				move_is_valid = 1;
			moveToFree = temp;
			temp = temp->next;
			free(moveToFree);
		}
		free(moves);
	}
	return move_is_valid;

}

void freeMoves(struct Moves* moves){
	if (moves != NULL){
		struct Move* temp = moves->firstMove;
		while (temp != NULL)
		{
			struct Move* moveToFree;
			moveToFree = temp;
			temp = temp->next;
			free(moveToFree);
		}
		free(moves);
	}

}

struct Moves* getAllMoves(int color, char some_board[BOARD_SIZE][BOARD_SIZE]){
	struct Moves* moves = calloc(1, sizeof(struct Moves));
	assert(moves != NULL);
	moves->firstMove = NULL;
	moves->lastMove = NULL;
	struct Moves* movesToAdd;
	if (color == WHITE){
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){

				if (isWhite(some_board[i][j])){
					movesToAdd = getMovesForPosition(i, j, some_board);
					if (movesToAdd != NULL)
						concatMovesLists(moves, movesToAdd);
				}
			}
		}
	}
	else{// (color == BLACK)
		for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++){
				if (isBlack(some_board[i][j])){
					movesToAdd = getMovesForPosition(i, j, some_board);
					if (movesToAdd != NULL)
						concatMovesLists(moves, movesToAdd);
				}
			}
		}
	}
	return moves;
}

void filterMoves(int color,struct Moves* moves){ // deletes from moves any move that causes a check to the playing player
	if (moves != NULL){
		struct Move* temp = moves->firstMove;
		while (temp != NULL)
		{
			struct Move* moveToFree;
			if (isCheck(color, temp->board_after_move)){
				if (temp->next != NULL){
					temp->next->prev = temp->prev;
				}
				if (temp->prev != NULL)
					temp->prev->next = temp->next;
				moveToFree = temp;
				if (moves->firstMove == temp)
					moves->firstMove = moves->firstMove->next;
				if (moves->lastMove == temp)
					moves->lastMove = moves->lastMove->prev;
				temp = temp->next;
				free(moveToFree);
				moveToFree = NULL;
			}
			else
				temp = temp->next;

		}


	}
}

int rookCheckKing(int color, struct Position kingpos, char some_board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	if (color == WHITE){
		//check if can eat up
		if (kingpos.y + 1 <= 7){
			i = kingpos.x;
			j = kingpos.y + 1;
			while (j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				j++;
			}
			if (some_board[i][j] == B_ROOK || some_board[i][j] == B_QUEEN)
				return 1;
		}
		// check if can eat right
		if (kingpos.x + 1 <= 7){
			i = kingpos.x + 1;
			j = kingpos.y;
			while (i < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
			}
			if (some_board[i][j] == B_ROOK || some_board[i][j] == B_QUEEN)
				return 1;
		}
		//check if can eat down
		if (kingpos.y - 1 >= 0){
			i = kingpos.x;
			j = kingpos.y - 1;
			while (j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				j--;
			}
			if (some_board[i][j] == B_ROOK || some_board[i][j] == B_QUEEN)
				return 1;
		}
		// check if can eat left
		if (kingpos.x - 1 >= 0){
			i = kingpos.x - 1;
			j = kingpos.y;
			while (i > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
			}
			if (some_board[i][j] == B_ROOK || some_board[i][j] == B_QUEEN)
				return 1;
		}
	}
	else{ // color== BLACK
		//check if can eat up
		if (kingpos.y + 1 <= 7){
			i = kingpos.x;
			j = kingpos.y + 1;
			while (j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				j++;
			}
			if (some_board[i][j] == W_ROOK || some_board[i][j] == W_QUEEN)
				return 1;
		}
		// check if can eat right
		if (kingpos.x + 1 <= 7){
			i = kingpos.x + 1;
			j = kingpos.y;
			while (i < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
			}
			if (some_board[i][j] == W_ROOK || some_board[i][j] == W_QUEEN)
				return 1;
		}
		//check if can eat down
		if (kingpos.y - 1 >= 0){
			i = kingpos.x;
			j = kingpos.y - 1;
			while (j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				j--;
			}
			if (some_board[i][j] == W_ROOK || some_board[i][j] == W_QUEEN)
				return 1;
		}
		// check if can eat left
		if (kingpos.x - 1 >= 0){
			i = kingpos.x - 1;
			j = kingpos.y;
			while (i > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
			}
			if (some_board[i][j] == W_ROOK || some_board[i][j] == W_QUEEN)
				return 1;
		}
	}
	return 0;
}
int bishopCheckKing(int color, struct Position kingpos, char some_board[BOARD_SIZE][BOARD_SIZE]){
	int i, j;
	if (color == WHITE){
		// check if can eat up-right
		if (kingpos.x + 1 <= 7 && kingpos.y + 1 <= 7){
			i = kingpos.x + 1;
			j = kingpos.y + 1;
			while (i < 7 && j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
				j++;
			}
			if (some_board[i][j] == B_BISHOP || some_board[i][j] == B_QUEEN){
				return 1;
			}
		}
		// check if can eat up-left
		if (kingpos.x - 1 >= 0 && kingpos.y + 1 <= 7){
			i = kingpos.x - 1;
			j = kingpos.y + 1;
			while (i > 0 && j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
				j++;
			}
			if (some_board[i][j] == B_BISHOP || some_board[i][j] == B_QUEEN)
				return 1;
		}
		//check if can eat down-left
		if (kingpos.x - 1 >= 0 && kingpos.y - 1 >= 0){
			i = kingpos.x - 1;
			j = kingpos.y - 1;
			while (i > 0 && j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
				j--;
			}
			if (some_board[i][j] == B_BISHOP || some_board[i][j] == B_QUEEN)
				return 1;
		}
		// check if can eat down-right
		if (kingpos.x + 1 <= 7 && kingpos.y - 1 >= 0){
			i = kingpos.x + 1;
			j = kingpos.y - 1;
			while (i < 7 && j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
				j--;
			}
			if (some_board[i][j] == B_BISHOP || some_board[i][j] == B_QUEEN)
				return 1;
		}
	}
	else{//color==BLACK
		// check if can eat up-right
		if (kingpos.x + 1 <= 7 && kingpos.y + 1 <= 7){
			i = kingpos.x + 1;
			j = kingpos.y + 1;
			while (i < 7 && j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
				j++;
			}
			if (some_board[i][j] == W_BISHOP || some_board[i][j] == W_QUEEN){
				return 1;
			}
		}
		// check if can eat up-left
		if (kingpos.x - 1 >= 0 && kingpos.y + 1 <= 7){
			i = kingpos.x - 1;
			j = kingpos.y + 1;
			while (i > 0 && j < 7){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
				j++;
			}
			if (some_board[i][j] == W_BISHOP || some_board[i][j] == W_QUEEN)
				return 1;
		}
		//check if can eat down-left
		if (kingpos.x - 1 >= 0 && kingpos.y - 1 >= 0){
			i = kingpos.x - 1;
			j = kingpos.y - 1;
			while (i > 0 && j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i--;
				j--;
			}
			if (some_board[i][j] == W_BISHOP || some_board[i][j] == W_QUEEN)
				return 1;
		}
		// check if can eat down-right
		if (kingpos.x + 1 <= 7 && kingpos.y - 1 >= 0){
			i = kingpos.x + 1;
			j = kingpos.y - 1;
			while (i < 7 && j > 0){
				if (some_board[i][j] != EMPTY)
					break;
				i++;
				j--;
			}
			if (some_board[i][j] == W_BISHOP || some_board[i][j] == W_QUEEN)
				return 1;
		}
	}
	return 0;
}

void SaveToFile(char* fileName)
{
	//open a new file. 'w' for writing mode.
	FILE *fb = fopen(fileName, "w");  

	//xml decleration 
	fprintf(fb, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");


	fprintf(fb, "<game>\n");

	//---settings---
	if (next_player == WHITE)
		fprintf(fb, "<next_turn>white</next_turn>\n");
	else
		fprintf(fb, "<next_turn>black</next_turn>\n");
	fprintf(fb, "<game_mode>%d</game_mode>\n", game_mode);
	if (difficulty == DIFF_BEST)
		fprintf(fb, "<difficulty>best</difficulty>\n");
	else
		fprintf(fb, "<difficulty>%d</difficulty>\n", difficulty);
	if (user_color == WHITE)
		fprintf(fb, "<user_color>white</user_color>\n");
	else
		fprintf(fb, "<user_color>black</user_color>\n");

	//---board---
	fprintf(fb, "<board>\n");
	for (int i = 7; i >= 0; i--) //for each row in the board
	{
		fprintf(fb, "<row_%d>", i + 1);

		for (int j = 0; j < 8; j++) //go for each col in the row and print 8 characters that represent the row's content.
		{
			char piece = board[j][i];
			if (piece == ' ')
			{ //put the char '_' for an empty spot
				piece = '_';
			}
			fprintf(fb, "%c", piece);
		}
		fprintf(fb, "</row_%d>\n", i + 1);
	}
	fprintf(fb, "</board>\n");
	fprintf(fb, "</game>\n");

	//close the file
	fclose(fb);

}


int FileExists(char* filename){
	FILE *file = fopen(filename, "r");
	if (file == NULL){
		//
		return 0;
	}
	fclose(file);
	return 1;
	
}