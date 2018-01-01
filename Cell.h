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
    void reveal(bool mineClicked, bool winner);
    void reset();
    bool isRevealed();
    void setPosition(int x, int y);
    void drawAllSprites(sf::RenderWindow& window);
    void faceWin();
    void faceLoss();
    void faceReset();
    void initMine();
    void initFace();
    void initShowSolution();
   
  private:
  
  public:

  private:
    sf::Sprite _sprite;
    int _number;
    enum CellType {Default, Empty, Number, Flag, Question, Mine, MineClicked, MineMissed, ShowSolution, Face, FaceLoss, FaceWin};
    CellType _trueState;
    CellType _visibleState;

};
#endif // CELL_H
