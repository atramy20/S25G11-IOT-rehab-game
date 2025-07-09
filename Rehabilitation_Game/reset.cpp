#include <Arduino.h>
#include "globals.h"          // for dfSerial & button pins



void waitForResetOnGameOver() {
  leds.clear();
  leds.setPixelColor(0, leds.Color(255, 0, 0));
  leds.setPixelColor(1, leds.Color(0, 255, 0));
  leds.show();
  u8g2.drawStr(3, 50, "Red: Mode Selection");
  u8g2.drawStr(3, 60, "Green: Play Again");
  u8g2.sendBuffer();
  while (true) {
    if (digitalRead(buttonPins[0]) == LOW) {
      while (digitalRead(buttonPins[0]) == LOW);  // wait for release
      modeConfirmed = false;  // ✅ go to mode selection
      return;
    }

    if (digitalRead(buttonPins[1]) == LOW) {
      while (digitalRead(buttonPins[1]) == LOW);
      modeConfirmed = true;
      return;
    }

    delay(100);
  }
}


