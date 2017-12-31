#include "Score.h"

Score::Score() {
  _number = 0;
  _timerRunning = false;
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
  if (number > 999)
    _number = 999;
  _number = number;
  updateDisplay();
}

void Score::startTimer() {
  // Only start if the timer hasn't been started
  if (!_timerRunning) {
    _timerRunning = true;
    _timerStopped = false;
    _tStart = std::clock();
    updateDisplay();
  }
}

void Score::stopTimer() {
  // Only set the end if start has been called
  if (_timerRunning) {
    _timerStopped = true;
    _tEnd = std::clock();
  }
}

void Score::resetTimer() {
  // forcing to show zeros by using the number mode (basically the newly create obj state)
  _timerRunning = false;
  _number = 0;
  _timerStopped = true;
}

void Score::updateDisplay() {
  if (_timerRunning == false) {
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
  long result;
  if (_timerStopped)
    result = floor((_tEnd - _tStart) / (double) CLOCKS_PER_SEC);
  else 
    result = floor((std::clock() - _tStart) / (double) CLOCKS_PER_SEC);
  if (result > 999)
    result = 999;
  return result;
}
