#pragma once

#include "Muxer.h"
#include <Arduino.h>

class PadControl {
public:
  PadControl(Muxer *m) : muxer(m) {}

  void begin();
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);

private:
  Muxer *muxer;
};
