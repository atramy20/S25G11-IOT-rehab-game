// scores_display.ino
#include "globals.h"
#include "audio.h"


void showPlayerScores() {
  u8g2.clearBuffer();

  if (currentPlayer >= 0) {
    char line[32];

    sprintf(line, "Player: %d", currentPlayer + 1);
    u8g2.drawStr(3, 10, line);

    sprintf(line, "Memory:  %d", players[currentPlayer].highScores[MODE_MEMORY]);
    u8g2.drawStr(3, 20, line);

    sprintf(line, "Focus:   %d", players[currentPlayer].highScores[MODE_FOCUS]);
    u8g2.drawStr(3, 30, line);

    u8g2.drawStr(3, 40, "Reaction E/M/H:");
    sprintf(line, "%d/%d/%d",
            players[currentPlayer].reactionHighScores[0],
            players[currentPlayer].reactionHighScores[1],
            players[currentPlayer].reactionHighScores[2]);
    u8g2.drawStr(3, 50, line);

    u8g2.drawStr(3, 60, "Red: Mode Selection");
    u8g2.sendBuffer();
    leds.clear();
    leds.setPixelColor(0, leds.Color(255, 0, 0));
    leds.show();

    // Wait for any button press
    bool pressed = false;
    while (!pressed) {
      if (digitalRead(buttonPins[0]) == LOW) {
        pressed = true;
        while (digitalRead(buttonPins[0]) == LOW);  // wait for release
        //delay(10);
        modeConfirmed = false;
        return;
      }
    }

    //Audio::playWrong();
    modeConfirmed = false;
  } else {
    u8g2.drawStr(3, 20, "No Player Connected");
    u8g2.sendBuffer();
    delay(1000);
  }
}
