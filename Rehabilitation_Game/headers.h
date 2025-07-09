// headers.h

#ifndef HEADERS_H
#define HEADERS_H

#include "globals.h"

// reaction_game.h
void runReactionGame();

// focus_game.h
void runFocusGame();

// memory_game.h
void runMemoryGame();
void generateSequence();
void playSequence();
void getPlayerInput();

// rfid_handler.h
void handleRFID();

// mode_selection.h
void showModeSelection();

// scores_display.h
void showPlayerScores();

#endif // HEADERS_H
