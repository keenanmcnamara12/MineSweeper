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
#include "CellGrid.h"

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
    void handleReturnCode(int code);
    void updateDifficulty(Game::Difficulty difficulty);
	 
  private:
	  GameState _gameState;
    Difficulty _difficulty;
	  sf::RenderWindow _mainWindow;
    CellGrid _cellGrid;
    Cell _showSolution;
    Face _face;
    Score _mineCountDisplay;
    Score _timeDisplay;
};

#endif // GAME_H
