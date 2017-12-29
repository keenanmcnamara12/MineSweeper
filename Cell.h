#include "Game.h"

#ifndef CELL_H
#define CELL_H

class Cell {
  public:
    Cell();
    void updateDisplay();
    int  clicked();
    int  rightClicked();
    bool isMine();
    bool isEmpty();
    void setSurroundingMineCount(int count);
    void reveal();
    void reset();
   
  private:
  
  public:
    enum CellType {Default, Empty, Number, Flag, Question, Mine, MineClicked, MineMissed, ShowSolution, Face, FaceLoss, FaceWin};
    CellType _trueState;
    CellType _visibleState;
    int _number;
    enum MineState {Clicked, Correct, Missed};
    MineState _mineState;
    sf::Sprite _sprite;
    sf::Texture _textureTest;

  private:

};
#endif // CELL_H
