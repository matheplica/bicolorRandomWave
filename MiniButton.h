#ifndef MINI_BUTTON_H
#define MINI_BUTTON_H

#include "Arduino.h"
class MiniButton {
  private :
    int pin;
    int timer;
    int debounce;
    unsigned long lastPushTime;
    bool mode;
    bool lastState;
    bool currentState;
  public :
  MiniButton();
    MiniButton(int pin);
    void init(int pin);
    void initPullUp(int pin);
    bool isRelease();
};
#endif
