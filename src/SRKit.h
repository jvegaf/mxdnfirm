#pragma once

#include <Arduino.h>

class SRKit {
private:
  uint8_t clockPin;
  uint8_t dataPin;
  uint8_t latchPin;
  uint8_t regsAmount;

public:
  SRKit(uint8_t clkPin, uint8_t dtPin, uint8_t lchPin, uint8_t nRegs);
  void clear();
  void begin();
  void sendState(uint8_t *regs);
};
