#include "Game.h"

Game::Game() {
  _gameState = Uninitialized;
  _mineCountDisplay.setPosition(4.0, 52.0);
  _mineCountDisplay.setNumber(_cellGrid.getMinesLeft());
  _mineCountDisplay.updateDisplay();
  _timeDisplay.setPosition(326.0,52.0);
  _face.setPosition(182, 52);
  _face.updateDisplay();
  _showSolution.initShowSolution();
  _showSolution.setPosition(360, 2);
  _showSolution.updateDisplay();
  _cellGrid.setPosition(2, 100);
}

Game::~Game() {
  // for (int i = 0; i < _rows; i++)
  //   delete _cells[i];
}

void Game::updateDifficulty(Game::Difficulty difficulty) {
  _difficulty = difficulty;
  if (_difficulty == Game::Beginner) {
  
  } else if (_difficulty == Game::Intermediate) {
  
  } else if (_difficulty == Game::Expert) {
  
  } else if (_difficulty == Game::Custom) {
  
  } 

}

void Game::initializeResources() {
  getTexture("MineSweeperImages/DefaultCell.png");
  getTexture("MineSweeperImages/EmptyCell.png");
  getTexture("MineSweeperImages/QuestionCell.png");
  getTexture("MineSweeperImages/FlagCell.png");
  getTexture("MineSweeperImages/MineClickedCell.png");
  getTexture("MineSweeperImages/MineCorrectCell.png");
  getTexture("MineSweeperImages/MineMissedCell.png");
  getTexture("MineSweeperImages/1Cell.png");
  getTexture("MineSweeperImages/2Cell.png");
  getTexture("MineSweeperImages/3Cell.png");
  getTexture("MineSweeperImages/4Cell.png");
  getTexture("MineSweeperImages/5Cell.png");
  getTexture("MineSweeperImages/6Cell.png");
  getTexture("MineSweeperImages/7Cell.png");
  getTexture("MineSweeperImages/8Cell.png");
  getTexture("MineSweeperImages/Smiley.png");
  getTexture("MineSweeperImages/SmileyDead.png");
  getTexture("MineSweeperImages/SmileyGlasses.png");
}


void Game::Start()
{
  if(_gameState != Uninitialized)
    return;
 
  // Specifying style to prevent resizing of the window
  _mainWindow.create(sf::VideoMode(404,502,32),"Sweeping Mines", sf::Style::Titlebar | sf::Style::Close);
  _gameState = Game::Playing;

  // Pre-load textures to the resource manager
  initializeResources();

  _cellGrid.initCells();

  // Draw all sprites (OW game loop waits for an event to draw all)
  drawAllSprites();

  while(!IsExiting())
    GameLoop();

  _mainWindow.close();
}

void Game::drawAllSprites() {
  _mainWindow.clear(sf::Color(220,220,220));  // grey
  _cellGrid.drawAllSprites(_mainWindow);
  _showSolution.drawAllSprites(_mainWindow);
  _face.drawAllSprites(_mainWindow);
  _mineCountDisplay.drawAllSprites(_mainWindow);
  _timeDisplay.drawAllSprites(_mainWindow);
  _mainWindow.display();
}

bool Game::IsExiting()
{
  if(_gameState == Game::Exiting) 
    return true;
  else 
    return false;
}

void Game::GameLoop()
{
  // making sure timer works even without events (also gets updated with the rest of sprites)
  drawAllSprites();
  
  sf::Event currentEvent;
  while(_mainWindow.pollEvent(currentEvent))
  {
    if(currentEvent.type == sf::Event::Closed)
      _gameState = Game::Exiting;
  
    switch(_gameState)
    {
      case Game::Playing:
        {
          if (currentEvent.type == sf::Event::MouseButtonReleased) {
            if (currentEvent.mouseButton.button == sf::Mouse::Left) {
            	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            	// std::cout << "Left: x = " << position.x << "\ty = " << position.y << "\n";
              if (_cellGrid.isBeingClicked(position)) {
                  handleReturnCode(_cellGrid.clicked(position));
              } else if (position.x >= 360 && position.x < 400 && position.y >= 2 && position.y <= 42) { 
                  handleReturnCode(_showSolution.clicked());
              } else if (_face.isBeingClicked(position)) {
                  handleReturnCode(_face.clicked());
              }
    				}
            
            // Cyle right click options on cells: Flag > Question > Default...
         		if (currentEvent.mouseButton.button == sf::Mouse::Right) {
            	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            	// std::cout << "Right: x = " << position.x << "\ty = " << position.y << "\n";
              if (_cellGrid.isBeingClicked(position)) {
                  handleReturnCode(_cellGrid.rightClicked(position));
              }
    				} 
          }
          
          drawAllSprites();
        }
      case Game::Revealed:
        if (currentEvent.type == sf::Event::MouseButtonReleased) {
          if (currentEvent.mouseButton.button == sf::Mouse::Left) {
          	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            if (_face.isBeingClicked(position))
              handleReturnCode(_face.clicked());
    			}
        }
        drawAllSprites();
        break;
      default:
        break;
    }
  }
}

// 0 - no message back from cell
// 1 - number cell revealed
// 2 - show solution
// 4 - mine was clicked... boom 
// 8 - empty cell -> also click all neighbor empty cells
// 16 - reset
// 32 - flag placed
// 64 - flag removed
// 128 - winner!
//
// Usage - add values from above to create an int. This number will
// be run through a mask to parse the indidual components
void Game::handleReturnCode(int rc) {
  // Empty Cell and neighbors revealed (includes bordering number cells) 
  if (rc & 8) {
    _timeDisplay.startTimer();
  }

  // Number cell revealed 
  if (rc & 1) {
    _timeDisplay.startTimer();
  }
 
  // Winner!
  if (rc & 128) {
    _face.win();
    _gameState = Game::Revealed;
    _cellGrid.showSolution(true);
    _timeDisplay.stopTimer();
  }

  // Show solution (could be force reveal or winner)
  if (rc & 2) {
    _gameState = Game::Revealed;
    _cellGrid.showSolution(false);
    _timeDisplay.stopTimer();
  }

  // Boom
  if (rc & 4) {
    _gameState = Game::Revealed;
    _face.loss();
    _cellGrid.showSolution(false);
    _timeDisplay.stopTimer();
  }

  // Restart (not in the game loop to make sure we don't have event hang)
  if (rc & 16) {
    _cellGrid.reset(); 
    _face.reset();
    _mineCountDisplay.setNumber(_cellGrid.getMinesLeft());
    _timeDisplay.resetTimer();
    _gameState = Game::Playing;
  }
  
  // Flag paced
  if (rc & 32) {
    _mineCountDisplay.setNumber(_cellGrid.getMinesLeft());
    _timeDisplay.startTimer();
  }

  // Flag removed (don't worry about timer, it's already going if we're removing)
  if (rc & 64)
    _mineCountDisplay.setNumber(_cellGrid.getMinesLeft());
}

int main(int argc, char** argv) {
  Game game;
  game.Start();
  return 0;
}
