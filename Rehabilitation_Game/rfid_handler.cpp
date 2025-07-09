// rfid_handler.ino

#include "globals.h"
#include "mode_selection.h"

void handleRFID() {
  // Light LED 0 red to indicate cancel option
  leds.clear();
  leds.setPixelColor(0, leds.Color(255, 0, 0));
  leds.show();

  u8g2.clearBuffer();
  u8g2.drawStr(3, 20, "Scan tag to connect");
  u8g2.drawStr(3, 40, "Press RED to cancel");
  u8g2.sendBuffer();

  unsigned long startTime = millis();
  while (true) {
    // Cancel if button 0 is pressed
    if (digitalRead(buttonPins[0]) == LOW) {
      while (digitalRead(buttonPins[0]) == LOW); // wait for release
      delay(100);

      leds.clear(); leds.show();
      u8g2.clearBuffer();
      u8g2.drawStr(3, 30, "Canceled.");
      u8g2.sendBuffer();
      delay(1000);
      modeConfirmed = false;
      return;
    }

    // Try reading an RFID tag
    if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) break;
  }

  // Tag read successfully, clear LED
  leds.clear(); leds.show();

  // Build UID string
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uid += '0';
    uid += String(rfid.uid.uidByte[i], HEX);
  }
  uid.toUpperCase();

  // Check if already registered
  for (int i = 0; i < playerCount; i++) {
    if (players[i].uid == uid) {
      currentPlayer = i;
      playerConnected = true;
      for (int m = 0; m < 3; m++) {
        char k[12];
        sprintf(k, "hs%d_%d", i, m);
        players[i].highScores[m] = prefs.getInt(k, 0);
      }
      for (int d = 0; d < 3; d++) {
        char k[12];
        sprintf(k, "r%d_%d", i, d);  // reaction scores by difficulty
        players[i].reactionHighScores[d] = prefs.getInt(k, 0);
      }
      u8g2.clearBuffer();
      u8g2.drawStr(3, 30, ("Player " + String(i + 1) + " connected").c_str());
      u8g2.sendBuffer();
      delay(1000);
      modeConfirmed = false;
      showModeSelection();
      return;
    }
  }

  // Register new player
  if (playerCount < 3) {
    int idx = playerCount++;
    players[idx].uid = uid;
    for (int m = 0; m < 3; m++) players[idx].highScores[m] = 0;

    char k[10];
    sprintf(k, "uid%d", idx); prefs.putString(k, uid);
    for (int m = 0; m < 3; m++) {
      sprintf(k, "hs%d_%d", idx, m);
      prefs.putInt(k, 0);
    }
    prefs.putInt("count", playerCount);

    currentPlayer = idx;
    playerConnected = true;
    u8g2.clearBuffer();
    u8g2.drawStr(3, 30, ("Player " + String(idx + 1) + " paired").c_str());
    u8g2.sendBuffer();
    delay(1000);
    modeConfirmed = false;
    showModeSelection();
  } else {
    u8g2.clearBuffer();
    u8g2.drawStr(3, 30, "Can't pair new");
    u8g2.sendBuffer();
    delay(1000);
  }
}
