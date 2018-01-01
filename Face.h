#include "Game.h"

#ifndef FACE_H
#define FACE_H

class Face {
  public:
    Face();
    void updateDisplay();
    int  clicked();
    int  rightClicked();
    void setPosition(int x, int y);
    void drawAllSprites(sf::RenderWindow& window);
    void win();
    void loss();
    void reset();
    bool isBeingClicked(sf::Vector2i position);
   
  private:
  
  public:

  private:
    sf::Sprite _sprite;
    enum FaceType {FaceDefault, FaceLoss, FaceWin};
    FaceType _visibleState;
    int _xLowerBound;
    int _xUpperBound;
    int _yLowerBound;
    int _yUpperBound;

};
#endif // FACE_H
