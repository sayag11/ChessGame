#include "Chess_GUI.h"

#define WIN_W 800
#define WIN_H 600
#define IMG_W 70
#define IMG_H 70
#define ALPHA 100
#define POPUP_DELAY 800

SDL_Surface *screen;
SDL_Color orange = { 255, 165, 0 };
SDL_Color blue = { 108, 214, 246 };
SDL_Color yellow = { 255, 255, 102 };
SDL_Color red = { 255, 0, 0 };
SDL_Color grey = { 160, 160, 160 };
SDL_Color dark_grey = { 64,64, 64 };
SDL_Color green = { 0, 255, 0 };
int game_on = 1;
char promotion = EMPTY;
void GUI_GameWindow()
{
	SDL_Event e;
	SDL_Rect rect_save_game = { 596, 13, 200, 100 };
	SDL_Rect rect_main_menu = { 594, 130, 200, 100 };
	SDL_Rect rect_quit = { 594, 457, 202, 81 };
	
	SDL_Rect best_move_1 = { 598, 354, 30, 30 };
	SDL_Rect best_move_2 = { 628, 354, 36, 30 };
	SDL_Rect best_move_3 = { 664, 354, 36, 30 };
	SDL_Rect best_move_4 = { 700, 354, 32, 30 };
	SDL_Rect best_move_best = { 732, 354, 55, 30 };
	SDL_Rect rect_src_move;
	SDL_Rect rect_dst_move;
	SDL_Rect rect_src_best_move;
	SDL_Rect rect_dst_best_move;
	SDL_Rect imgrect;
	rect_src_best_move.w = IMG_W-3;
	rect_src_best_move.h = IMG_H-2;
	rect_dst_best_move.w = IMG_W - 3;
	rect_dst_best_move.h = IMG_H - 2;
	rect_src_move.w = IMG_W - 3;
	rect_src_move.h = IMG_H - 2;
	rect_dst_move.w = IMG_W - 3;
	rect_dst_move.h = IMG_H - 2;
	imgrect.w = IMG_W;
	imgrect.h = IMG_H;
	struct Position pos_to_move = { -1, -1 };
	SDL_Surface *background = SDL_LoadBMP("images/GameWindow.bmp");
	SDL_Surface *screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return ;
	}

	/* Create window surface*/

	if (screen == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return ;
	}

	/* Define the rects we need*/


	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return ;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return ;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

	while (!quit) {
		if (game_on)
			drawNextPlayer(background);
		drawBoard();
			/* Poll for keyboard & mouse events*/
		//if computer starts the game
		if (game_mode == MODE_PLAYER_VS_AI){
			if (user_color != next_player){
				if (difficulty == DIFF_BEST){
					difficulty = bestDepth(next_player, board);
				}
				alphaBeta(next_player, board, INT_MIN, INT_MAX, difficulty,1);
				if (move_to_do != NULL&& is_allocd_move_to_do){
					MakeMove(move_to_do, board);
					rect_src_move.x = 31 + move_to_do->src.x*IMG_H;
					rect_src_move.y = 497 - move_to_do->src.y*IMG_W;
					
					rect_dst_move.x = 31 + move_to_do->dst.x*IMG_H;
					rect_dst_move.y = 497 - move_to_do->dst.y*IMG_W;
					

					refreshToBackground(background);
					drawBoard();
					MarkRect(rect_src_move, dark_grey,1);
					MarkRect(rect_dst_move, grey,1);
					free(move_to_do);
					is_allocd_move_to_do = 0;
				}
				if (isMate(!next_player, board)){
					if (next_player == WHITE)
						Popup_Message(POPUP_DELAY * 4, background, "images/WhiteWins.bmp");
					else
						Popup_Message(POPUP_DELAY * 4, background, "images/BlackWins.bmp");
					game_on = 0;
					refreshToBackground(background);

				}
				else if (isTie(!next_player, board) || isTie(next_player, board)){
					Popup_Message(POPUP_DELAY * 4, background, "images/tie.bmp");
					game_on = 0;
					refreshToBackground(background);
				}
				else if (isCheck(!next_player, board))
					Popup_Message(POPUP_DELAY, background, "images/Check.bmp");
				next_player = user_color;
			}
		}
		
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_save_game.x) && (e.button.x < rect_save_game.x + rect_save_game.w) && (e.button.y > rect_save_game.y) && (e.button.y < rect_save_game.y + rect_save_game.h))
				{
					GUI_SaveGameWindow();
					quit = 1;
				}
			   if ((e.button.x > rect_main_menu.x) && (e.button.x < rect_main_menu.x + rect_main_menu.w) && (e.button.y > rect_main_menu.y) && (e.button.y < rect_main_menu.y + rect_main_menu.h))
			    {
				 GUI_MainWindow();
				quit = 1;
				}
			   if ((e.button.x > rect_quit.x) && (e.button.x < rect_quit.x + rect_quit.w) && (e.button.y > rect_quit.y) && (e.button.y < rect_quit.y + rect_quit.h))
			   {
				   quit = 1;
			   }
			   if ((e.button.x > best_move_1.x) && (e.button.x < best_move_1.x + best_move_1.w) && (e.button.y > best_move_1.y) && (e.button.y < best_move_1.y + best_move_1.h))
			   {
				   if (game_on)
					 showBestMove(1, rect_src_best_move, rect_dst_best_move, background);
			   }
			   if ((e.button.x > best_move_2.x) && (e.button.x < best_move_2.x + best_move_2.w) && (e.button.y > best_move_2.y) && (e.button.y < best_move_2.y + best_move_2.h))
			   {
				   if (game_on)
					 showBestMove(2, rect_src_best_move, rect_dst_best_move, background);
			   }
			   if ((e.button.x > best_move_3.x) && (e.button.x < best_move_3.x + best_move_3.w) && (e.button.y > best_move_3.y) && (e.button.y < best_move_3.y + best_move_3.h))
			   {
				   if (game_on)
					showBestMove(3, rect_src_best_move, rect_dst_best_move, background);
			   }
			   if ((e.button.x > best_move_4.x) && (e.button.x < best_move_4.x + best_move_4.w) && (e.button.y > best_move_4.y) && (e.button.y < best_move_4.y + best_move_4.h))
			   {
				   if (game_on)
					showBestMove(4, rect_src_best_move, rect_dst_best_move, background);
			   }
			   if ((e.button.x > best_move_best.x) && (e.button.x < best_move_best.x + best_move_best.w) && (e.button.y > best_move_best.y) && (e.button.y < best_move_best.y + best_move_best.h))
			   {
				   if (game_on){
					   int bestDiffForPlayer = bestDepth(next_player, board);
					   showBestMove(bestDiffForPlayer, rect_src_best_move, rect_dst_best_move, background);
				   }
			   }


			   if (game_on){
				   for (int i = 0; i < 8; i++){
					   for (int j = 0; j < 8; j++)
					   {
						   imgrect.x = 43 + i*IMG_H;
						   imgrect.y = 503 - j*IMG_W;

						   if ((e.button.x > imgrect.x) && (e.button.x < imgrect.x + imgrect.w) && (e.button.y > imgrect.y) && (e.button.y < imgrect.y + imgrect.h))
						   {
							   if (pos_to_move.x == -1){
								   pos_to_move.x = i;
								   pos_to_move.y = j;

							   }
						   }
					   }
				   }
			   }

			   break;
			case (SDL_MOUSEBUTTONUP) :
				for (int i = 0; i < 8; i++){
					for (int j = 0; j < 8; j++)
					{
						imgrect.x = 43 + i*IMG_H;
						imgrect.y = 503 - j*IMG_W;

						if ((e.button.x > imgrect.x) && (e.button.x < imgrect.x + imgrect.w) && (e.button.y > imgrect.y) && (e.button.y < imgrect.y + imgrect.h))
						{
							if (pos_to_move.x != -1){
								if ((game_mode == MODE_PLAYER_VS_AI&&user_color == BLACK&& next_player == BLACK && !isBlack(board[pos_to_move.x][pos_to_move.y])) || (game_mode == MODE_2PLAYERS&&next_player == BLACK && !isBlack(board[pos_to_move.x][pos_to_move.y])))
								{ //if player is black but the piece in <x,y> isn't black
									PopUp(POPUP_DELAY, background, "images/IllegalMove.bmp"); 
									pos_to_move.x = -1;
									pos_to_move.y = -1;
								}
								else if ((game_mode == MODE_PLAYER_VS_AI&&user_color == WHITE && next_player == WHITE && !isWhite(board[pos_to_move.x][pos_to_move.y])) || (game_mode == MODE_2PLAYERS&&next_player == WHITE && !isWhite(board[pos_to_move.x][pos_to_move.y])))
								{ //if player is white but the piece in <x,y> isn't white
									PopUp(POPUP_DELAY, background, "images/IllegalMove.bmp");
									pos_to_move.x = -1;
									pos_to_move.y = -1;
								}
								else if (!CheckValidMove(board, pos_to_move.x, pos_to_move.y, i, j)) 
								{
									PopUp(POPUP_DELAY, background, "images/IllegalMove.bmp");
									pos_to_move.x = -1;
									pos_to_move.y = -1;
								}
								else{
									char board_after_move[BOARD_SIZE][BOARD_SIZE];
									CopyBoard(board_after_move, board);
									board_after_move[i][j] = board_after_move[pos_to_move.x][pos_to_move.y];
									board_after_move[pos_to_move.x][pos_to_move.y] = EMPTY;
									if (isCheck(next_player, board_after_move)){
										PopUp(POPUP_DELAY, background, "images/IllegalMove.bmp"); // you cannot have your own king be threatened after your move
									}
									else{
										if ((j == 7 && board[pos_to_move.x][pos_to_move.y] == W_PAWN) || (j == 0 && board[pos_to_move.x][pos_to_move.y] == B_PAWN)){
											char promotion = GUI_PromotionWindow();
											board[i][j] = promotion;
											board[pos_to_move.x][pos_to_move.y] = EMPTY;
											refreshToBackground(background);

										}
										else{	//regular move

											board[i][j] = board[pos_to_move.x][pos_to_move.y];
											board[pos_to_move.x][pos_to_move.y] = EMPTY;
										}
										pos_to_move.x = -1;
										pos_to_move.y = -1;
										refreshToBackground(background);
										drawBoard();
										if (isMate(!next_player, board)){ // is mate
											if (next_player == WHITE)
												Popup_Message(POPUP_DELAY * 4, background, "images/WhiteWins.bmp");
											else
												Popup_Message(POPUP_DELAY * 4, background, "images/BlackWins.bmp");
											game_on = 0;
											refreshToBackground(background);
										}
										else if (isTie(!next_player, board) || isTie(next_player, board)){
											Popup_Message(POPUP_DELAY * 4, background, "images/tie.bmp");
											game_on = 0;
											refreshToBackground(background);
										}
										else if (isCheck(!next_player, board))
											Popup_Message(POPUP_DELAY, background, "images/Check.bmp");
										// if it's playerVsAI - it's the computer's turn now
										if (game_mode == MODE_PLAYER_VS_AI){
											if (user_color == WHITE){
												next_player = BLACK;
											}
											else{
												next_player = WHITE;
											}
										}
										// 2 player mode - change next_player
										else{
											if (next_player == WHITE)
												next_player = BLACK;
											else
												next_player = WHITE;
										}
									}

								}
							}
						}
					}
				}

			default:
				break;
			}

		}

		/* Wait a little before redrawing*/
		SDL_Delay(400);
	}

	SDL_FreeSurface(background);
	return ;
}
void GUI_Main(){
	
	screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	GUI_MainWindow();
	SDL_Quit();

}
void GUI_MainWindow(){
	SDL_Event e;
	SDL_Rect rect_newGame = { 190, 260, 550, 80 };
	SDL_Rect rect_loadGame = { 190, 371, 550, 80 };
	SDL_Rect rect_quit = { 185, 476, 550, 80 };
	SDL_Surface *background = SDL_LoadBMP("images/MainWindow.bmp");
	
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}
	SDL_WM_SetCaption("Chess by Amit and Yehonatan", NULL);

	/* Create window surface*/

	if (screen == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return;
	}

	/* Define the rects we need*/


	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}	
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

	
	while (!quit) {

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_newGame.x) && (e.button.x < rect_newGame.x + rect_newGame.w) && (e.button.y > rect_newGame.y) && (e.button.y < rect_newGame.y + rect_newGame.h))
				{
					InitFullBoard();
					setDeafultSettings();
					GUI_PlayerSelectionWindow();
					quit = 1;
				}
				else{
					if ((e.button.x > rect_quit.x) && (e.button.x < rect_quit.x + rect_quit.w) && (e.button.y > rect_quit.y) && (e.button.y < rect_quit.y + rect_quit.h))
					{
						quit = 1;
					}
					else{


					}
					if ((e.button.x > rect_loadGame.x) && (e.button.x < rect_loadGame.x + rect_loadGame.w) && (e.button.y > rect_loadGame.y) && (e.button.y < rect_loadGame.y + rect_loadGame.h))
					{
						GUI_LoadGameWindow();
						quit = 1;
					}
				}
				break;




			case (SDL_MOUSEMOTION) :




			default:
				break;
			}
		}
	}

	SDL_FreeSurface(background);
	return;
}
void GUI_PlayerSelectionWindow(){
	SDL_Event e;
	SDL_Rect rect_cancel = {20, 466, 200, 100 };
	SDL_Rect rect_startGame = { 28, 273, 192, 137 };
	SDL_Rect rect_setBoard = { 292, 276, 190, 137 };
	SDL_Rect rect_playerVsAI = { 510, 280, 250, 130 };
	SDL_Rect rect_playerVsPlayer = { 510, 433, 250, 130 };
	SDL_Rect next_player_white = { 311, 512, 53, 48 };
	SDL_Rect next_player_black = { 400, 512, 53, 48 };

	SDL_Surface *background = SDL_LoadBMP("images/PlayerSelectionWindow.bmp");
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	/* Create window surface*/
	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (next_player==BLACK)
		MarkRect(next_player_black, orange,0);
	else
		MarkRect(next_player_white,orange,0);
	if (game_mode == MODE_PLAYER_VS_AI)
		MarkRect(rect_playerVsAI,orange,0);
	else
		MarkRect(rect_playerVsPlayer,orange,0);
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	

	while (!quit) {

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_cancel.x) && (e.button.x < rect_cancel.x + rect_cancel.w) && (e.button.y > rect_cancel.y) && (e.button.y < rect_cancel.y + rect_cancel.h))
				{
					GUI_MainWindow();
					quit = 1;
				}
			if ((e.button.x > rect_startGame.x) && (e.button.x < rect_startGame.x + rect_startGame.w) && (e.button.y > rect_startGame.y) && (e.button.y < rect_startGame.y + rect_startGame.h))
				{
					if (checkKing()){
						game_on = 1;
						GUI_GameWindow();
						quit = 1;
					}
					else{
						PopUp(POPUP_DELAY, background, "images/WrongBoard.bmp");
						if (game_mode == MODE_PLAYER_VS_AI)
							MarkRect(rect_playerVsAI, orange,0);
						else
							MarkRect(rect_playerVsPlayer, orange,0);
						if (next_player == BLACK)
							MarkRect(next_player_black, orange,0);
						else
							MarkRect(next_player_white, orange,0);
					}


				}
				 if ((e.button.x > rect_setBoard.x) && (e.button.x < rect_setBoard.x + rect_setBoard.w) && (e.button.y > rect_setBoard.y) && (e.button.y < rect_setBoard.y + rect_setBoard.h))
				{
					GUI_SetBoardWindow();
					quit = 1;
				}
				 if ((e.button.x > rect_playerVsAI.x) && (e.button.x < rect_playerVsAI.x + rect_playerVsAI.w) && (e.button.y > rect_playerVsAI.y) && (e.button.y < rect_playerVsAI.y + rect_playerVsAI.h))
				{
					game_mode = MODE_PLAYER_VS_AI;
					GUI_AISettingsWindow();
					quit = 1;
				}
				 if ((e.button.x > rect_playerVsPlayer.x) && (e.button.x < rect_playerVsPlayer.x + rect_playerVsPlayer.w) && (e.button.y > rect_playerVsPlayer.y) && (e.button.y < rect_playerVsPlayer.y + rect_playerVsPlayer.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					if (next_player==BLACK)
						MarkRect(next_player_black, orange,0);
					else
						MarkRect(next_player_white, orange,0);
					game_mode = MODE_2PLAYERS;
					MarkRect(rect_playerVsPlayer, orange,0);
				//	quit = 1;
				}
				 if ((e.button.x > next_player_white.x) && (e.button.x < next_player_white.x + next_player_white.w) && (e.button.y > next_player_white.y) && (e.button.y < next_player_white.y + next_player_white.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}

					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					next_player = WHITE;
					MarkRect(next_player_white, orange,0);
					if (game_mode == MODE_PLAYER_VS_AI)
						MarkRect(rect_playerVsAI, orange,0);
					else
						MarkRect(rect_playerVsPlayer, orange,0);
					/* Creating the surface. */
				}
				 if ((e.button.x > next_player_black.x) && (e.button.x < next_player_black.x + next_player_black.w) && (e.button.y > next_player_black.y) && (e.button.y < next_player_black.y + next_player_black.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					next_player = BLACK;

					MarkRect(next_player_black, orange,0);
					if (game_mode == MODE_PLAYER_VS_AI)
						MarkRect(rect_playerVsAI, orange,0);
					else
						MarkRect(rect_playerVsPlayer, orange,0);
				//	quit = 1;
				}
									   break;
			case (SDL_MOUSEMOTION) :

			default:
				break;
			}
		}
	}

	SDL_FreeSurface(background);
	return;
}
void GUI_LoadGameWindow(){
	SDL_Event e;
	SDL_Rect rect_slots[SLOTS_NUMBER] = { { 217, 109, 370, 62 }, { 217, 171, 370, 62 }, { 217, 234, 370, 63 }, { 217, 296, 370, 62 },
	{ 217, 356, 370, 59 }, { 217, 415, 370, 61 }, { 217, 476, 370, 75 } };
	SDL_Rect rect_back = { 600, 500, 200, 100 };
	//rect_slots[1] = { 217, 109, 370, 62 };
	//rect_slots[2] = { 217, 171, 370, 62 };
	//rect_slots[3] = { 217, 234, 370, 63 };
	//rect_slots[4] = { 217, 296, 370, 62 };
	//rect_slots[5] = { 217, 356, 370, 59 };
	//rect_slots[6] = { 217, 415, 370, 61 };
	//rect_slots[7] = { 217, 476, 370, 75 };
	SDL_Surface *background = SDL_LoadBMP("images/LoadGameWindow.bmp");
	SDL_Surface *screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	MarkSlots(rect_slots[0], red, "slot1.xml");
	MarkSlots(rect_slots[1], red, "slot2.xml");
	MarkSlots(rect_slots[2], red, "slot3.xml");
	MarkSlots(rect_slots[3], red, "slot4.xml");
	MarkSlots(rect_slots[4], red, "slot5.xml");
	MarkSlots(rect_slots[5], red, "slot6.xml");
	MarkSlots(rect_slots[6], red, "slot7.xml");
	while (!quit) {

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_back.x) && (e.button.x < rect_back.x + rect_back.w) && (e.button.y > rect_back.y) && (e.button.y < rect_back.y + rect_back.h))
				{
					GUI_MainWindow();
					quit = 1;

				}
				if ((e.button.x > rect_slots[0].x) && (e.button.x < rect_slots[0].x + rect_slots[0].w) && (e.button.y > rect_slots[0].y) && (e.button.y < rect_slots[0].y + rect_slots[0].h))
				{
					if (!loadGame("slot1.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
				if ((e.button.x > rect_slots[1].x) && (e.button.x < rect_slots[1].x + rect_slots[1].w) && (e.button.y > rect_slots[1].y) && (e.button.y < rect_slots[1].y + rect_slots[1].h))
				{
		
					if (!loadGame("slot2.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   if ((e.button.x > rect_slots[2].x) && (e.button.x < rect_slots[2].x + rect_slots[2].w) && (e.button.y > rect_slots[2].y) && (e.button.y < rect_slots[2].y + rect_slots[2].h))
									   {
			
					if (!loadGame("slot3.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   if ((e.button.x > rect_slots[3].x) && (e.button.x < rect_slots[3].x + rect_slots[3].w) && (e.button.y > rect_slots[3].y) && (e.button.y < rect_slots[3].y + rect_slots[3].h))
									   {
				
					if (!loadGame("slot4.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");

					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   if ((e.button.x > rect_slots[4].x) && (e.button.x < rect_slots[4].x + rect_slots[4].w) && (e.button.y > rect_slots[4].y) && (e.button.y < rect_slots[4].y + rect_slots[4].h))
									   {
			
					if (!loadGame("slot5.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   if ((e.button.x > rect_slots[5].x) && (e.button.x < rect_slots[5].x + rect_slots[5].w) && (e.button.y > rect_slots[5].y) && (e.button.y < rect_slots[5].y + rect_slots[5].h))
									   {
				
					if (!loadGame("slot6.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   if ((e.button.x > rect_slots[6].x) && (e.button.x < rect_slots[6].x + rect_slots[6].w) && (e.button.y > rect_slots[6].y) && (e.button.y < rect_slots[6].y + rect_slots[6].h))
									   {
					if (!loadGame("slot7.xml")){
						PopUp(POPUP_DELAY, background, "images/SlotIsEmpty.bmp");
					}
					else{
						PopUp(POPUP_DELAY, background, "images/SlotLoaded.bmp");
						GUI_PlayerSelectionWindow();
						quit = 1;
					}
					MarkSlots(rect_slots[0], red, "slot1.xml");
					MarkSlots(rect_slots[1], red, "slot2.xml");
					MarkSlots(rect_slots[2], red, "slot3.xml");
					MarkSlots(rect_slots[3], red, "slot4.xml");
					MarkSlots(rect_slots[4], red, "slot5.xml");
					MarkSlots(rect_slots[5], red, "slot6.xml");
					MarkSlots(rect_slots[6], red, "slot7.xml");

				}
									   break;
			case (SDL_MOUSEMOTION) :

			default:
				break;
			}
		}
	}

	SDL_FreeSurface(background);
	return;
}
void GUI_SaveGameWindow(){
	SDL_Event e;
	SDL_Rect rect_back = { 600, 500, 200, 100 };
	SDL_Rect rect_slots[SLOTS_NUMBER] = { { 217, 109, 370, 62 }, { 217, 171, 370, 62 }, { 217, 234, 370, 63 }, { 217, 296, 370, 62 },
	{ 217, 356, 370, 59 }, { 217, 415, 370, 61 }, { 217, 476, 370, 75 } };
	SDL_Surface *background = SDL_LoadBMP("images/SaveGameWindow.bmp");
	SDL_Surface *screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	MarkSlots(rect_slots[0], green, "slot1.xml");
	MarkSlots(rect_slots[1], green, "slot2.xml");
	MarkSlots(rect_slots[2], green, "slot3.xml");
	MarkSlots(rect_slots[3], green, "slot4.xml");
	MarkSlots(rect_slots[4], green, "slot5.xml");
	MarkSlots(rect_slots[5], green, "slot6.xml");
	MarkSlots(rect_slots[6], green, "slot7.xml");
	while (!quit) {

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_back.x) && (e.button.x < rect_back.x + rect_back.w) && (e.button.y > rect_back.y) && (e.button.y < rect_back.y + rect_back.h))
				{
					GUI_GameWindow();
					quit = 1;

				}
									   if ((e.button.x > rect_slots[0].x) && (e.button.x < rect_slots[0].x + rect_slots[0].w) && (e.button.y > rect_slots[0].y) && (e.button.y < rect_slots[0].y + rect_slots[0].h))
									   {
										   
										   if (FileExists("slot1.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										 else
											 PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot1.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[1].x) && (e.button.x < rect_slots[1].x + rect_slots[1].w) && (e.button.y > rect_slots[1].y) && (e.button.y < rect_slots[1].y + rect_slots[1].h))
									   {
										   
										   if (FileExists("slot2.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot2.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[2].x) && (e.button.x < rect_slots[2].x + rect_slots[2].w) && (e.button.y > rect_slots[2].y) && (e.button.y < rect_slots[2].y + rect_slots[2].h))
									   {
										   
										   if (FileExists("slot3.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot3.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[3].x) && (e.button.x < rect_slots[3].x + rect_slots[3].w) && (e.button.y > rect_slots[3].y) && (e.button.y < rect_slots[3].y + rect_slots[3].h))
									   {
										   
										   if (FileExists("slot4.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot4.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[4].x) && (e.button.x < rect_slots[4].x + rect_slots[4].w) && (e.button.y > rect_slots[4].y) && (e.button.y < rect_slots[4].y + rect_slots[4].h))
									   {
										   
										   if (FileExists("slot5.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot5.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[5].x) && (e.button.x < rect_slots[5].x + rect_slots[5].w) && (e.button.y > rect_slots[5].y) && (e.button.y < rect_slots[5].y + rect_slots[5].h))
									   {
										   
										   if (FileExists("slot6.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot6.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   if ((e.button.x > rect_slots[6].x) && (e.button.x < rect_slots[6].x + rect_slots[6].w) && (e.button.y > rect_slots[6].y) && (e.button.y < rect_slots[6].y + rect_slots[6].h))
									   {
										   
										   if (FileExists("slot7.xml")) //slot exists
											   PopUp(POPUP_DELAY, background, "images/GameSavedOverRide.bmp");
										   else
											   PopUp(POPUP_DELAY, background, "images/GameSaved.bmp");
										   SaveToFile("slot7.xml");
										   MarkSlots(rect_slots[0], green, "slot1.xml");
										   MarkSlots(rect_slots[1], green, "slot2.xml");
										   MarkSlots(rect_slots[2], green, "slot3.xml");
										   MarkSlots(rect_slots[3], green, "slot4.xml");
										   MarkSlots(rect_slots[4], green, "slot5.xml");
										   MarkSlots(rect_slots[5], green, "slot6.xml");
										   MarkSlots(rect_slots[6], green, "slot7.xml");
									   }
									   break;
			case (SDL_MOUSEMOTION) :

			default:
				break;

			}
		}
	}
	SDL_FreeSurface(background);
	return;
}

void GUI_SetBoardWindow(){
	SDL_Event e;
	SDL_Rect rect_save_and_return = { 595, 18, 200, 76 };
	SDL_Rect rect_w_rook = { 600, 115, 40, 55 };
	SDL_Rect rect_w_queen = { 650, 115, 40, 55 };
	SDL_Rect rect_w_bishop = { 702, 115, 40, 55 };
	SDL_Rect rect_w_pawn = { 600, 184, 40, 55 };
	SDL_Rect rect_w_knight = { 650, 184, 40, 55 };
	SDL_Rect rect_w_king = { 702, 184, 40, 55 };
	SDL_Rect rect_b_rook = { 600, 261, 40, 55 };
	SDL_Rect rect_b_queen = { 650, 261, 40, 55 };
	SDL_Rect rect_b_bishop = { 700, 261, 40, 55 };
	SDL_Rect rect_b_pawn = { 600, 328, 40, 55 };
	SDL_Rect rect_b_knight = { 650, 328, 40, 55 };
	SDL_Rect rect_b_king = { 702, 328, 40, 55 };
	SDL_Rect rect_trash = { 595, 455, 65, 85 };
	SDL_Rect rect_clear = { 675, 473, 100, 50 };
	SDL_Rect imgrect;
	imgrect.h = IMG_H;
	imgrect.w = IMG_H;
	struct Position pos_to_move = { -1, -1 };
	SDL_Surface *background = SDL_LoadBMP("images/SetBoardWindow.bmp");
	SDL_Surface *screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	int quit = 0;
	char piece_to_set = EMPTY;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	/* Create window surface*/

	if (screen == NULL) {
		printf("ERROR: failed to set video mode: %s\n", SDL_GetError());
		return;
	}

	/* Define the rects we need*/


	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

	while (!quit) {
		drawBoard();

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_save_and_return.x) && (e.button.x < rect_save_and_return.x + rect_save_and_return.w) && (e.button.y > rect_save_and_return.y) && (e.button.y < rect_save_and_return.y + rect_save_and_return.h))
				{
					GUI_PlayerSelectionWindow();
					quit = 1;
				}
// w_rook 
				if ((e.button.x > rect_w_rook.x) && (e.button.x < rect_w_rook.x + rect_w_rook.w) && (e.button.y > rect_w_rook.y) && (e.button.y < rect_w_rook.y + rect_w_rook.h))
				{
					refreshToBackground(background);
					piece_to_set = W_ROOK;
				}

// w_king 

				if ((e.button.x > rect_w_king.x) && (e.button.x < rect_w_king.x + rect_w_king.w) && (e.button.y > rect_w_king.y) && (e.button.y < rect_w_king.y + rect_w_king.h))
				{
					refreshToBackground(background);
					piece_to_set = W_KING;
				}

// w_bishop 
				if ((e.button.x > rect_w_bishop.x) && (e.button.x < rect_w_bishop.x + rect_w_bishop.w) && (e.button.y > rect_w_bishop.y) && (e.button.y < rect_w_bishop.y + rect_w_bishop.h))
				{
					refreshToBackground(background);
					piece_to_set = W_BISHOP;
				}

// w_pawn 
				if ((e.button.x > rect_w_pawn.x) && (e.button.x < rect_w_pawn.x + rect_w_pawn.w) && (e.button.y > rect_w_pawn.y) && (e.button.y < rect_w_pawn.y + rect_w_pawn.h))
				{
					refreshToBackground(background);
					piece_to_set = W_PAWN;
				}

// w_knight 

				if ((e.button.x > rect_w_knight.x) && (e.button.x < rect_w_knight.x + rect_w_knight.w) && (e.button.y > rect_w_knight.y) && (e.button.y < rect_w_knight.y + rect_w_knight.h))
				{
					refreshToBackground(background);
					piece_to_set = W_KNIGHT;
				}

// w_queen 
				if ((e.button.x > rect_w_queen.x) && (e.button.x < rect_w_queen.x + rect_w_queen.w) && (e.button.y > rect_w_queen.y) && (e.button.y < rect_w_queen.y + rect_w_queen.h))
				{
					refreshToBackground(background);
					piece_to_set = W_QUEEN;
				}


// b_rook 

				if ((e.button.x > rect_b_rook.x) && (e.button.x < rect_b_rook.x + rect_b_rook.w) && (e.button.y > rect_b_rook.y) && (e.button.y < rect_b_rook.y + rect_b_rook.h))
				{
					refreshToBackground(background);
					piece_to_set = B_ROOK;
				}


// b_king 
				if ((e.button.x > rect_b_king.x) && (e.button.x < rect_b_king.x + rect_b_king.w) && (e.button.y > rect_b_king.y) && (e.button.y < rect_b_king.y + rect_b_king.h))
				{
					refreshToBackground(background);
					piece_to_set = B_KING;
				}


// b_bishop 

				if ((e.button.x > rect_b_bishop.x) && (e.button.x < rect_b_bishop.x + rect_b_bishop.w) && (e.button.y > rect_b_bishop.y) && (e.button.y < rect_b_bishop.y + rect_b_bishop.h))
				{
					refreshToBackground(background);
					piece_to_set = B_BISHOP;
				}


// b_pawn 

				if ((e.button.x > rect_b_pawn.x) && (e.button.x < rect_b_pawn.x + rect_b_pawn.w) && (e.button.y > rect_b_pawn.y) && (e.button.y < rect_b_pawn.y + rect_b_pawn.h))
				{
					refreshToBackground(background);
					piece_to_set = B_PAWN;
				}

// b_knight 
				if ((e.button.x > rect_b_knight.x) && (e.button.x < rect_b_knight.x + rect_b_knight.w) && (e.button.y > rect_b_knight.y) && (e.button.y < rect_b_knight.y + rect_b_knight.h))
				{
					refreshToBackground(background);
					piece_to_set = B_KNIGHT;
				}

// b_queen 
				if ((e.button.x > rect_b_queen.x) && (e.button.x < rect_b_queen.x + rect_b_queen.w) && (e.button.y > rect_b_queen.y) && (e.button.y < rect_b_queen.y + rect_b_queen.h))
				{
					refreshToBackground(background);
					piece_to_set = B_QUEEN;
				}

// boardPiece 

				for (int i = 0; i < 8; i++){
					for (int j = 0; j < 8; j++)
					{
						imgrect.x = 30 + i*IMG_H;
						imgrect.y = 492 - j*IMG_W;
						if ((e.button.x > imgrect.x) && (e.button.x < imgrect.x + imgrect.w) && (e.button.y > imgrect.y) && (e.button.y < imgrect.y + imgrect.h))
						{
							refreshToBackground(background);
							pos_to_move.x = i;
							pos_to_move.y = j;
							if (board[pos_to_move.x][pos_to_move.y]!=EMPTY)
								piece_to_set = board[pos_to_move.x][pos_to_move.y];
						}
					}
				}

				if ((e.button.x > rect_clear.x) && (e.button.x < rect_clear.x + rect_clear.w) && (e.button.y > rect_clear.y) && (e.button.y < rect_clear.y + rect_clear.h))
				{
					clearBoard(board);
					drawBoard();
					refreshToBackground(background);
				}

									   break;
			case (SDL_MOUSEBUTTONUP) :
				if ((e.button.x > rect_trash.x) && (e.button.x < rect_trash.x + rect_trash.w) && (e.button.y > rect_trash.y) && (e.button.y < rect_trash.y + rect_trash.h))
				{
					if (pos_to_move.x != -1){
						removePiece(pos_to_move);
						pos_to_move.x = -1;
						pos_to_move.y = -1;
						refreshToBackground(background);

					}
				}
				
				for (int i = 0; i < 8; i++){
					for (int j = 0; j < 8; j++)
					{
						imgrect.x = 43 + i*IMG_H;
						imgrect.y = 503 - j*IMG_W;

						if ((e.button.x > imgrect.x) && (e.button.x < imgrect.x + imgrect.w) && (e.button.y > imgrect.y) && (e.button.y < imgrect.y + imgrect.h))
						{
							if (piece_to_set != EMPTY){
								if (pos_to_move.x != -1){
									removePiece(pos_to_move);
									pos_to_move.x = -1;
									pos_to_move.y = -1;
									refreshToBackground(background);
								}
								struct Position pos_to_set = { i, j };
								if(!setPiece(pos_to_set, piece_to_set))
									PopUp(POPUP_DELAY, background, "images/InvalidPos.bmp");
								piece_to_set = EMPTY;
							}
						}
					}
				}
				drawBoard();
				break;
			default:
				break;
			}
		}

		/* Wait a little before redrawing*/
		//SDL_Delay(100);
	}

	SDL_FreeSurface(background);
	return;
}
void GUI_AISettingsWindow(){
	SDL_Event e;
	SDL_Rect rect_back = { 600, 500, 200, 100 };
	SDL_Rect diff_1 = { 30, 265, 63, 60 };
	SDL_Rect diff_2 = { 93, 265, 79, 60 };
	SDL_Rect diff_3 = { 172, 265, 71, 60 };
	SDL_Rect diff_4 = { 243, 265, 71, 60 };
	SDL_Rect diff_best = { 314, 265, 116, 60 };
	SDL_Rect user_color_white = { 27, 446, 195, 68 };
	SDL_Rect user_color_black = { 223, 446, 210, 68 };

	SDL_Surface *background = SDL_LoadBMP("images/AISettingsWindow.bmp");
	int quit = 0;
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return;
	}

	/* Create window surface*/
	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (user_color==BLACK)
		MarkRect(user_color_black,orange,0);
	else
		MarkRect(user_color_white, orange,0);
	switch (difficulty){
	case 1:
		MarkRect(diff_1, orange, 0);
		break;
	case 2:
		MarkRect(diff_2, orange, 0);
		break;
	case 3:
		MarkRect(diff_3, orange, 0);
		break;
	case 4:
		MarkRect(diff_4, orange, 0);
		break;
	case DIFF_BEST:
		MarkRect(diff_best, orange, 0);
		break;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

	while (!quit) {

		/* Poll for keyboard & mouse events*/

		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_QUIT) :
				quit = 1;
				break;
			case (SDL_KEYUP) :
				if (e.key.keysym.sym == SDLK_ESCAPE) quit = 1;
				break;
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_back.x) && (e.button.x < rect_back.x + rect_back.w) && (e.button.y > rect_back.y) && (e.button.y < rect_back.y + rect_back.h))
				{
					GUI_PlayerSelectionWindow();
					quit = 1;

				}
				if ((e.button.x > diff_1.x) && (e.button.x < diff_1.x + diff_1.w) && (e.button.y > diff_1.y) && (e.button.y < diff_1.y + diff_1.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					difficulty = 1;

					MarkRect(diff_1, orange, 0);
					if (user_color==WHITE)
						MarkRect(user_color_white, orange, 0);
					else
						MarkRect(user_color_black, orange, 0);
				}
				if ((e.button.x > diff_2.x) && (e.button.x < diff_2.x + diff_2.w) && (e.button.y > diff_2.y) && (e.button.y < diff_2.y + diff_2.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					difficulty = 2;

					MarkRect(diff_2, orange, 0);
					if (user_color==WHITE)
						MarkRect(user_color_white, orange, 0);
					else
						MarkRect(user_color_black, orange, 0);
				}
				if ((e.button.x > diff_3.x) && (e.button.x < diff_3.x + diff_3.w) && (e.button.y > diff_3.y) && (e.button.y < diff_3.y + diff_3.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					difficulty = 3;

					MarkRect(diff_3, orange, 0);
					if (user_color==WHITE)
						MarkRect(user_color_white, orange, 0);
					else
						MarkRect(user_color_black, orange, 0);
				}
				if ((e.button.x > diff_4.x) && (e.button.x < diff_4.x + diff_4.w) && (e.button.y > diff_4.y) && (e.button.y < diff_4.y + diff_4.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					difficulty = 4;

					MarkRect(diff_4, orange, 0);
					if (user_color==WHITE)
						MarkRect(user_color_white, orange, 0);
					else
						MarkRect(user_color_black, orange, 0);
				}
				if ((e.button.x > diff_best.x) && (e.button.x < diff_best.x + diff_best.w) && (e.button.y > diff_best.y) && (e.button.y < diff_best.y + diff_best.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					difficulty = DIFF_BEST;

					MarkRect(diff_best, orange, 0);
					if (user_color==WHITE)
						MarkRect(user_color_white, orange, 0);
					else
						MarkRect(user_color_black, orange, 0);
				}
				if ((e.button.x > user_color_white.x) && (e.button.x < user_color_white.x + user_color_white.w) && (e.button.y > user_color_white.y) && (e.button.y < user_color_white.y + user_color_white.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					user_color = WHITE;
					MarkRect(user_color_white, orange, 0);
					switch (difficulty){
					case 1:
						MarkRect(diff_1, orange, 0);
						break;
					case 2:
						MarkRect(diff_2, orange, 0);
						break;
					case 3:
						MarkRect(diff_3, orange, 0);
						break;
					case 4:
						MarkRect(diff_4, orange,0);
						break;
					case DIFF_BEST:
						MarkRect(diff_best, orange,0);
						break;
					}
				}
				if ((e.button.x > user_color_black.x) && (e.button.x < user_color_black.x + user_color_black.w) && (e.button.y > user_color_black.y) && (e.button.y < user_color_black.y + user_color_black.h))
				{
					if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
						SDL_FreeSurface(background);
						printf("ERROR: failed to blit image: %s\n", SDL_GetError());
						return;
					}
					if (SDL_Flip(screen) != 0) {
						printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
						return;
					}
					user_color = BLACK;
					MarkRect(user_color_black, orange,0);
					switch (difficulty){
					case 1:
						MarkRect(diff_1, orange, 0);
						break;
					case 2:
						MarkRect(diff_2, orange, 0);
						break;
					case 3:
						MarkRect(diff_3, orange, 0);
						break;
					case 4:
						MarkRect(diff_4, orange, 0);
						break;
					case DIFF_BEST:
						MarkRect(diff_best, orange, 0);
						break;
					}
				}
				break;
			case (SDL_MOUSEMOTION) :

			default:
				break;
			}
		}
	}

	SDL_FreeSurface(background);
	return;
}

char GUI_PromotionWindow(){
	SDL_Event e;
	SDL_Rect rect_knight = { 0, 330, 200, 175 };
	SDL_Rect rect_rook = { 215, 345, 150, 160 };
	SDL_Rect rect_queen = { 385, 330, 190, 175 };
	SDL_Rect rect_bishop = { 600, 315, 175, 200 };
	SDL_Surface *background = SDL_LoadBMP("images/PromotionWindow.bmp");
	SDL_Surface *screen = SDL_SetVideoMode(WIN_W, WIN_H, 0, SDL_HWSURFACE | SDL_DOUBLEBUF);
	/* Initialize SDL and make sure it quits*/
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
		return EMPTY;
	}

	/* Load test spritesheet image*/

	if (background == NULL) {
		printf("ERROR: failed to load image: %s\n", SDL_GetError());
		return EMPTY;
	}
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return EMPTY;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return EMPTY;
	}

	/* Poll for keyboard & mouse events*/
	while (1){
		while (SDL_PollEvent(&e) != 0) {
			switch (e.type) {
			case (SDL_MOUSEBUTTONDOWN) :
				if ((e.button.x > rect_knight.x) && (e.button.x < rect_knight.x + rect_knight.w) && (e.button.y > rect_knight.y) && (e.button.y < rect_knight.y + rect_knight.h))
				{
					if (next_player == WHITE)
						promotion = 'n';
					else
						promotion = 'N';
					SDL_FreeSurface(background);
					return promotion;

				}
									   if ((e.button.x > rect_rook.x) && (e.button.x < rect_rook.x + rect_rook.w) && (e.button.y > rect_rook.y) && (e.button.y < rect_rook.y + rect_rook.h))
									   {
										   if (next_player == WHITE)
											   promotion = 'r';
										   else
											   promotion = 'R';
										   SDL_FreeSurface(background);
										   return promotion;
									   }
									   
									   if ((e.button.x > rect_queen.x) && (e.button.x < rect_queen.x + rect_queen.w) && (e.button.y > rect_queen.y) && (e.button.y < rect_queen.y + rect_queen.h))
									   {
										   if (next_player == WHITE)
											   promotion = 'q';
										   else
											   promotion = 'Q';
										   SDL_FreeSurface(background);
										   return promotion;

									   }
									   if ((e.button.x > rect_bishop.x) && (e.button.x < rect_bishop.x + rect_bishop.w) && (e.button.y > rect_bishop.y) && (e.button.y < rect_bishop.y + rect_bishop.h))
									   {
										   if (next_player == WHITE)
											   promotion = 'b';
										   else
											   promotion = 'B';
										   SDL_FreeSurface(background);
										   return promotion;

									   }
			}
		}
	}
}

void drawBoard(){
	char* pieceBitmap;
	SDL_Rect imgrect;
	for (int i = 0; i < 8; i++){
			for (int j = 0; j < 8; j++)
			{
				pieceBitmap = getPieceBitmapFromBoard(board[i][j]);
				if (!strcmp(pieceBitmap, "empty"))
					continue;
				SDL_Surface *piece = SDL_LoadBMP(pieceBitmap);
				imgrect.x = 43 + i*IMG_H;
				imgrect.y = 503 - j*IMG_W;
				if (SDL_BlitSurface(piece, NULL, screen, &imgrect) != 0) {
					SDL_FreeSurface(piece);
					printf("ERROR: failed to blit image: %s\n", SDL_GetError());
					return ;
				}
				/* We finished drawing*/
				if (SDL_Flip(screen) != 0) {
					printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
					return ;
				}
			}
		}
}
void drawNextPlayer(SDL_Surface* background){
	SDL_Rect rect_n_player = { 595, 401, 200, 40};
	SDL_Rect rect_comp_thinking = { 593, 221, 200,40 };
	SDL_Surface *n_player;
	SDL_Surface *comp_think=NULL;
	if (next_player == WHITE){
		n_player = SDL_LoadBMP("images/WhiteTurn.bmp");
	}
	else{
		n_player = SDL_LoadBMP("images/BlackTurn.bmp");
	}
	if (game_mode == MODE_PLAYER_VS_AI && difficulty>1){ // show this "computer thinking" popup only on long-time minimax depth
		if (!user_color == next_player){
			comp_think = SDL_LoadBMP("images/CompThink.bmp");
		}
		else{
			comp_think = NULL;
		//	refreshToBackground(background);
		}
	}
	if (SDL_BlitSurface(n_player, NULL, screen, &rect_n_player) != 0) {
		SDL_FreeSurface(n_player);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	/* We finished drawing*/
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	if (comp_think != NULL){
		if (SDL_BlitSurface(comp_think, NULL, screen, &rect_comp_thinking) != 0) {
			SDL_FreeSurface(comp_think);
			printf("ERROR: failed to blit image: %s\n", SDL_GetError());
			return;
		}
		/* We finished drawing*/
		if (SDL_Flip(screen) != 0) {
			printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
			return;
		}
	}
}
void refreshToBackground(SDL_Surface* background){
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
}
void MarkRect(SDL_Rect rect_to_mark,SDL_Color col,int cent_or_strech){
	SDL_Surface *mark_area=NULL;
	if (!cent_or_strech){
		rect_to_mark.x = rect_to_mark.x + rect_to_mark.w / 4;
		rect_to_mark.y = rect_to_mark.y + rect_to_mark.h / 4;
		
		mark_area = SDL_CreateRGBSurface(0, rect_to_mark.w / 2, rect_to_mark.h / 2, 32, 0, 0, 0, 0);
		SDL_FillRect(mark_area, NULL, SDL_MapRGB(mark_area->format, col.r, col.g, col.b));
		SDL_SetAlpha(mark_area, SDL_SRCALPHA, ALPHA);
	}
	else{
		mark_area = SDL_CreateRGBSurface(0, rect_to_mark.w, rect_to_mark.h, 11, 0, 0, 0, 0);
		SDL_FillRect(mark_area, NULL, SDL_MapRGB(mark_area->format, col.r, col.g, col.b));
		SDL_SetAlpha(mark_area, SDL_SRCALPHA, ALPHA+80);
	}



	if (SDL_BlitSurface(mark_area, NULL, screen, &rect_to_mark) != 0) {
		SDL_FreeSurface(mark_area);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
}
void setDeafultSettings(){
	game_mode = MODE_2PLAYERS;
	difficulty = 1;
	next_player = WHITE;
	user_color = WHITE;
}
void PopUp(int delay, SDL_Surface *background, char* popUpBitmap){
	SDL_Rect rect_popup = { 600, 100, 200, 100 };
	SDL_Surface *popUp = SDL_LoadBMP(popUpBitmap);
	if (SDL_BlitSurface(popUp, NULL, screen, &rect_popup) != 0) {
		SDL_FreeSurface(popUp);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	/* We finished drawing*/
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	SDL_Delay(delay);
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	/* We finished drawing*/
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

}

void Popup_Message(int delay, SDL_Surface *background, char* popUpBitmap){
	SDL_Rect rect_popup = { 200, 100, 200, 100 };
	SDL_Surface *popUp = SDL_LoadBMP(popUpBitmap);
	if (SDL_BlitSurface(popUp, NULL, screen, &rect_popup) != 0) {
		SDL_FreeSurface(popUp);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	/* We finished drawing*/
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}
	SDL_Delay(delay);
	if (SDL_BlitSurface(background, NULL, screen, NULL) != 0) {
		SDL_FreeSurface(background);
		printf("ERROR: failed to blit image: %s\n", SDL_GetError());
		return;
	}
	/* We finished drawing*/
	if (SDL_Flip(screen) != 0) {
		printf("ERROR: failed to flip buffer: %s\n", SDL_GetError());
		return;
	}

}
char* getPieceBitmapFromBoard(char piece){
	switch (piece){
	case W_PAWN:
		return "images/w_pawn.bmp";
	case W_BISHOP:
		return "images/w_bishop.bmp";
	case W_ROOK:
		return "images/w_rook.bmp";
	case W_KNIGHT:
		return "images/w_knight.bmp";
	case W_QUEEN:
		return "images/w_queen.bmp";
	case W_KING:
		return "images/w_king.bmp";
	case B_PAWN:
		return "images/b_pawn.bmp";
	case B_BISHOP:
		return "images/b_bishop.bmp";
	case B_ROOK:
		return "images/b_rook.bmp";
	case B_KNIGHT:
		return "images/b_knight.bmp";
	case B_QUEEN:
		return "images/b_queen.bmp";
	case B_KING:
		return "images/b_king.bmp";
	case EMPTY:
		return "empty";
	}
	return "empty";
}

void showBestMove(int diff_wanted, SDL_Rect rect_src_best_move, SDL_Rect rect_dst_best_move, SDL_Surface* background){
	int old_difficulty = difficulty;
	difficulty = diff_wanted;
	alphaBeta(next_player, board, INT_MIN, INT_MAX, difficulty,1);
	if (move_to_do != NULL&& is_allocd_move_to_do){
		rect_src_best_move.x = 31 + move_to_do->src.x*IMG_H;
		rect_src_best_move.y = 497 - move_to_do->src.y*IMG_W;

		rect_dst_best_move.x = 31 + move_to_do->dst.x*IMG_H;
		rect_dst_best_move.y = 497 - move_to_do->dst.y*IMG_W;

		free(move_to_do);
		refreshToBackground(background);
		drawBoard();
		MarkRect(rect_src_best_move, blue, 1);
		MarkRect(rect_dst_best_move, yellow, 1);
		is_allocd_move_to_do = 0;
	}
	difficulty = old_difficulty;
}

void MarkSlots(SDL_Rect rect_slot,SDL_Color c1, char* fileName){
	if (!FileExists(fileName)){ //slot doesn't exists
		MarkRect(rect_slot, c1, 1);
	}
}