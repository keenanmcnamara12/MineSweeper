#include "Score.h"

Score::Score() {
  _number = 0;
  _timerMode = false;
  _timerStopped = true;
  _text.setString("000");
  _text.setCharacterSize(38);
  _text.setColor(sf::Color::Red);
  _font.loadFromFile("Crashed Scoreboard.ttf");
  _text.setFont(_font);
  _background.setFillColor(sf::Color(0,0,0));  // black
  _background.setSize(sf::Vector2f(74,40));
}

void Score::setPosition(float x, float y) {
  _background.setPosition(x, y);
  _text.setPosition(x + 4, y - 4);  // getting rid of some buffer space that the font seems to have
}
    
void Score::setNumber(int number) {
  _number = number;
  updateDisplay();
}

void Score::startTimer() {
  _timerMode = true;
  _timerStopped = false;
  _tStart = std::clock();
  updateDisplay();
}

void Score::stopTimer() {
  _timerMode = true;
  _timerStopped = true;
  _tEnd = std::clock();
}

void Score::resetTimer() {
  _timerMode = true;
  startTimer();
}

void Score::updateDisplay() {
  if (_timerMode == false) {
      std::ostringstream sstream;
      sstream << setfill('0') << setw(3) << _number;
      _text.setString(sstream.str());
  } else {
      std::ostringstream sstream;
      sstream << setfill('0') << setw(3) << timePassed();
      _text.setString(sstream.str());
  }
}

void Score::drawAllSprites(sf::RenderWindow& window) {
  updateDisplay();
  window.draw(_background);
  window.draw(_text);
}

long Score::timePassed() {
  if (_timerStopped)
    return floor((_tEnd - _tStart) / (double) CLOCKS_PER_SEC);
  return  floor((std::clock() - _tStart) / (double) CLOCKS_PER_SEC);
}