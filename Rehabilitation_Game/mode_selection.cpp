#include "globals.h"
#include "audio.h"

void showModeSelection() {
  int modeIndex = 0;

  while (!modeConfirmed) {
    // LEDs: BLUE for toggle, GREEN for confirm
    leds.clear();
    leds.setPixelColor(0, leds.Color(0, 0, 255));  // BLUE button (toggle)
    leds.setPixelColor(1, leds.Color(0, 255, 0));  // GREEN button (confirm)
    leds.show();

    // Determine max modes based on player connection
    int availableModes = playerConnected ? 6 : 4; // If not connected, skip MODE_DISPLAY_SCORES
    if (!playerConnected && modeIndex == MODE_DISPLAY_SCORES) {
      modeIndex = 0;
    }

    // Draw screen
    u8g2.clearBuffer();
    u8g2.drawStr(3, 10, "Select Game Mode");
    u8g2.drawStr(3, 25, modeNames[modeIndex]);
    u8g2.drawStr(3, 40, "Blue: toggle");
    u8g2.drawStr(3, 55, "Green: confirm");
    u8g2.sendBuffer();

    // Toggle mode
    if (digitalRead(buttonPins[0]) == LOW) {
      delay(200);
      modeIndex = (modeIndex + 1) % availableModes;

      // Skip MODE_DISPLAY_SCORES if not connected
      if (!playerConnected && modeIndex == MODE_DISPLAY_SCORES) {
        modeIndex = (modeIndex + 1) % availableModes;
      }
      
      if (playerConnected && modeIndex == MODE_CONNECT_PLAYER) {
        modeIndex = modeIndex + 1;
      }

      Audio::playWrong();
      while (digitalRead(buttonPins[0]) == LOW);
    }

    // Confirm mode
    if (digitalRead(buttonPins[1]) == LOW) {
      delay(200);
      Audio::playCorrect();
      
      // If user selects "Disconnect"
      if (playerConnected && modeIndex == MODE_DISCONNECT) {
        currentPlayer = -1;
        playerConnected = false;

        u8g2.clearBuffer();
        u8g2.drawStr(3, 30, "Player Disconnected");
        u8g2.sendBuffer();
        delay(1000);
        modeIndex = 0; // Reset to first mode
        continue;
      }

      currentMode = static_cast<GameMode>(modeIndex);  
      modeConfirmed = true;

      // Acknowledge selection
      u8g2.clearBuffer();
      u8g2.drawStr(3, 20, "Mode Selected:");
      u8g2.drawStr(3, 40, modeNames[modeIndex]);
      u8g2.sendBuffer();

      Serial.print("Confirmed mode: ");
      Serial.println(currentMode); 
      delay(1000);

      // Turn off LEDs
      leds.clear(); leds.show();
    }
  }
}
