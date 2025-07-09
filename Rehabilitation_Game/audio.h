#ifndef AUDIO_H
#define AUDIO_H

#include <Arduino.h>
#include "globals.h"          // for dfSerial & button pins

/********  YX5200 / Open-Smart short-frame commands  ********/

namespace Audio {

// Low-level sender
inline void sendCmd(const uint8_t *buf, size_t len) {
  dfSerial.write(buf, len);
}

// --- one-time init ---
inline void selectTF() {                     // choose SD card
  static const uint8_t CMD[] = {0x7E, 0x03, 0x35, 0x01, 0xEF};
  sendCmd(CMD, sizeof(CMD));
  delay(200);
}
inline void volume(uint8_t v) {              // 0-30
  uint8_t cmd[] = {0x7E, 0x03, 0x31, v, 0xEF};
  sendCmd(cmd, sizeof(cmd));
  delay(50);
}

// --- play n in folder 01 ---
inline void playTrack(uint8_t n) {           // n = 1-255
  uint8_t cmd[] = {0x7E, 0x04, 0x41, 0x00, n, 0xEF};
  sendCmd(cmd, sizeof(cmd));
}

// High-level aliases
enum SoundID { SND_CORRECT = 2, SND_WRONG = 1, SND_GAMEOVER = 3, SND_WIN = 4, SND_RED = 5, SND_GREEN = 6, SND_BLUE = 7, SND_YELLOW = 8, SND_WHITE = 9};
inline void playSound(SoundID id) { playTrack(id); }
inline void playColor(int index) { playTrack(5 + index); }

inline void playCorrect()   { playSound(SND_CORRECT); }
inline void playWrong()     { playSound(SND_WRONG);   }
inline void playGameOver()  { playSound(SND_GAMEOVER);}
inline void playWin()       { playSound(SND_WIN);     }

}

#endif
