all: Game

Game: Game.o Cell.o ResourceManager.o
	g++ -o Game Game.o Cell.o ResourceManager.o -lsfml-graphics -lsfml-window -lsfml-system -Werror -Wall

Game.o: Game.cpp Game.h
	g++ -c Game.cpp -Werror -Wall

Cell.o: Cell.cpp Cell.h
	g++ -c Cell.cpp -Werror -Wall

ResourceManager.o: ResourceManager.cpp ResourceManager.h
	g++ -c ResourceManager.cpp -Werror -Wall
