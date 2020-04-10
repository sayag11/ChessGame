#include "Chess_GUI.h"

int console_or_gui; // default mode is console

int main(int argc, char* args[])
{
	//console_or_gui = 0;
	//if (args[1]!=NULL){
	//	//chessprog is set to 0 for console mode 1 for console mode
	//	if (!strcmp("console", args[1]))
	//		console_or_gui = 0;
	//	else{
	//		if (!strcmp("gui", args[1]))
	//			console_or_gui = 1;
	//	}
	//}
	//if (console_or_gui)
	//	GUI_Main();
	//else
	//	CommandLine();
	GUI_Main();
	return 0;
}