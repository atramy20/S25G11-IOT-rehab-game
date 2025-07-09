#include "globals.h"
#include "headers.h"
#include "audio.h"
#include "reset.h"

int focusScore = 0;
int currentPermutation[NUM_BUTTONS];
int focusTargetIndex = 0;
bool focusGameOver = false;

void startFocusTurn();
void displayFocusPrompt();
int getPressedButton();
void showFocusGameOver();

unsigned long buttonTimestamps[NUM_BUTTONS] = {0};
const unsigned long debounceDelay = 200;

void runFocusGame() {
  focusScore = 0;
  focusGameOver = false;
  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Focus Mode");
  u8g2.sendBuffer();
  Audio::playCorrect();
  delay(1000);

  while (!focusGameOver) {
    startFocusTurn();
    displayFocusPrompt();

    int selected = -1;
    while (selected == -1) {
      selected = getPressedButton();
    }

    if (selected == focusTargetIndex) {
      Audio::playCorrect();
      focusScore++;
    } else {
      //Audio::playWrong();
      focusGameOver = true;
    }

    delay(300);
  }

  showFocusGameOver();
  waitForResetOnGameOver();  // handles retry or return to menu
}

void startFocusTurn() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    currentPermutation[i] = i;
  }
  for (int i = NUM_BUTTONS - 1; i > 0; i--) {
    int j = random(i + 1);
    int tmp = currentPermutation[i];
    currentPermutation[i] = currentPermutation[j];
    currentPermutation[j] = tmp;
  }
  focusTargetIndex = random(NUM_BUTTONS);

  for (int i = 0; i < NUM_BUTTONS; i++) {
    leds.setPixelColor(i, colors[currentPermutation[i]]);
  }
  leds.show();
}

void displayFocusPrompt() {
  u8g2.clearBuffer();
  if (currentPlayer >= 0) {
    u8g2.drawStr(3, 10, (String("P") + String(currentPlayer + 1)).c_str());
    u8g2.drawStr(40, 10, (String("HS:") + String(players[currentPlayer].highScores[MODE_FOCUS])).c_str());
  } else {
    u8g2.drawStr(3, 10, "Guest Mode");
  }
  u8g2.drawStr(3, 25, "Press:");
  u8g2.drawStr(40, 25, colorNames[currentPermutation[focusTargetIndex]]);
  Audio::playColor(currentPermutation[focusTargetIndex]);
  u8g2.drawStr(3, 40, (String("Score:") + focusScore).c_str());
  u8g2.sendBuffer();
}

int getPressedButton() {
  for (int i = 0; i < NUM_BUTTONS; i++) {
    if (digitalRead(buttonPins[i]) == LOW && millis() - buttonTimestamps[i] > debounceDelay) {
      buttonTimestamps[i] = millis();
      while (digitalRead(buttonPins[i]) == LOW);
      delay(50);
      return i;
    }
  }
  return -1;
}

void showFocusGameOver() {
  leds.clear();
  for (int i = 0; i < NUM_BUTTONS; i++) {
    leds.setPixelColor(i, leds.Color(255, 0, 0));
  }
  leds.show();

  if (currentPlayer >= 0 && focusScore > players[currentPlayer].highScores[MODE_FOCUS]) {
    players[currentPlayer].highScores[MODE_FOCUS] = focusScore;
    char k[12];
    sprintf(k, "hs%d_%d", currentPlayer, MODE_FOCUS);
    prefs.putInt(k, focusScore);
    Audio::playWin();
  } else {
    Audio::playGameOver();
  }

  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "GAME OVER");

  char buf[30];
  sprintf(buf, "Score: %d", focusScore);
  u8g2.drawStr(3, 30, buf);

  if (currentPlayer >= 0) {
    sprintf(buf, "High Score:  %d", players[currentPlayer].highScores[MODE_FOCUS]);
    u8g2.drawStr(3, 40, buf);
  }

  //waitForResetOnGameOver();
}
