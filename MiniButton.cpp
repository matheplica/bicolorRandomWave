#include "MiniButton.h"
MiniButton::MiniButton() {
  lastPushTime = 0;
  pin = 0;
  debounce = 150;
  mode = HIGH;
  currentState = false;
  lastState = false;
}
MiniButton::MiniButton(int thePin) {
  lastPushTime = 0;
  pin = thePin;
  debounce = 150;
  mode = HIGH;
  currentState = false;
  lastState = false;
}
void MiniButton::init(int thePin) {
  pin = thePin;
  pinMode(pin, INPUT);
}
void MiniButton::initPullUp(int thePin) {
  pin = thePin;
  mode = LOW;
  pinMode(pin, INPUT_PULLUP);
}
bool MiniButton::isRelease() {
  boolean state;
  if (digitalRead(pin) == mode) state = true;
  else state = false;
  if (state != currentState) {
    if ((millis() - lastPushTime) > debounce) {
      currentState = state;
      lastPushTime = millis();
      if (currentState == false) return true;
      else return false;
    }
  }else return false;
}
