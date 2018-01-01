#include "Game.h"

#ifndef CELLGRID_H
#define CELLGRID_H

class CellGrid {
  
  public:
    CellGrid();
    void initCells();
    void setPosition(int x, int y);
    void drawAllSprites(sf::RenderWindow& window);
    bool isBeingClicked(sf::Vector2i position);
    int clicked(sf::Vector2i position);
    int rightClicked(sf::Vector2i position);
    void showSolution(bool winner);
    void reset();
    int getMinesLeft();
   
  private:
    int isMineWrapper(int i, int j);
    bool validRowColIndex(int i, int j);
    int getCellRowIndex(sf::Vector2i position);
    int getCellColIndex(sf::Vector2i position);
    int handleReturnCode(int rc, int i, int j);
    void clickNeighborEmpties(int i, int j);
    void clickNeighborEmptiesRecur(int** visited, int i, int j, int originI, int originJ);
  
  public:

  private:
    int _rows;
    int _cols;
    int _totalCells;
    int _cellsRevealed;
    int _mines;
    int _minesLeft;
    Cell **_cells;
    int _xLowerBound;
    int _xUpperBound;
    int _yLowerBound;
    int _yUpperBound;

};
#endif // CELLGRID_H
