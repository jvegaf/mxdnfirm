#pragma once

#include "pin_map.h"
#include <Arduino.h>

class Mux {
private:
  const uint8_t sigPin;
  const uint8_t position;
  uint16_t pState = 0;
  uint16_t cState = 0;

  unsigned long lastdebouncetime = 0;
  unsigned long debouncedelay = 20;

  void setMuxChannel(uint8_t channel);

public:
  Mux(const uint8_t sig, const uint8_t pos) : sigPin(sig), position(pos) {}

  void begin();
  void read(void (*func)());
};
