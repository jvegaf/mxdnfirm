#pragma once

#include "Muxer.h"
#include <Arduino.h>

class DeckControl {
public:
  DeckControl(Muxer *m) : muxer(m), isInitialized(false) {}

  bool begin();  // Return bool for error checking
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
  bool isReady() const { return isInitialized; }

private:
  Muxer *muxer;
  bool isInitialized;
};
