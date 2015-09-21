#ifndef CHESS_H
#define CHESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

#define EMPTY ' '
#define W_PAWN 'm'
#define W_BISHOP 'b'
#define W_ROOK 'r'
#define W_KNIGHT 'n'
#define W_QUEEN 'q'
#define W_KING 'k'

#define B_PAWN 'M'
#define B_BISHOP 'B'
#define B_ROOK 'R'
#define B_KNIGHT 'N'
#define B_QUEEN 'Q'
#define B_KING 'K'

#define MODE_2PLAYERS 1
#define MODE_PLAYER_VS_AI 2
#define DIFF_BEST 5

#define WIN_SCORE 600
#define TIE_SCORE -500

#define WHITE 0
#define BLACK 1

#define MOVES_BOUND 28

#define WORDS_IN_LINE 7

#define perror_message(func_name) (perror("Error: standard function %s has failed", func_name))
#define print_message(message) (printf("%s", message));
#define ILLEGAL_COMMAND "Illegal command, please try again\n"
#define WRONG_POSITION "Invalid position on the board\n"
#define NOT_YOUR_PIECE "The specified position does not contain your piece\n"
#define ILLEGAL_MOVE "Illegal move\n"


extern char* words[7];
int extern game_mode;
int extern console_or_gui;
int extern difficulty;
int extern next_player;// which player (1st or 2nd ) is playing now
int extern user_color;
#define BOARD_SIZE 8
#define SLOTS_NUMBER 7
char extern board[BOARD_SIZE][BOARD_SIZE];

int extern game_on;

int extern cnt_test_minimax;

extern struct Move* move_to_do;
int extern is_allocd_move_to_do;

int extern w_pawn;
int extern w_bishop;
int extern w_rook;
int extern w_knight;
int extern w_queen;
int extern w_king;
int extern b_pawn;
int extern b_bishop;
int extern b_rook;
int extern b_knight;
int extern b_queen;
int extern b_king;

struct Position{
	int x;
	int y;
};

struct Move{
	struct Position src;
	struct Position dst;
	char promotion;
	char board_after_move[BOARD_SIZE][BOARD_SIZE];
	struct Move* next;
	struct Move* prev;
	int score;
};

struct Moves{
	struct Move* firstMove;
	struct Move* lastMove;
};





void SettingsState();
void getInput();

void CommandLine();

//Command Line Functions:
void print_board(char some_board[BOARD_SIZE][BOARD_SIZE]);
void print_line();
void SettingsState();
void TransitionState();
void GameState();
void CommandLine();
void getInput();
void printInvalidPos();
void printMoves(struct Moves* moves);
void printMove(struct Move* temp);
void EnterMovesMessage();


//Setting State Functions:
void setGameMode(int mode);  //gets a mode value, 1 or 2, sets the game mode (a global variable) 
void setDifficulty(int diff);
void setUserColor(int color);
int loadGame(char* path);
void clearBoard(char some_board[BOARD_SIZE][BOARD_SIZE]);
void setNextPlayer(int n_player);
void removePiece(struct Position pos);
int setPiece(struct Position pos, char piece);
int checkBoard();
int checkKing();
void incPieceCount(char piece);
void decPieceCount(char piece);
char* findSubstring(char* line, char* opt1, char* opt2);
char* findDifficulty(char* line, char* opt1, char* opt2, char* opt3, char* opt4, char* opt5);
void InitFullBoard();
void InitPiecesCount();
void freeLine(char* words[WORDS_IN_LINE]);


//Game Logic
void CopyBoard(char dest[BOARD_SIZE][BOARD_SIZE], char src[BOARD_SIZE][BOARD_SIZE]);
void MakeMove(struct Move* move, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Position findKing(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int isBlack(char p);
int isWhite(char p);
struct Move* createMove(int x, int y, int i, int j, char some_board[BOARD_SIZE][BOARD_SIZE], char promotion);
struct Moves* PawnMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Moves* BishopMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Moves* RookMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Moves* KnightMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Moves* QueenMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
struct Moves* KingMoves(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
void addMoveToMovesList(struct Move* move, struct Moves* moves);
void CopyMove(struct Move* moveDst, struct Move* moveSrc);
void concatMovesLists(struct Moves* moves1, struct Moves* moves2);
struct Moves* getMovesForPosition(int x, int y, char some_board[BOARD_SIZE][BOARD_SIZE]);
int isPlayingColor(char p);
int CheckValidMove(char some_board[BOARD_SIZE][BOARD_SIZE], int x, int y, int i, int j);
struct Moves* getAllMoves(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int isCheck(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int isMate(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int isTie(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
void freeMoves(struct Moves* moves);
void filterMoves(int color, struct Moves* moves);
int rookCheckKing(int color, struct Position kingpos, char some_board[BOARD_SIZE][BOARD_SIZE]);
int bishopCheckKing(int color, struct Position kingpos, char some_board[BOARD_SIZE][BOARD_SIZE]);
void SaveToFile(char* fileName);
int FileExists(char* filename);


//Minimax
int Score(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int bestDepth(int color, char some_board[BOARD_SIZE][BOARD_SIZE]);
int alphaBeta(int color, char some_board[BOARD_SIZE][BOARD_SIZE], int alpha, int beta, int depthleft, int maxi);




#endif