// Rehabilitation_Game.ino

#include "globals.h"
#include "headers.h"
#include "audio.h"


void setup() {
  Serial.begin(115200);
  SPI.begin();          // 🔹 Required for MFRC522 to function
  rfid.PCD_Init();      // 🔹 Initializes RFID module (must follow SPI.begin)
  dfSerial.begin(9600, SERIAL_8N1, 16, 17); // RX2, TX2
  Audio::selectTF();          // one-time SD select
  Audio::volume(10);          // loudness 0-30
  // Initialize display
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);  // Smaller font for full text display


  // Initialize LED strip
  leds.begin();
  leds.clear();
  leds.show();
  //Audio::playCorrect();

  // Initialize buttons
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Initialize preferences
  prefs.begin("rehab_game", false);
  playerCount = prefs.getInt("count", 0);
  for (int i = 0; i < playerCount; i++) {
  char key[12];
  // Load UID
  sprintf(key, "uid%d", i);
  players[i].uid = prefs.getString(key, "");
  // Load high scores for all modes
  for (int m = 0; m < 3; m++) {
    sprintf(key, "hs%d_%d", i, m);
    players[i].highScores[m] = prefs.getInt(key, 0);
  }
}

          // *******this is only for reseting highscores********//
// while (!Serial && millis() < 5000);   // wait for Serial Monitor to open

// Serial.println("Type 'reset' + Enter to clear highscores.");

// unsigned long t0 = millis();
// String cmd = "";
// while (millis() - t0 < 10000) {       // 10-second listening window
//   if (Serial.available()) {
//     cmd = Serial.readStringUntil('\n');
//     cmd.trim();
//     break;
//   }
// }

// if (cmd.equalsIgnoreCase("reset")) {
//   Serial.println("Clearing all highscores...");
//   int count = prefs.getInt("count", 0);        // how many players stored
//   for (int p = 0; p < count; p++) {
//     for (int m = 0; m < 3; m++) {
//       char k[12];
//       sprintf(k, "hs%d_%d", p, m);
//       prefs.putInt(k, 0);
//     }
//   }
//   prefs.end();                                 // flush & close
//   prefs.begin("rehab_game", false);            // reopen for normal use
//   Serial.println("Highscores reset!");
//   u8g2.clearBuffer();
//   u8g2.drawStr(3, 30, "Highscores Reset!");
//   u8g2.sendBuffer();
//   delay(2000);
// }

  showModeSelection();
}

void loop() {
  if (!modeConfirmed) {
    showModeSelection();       
    return;                      
  }
  if (modeConfirmed) {
    switch (currentMode) {
      case MODE_REACTION:
        runReactionGame();
        return;
      case MODE_FOCUS:
        runFocusGame();
        return;
      case MODE_MEMORY:
        runMemoryGame();
        return;
      case MODE_CONNECT_PLAYER:
        if (!playerConnected) {
        handleRFID();                
        return;               
      }
      case MODE_DISPLAY_SCORES:
        showPlayerScores();
        return;
      modeConfirmed = false;
    }
    delay(3000);
    modeConfirmed = false;
  }
}


