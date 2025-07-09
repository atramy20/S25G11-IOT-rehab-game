#include "globals.h"
#include "audio.h"
#include "reset.h"

int reactionScore = 0;
bool reactionGameOver = false;
unsigned long ledOnTime = 0;
int reactionTarget = -1;
unsigned long reactionTimeout = 900;  // will be updated by difficulty

void selectReactionDifficulty() {
  u8g2.clearBuffer();
  u8g2.drawStr(3, 10, "Select Difficulty:");
  u8g2.drawStr(3, 25, "Green = Easy");
  u8g2.drawStr(3, 35, "Yellow = Medium");
  u8g2.drawStr(3, 45, "Red = Hard");
  u8g2.sendBuffer();

  // Show colors
  leds.clear();
  leds.setPixelColor(1, leds.Color(0, 255, 0));    // Green
  leds.setPixelColor(2, leds.Color(255, 255, 0));  // Yellow
  leds.setPixelColor(3, leds.Color(255, 0, 0));    // Red
  leds.show();

  while (true) {
    if (digitalRead(buttonPins[1]) == LOW) {  // Green = Easy
      reactionTimeout = 1500;
      currentReactionDifficulty = EASY;
      Audio::playCorrect();
      break;
    } else if (digitalRead(buttonPins[2]) == LOW) {  // Yellow = Medium
      reactionTimeout = 1000;
      currentReactionDifficulty = MEDIUM;
      Audio::playCorrect();
      break;
    } else if (digitalRead(buttonPins[3]) == LOW) {  // Red = Hard
      reactionTimeout = 500;
      currentReactionDifficulty = HARD;
      Audio::playCorrect();
      break;
    }
  }

  // Wait for button release and clear LEDs
  delay(300);
  while (digitalRead(buttonPins[1]) == LOW || digitalRead(buttonPins[2]) == LOW || digitalRead(buttonPins[3]) == LOW);
  leds.clear(); leds.show();
}

void runReactionGame() {
  reactionScore = 0;
  reactionGameOver = false;

  selectReactionDifficulty();

  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Reaction Mode");
  u8g2.sendBuffer();
  delay(1000);

  while (!reactionGameOver) {
    reactionTarget = random(NUM_LEDS);
    leds.clear();
    leds.setPixelColor(reactionTarget, leds.Color(0, 0, 255));  // Blue = target
    leds.show();
    ledOnTime = millis();

    bool responded = false;
    while (!responded && millis() - ledOnTime < reactionTimeout) {
      for (int i = 0; i < NUM_BUTTONS; i++) {
        if (digitalRead(buttonPins[i]) == LOW) {
          responded = true;

          if (i == reactionTarget) {
            Audio::playCorrect();
            leds.setPixelColor(i, leds.Color(0, 255, 0));  // Green
            leds.show(); delay(300);
            reactionScore++;
          } else {
            Audio::playWrong();
            leds.setPixelColor(i, leds.Color(255, 0, 0));  // Red
            leds.show(); delay(500);
            reactionGameOver = true;
          }

          while (digitalRead(buttonPins[i]) == LOW);
          delay(100);
          break;
        }
      }
    }

    if (!responded && millis() - ledOnTime >= reactionTimeout) {
      Audio::playWrong();
      reactionGameOver = true;
    }

    leds.clear(); leds.show();
    delay(200);
  }

  // Save high score if needed
  if (currentPlayer >= 0) {
    int& hs = players[currentPlayer].reactionHighScores[currentReactionDifficulty];
    if (reactionScore > hs) {
      hs = reactionScore;

      char key[16];
      sprintf(key, "r%d_%d", currentPlayer, currentReactionDifficulty); // e.g. "r0_1" for player 0, medium
      prefs.putInt(key, hs);
    }
  }

  int highScore = (currentPlayer >= 0)
                  ? players[currentPlayer].reactionHighScores[currentReactionDifficulty]
                  : 0;

  char buffer[32];
  sprintf(buffer, "Score:%2d  High:%2d", reactionScore, highScore);

  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Game Over");
  u8g2.drawStr(3, 30, buffer);
  u8g2.sendBuffer();

  waitForResetOnGameOver();
}
