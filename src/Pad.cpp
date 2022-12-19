#include "Pad.h"

void PadControl::begin() { muxer->begin(); }

void PadControl::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  muxer->read(func, midiCh);
}
