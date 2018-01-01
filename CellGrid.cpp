#include "CellGrid.h"

CellGrid::CellGrid() {
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
  _xLowerBound = 0;
  _xUpperBound = 0;
  _yLowerBound = 0;
  _yUpperBound = 0;
}

void CellGrid::drawAllSprites(sf::RenderWindow& window) {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _cells[i][j].drawAllSprites(window);
    } 
  }
}

bool CellGrid::isBeingClicked(sf::Vector2i position) {
  if (position.x >= _xLowerBound &&
      position.x <= _xUpperBound &&
      position.y >= _yLowerBound &&
      position.y <= _yUpperBound)
    return true;
  return false;
}

int CellGrid::clicked(sf::Vector2i position) {
  int i = getCellRowIndex(position);
  int j = getCellColIndex(position);
  return handleReturnCode(_cells[i][j].clicked(), i, j);
}

int CellGrid::rightClicked(sf::Vector2i position) {
  int i = getCellRowIndex(position);
  int j = getCellColIndex(position);
  return handleReturnCode(_cells[i][j].rightClicked(), i, j);
}

void CellGrid::showSolution(bool winner) {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
      _cells[i][j].reveal(winner);
    } 
  }
}


int CellGrid::handleReturnCode(int rc, int i, int j) {
  // Empty Cell and neighbors revealed (includes bordering number cells) 
  if (rc & 8)
    clickNeighborEmpties(i, j);

  // Number cell revealed 
  if (rc & 1)
    _cellsRevealed++;
  
  if (_totalCells - _mines == _cellsRevealed)
    rc += 128;  // we have a winner
  
  // Flag paced
  if (rc & 32)
    _minesLeft--;

  // Flag removed
  if (rc & 64)
    _minesLeft++;

  return rc;
}

void CellGrid::setPosition(int x, int y) {
  _xLowerBound = x;
  _xUpperBound = x + (_rows * 40);
  _yLowerBound = y;
  _yUpperBound = y + (_cols * 40);
  
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
	    _cells[i][j].setPosition(i*40 + x, j*40 + y);
    } 
  }
}

int CellGrid::getCellRowIndex(sf::Vector2i position) {
  return (position.x - _xLowerBound) / 40;
}

int CellGrid::getCellColIndex(sf::Vector2i position) {
  return (position.y - _yLowerBound) / 40;
}

void CellGrid::clickNeighborEmpties(int i, int j) {
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

void CellGrid::clickNeighborEmptiesRecur(int** visited, int i, int j, int originI, int originJ) {
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

bool CellGrid::validRowColIndex(int i, int j) {
  if (i < _rows && i >= 0 && j < _cols && j >= 0)
    return true;
  return false;
}

int CellGrid::isMineWrapper(int i, int j) {
  if (!validRowColIndex(i, j))
    return 0;
  return _cells[i][j].isMine();
}

void CellGrid::reset() {
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
	    _cells[i][j].reset();
    } 
  }
  _minesLeft = _mines;
  _cellsRevealed = 0;
  
  initCells();
}

int CellGrid::getMinesLeft() {
  return _minesLeft;
}

void CellGrid::initCells() {
  // Initialize cells (mines and number)
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

  // Initialize cells (display)
  for (int i = 0; i < _rows; i++) {
    for (int j = 0; j < _cols; j++) {
	    _cells[i][j].setPosition(i*40 + 2, j*40 + 100);	 // upper left corner
    } 
  }
}
