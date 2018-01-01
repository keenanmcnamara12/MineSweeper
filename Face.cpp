#include "Game.h"

Face::Face() {
  _visibleState = Face::FaceDefault;
  updateDisplay();
  _xLowerBound = 0; 
  _xUpperBound = 0; 
  _yLowerBound = 0; 
  _yUpperBound = 0; 
}

bool Face::isBeingClicked(sf::Vector2i position) {
  if (position.x >= _xLowerBound &&
      position.x <= _xUpperBound &&
      position.y >= _yLowerBound &&
      position.y <= _yUpperBound)
    return true;
  return false;
}

void Face::setPosition(int x, int y) {
  _sprite.setPosition(x, y);
  _xLowerBound = x;
  _xUpperBound = x + 40;
  _yLowerBound = y;
  _yUpperBound = y + 40;
}

void Face::drawAllSprites(sf::RenderWindow& window) {
  updateDisplay();
  window.draw(_sprite);
}

void Face::win() {
  _visibleState = Face::FaceWin;
  updateDisplay();
}

void Face::loss() {
  _visibleState = Face::FaceLoss;
  updateDisplay();
}

void Face::reset() {
  _visibleState = Face::FaceDefault;
  updateDisplay();
}

int Face::clicked() {
  int rc = 16; 
  reset();
  return rc;
}

int  Face::rightClicked() {
  // Unimplemented
  return 0;
}

void Face::updateDisplay() {
  switch (_visibleState)
  {
    case Face::FaceDefault:
      _sprite.setTexture(*getTexture("MineSweeperImages/Smiley.png"));
      break;
    case Face::FaceLoss:
      _sprite.setTexture(*getTexture("MineSweeperImages/SmileyDead.png"));
      break;
    case Face::FaceWin:
      _sprite.setTexture(*getTexture("MineSweeperImages/SmileyGlasses.png"));
      break;
    default:
      _sprite.setTexture(*getTexture("MineSweeperImages/DefaultCell.png"));
  }
}
