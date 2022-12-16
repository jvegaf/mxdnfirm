#pragma once

#include "pin_map.h"
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
  Muxer(const uint8_t sig, const uint8_t *el, const uint8_t t_el)
      : sigPin(sig), elements(el), tElements(t_el) {
    pState = new uint16_t[t_el]();
    cState = new uint16_t[t_el]();
    lastdebouncetime = new uint32_t[t_el]();
  }

  void begin();
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
};
