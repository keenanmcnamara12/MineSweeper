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
#include "Textures.h"
#include "Face.h"

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace std;

class Game {
	public:
    Game();
    ~Game();
	  void Start();
  
  public:
    enum GameState { Uninitialized, Playing, Revealed, Restarting, Exiting };
    enum Difficulty {Beginner, Intermediate, Expert, Custom};
	
	private:
	  bool IsExiting();
	  void GameLoop();
    void initializeCells();
    void initializeResources();
    void drawAllSprites();
    void handleReturnCode(int code, int i, int j);
    void clickNeighborEmpties(int i, int j);
    void clickNeighborEmptiesRecur(int** visited, int i, int j, int originI, int originJ);
    bool validRowColIndex(int i, int j);
    int isMineWrapper(int i, int j);
    void updateDifficulty(Game::Difficulty difficulty);
    bool isCellBeingClicked(sf::Vector2i position);
    int getCellRowIndex(sf::Vector2i position);
    int getCellColIndex(sf::Vector2i position);
	 
  private:
	  GameState _gameState;
    Difficulty _difficulty;
	  sf::RenderWindow _mainWindow;
    int _rows;
    int _cols;
    int _totalCells;
    int _cellsRevealed;
    int _mines;
    int _minesLeft;
    Cell **_cells;
    Cell _showSolution;
    Face _face;
    Score _mineCountDisplay;
    Score _timeDisplay;
};

#endif // GAME_H
