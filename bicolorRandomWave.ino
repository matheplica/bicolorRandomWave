#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "MiniFont.h"
#include "MiniButton.h"
#include "MiniWave.h"
MiniButton push[4] = new MiniButton();
byte pot1, pot2, memPot1, memPot2, pulse = 120, index, mode;
boolean showPotVal, goRandom;
float rate;
byte mem[64] = {};
byte colPot1 = 0, colPot2 = 2;
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
void setup() {
  //Serial.begin(9600);
  matrix.begin(0x70);
  matrix.setRotation(1);
  matrix.clear();
  matrix.writeDisplay();
  push[0].initPullUp(47);
  push[1].initPullUp(49);
  push[2].initPullUp(51);
  push[3].initPullUp(53);
  createRandom();
}
void drawSine(byte y) {
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 4; j++) {
      byte waveId = byte((i + index) * rate) % 32;
      if (j == sine[waveId]) matrix.drawPixel(i, y + j, 1);
      else matrix.drawPixel(i, y + j, 3);
    }
  }
}
void drawTri() {
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 4; j++) {
      byte waveId = byte((i + index) * rate) % 16;
      if (j == tri[waveId]) matrix.drawPixel(i, j, 2);
    }
  }
}
void drawSaw(byte y) {
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 4; j++) {
      byte waveId = byte((i + index) * rate) % 16;
      if (j == saw[waveId]) matrix.drawPixel(i, y + j, 2);
      else matrix.drawPixel(i, y + j, 3);
    }
  }
}
void loop() {
  if (millis() % pulse == 0) {
    index++;
    memPot1 = pot1;
    memPot2 = pot2;
    pot1 = catchAnalogVal(A6, 4) + 1;
    pot2 = catchAnalogVal(A7, 6) + 1;
    if (abs(pot1 - memPot1) > 1) showPotVal = true;
    else if (abs(pot2 - memPot2) > 2) showPotVal = false;
    pulse = pot1 * 2;
    rate = map(pot2, 1, 16, 1, 4);
    matrix.clear();
    if (push[0].isRelease()) mode = 1;
    if (push[1].isRelease()) mode = 2;
    if (push[2].isRelease()) {
      mode = 0;
      createRandom();
    }
    if (push[3].isRelease()) goRandom = ! goRandom;
    //drawing operations
    if (mode == 0) {
      drawRandom();
      if (goRandom ) turnRandom();
    }
    else if (mode == 1) drawSaw(4);
    else if (mode == 2) drawSine(4);
    if (mode > 0) {
      if (showPotVal) drawPotVal(pot1, 0, 0, 2);
      else drawPotVal(pot2, 1, 0, 2);
    }
    matrix.writeDisplay();
  }
}
void createRandom() {
  for (int i = 0; i < 64; i++) mem[i] = int(random(4));
}
void drawRandom() {
  for (int i = 0; i < 64; i++) matrix.drawPixel(i % 8, int(i / 8), mem[i]);
}
void turnRandom() {
  if(random()>0.8) for (int i = 0; i < 64; i++) mem[i] = ++mem[i] % 4;
}
void drawPotVal(byte v, byte x, byte y, byte colorId) {
  String str = String(v, DEC);
  for (byte i = 0; i < str.length(); i++) {
    byte c = byte(str.charAt(str.length() - i - 1) - '0');
    drawLetter(x + 4 - i * 4, y, c, colorId);
  }
}
int catchAnalogVal(char aPin, byte resolution) {
  return analogRead(aPin) >> resolution;
}
void drawLetter(byte x, byte y, byte caract, byte colorId) {
  uint16_t letter = pgm_read_word_near(font + caract);
  for (int8_t i = 11; i >= 0; i--) {
    if ((letter & (1 << i)) > 0) matrix.drawPixel(x + (2 - (i % 3)), y + ((11 - i) / 3), colorId);
  }
}
