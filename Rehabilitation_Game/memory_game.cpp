// memory_game.cpp
#include "globals.h"
#include "headers.h"
#include "audio.h"
#include "reset.h"

void runMemoryGame() {
  leds.clear(); leds.show();
  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Memory Mode");
  u8g2.sendBuffer();
  Audio::playCorrect();
  delay(1000);
  currentSequenceLength = 1; gameOver = false;

  while (!gameOver) {
    generateSequence();
    playSequence();
    // if (currentPlayer >= 0 && players[currentPlayer].highScores[MODE_MEMORY] < currentSequenceLength - 1) {
    // players[currentPlayer].highScores[MODE_MEMORY] = currentSequenceLength - 1;
    // char k[10];
    // sprintf(k, "hs%d_%d", currentPlayer, MODE_MEMORY);
    // prefs.putInt(k, currentSequenceLength - 1);
    // u8g2.clearBuffer();
    // u8g2.drawStr(3, 30, "New High Score!");
    // u8g2.sendBuffer();
    // delay(2000);   
    // }
    u8g2.clearBuffer();
    u8g2.drawStr(3, 20, "Repeat the sequence!");

    char scoreBuf[32];
    int currentScore = currentSequenceLength - 1;
    int highScore    = (currentPlayer >= 0)
                       ? players[currentPlayer].highScores[MODE_MEMORY]
                       : 0;                         // guest mode

    sprintf(scoreBuf, "Score:%2d  High:%2d", currentScore, highScore);
    u8g2.drawStr(3, 40, scoreBuf);
    u8g2.sendBuffer();
    getPlayerInput();

    if (!gameOver) {
      currentSequenceLength++;
      if (currentSequenceLength > MAX_SEQUENCE_LENGTH) {
        int finalScore = currentSequenceLength - 1;          // 10 if MAX_SEQUENCE_LENGTH = 10
        if (currentPlayer >= 0 &&
            players[currentPlayer].highScores[MODE_MEMORY] < finalScore) {

            players[currentPlayer].highScores[MODE_MEMORY] = finalScore;

            char k[12];
            sprintf(k, "hs%d_%d", currentPlayer, MODE_MEMORY);
            prefs.putInt(k, finalScore);                     // store in flash
        }
        Audio::playWin();        
        u8g2.clearBuffer();
        u8g2.drawStr(3, 25, "You Win!");
        u8g2.sendBuffer();
        //u8g2.drawStr(3, 40, "Hold B0 3s to reset");
        //u8g2.sendBuffer();
        waitForResetOnGameOver(); 
      }
    }
  }

  if (gameOver) {
    Audio::playGameOver();
    u8g2.clearBuffer();
    u8g2.drawStr(3, 20, "Wrong! Game Over");
    char finalBuf[30];                                   
    sprintf(finalBuf, "Score: %d", currentSequenceLength - 1);
    u8g2.drawStr(3, 30, finalBuf); 
    //u8g2.drawStr(3, 40, "Hold B0 3s to reset");
    //u8g2.sendBuffer();
    waitForResetOnGameOver(); 
  }
}

void generateSequence() {
  Serial.print("Sequence: ");
  for (int i = 0; i < currentSequenceLength; i++) {
    memorySequence[i] = random(NUM_BUTTONS);
    Serial.print(memorySequence[i]); Serial.print(" ");
  }
  Serial.println();
}

void playStep(int index) {
  if (index < 0 || index >= NUM_LEDS) return;
  leds.clear();
  leds.setPixelColor(index, colors[index]);
  leds.show();
  delay(600);
  leds.setPixelColor(index, 0);
  leds.show();
  delay(200);
}

void playSequence() {
  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Watch...");
  u8g2.sendBuffer();
  delay(500);
  for (int i = 0; i < currentSequenceLength; i++) {
    playStep(memorySequence[i]);
  }
  leds.clear();
  leds.show();
}

void getPlayerInput() {
  leds.clear(); leds.show();
  for (int i = 0; i < currentSequenceLength; i++) {
    bool pressed = false; int selected = -1;
    while (!pressed) {
      for (int j = 0; j < NUM_BUTTONS; j++) {
        if (digitalRead(buttonPins[j]) == LOW) {
          selected = j;
          if (selected != memorySequence[i]) {
            Audio::playWrong();
            leds.setPixelColor(j, leds.Color(255, 0, 0));
            leds.show(); delay(500); leds.setPixelColor(j, 0); leds.show();
            gameOver = true;
            return;
          } else {
            Audio::playCorrect();
            leds.setPixelColor(j, leds.Color(0, 255, 0));
            leds.show(); delay(300); leds.setPixelColor(j, 0); leds.show();
          }
          pressed = true;
          while (digitalRead(buttonPins[j]) == LOW){
            waitForResetOnGameOver();
          }
          delay(100);
        }
      }
    }
  }
    if (currentPlayer >= 0 && players[currentPlayer].highScores[MODE_MEMORY] < currentSequenceLength) {
    players[currentPlayer].highScores[MODE_MEMORY] = currentSequenceLength;
    char k[12];
    sprintf(k, "hs%d_%d", currentPlayer, MODE_MEMORY);
    prefs.putInt(k, currentSequenceLength);
    
    u8g2.clearBuffer();
    u8g2.drawStr(3, 30, "New High Score!");
    u8g2.sendBuffer();
    delay(2000);
  }
}
