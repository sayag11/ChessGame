TARGET:= chessprog
O_FILES:= Chess_Main.o Chess_CommandLine.o Chess_GUI.o Chess_Logic.o Minimax.o SettingState.o

all: $(TARGET)

$(TARGET): $(O_FILES)
	gcc  -o $(TARGET) $(O_FILES) -lm -std=c99 -pedantic-errors -g  `sdl-config --libs`

Chess_Main.o: Chess_Main.c Chess.h Chess_GUI.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Chess_Main.c `sdl-config --cflags`

Chess_CommandLine.o: Chess_CommandLine.c Chess.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Chess_CommandLine.c
 
Chess_GUI.o: Chess_GUI.c Chess.h Chess_GUI.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Chess_GUI.c `sdl-config --cflags`

Chess_Logic.o: Chess_Logic.c Chess.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Chess_Logic.c

Minimax.o: Minimax.c Chess.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm Minimax.c

SettingState.o: SettingState.c Chess.h
	gcc  -std=c99 -pedantic-errors -c -Wall -g -lm SettingState.c



clean:
	-rm $(O_FILES) $(TARGET)