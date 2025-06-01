#include "Deck.h"

bool DeckControl::begin() { 
  if (muxer == nullptr) {
    return false;
  }
  
  muxer->begin(); 
  isInitialized = true;
  return true;
}

void DeckControl::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!isInitialized || func == nullptr) {
    return;
  }
  
  muxer->read(func, midiCh);
}
