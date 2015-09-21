#include "Chess.h"


char* words[WORDS_IN_LINE] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };

void CommandLine()
{
	InitFullBoard();
	print_board(board);
	SettingsState();
}

void SettingsState(){
	print_message("Enter game settings:\n");
	//free all previous words before getting a new input
	freeLine(words);
	getInput();

	while (strcmp(words[0], "quit") && strcmp(words[0], "start"))  //main loop in setting state
	{
		// clear

		if (!strcmp(words[0], "clear")){
			clearBoard(board);
		}

		// print

		else if (!strcmp(words[0], "print")){
			print_board(board);
		}
//game mode
		else if (!strcmp(words[0], "game_mode")){

			char* mode=NULL;

			if (!strcmp(words[1], "1")) //if user typed 1 for mode 
			{
				mode = "2 players";
				setGameMode(MODE_2PLAYERS); //send to settingState for update
			}
			else if (!strcmp(words[1], "2"))//if user typed 2 for mode
			{
				mode = "player vs. AI";
				setGameMode(MODE_PLAYER_VS_AI);//send to settingState for update
			}
			else
			{
				print_message("Wrong game mode\n");
			}
			if (mode!=NULL)
				printf("Running game in %s mode\n", mode);
		}

// difficulty

		else if (!strcmp(words[0], "difficulty")){
			if (game_mode != MODE_PLAYER_VS_AI)
			{
				print_message(ILLEGAL_COMMAND);
			}
			else
			{
				if (!strcmp(words[1], "best")) //if user typed 'best' for difficulty 
				{
					setDifficulty(DIFF_BEST);
				}
				else if (!strcmp(words[1], "depth"))
				{
					if (((int)atoi(words[2])>4) || ((int)atoi(words[2])<1)) //if depth isn't between 1 to 4
					{
						print_message("Wrong value for minimax depth. The value should be between 1 to 4\n");
					}
					else
					{
						setDifficulty((int)atoi(words[2]));
					}
				}
				else
				{
					print_message("Wrong value for difficulty.\n");
				}

			}
		}


// user_color

		else if (!strcmp(words[0], "user_color")){
			if (game_mode != MODE_PLAYER_VS_AI)
			{
				print_message(ILLEGAL_COMMAND);
			}
			else
			{
				if (strcmp(words[1], "white") && strcmp(words[1], "black")) // if user typed a wrong color- not black and not white
				{
					print_message("Wrong value for user color. The value should be either black or white.\n");
				}
				else
				{
					if (!strcmp(words[1], "white"))
						setUserColor(WHITE);
					else
						setUserColor(BLACK);
				}
			}
		}

// load

		else if (!strcmp(words[0], "load")){
			if (!loadGame(words[1]))
			{
				print_message("Wrong file name\n");
			}
			else
			{
				print_board(board);
				SettingsState();
			}
		}


// next_player

		else if (!strcmp(words[0], "next_player")){ 

			if (strcmp(words[1], "white") && strcmp(words[1], "black")) // if user typed a wrong color- not black and not white
			{
				print_message("Wrong value for next player. The value should be either black or white.\n");
			}
			else
			{
				if (!strcmp(words[1], "white"))
					setNextPlayer(WHITE);
				else
					setNextPlayer(BLACK);
			}
			
		}


// remove

		else if (!strcmp(words[0], "rm")){

			int x = (int)(words[1][1] - 'a');// char x is mapped to a position in the board [0.....7]
			int y = words[1][3] - (int)('1');
			struct Position pos = { x, y };
			if (x < 0 || x>7 || y < 0 || y>7){
				print_message(WRONG_POSITION);
			}
			else{
				removePiece(pos);
			}
		}

// set


		else if (!strcmp(words[0], "set")){ //if the first word is 'set'

			int x = (int)(words[1][1] - 'a');// char x is mapped to a position in the board [0.....7]
			int y = words[1][3] - (int)('1');// char y is mapped to a position in the board [0.....7]
			struct Position pos = { x, y };
			if (x < 0 || x>7 || y < 0 || y>7){ //if x or y are not between 0 to 7
				print_message(WRONG_POSITION);
			}
			else{ //if the position is valid

				char piece = EMPTY;

				if (!strcmp(words[3], "king") || !strcmp(words[3], "queen") || !strcmp(words[3], "rook") || !strcmp(words[3], "knight") || !strcmp(words[3], "bishop") || !strcmp(words[3], "pawn"))
				{

					if (strcmp(words[2], "black") || strcmp(words[2], "white"))
					{
						if (!strcmp(words[3], "king"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_KING;
							else if (!strcmp(words[2], "black"))
								piece = B_KING;

						}
						else if (!strcmp(words[3], "queen"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_QUEEN;
							else if (!strcmp(words[2], "black"))
								piece = B_QUEEN;
						}
						else if (!strcmp(words[3], "rook"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_ROOK;
							else if (!strcmp(words[2], "black"))
								piece = B_ROOK;
						}
						else if (!strcmp(words[3], "knight"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_KNIGHT;
							else if (!strcmp(words[2], "black"))
								piece = B_KNIGHT;
						}
						else if (!strcmp(words[3], "bishop"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_BISHOP;
							else if (!strcmp(words[2], "black"))
								piece = B_BISHOP;
						}
						else if (!strcmp(words[3], "pawn"))
						{
							if (!strcmp(words[2], "white"))
								piece = W_PAWN;
							else if (!strcmp(words[2], "black"))
								piece = B_PAWN;
						}

						
						setPiece(pos, piece);

					}
					else //if third isn't black or white
					{
						print_message("Wrong value for color. The value should be either black or white.\n");
					}


				}
				else
				{
					print_message("Wrong value for piece type. The value should be king, queen, rook, knight, bishop or pawn.\n");
				}



			}
		}




		else // the command is not a legal command
		{
			print_message(ILLEGAL_COMMAND);
		}
		freeLine(words);
		print_message("Enter game settings:\n");
		getInput();

	} //end main loop of setting state

	if (!strcmp(words[0], "quit"))
	{
		freeLine(words);
		exit(1);
	}
	else // "start"
	{
		freeLine(words);
		printf("\n");
		TransitionState();
	}
}

void TransitionState(){
	//check if one of the kings is missing,if so, print a message and go back to setting state, else, go to game state.
	if (!checkKing())
	{
		print_message("Wrong board initialization\n");
		SettingsState();
	}
	else
	{
		GameState();
	}

}

void GameState()
{

	if (game_mode == MODE_2PLAYERS)
	{
		if (next_player == WHITE)
			printf("White player - enter your move:\n");
		else
			printf("Black player - enter your move:\n");
	}
	else //mode player vs. computer 
	{
		if (user_color == WHITE&& next_player == WHITE)
			printf("White player - enter your move:\n");
		else if (user_color==BLACK && next_player==BLACK)
			printf("Black player - enter your move:\n");
		else{
			alphaBeta(next_player, board, INT_MIN, INT_MAX, difficulty,1);
			if (move_to_do != NULL){ // if the computer promotes a pawn
				MakeMove(move_to_do, board);
			}
			print_board(board);
			next_player = user_color;
			if (user_color == WHITE&& next_player == WHITE)
				printf("White player - enter your move:\n");
			else if (user_color == BLACK && next_player == BLACK)
				printf("Black player - enter your move:\n");
		}
			
	}

	freeLine(words);
	getInput();

	while (1) { //main loop in game state
		if (!strcmp(words[0], "quit"))
		{
			freeLine(words);
			exit(1);
		}
	
// move

		if (!strcmp(words[0], "move")){

			int x = (int)(words[1][1] - 'a');// char x is mapped to a position in the board [0.....7]
			int y = words[1][3] - (int)('1');// char y is mapped to a position in the board [0.....7]

			int i = (int)(words[3][1] - 'a');// char i is mapped to a position in the board [0.....7]
			int j = words[3][3] - (int)('1');// char j is mapped to a position in the board [0.....7]

			//struct Position src = { x, y };
			//struct Position dest = { i, j };

			if (x < 0 || x>7 || y < 0 || y>7){ //if x or y are not between 0 to 7
				print_message(WRONG_POSITION);
			}
			else if (i < 0 || i>7 || j < 0 || j>7){ //if i or j are not between 0 to 7
				print_message(WRONG_POSITION);
			}
			else if ((game_mode == MODE_PLAYER_VS_AI&&user_color == BLACK && next_player == BLACK && !isBlack(board[x][y])) || (game_mode == MODE_2PLAYERS&&next_player == BLACK && !isBlack(board[x][y])))
			{ //if player is black but the piece in <x,y> isn't black
				printf(NOT_YOUR_PIECE);
			}
			else if ((game_mode == MODE_PLAYER_VS_AI&&user_color == WHITE && next_player == WHITE && !isWhite(board[x][y])) || (game_mode == MODE_2PLAYERS&&next_player == WHITE && !isWhite(board[x][y])))
			{ //if player is white but the piece in <x,y> isn't white
				printf(NOT_YOUR_PIECE);
			}
			else if (!CheckValidMove(board, x, y, i, j)) 
			{
				printf(ILLEGAL_MOVE);
			}
			else{ //if the positions are valid
				char board_after_move[BOARD_SIZE][BOARD_SIZE];
				CopyBoard(board_after_move, board);
				board_after_move[i][j] = board_after_move[x][y];
				board_after_move[x][y] = EMPTY;
				if (isCheck(next_player, board_after_move))
					printf(ILLEGAL_MOVE); // you cannot have your own king be threatened after your move
				else if (words[4] == NULL && ((j == 7 && board_after_move[i][j] == W_PAWN) || (j == 0 && board_after_move[i][j] == B_PAWN)))
					printf(ILLEGAL_MOVE);
				else{
					if (words[4] != NULL && j == 7 && board[x][y] == W_PAWN){ // white pawn is promoted
						if (!strcmp(words[4], "queen")){
							board[i][j] = W_QUEEN;
							board[x][y] = EMPTY;
						}
						else if (!strcmp(words[4], "rook")){
							board[i][j] = W_ROOK;
							board[x][y] = EMPTY;
						}
						else if (!strcmp(words[4], "bishop")){
							board[i][j] = W_BISHOP;
							board[x][y] = EMPTY;
						}
						else if (!strcmp(words[4], "knight")){
							board[i][j] = W_KNIGHT;
							board[x][y] = EMPTY;
						}

					}
					else if (words[4]!=NULL && j == 0 && board[x][y] == B_PAWN){ // black pawn is promoted
						if (!strcmp(words[4], "queen")){
							board[i][j] = B_QUEEN;
							board[x][y] = EMPTY;
						}
						if (!strcmp(words[4], "rook")){
							board[i][j] = B_ROOK;
							board[x][y] = EMPTY;
						}
						if (!strcmp(words[4], "bishop")){
							board[i][j] = B_BISHOP;
							board[x][y] = EMPTY;
						}
						if (!strcmp(words[4], "knight")){
							board[i][j] = B_KNIGHT;
							board[x][y] = EMPTY;
						}
					}

					else{ // regular move, no promotion
						board[i][j] = board[x][y];
						board[x][y] = EMPTY;
					}

					print_board(board);

					if (isMate(!next_player, board)){ // is mate
						if (next_player == WHITE)
							printf("Mate! White player wins the game\n");
						else
							printf("Mate! Black player wins the game\n");
						freeLine(words);
						exit(1);
					}
					else if (isTie(!next_player, board)){
						printf("The game ends in a tie\n");
						freeLine(words);
						exit(1);
					}
					else if (isCheck(!next_player, board))
						printf("Check!\n");
					// if it's playerVsAI - it's the computer's turn now
					if (game_mode == MODE_PLAYER_VS_AI){
						if (user_color == WHITE){
							next_player = BLACK;
							alphaBeta(BLACK, board, INT_MIN, INT_MAX, difficulty,1);
						}
						else{
							next_player = WHITE;
							alphaBeta(WHITE,board,INT_MIN, INT_MAX, difficulty,1);

						}
						if (move_to_do != NULL){ // if the computer promotes a pawn
							if ((board[move_to_do->src.x][move_to_do->src.y] == W_PAWN && move_to_do->dst.y == 7)
								|| (board[move_to_do->src.x][move_to_do->src.y] == B_PAWN && move_to_do->dst.y == 0))
							{
								board[move_to_do->dst.x][move_to_do->dst.y] = move_to_do->promotion;
								board[move_to_do->src.x][move_to_do->src.y] = EMPTY;
							}
							else{ // computer made a regular move
								board[move_to_do->dst.x][move_to_do->dst.y] = board[move_to_do->src.x][move_to_do->src.y];
								board[move_to_do->src.x][move_to_do->src.y] = EMPTY;
							}
						}
						if (is_allocd_move_to_do){
							free(move_to_do);
							is_allocd_move_to_do = 0;
						}
						print_board(board);
						next_player = user_color;
					}
					// 2 player mode - change next_player
					else{
						if (next_player == WHITE){
							next_player = BLACK;
						}
						else{
							next_player = WHITE;
						}

					}
				}
			}
		}

// get_moves

		else if (!strcmp(words[0], "get_moves")){
			int pos_x = (int)(words[1][1] - 'a');
			int pos_y = words[1][3] - (int)('1');
			if (isPlayingColor(board[pos_x][pos_y])){
				struct Moves* moves = getMovesForPosition(pos_x, pos_y, board);
				printMoves(moves);
			//	freeMoves(moves);
			}
		}


// get_best_moves

		else if (!strcmp(words[0], "get_best_moves")){
				int depth_to_send;
				if (!strcmp(words[1], "1"))
					depth_to_send = 1;
				else if (!strcmp(words[1], "2"))
					depth_to_send = 2;
				else if (!strcmp(words[1], "3"))
					depth_to_send = 3;
				else if (!strcmp(words[1], "4"))
					depth_to_send = 4;
				else if (!strcmp(words[1], "best")){
					depth_to_send = bestDepth(next_player, board);
				}
				struct Moves* moves = getAllMoves(next_player, board);
				filterMoves(next_player, moves);
				int max_score = INT_MIN;
				int cur_score;
				struct Move* temp = moves->firstMove;
				while (temp != NULL)
				{
					cur_score = alphaBeta(!next_player, temp->board_after_move, INT_MIN, INT_MAX, depth_to_send - 1, 0);
					if (cur_score > max_score){
						max_score = cur_score;
					}
					temp = temp->next;
				}
				freeMoves(moves);
				// maximum score was found. now we compare each move's score to the maximum
				moves = getAllMoves(next_player, board);
				temp = moves->firstMove;
				while (temp != NULL)
				{
					cur_score = alphaBeta(!next_player, temp->board_after_move, INT_MIN, INT_MAX, depth_to_send - 1, 0);
					if (cur_score == max_score){
						printMove(temp);
					}
					temp = temp->next;
				}
				freeMoves(moves);
		}

// get_score

		else if (!strcmp(words[0], "get_score")){
			if (words[5] == NULL){
				printf(ILLEGAL_COMMAND);
			}
			else{
				int depth_to_send;
				if (!strcmp(words[1], "1"))
					depth_to_send = 1;
				else if (!strcmp(words[1], "2"))
					depth_to_send = 2;
				else if (!strcmp(words[1], "3"))
					depth_to_send = 3;
				else if (!strcmp(words[1], "4"))
					depth_to_send = 4;
				else if (!strcmp(words[1], "best")){
					depth_to_send = bestDepth(next_player, board);
				}


				int x = (int)(words[3][1] - 'a');// char x is mapped to a position in the board [0.....7]
				int y = words[3][3] - (int)('1');// char y is mapped to a position in the board [0.....7]

				int i = (int)(words[5][1] - 'a');// char i is mapped to a position in the board [0.....7]
				int j = words[5][3] - (int)('1');// char j is mapped to a position in the board [0.....7]

				//struct Position src = { x, y };
				//struct Position dest = { i, j };
				char promotion = EMPTY;

				// we can assume the positions are valid
				if (words[6] != NULL && j == 7 && board[x][y] == W_PAWN){ // white pawn is promoted
					if (!strcmp(words[6], "queen")){
						promotion = W_QUEEN;
					}
					else if (!strcmp(words[6], "rook")){
						promotion = W_ROOK;
					}
					else if (!strcmp(words[6], "bishop")){
						promotion = W_BISHOP;
					}
					else if (!strcmp(words[6], "knight")){
						promotion = W_KNIGHT;
					}

				}
				else if (words[6] != NULL && j == 0 && board[x][y] == B_PAWN){ // white pawn is promoted
					if (!strcmp(words[6], "queen")){
						promotion = B_QUEEN;
					}
					else if (!strcmp(words[6], "rook")){
						promotion = B_ROOK;
					}
					else if (!strcmp(words[6], "bishop")){
						promotion = B_BISHOP;
					}
					else if (!strcmp(words[6], "knight")){
						promotion = B_KNIGHT;
					}
				}
				if (CheckValidMove(board, x, y, i, j)){

					struct Move* temp_move = createMove(x, y, i, j, board, promotion);
					if (!isCheck(next_player, temp_move->board_after_move)){
						int score_of_move = alphaBeta(!next_player, temp_move->board_after_move, INT_MIN, INT_MAX, depth_to_send-1, 0);
						printf("%d\n", score_of_move);
						free(temp_move);
					}

				}
			}
		}

// save

		else if (!strcmp(words[0], "save")){
			SaveToFile(words[1]);
		}

		else
		{
			print_message(ILLEGAL_COMMAND);
		}
		EnterMovesMessage();
		freeLine(words);
		getInput();

	} //end of main loop
	if (!strcmp(words[0], "quit"))
	{
		freeLine(words);
		if (is_allocd_move_to_do){
			free(move_to_do);
			is_allocd_move_to_do = 0;
		}
		exit(1);
	}

}

//prints the board to console
void print_board(char some_board[BOARD_SIZE][BOARD_SIZE])
{
	int i, j;
	print_line();
	for (j = BOARD_SIZE - 1; j >= 0; j--)
	{
		printf("%d", j + 1);
		for (i = 0; i < BOARD_SIZE; i++){ 
			printf("| %c ", some_board[i][j]);
		}
		printf("|\n");
		print_line();
	}
//	printf("   ");
	for (j = 0; j < BOARD_SIZE; j++){
		printf("   %c", (char)('a' + j));
	}
	printf("  ");
	printf("\n");
}



void printInvalidPos(){
	printf(WRONG_POSITION);
}

void print_line(){
	int i;
	printf(" |");
	for (i = 1; i < BOARD_SIZE * 4; i++){
		printf("-");
	}
	printf("|\n");
}

void getInput(){ // we get the input line from the user and return it as a 5 size array
	char c; // the next character to be read from user
	int j = 1; // counts the length of a word
	int k = 0; // index of the word in the word[] array [0,1,2,3,4,5,6];
	words[k] = (char*)calloc(1, sizeof(char)); //allocate one char to the first word
	assert(words[k] != NULL);
	c = '\0';
	while (c != '\n')
	{
		c = getc(stdin);     //read the input from keyboard standard input
		words[k] = (char*)realloc(words[k], j*sizeof(char)+1);    //re-allocate (resize) memory for character read to be stored
		assert(words[k] != NULL);
		if (c == ' '){
			c = getc(stdin);
			words[k][j - 1] = '\0';
			j = 1;
			k++;
			if (k >= WORDS_IN_LINE){
				printf("too many arguments.. 8th word will be ignored");
				return;
			}
			words[k] = (char*)calloc(1, sizeof(char));
			assert(words[k] != NULL);
		}
		words[k][j - 1] = c;  //store read character by making pointer point to c
		j++;
	}
	words[k][j - 2] = '\0';   //at the end append null character to mark end of string
}
 
void printMoves(struct Moves* moves){
	if (moves != NULL){
		struct Move* temp = moves->firstMove;
		while (temp != NULL)
		{
			struct Move* moveToFree;
			if (!isCheck(next_player, temp->board_after_move)){
				printMove(temp);
			}
			moveToFree = temp;
			temp = temp->next;
			free(moveToFree);
		}
		free(moves);
	}
}

void printMove(struct Move* temp){
	char x = temp->src.x + 'a';
	int y = temp->src.y + 1;
	char i = temp->dst.x + 'a';
	char j = temp->dst.y + 1;
	printf("<%c,%d> to <%c,%d>", x, y, i, j);
	if (temp->promotion != EMPTY){
		if (temp->promotion == W_KNIGHT || temp->promotion == B_KNIGHT)
			printf(" knight");
		else if (temp->promotion == W_QUEEN || temp->promotion == B_QUEEN)
			printf(" queen");
		else if (temp->promotion == W_BISHOP || temp->promotion == B_BISHOP)
			printf(" bishop");
		else if (temp->promotion == W_ROOK || temp->promotion == B_ROOK)
			printf(" rook");
	}
	printf("\n");
}

void EnterMovesMessage(){
	if (game_mode == MODE_2PLAYERS)
	{
		if (next_player == WHITE)
			printf("White player - enter your move:\n");
		else
			printf("Black player - enter your move:\n");
	}
	else //mode player vs. computer 
	{
		if (user_color == WHITE&& next_player == WHITE)
			printf("White player - enter your move:\n");
		else if (user_color == BLACK && next_player == BLACK)
			printf("Black player - enter your move:\n");
		else{
			alphaBeta(next_player, board, INT_MIN, INT_MAX, difficulty, 1);
			if (move_to_do != NULL){ // if the computer promotes a pawn
				MakeMove(move_to_do, board);
			}
			print_board(board);
			next_player = user_color;
			if (user_color == WHITE&& next_player == WHITE)
				printf("White player - enter your move:\n");
			else if (user_color == BLACK && next_player == BLACK)
				printf("Black player - enter your move:\n");
		}

	}
}

void freeLine(char* words[WORDS_IN_LINE]){
	for (int i = 0; i < WORDS_IN_LINE; i++){
		if (words[i] != NULL){
			free(words[i]);
			words[i] = NULL;
		}
	}
}