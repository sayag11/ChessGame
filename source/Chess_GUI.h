#include "SDL.h"
#include "SDL_video.h"
#include "Chess.h"

extern SDL_Surface *screen;

// GUI Functions:
void GUI_GameWindow();
void GUI_MainWindow();
void GUI_PlayerSelectionWindow();
void GUI_LoadGameWindow();
void GUI_SaveGameWindow();
void GUI_SetBoardWindow();
void GUI_AISettingsWindow();
char GUI_PromotionWindow();
void drawBoard();
void drawNextPlayer();
char* getPieceBitmapFromBoard(char piece);
void GUI_Main();
void MarkRect(SDL_Rect rect_to_mark, SDL_Color col, int cent_or_strech);
void PopUp(int delay, SDL_Surface *background, char* popUpBitmap);
void setDeafultSettings();
void refreshToBackground(SDL_Surface* background);
void Popup_Message(int delay, SDL_Surface *background, char* popUpBitmap);
void showBestMove(int diff_wanted, SDL_Rect rect_src_best_move, SDL_Rect rect_dst_best_move, SDL_Surface* background);
void MarkSlots(SDL_Rect rect_slot, SDL_Color c1, char* fileName);