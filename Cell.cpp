#include "Game.h"

Cell::Cell() {
  _visibleState = Cell::Default;
  updateDisplay();
}

int Cell::clicked() {
  int rc = 0;
  if (_visibleState != Cell::Flag) {
    switch (_trueState)
    {
      case Cell::Number:
        _visibleState = Cell::Number;
        break;
      case Cell::Mine:
        _trueState = Cell::MineClicked;   // since reveal will set truestate to visible
        _visibleState = Cell::MineClicked;
        rc = 4;
        break;
      case Cell::Empty:
        _visibleState = Cell::Empty;
        rc = 8;
        break;
      case Cell::ShowSolution:
        rc = 2;
        break;
      case Cell::Face:
        rc = 16;
        break;
      default:
        break;
    }
  
    updateDisplay();
  }
  return rc;
}

int Cell::rightClicked() {
  int rc = 0;
  switch (_visibleState)
  {
    case Cell::Default:
      _visibleState = Cell::Flag;
      break;
    case Cell::Flag:
      _visibleState = Cell::Question;
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

void Cell::reveal() {
  if (_trueState == Cell::Mine && _visibleState == Cell::Flag) {
      _visibleState = Cell::Mine;
  } else if (_trueState == Cell::Mine) {
      _visibleState = Cell::MineMissed;
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
    default:
      _sprite.setTexture(*getTexture("MineSweeperImages/DefaultCell.png"));
  }
}
