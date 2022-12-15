#pragma once

#include <Arduino.h>

class VU {
public:
  VU(uint8_t p_data, uint8_t p_latch, uint8_t p_clock);
  void begin();
  void setLevel(uint16_t level);

private:
  uint8_t dataPin;
  uint8_t latchPin;
  uint8_t clockPin;
  void clear();
};
