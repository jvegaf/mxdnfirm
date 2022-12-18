#pragma once

#include "pin_map.h"
#include "base.h"
#include <Arduino.h>

class Muxer {
private:
  const uint8_t sigPin;
  const uint8_t *elements;
  const uint8_t tElements;
  uint16_t *pState;
  uint16_t *cState;

  uint32_t *lastdebouncetime;
  const uint32_t debouncedelay = 20;

  void setMuxChannel(uint8_t channel);

public:
  Muxer(const uint8_t sig, ElemCollection col)
      : sigPin(sig), elements(col.elements), tElements(col.size) {
    pState = new uint16_t[col.size]();
    cState = new uint16_t[col.size]();
    lastdebouncetime = new uint32_t[col.size]();
  }

  void begin();
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
};
