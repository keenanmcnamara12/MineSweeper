#include <ctime>
#include <iomanip>
#include <math.h>
#include "Game.h"

#ifndef SCORE_H
#define SCORE_H

class Score {
  public:
    Score();
    void setNumber(int number);
    void setPosition(float x, float y);
    void startTimer();
    void stopTimer();
    void resetTimer();
    void updateDisplay();
    void drawAllSprites(sf::RenderWindow& window);

  private:
    long timePassed();

  public:

  private:
    double _number;
    bool _timerRunning;
    bool _timerStopped;
    clock_t _tStart;
    clock_t _tEnd;
    sf::RectangleShape _background;
    sf::Text _text;
    sf::Font _font;

};
#endif  // SCORE_H
