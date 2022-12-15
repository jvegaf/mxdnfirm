#include "vu.h"

VU::VU(uint8_t p_data, uint8_t p_latch, uint8_t p_clock)
    : dataPin(p_data), latchPin(p_latch), clockPin(p_clock) {}

void VU::begin() {
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);
  clear();
}

void VU::setLevel(uint16_t level) {
  digitalWrite(latchPin, LOW);
  // shift out lowbyte
  shiftOut(dataPin, clockPin, LSBFIRST, level);
  // shift out highbyte
  shiftOut(dataPin, clockPin, LSBFIRST, (level >> 8));
  digitalWrite(latchPin, HIGH);
}

void VU::clear() { setLevel(0x00); }
