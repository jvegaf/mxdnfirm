#pragma once

#include "Muxer.h"
#include <Arduino.h>

class DeckControl {
public:
  DeckControl(Muxer *m) : muxer(m) {}

  void begin();
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);

private:
  Muxer *muxer;
};
