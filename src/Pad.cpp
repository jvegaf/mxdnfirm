#include "Pad.h"

bool PadControl::begin() { 
  if (muxer == nullptr) {
    isInitialized = false;
    return false;
  }
  
  try {
    muxer->begin(); 
    isInitialized = true;
    lastReadTime = millis();
    return true;
  } catch (...) {
    isInitialized = false;
    return false;
  }
}

void PadControl::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!validateParameters(func)) {
    return;
  }
  
  lastReadTime = millis();
  muxer->read(func, midiCh);
}

unsigned long PadControl::getTimeSinceLastRead() const {
  return millis() - lastReadTime;
}

void PadControl::reset() {
  lastReadTime = millis();
  // Here other internal states could be reset if necessary
}

bool PadControl::validateParameters(void (*func)(uint8_t, uint8_t, uint8_t)) const {
  return isInitialized && func != nullptr && muxer != nullptr;
}
