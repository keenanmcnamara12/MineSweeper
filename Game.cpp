#include "Game.h"

std::map<std::string, sf::Texture*> textureGlobal;

Game::Game() {
  _rows = 10;
  _cols = 10;
  _totalCells = _rows * _cols;
  _cellsRevealed = 0;
  _mines = 10;
  _cells = new Cell*[_rows];
  for (int i = 0; i < _rows; i++) {
    _cells[i] = new Cell[_cols];
  }
  _gameState = Uninitialized;
}

Game::~Game() {
  // for (int i = 0; i < _rows; i++)
  //   delete _cells[i];
  // delete _cells;
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

sf::Texture* getTexture(std::string file) {
  map<string, sf::Texture*>::iterator a = textureGlobal.find(file);
  
  // If the texture is already loaded in the map, return it
  if (a != textureGlobal.end())
    return a->second;
  
  sf::Texture *texture = new sf::Texture();
  if (texture->loadFromFile(file)) {
    textureGlobal[file] = texture;
    return textureGlobal[file];
  } 
  
  delete texture; 
  std::cout << "Error: Loading from file\n";
  return textureGlobal[file];
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
	    _cells[i][j]._sprite.setPosition(i*40 + 2, j*40 + 100);	 // upper left corner
    } 
  }
  // Initialize reset button
  _showSolution._trueState = Cell::ShowSolution;
  _showSolution._sprite.setPosition(360, 2);
  _showSolution.updateDisplay();

  // Initialize the face
  _face._trueState = Cell::Face;
  _face._visibleState = Cell::Face;
  _face._sprite.setPosition(182, 52);
  _face.updateDisplay();

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
      _mainWindow.draw(_cells[i][j]._sprite);
    } 
  }
  _mainWindow.draw(_showSolution._sprite);
  _mainWindow.draw(_face._sprite);
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
            	std::cout << "Left: x = " << position.x << "\ty = " << position.y << "\n";
              // find button that was clicked an update that sprite
              int i = (position.x - 2) / 40;
              int j = (position.y - 100) / 40;
              // When you click in the 40 pixels above the top row, even you'll have -12/40 = 0 (for example)
              if (position.y < 100)
                j = -1;
              if (i < _rows && i >= 0 && j < _cols && j >= 0)
                handleReturnCode(_cells[i][j].clicked(), i, j);
              else if (position.x >= 360 && position.x < 400 && position.y >= 2 && position.y <= 42)
                handleReturnCode(_showSolution.clicked(), -1, -1);
              else if (position.x >= 182 && position.x < 222 && position.y >= 52 && position.y <= 92)
                handleReturnCode(_face.clicked(), -1, -1);
    				}
            
            // Cyle right click options on cells: Flag > Question > Default...
         		if (currentEvent.mouseButton.button == sf::Mouse::Right) {
            	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            	// std::cout << "Right: x = " << position.x << "\ty = " << position.y << "\n";
              int i = (position.x - 2) / 40;
              int j = (position.y - 100) / 40;
              if (i < _rows && i >= 0 && j < _cols && j >= 0)
                handleReturnCode(_cells[i][j].rightClicked(), i, j);
    				} 
          }
          
          drawAllSprites();
        }
      case Game::Revealed:
        if (currentEvent.type == sf::Event::MouseButtonReleased) {
          if (currentEvent.mouseButton.button == sf::Mouse::Left) {
          	sf::Vector2i position = sf::Mouse::getPosition(_mainWindow);
            if (position.x >= 182 && position.x < 222 && position.y >= 52 && position.y <= 92)
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
// 
// Usage - add values from above to create an int. This number will
// be run through a mask to parse the indidual components
void Game::handleReturnCode(int rc, int i, int j) {
  // Empty Cell and neighbors revealed (includes bordering number cells) 
  if (rc & 8)
    clickNeighborEmpties(i, j);

  // Number cell revealed 
  if (rc & 1)
    _cellsRevealed++;
  
  std::cout << "_cellsRevealed = " << _cellsRevealed << "\n";
  if (_totalCells - _mines == _cellsRevealed) {
    // Winner!
    _face._visibleState = Cell::FaceWin;
    _face._trueState = Cell::FaceWin;
    _face.updateDisplay();
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
  }

  // Boom
  if (rc & 4) {
    _gameState = Game::Revealed;
    _face._visibleState = Cell::FaceLoss;
    _face._trueState = Cell::FaceLoss;    // need to keep in sync for Cell::click to work
    _face.updateDisplay();
    for (int k = 0; k < _rows; k++) {
      for (int m = 0; m < _cols; m++) {
        _cells[k][m].reveal(false, false);
      } 
    }
  }

  // Restart (not in the game loop to make sure we don't have event hang)
  if (rc & 16) {
    for (int i = 0; i < _rows; i++) {
      for (int j = 0; j < _cols; j++) {
	      _cells[i][j].reset();
      } 
    }
    
    _face._visibleState = Cell::Face;
    _face._trueState = Cell::Face;    // need to keep in sync for Cell::click to work
    _face.updateDisplay();
    
    initializeCells();
    _cellsRevealed = 0;

    _gameState = Game::Playing;
    drawAllSprites();
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
  if (i < 0 || i >= _rows || j < 0 || j >= _cols)
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

void Game::initializeCells() {
    srand(time(NULL));

    int cellsSize = _rows * _cols;
    int mineCount = _mines;
    int i = 0;
    int j = 0;
    
    // allocate mines at random
    while (mineCount > 0) {
      if (std::rand() % cellsSize == 0 && _cells[i][j].isMine() != true) {
        _cells[i][j]._trueState = Cell::Mine;
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
   
    // Initialize all the number cells (either number or empty)
    // initialize inner cells
    int surroundingMineCount;
    for (int i = 1; i < _rows - 1; i++) {
      for (int j = 1; j < _cols - 1; j++) {
        if (!_cells[i][j].isMine()) {
          surroundingMineCount = 0;
          if (_cells[i-1][j-1].isMine())
            surroundingMineCount++;
          if (_cells[i][j-1].isMine())
            surroundingMineCount++;
          if (_cells[i+1][j-1].isMine())
            surroundingMineCount++;
          if (_cells[i-1][j].isMine())
            surroundingMineCount++;
          if (_cells[i+1][j].isMine())
            surroundingMineCount++;
          if (_cells[i-1][j+1].isMine())
            surroundingMineCount++;
          if (_cells[i][j+1].isMine())
            surroundingMineCount++;
          if (_cells[i+1][j+1].isMine())
            surroundingMineCount++;
          
          _cells[i][j].setSurroundingMineCount(surroundingMineCount);
        }
      }
    }
    
    // Four corners
    // Upper Left
    if (!_cells[0][0].isMine()) {
      surroundingMineCount = 0;
      if (_cells[0][1].isMine()) 
        surroundingMineCount++; 
      if (_cells[1][0].isMine())
        surroundingMineCount++;
      if (_cells[1][1].isMine())
        surroundingMineCount++;
      
      _cells[0][0].setSurroundingMineCount(surroundingMineCount);
    }

    // Upper right
    if (!_cells[0][_cols-1].isMine()) {
      surroundingMineCount = 0;
      if (_cells[0][_cols-2].isMine())
        surroundingMineCount++;
      if (_cells[1][_cols-2].isMine())
        surroundingMineCount++;
      if (_cells[1][_cols-1].isMine())
        surroundingMineCount++;
      
      _cells[0][_cols-1].setSurroundingMineCount(surroundingMineCount);
    }

    // Lower left 
    if (!_cells[_rows-1][0].isMine()) {
      surroundingMineCount = 0;
      if (_cells[_rows-2][0].isMine())
        surroundingMineCount++;
      if (_cells[_rows-2][1].isMine())
        surroundingMineCount++;
      if (_cells[_rows-1][1].isMine())
        surroundingMineCount++;
      
      _cells[_rows-1][0].setSurroundingMineCount(surroundingMineCount);
    }

    // Lower Right 
    if (!_cells[_rows-1][_cols-1].isMine()) {
      surroundingMineCount = 0;
      if (_cells[_rows-2][_cols-1].isMine())
        surroundingMineCount++;
      if (_cells[_rows-2][_cols-2].isMine())
        surroundingMineCount++;
      if (_cells[_rows-1][_cols-2].isMine())
        surroundingMineCount++;
      
      _cells[_rows-1][_cols-1].setSurroundingMineCount(surroundingMineCount);
    }

    // Left column
    for (int i = 1; i < _rows-1; i++) {
      if (!_cells[i][0].isMine()) {
        surroundingMineCount = 0;
        if (_cells[i-1][0].isMine())
          surroundingMineCount++;
        if (_cells[i+1][0].isMine())
          surroundingMineCount++;
        if (_cells[i-1][1].isMine())
          surroundingMineCount++;
        if (_cells[i][1].isMine())
          surroundingMineCount++;
        if (_cells[i+1][1].isMine())
          surroundingMineCount++;
        
        _cells[i][0].setSurroundingMineCount(surroundingMineCount);
      }
    }
    
    // Right Column
    for (int i = 1; i < _rows-1; i++) {
      if (!_cells[i][_cols-1].isMine()) {
        surroundingMineCount = 0;
        if (_cells[i-1][_cols-1].isMine())
          surroundingMineCount++;
        if (_cells[i+1][_cols-1].isMine())
          surroundingMineCount++;
        if (_cells[i-1][_cols-2].isMine())
          surroundingMineCount++;
        if (_cells[i][_cols-2].isMine())
          surroundingMineCount++;
        if (_cells[i+1][_cols-2].isMine())
          surroundingMineCount++;
        
        _cells[i][_cols-1].setSurroundingMineCount(surroundingMineCount);
      }
    }

    // Top row
    for (int j = 1; j < _cols-1; j++) {
      if (!_cells[0][j].isMine()) {
        surroundingMineCount = 0;
        if (_cells[0][j-1].isMine())
          surroundingMineCount++;
        if (_cells[0][j+1].isMine())
          surroundingMineCount++;
        if (_cells[1][j-1].isMine())
          surroundingMineCount++;
        if (_cells[1][j].isMine())
          surroundingMineCount++;
        if (_cells[1][j+1].isMine())
          surroundingMineCount++;
        
        _cells[0][j].setSurroundingMineCount(surroundingMineCount);
      }
    }

    // Bottom row
    for (int j = 1; j < _cols-1; j++) {
      if (!_cells[_rows-1][j].isMine()) {
        surroundingMineCount = 0;
        if (_cells[_rows-1][j-1].isMine())
          surroundingMineCount++;
        if (_cells[_rows-1][j+1].isMine())
          surroundingMineCount++;
        if (_cells[_rows-2][j-1].isMine())
          surroundingMineCount++;
        if (_cells[_rows-2][j].isMine())
          surroundingMineCount++;
        if (_cells[_rows-2][j+1].isMine())
          surroundingMineCount++;
        
        _cells[_rows-1][j].setSurroundingMineCount(surroundingMineCount);
      }
    }
}

int main(int argc, char** argv) {
  Game game;
  game.Start();
  // probably should clean up resources that I have in a global/heap (but we'll let the OS clear them for now)
  return 0;
}
