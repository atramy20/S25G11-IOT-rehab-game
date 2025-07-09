#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>
#include <DFRobotDFPlayerMini.h>

// Constants
#define LED_PIN    13
#define NUM_LEDS   5
#define NUM_LEDS 5
#define NUM_BUTTONS 5
#define MAX_SEQUENCE_LENGTH 20

// Enums
enum GameMode {
  MODE_REACTION = 0,
  MODE_FOCUS = 1,
  MODE_MEMORY = 2,
  MODE_CONNECT_PLAYER = 3,
  MODE_DISPLAY_SCORES = 4,
  MODE_DISCONNECT = 5
};


// Structures
struct Player {
  String uid;
  int highScores[3]; // for memory, focus, reaction (total)
  int reactionHighScores[3]; // easy, medium, hard
};

enum ReactionDifficulty { EASY = 0, MEDIUM = 1, HARD = 2 };
extern ReactionDifficulty currentReactionDifficulty;

// Global variables
extern GameMode currentMode;
extern bool playerConnected;
extern int currentPlayer;
extern Player players[3];
extern int playerCount;
extern Preferences prefs;
extern MFRC522 rfid;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern Adafruit_NeoPixel leds;
extern int buttonPins[NUM_BUTTONS];
extern DFRobotDFPlayerMini dfp;


// Reaction game
extern bool reactionStarted;
extern unsigned long lastTrigger;
extern int targetIndex;

// Memory game
extern int memorySequence[MAX_SEQUENCE_LENGTH];
extern int currentSequenceLength;
extern bool gameOver;

// Colors
extern uint32_t colors[NUM_LEDS];

// Mode selection
extern const char* modeNames[6];
extern bool modeConfirmed;
extern int modeIndex;

// DFPlayer Serial
extern HardwareSerial dfSerial;

extern const char* colorNames[NUM_LEDS];


#endif
