all: Game

Game: Game.o Cell.o Score.o Textures.o Face.o CellGrid.o
	g++ -o Game Game.o Cell.o Score.o Textures.o Face.o CellGrid.o -lsfml-graphics -lsfml-window -lsfml-system -Werror -Wall

Game.o: Game.cpp Game.h
	g++ -c Game.cpp -Werror -Wall

Cell.o: Cell.cpp Cell.h
	g++ -c Cell.cpp -Werror -Wall

Score.o: Score.cpp Score.h
	g++ -c Score.cpp -Werror -Wall

Textures.o: Textures.cpp Textures.h
	g++ -c Textures.cpp -Werror -Wall

Face.o: Face.cpp Face.h
	g++ -c Face.cpp -Werror -Wall

CellGrid.o: CellGrid.cpp CellGrid.h
	g++ -c CellGrid.cpp -Werror -Wall
