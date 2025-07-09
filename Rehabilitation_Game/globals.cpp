#include "globals.h"
DFRobotDFPlayerMini dfp;
GameMode currentMode = MODE_CONNECT_PLAYER;
bool playerConnected = false;
int currentPlayer = -1;
Player players[3];
int playerCount = 0;
Preferences prefs;
MFRC522 rfid(5, 27);
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
Adafruit_NeoPixel leds(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
int buttonPins[NUM_BUTTONS] = {12, 14, 27, 26, 25};

// Reaction game
bool reactionStarted = false;
unsigned long lastTrigger = 0;
int targetIndex = -1;

// Memory game
int memorySequence[MAX_SEQUENCE_LENGTH];
int currentSequenceLength = 0;
bool gameOver = false;

// Colors for memory game
uint32_t colors[NUM_LEDS] = {
  leds.Color(255, 0, 0),
  leds.Color(0, 255, 0),
  leds.Color(0, 0, 255),
  leds.Color(255, 255, 0),
  leds.Color(255, 255, 255)
};

const char* modeNames[] = {
  "Reaction",
  "Focus",
  "Memory",
  "Connect Player",
  "Display Scores",
  "Disconnect"
};
bool modeConfirmed = false;
int modeIndex = 0;

// Define the DFPlayer serial (using UART2)
HardwareSerial dfSerial(2);

const char* colorNames[NUM_LEDS] = {
  "RED", "GREEN", "BLUE", "YELLOW", "WHITE"
};

ReactionDifficulty currentReactionDifficulty = MEDIUM;
