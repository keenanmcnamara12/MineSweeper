#ifndef GAME_H
#define GAME_H

#pragma once
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include <stdio.h>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <iostream>
#include <cassert>
#include <sstream>
#include <ctime>
#include <string>
#include "Cell.h"
#include "Score.h"

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;
sf::Texture* getTexture(std::string);

class Game {
	public:
    Game();
    ~Game();
	  void Start();
  
  public:
    enum GameState { Uninitialized, Playing, Revealed, Restarting, Exiting };
	
	private:
	  bool IsExiting();
	  void GameLoop();
    void initializeCells();
    void initializeResources();
    void drawAllSprites();
    void handleReturnCode(int code, int i, int j);
    void clickNeighborEmpties(int i, int j);
    void clickNeighborEmptiesRecur(int** visited, int i, int j, int originI, int originJ);
	 
  private:
	  GameState _gameState;
	  sf::RenderWindow _mainWindow;
    int _rows;
    int _cols;
    int _totalCells;
    int _cellsRevealed;
    int _mines;
    int _minesLeft;
    Cell **_cells;
    Cell _showSolution;
    Cell _face;
    Score _mineCountDisplay;
    Score _timeDisplay;
    bool _timerStarted;
};

#endif // GAME_H
