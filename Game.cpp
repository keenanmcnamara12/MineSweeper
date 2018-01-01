#include "Game.h"

Game::Game() {
  _rows = 10;
  _cols = 10;
  _totalCells = _rows * _cols;
  _cellsRevealed = 0;
  _mines = 10;
  _minesLeft = _mines;
  _cells = new Cell*[_rows];
  for (int i = 0; i < _rows; i++) {
    _cells[i] = new Cell[_cols];
  }
  _gameState = Uninitialized;
  _mineCountDisplay.setPosition(4.0, 52.0);
  _mineCountDisplay.setNumber(_minesLeft);
  _mineCountDisplay.updateDisplay();
  _timeDisplay.setPosition(326.0,52.0);
  _face.setPosition(182, 52);
  _face.updateDisplay();
  _showSolution.initShowSolution();
  _showSolution.setPosition(360, 2);
  _showSolution.updateDisplay();
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

  // Initialize cells (mines and number)
  initializeCells();

  // Initialize cells (display)
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
	    _cells[i][j].setPosition(i*40 + 2, j*40 + 100);	 // upper left corner
    } 
  }

  // Draw all sprites (OW game loop waits for an event to draw all)
  drawAllSprites();

  while(!IsExiting())
    GameLoop();

  _mainWindow.close();
}

void Game::drawAllSprites() {
  _mainWindow.clear(sf::Color(220,220,220));  // grey
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _cells[i][j].drawAllSprites(_mainWindow);
    } 
  }
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

bool Game::isCellBeingClicked(sf::Vector2i position) {
  if (position.x < 2)
    return false;
  if (position.x > 2 + (_cols * 40))
    return false;
  if (position.y < 100)
    return false;
  if (position.y > 100 + (_rows * 40))
    return false;
  return true;
}

int Game::getCellRowIndex(sf::Vector2i position) {
  return (position.x - 2) / 40;
}

int Game::getCellColIndex(sf::Vector2i position) {
  return (position.y - 100) / 40;
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
              if (isCellBeingClicked(position)) {
                  int i = getCellRowIndex(position);
                  int j = getCellColIndex(position);
                  handleReturnCode(_cells[i][j].clicked(), i, j);
              } else if (position.x >= 360 && position.x < 400 && position.y >= 2 && position.y <= 42) { 
                  handleReturnCode(_showSolution.clicked(), -1, -1);
              } else if (_face.isBeingClicked(position)) {
                  handleReturnCode(_face.clicked(), -1, -1);
              }
    				}
            
            // Cyle right click options on cells: Flag > Question > Default...
         		if (currentEvent.mouseButton.button == sf::Mouse::Right) {
            	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            	// std::cout << "Right: x = " << position.x << "\ty = " << position.y << "\n";
              if (isCellBeingClicked(position)) {
                  int i = getCellRowIndex(position);
                  int j = getCellColIndex(position);
                  handleReturnCode(_cells[i][j].rightClicked(), i, j);
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
              handleReturnCode(_face.clicked(), -1, -1);
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
//
// Usage - add values from above to create an int. This number will
// be run through a mask to parse the indidual components
void Game::handleReturnCode(int rc, int i, int j) {
  // Empty Cell and neighbors revealed (includes bordering number cells) 
  if (rc & 8) {
    clickNeighborEmpties(i, j);
    _timeDisplay.startTimer();
  }

  // Number cell revealed 
  if (rc & 1) {
    _cellsRevealed++;
    _timeDisplay.startTimer();
  }
  
  if (_totalCells - _mines == _cellsRevealed) {
    // Winner!
    _face.win();
    rc += 2;  // we have a winner, leverage "show solution" case to reveal
  }

  // Show solution (could be force reveal or winner)
  if (rc & 2) {
    _gameState = Game::Revealed;
    for (int k = 0; k < _rows; k++) {
      for (int m = 0; m < _cols; m++) {
        _cells[k][m].reveal(false, true);
      } 
    }
    _timeDisplay.stopTimer();
  }

  // Boom
  if (rc & 4) {
    _gameState = Game::Revealed;
    _face.loss();
    for (int k = 0; k < _rows; k++) {
      for (int m = 0; m < _cols; m++) {
        _cells[k][m].reveal(false, false);
      } 
    }
    
    _timeDisplay.stopTimer();
  }

  // Restart (not in the game loop to make sure we don't have event hang)
  if (rc & 16) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
	      _cells[i][j].reset();
      } 
    }
    
    _face.reset();

    _minesLeft = _mines;
    _mineCountDisplay.setNumber(_minesLeft);
  
    _timeDisplay.resetTimer();
    
    initializeCells();
    _cellsRevealed = 0;

    _gameState = Game::Playing;
    drawAllSprites();
  }
  
  // Flag paced
  if (rc & 32) {
    _minesLeft--;
    _mineCountDisplay.setNumber(_minesLeft);
    _timeDisplay.startTimer();
  }

  // Flag removed
  if (rc & 64) {
    _minesLeft++;
    _mineCountDisplay.setNumber(_minesLeft);
  } 
}

void Game::clickNeighborEmpties(int i, int j) {
  int **visited;
  visited = new int*[_rows];
  for (int k = 0; k < _rows; k++) {
    visited[k] = new int[_cols];
  }
  for (int k = 0; k < _rows; k++) {
    for (int m = 0; m < _cols; m++) {
      visited[k][m] = 0; 
    } 
  }
  clickNeighborEmptiesRecur(visited, i, j, i, j);
  for (int k = 0; k < _rows; k++)
    delete visited[k];
  delete visited;
}

void Game::clickNeighborEmptiesRecur(int** visited, int i, int j, int originI, int originJ) {
  // Base case - off the board
  if (!validRowColIndex(i, j))
    return;
  
  // Base case - cell already visited
  if (visited[i][j] == 1)
    return;
  
  // Base case - cell already revealed (and not the origin cell)
  if (_cells[i][j].isRevealed() && !((i == originI) && (j == originJ)))
    return;

  // Mark cell as visited (assuming it's not off limits)
  visited[i][j] = 1;
  
  // Whether it's an empty cell or not we want to make sure it was clicked
  // because we want the bordering number cells that surround the empty
  // grouping to be clicked when an empty region is found.
  _cells[i][j].clicked(); 

  // Increment cells reveal count since we aren't calling to return code
  // handler to prevent extra recursions
  _cellsRevealed++;
  
  // Base case - not an empty cell
  if (!_cells[i][j].isEmpty())
    return;
  
  clickNeighborEmptiesRecur(visited, i-1, j-1, originI, originJ);
  clickNeighborEmptiesRecur(visited, i-1, j, originI, originJ);
  clickNeighborEmptiesRecur(visited, i-1, j+1, originI, originJ);
  clickNeighborEmptiesRecur(visited, i, j-1, originI, originJ);
  clickNeighborEmptiesRecur(visited, i, j+1, originI, originJ);
  clickNeighborEmptiesRecur(visited, i+1, j-1, originI, originJ);
  clickNeighborEmptiesRecur(visited, i+1, j, originI, originJ);
  clickNeighborEmptiesRecur(visited, i+1, j+1, originI, originJ);
}

bool Game::validRowColIndex(int i, int j) {
  if (i < _rows && i >= 0 && j < _cols && j >= 0)
    return true;
  return false;
}

int Game::isMineWrapper(int i, int j) {
  if (!validRowColIndex(i, j))
    return 0;
  return _cells[i][j].isMine();
}

void Game::initializeCells() {
    srand(time(NULL));

    int cellsSize = _rows * _cols;
    int mineCount = _mines;
    int i = 0;
    int j = 0;
    
    // allocate mines at random
    while (mineCount > 0) {
      if (std::rand() % cellsSize == 0 && _cells[i][j].isMine() != true) {
        _cells[i][j].initMine();
        mineCount--;
      }
      if (i == _rows-1 && j == _cols-1) {
          i = 0;
          j = 0;
      } else if (j == _cols-1) {
          i++;
          j = 0;
      } else {
          j++; 
      }
    }

    int surroundingMineCount;
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
        if (_cells[i][j].isMine())
          continue;
        surroundingMineCount = 0;
        surroundingMineCount += isMineWrapper(i - 1, j - 1);
        surroundingMineCount += isMineWrapper(i - 1, j);
        surroundingMineCount += isMineWrapper(i - 1, j + 1);
        surroundingMineCount += isMineWrapper(i, j - 1);
        surroundingMineCount += isMineWrapper(i, j + 1);
        surroundingMineCount += isMineWrapper(i + 1, j - 1);
        surroundingMineCount += isMineWrapper(i + 1, j);
        surroundingMineCount += isMineWrapper(i + 1, j + 1);
        _cells[i][j].setSurroundingMineCount(surroundingMineCount);
      } 
    }
}

int main(int argc, char** argv) {
  Game game;
  game.Start();
  return 0;
}
