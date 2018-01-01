#include "Game.h"

Cell::Cell() {
  _visibleState = Cell::Default;
  updateDisplay();
}

int Cell::clicked() {
  // reveal(_trueState == Cell::Mine);
 
  int rc = 0;
  if ((_visibleState != Cell::Flag) && (_visibleState != _trueState)) {

    switch (_trueState)
    {
      case Cell::Number:
        _visibleState = Cell::Number;
        rc += 1;
        break;
      case Cell::ShowSolution:
        rc += 2;
        break;
      case Cell::Mine:
        _trueState = Cell::MineClicked;   // since reveal will set truestate to visible
        _visibleState = Cell::MineClicked;
        rc += 4;
        break;
      case Cell::Empty:
        _visibleState = Cell::Empty;
        rc += 8;
        break;
      default:
        break;
    }  
  } else if (_visibleState == Cell::Face || _visibleState == Cell::FaceLoss || _visibleState == Cell::FaceWin) {
      rc += 16; 
  }
  updateDisplay();
  return rc;
}

int Cell::rightClicked() {
  int rc = 0;
  switch (_visibleState)
  {
    case Cell::Default:
      _visibleState = Cell::Flag;
      rc += 32;
      break;
    case Cell::Flag:
      _visibleState = Cell::Question;
      rc += 64;
      break;
    case Cell::Question:
      _visibleState = Cell::Default;
      break;
    case Cell::ShowSolution:
      rc = 2;
      break;
    default:
      break;
  }
  updateDisplay();
  return rc;
}

bool Cell::isRevealed() {
  if (_trueState == _visibleState)
    return true;
  return false;
}

void Cell::reveal(bool mineClicked, bool winner) {
  if ((_trueState == Cell::Mine && _visibleState == Cell::Flag) || (_trueState == Cell::Mine && winner)) {
      _visibleState = Cell::Mine;
  } else if (_trueState == Cell::Mine) {
      _visibleState = Cell::MineMissed;
  } else if (mineClicked == true) {
      _visibleState = Cell::MineClicked;
      _trueState = Cell::MineClicked;
  } else {
      _visibleState = _trueState;
  }
  updateDisplay();
}

void Cell::reset() {
  _visibleState = Cell::Default;
  _trueState = Cell::Empty;
  updateDisplay();
}

bool Cell::isMine() {
  if (_trueState == Cell::Mine)
    return true;
  return false;
}

bool Cell::isEmpty() {
  if (_trueState == Cell::Empty)
    return true;
  return false;
}

void Cell::setSurroundingMineCount(int count) {
  if (count == 0) {
    _trueState = Cell::Empty;
  }
  else {
    _trueState = Cell::Number; 
    _number = count;
  }
}

void Cell::setPosition(int x, int y) {
  _sprite.setPosition(x, y);
}

void Cell::initMine() {
  _trueState = Cell::Mine;
}

void Cell::initFace() {
  _trueState = Cell::Face;
  _visibleState = Cell::Face;
}

void Cell::initShowSolution() {
  _trueState = Cell::ShowSolution;
}

void Cell::drawAllSprites(sf::RenderWindow& window) {
  updateDisplay();
  window.draw(_sprite);
}

void Cell::faceWin() {
  _visibleState = Cell::FaceWin;
  _trueState = Cell::FaceWin;
  updateDisplay();
}

void Cell::faceLoss() {
  _visibleState = Cell::FaceLoss;
  _trueState = Cell::FaceLoss;    // need to keep in sync for Cell::click to work
  updateDisplay();
}

void Cell::faceReset() {
  _visibleState = Cell::Face;
  _trueState = Cell::Face;    // need to keep in sync for Cell::click to work
  updateDisplay();
}

void Cell::updateDisplay() {
  switch (_visibleState)
  {
    case Cell::Default:
      _sprite.setTexture(*getTexture("MineSweeperImages/DefaultCell.png"));
      break;
    case Cell::Empty:
      _sprite.setTexture(*getTexture("MineSweeperImages/EmptyCell.png"));
      break;
    case Cell::Number:
      switch (_number)
      {
        case 1:
          _sprite.setTexture(*getTexture("MineSweeperImages/1Cell.png"));
          break;
        case 2:
          _sprite.setTexture(*getTexture("MineSweeperImages/2Cell.png"));
          break;
        case 3:
          _sprite.setTexture(*getTexture("MineSweeperImages/3Cell.png"));
          break;
        case 4:
          _sprite.setTexture(*getTexture("MineSweeperImages/4Cell.png"));
          break;
        case 5:
          _sprite.setTexture(*getTexture("MineSweeperImages/5Cell.png"));
          break;
        case 6:
          _sprite.setTexture(*getTexture("MineSweeperImages/6Cell.png"));
          break;
        case 7:
          _sprite.setTexture(*getTexture("MineSweeperImages/7Cell.png"));
          break;
        case 8:
          _sprite.setTexture(*getTexture("MineSweeperImages/8Cell.png"));
          break;
      }
      break;
    case Cell::Flag:
      _sprite.setTexture(*getTexture("MineSweeperImages/FlagCell.png"));
      break;
    case Cell::Question:
      _sprite.setTexture(*getTexture("MineSweeperImages/QuestionCell.png"));
      break;
    case Cell::MineClicked:
      _sprite.setTexture(*getTexture("MineSweeperImages/MineClickedCell.png"));
      break;
    case Cell::Mine:
      _sprite.setTexture(*getTexture("MineSweeperImages/MineCorrectCell.png"));
      break;
    case Cell::MineMissed:
      _sprite.setTexture(*getTexture("MineSweeperImages/MineMissedCell.png"));
      break;
    case Cell::Face:
      _sprite.setTexture(*getTexture("MineSweeperImages/Smiley.png"));
      break;
    case Cell::FaceLoss:
      _sprite.setTexture(*getTexture("MineSweeperImages/SmileyDead.png"));
      break;
    case Cell::FaceWin:
      _sprite.setTexture(*getTexture("MineSweeperImages/SmileyGlasses.png"));
      break;
    default:
      _sprite.setTexture(*getTexture("MineSweeperImages/DefaultCell.png"));
  }
}
