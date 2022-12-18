#include "Deck.h"

void Deck::begin() { muxer->begin(); }

void Deck::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  muxer->read(func, midiCh);
}
