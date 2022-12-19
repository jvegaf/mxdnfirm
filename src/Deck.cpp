#include "Deck.h"

void DeckControl::begin() { muxer->begin(); }

void DeckControl::read(void (*func)(uint8_t, uint8_t, uint8_t),
                       uint8_t midiCh) {
  muxer->read(func, midiCh);
}
