#include "MDCore.h"
#include "base.h"
#include "midi_map.h"
#include <stdint.h>

namespace MDCore {

// Error tracking
static bool isInitialized = false;
static uint8_t lastError = 0;

// Constants for better maintainability
static const uint8_t MAX_PAD_VALUE = 127;
static const uint8_t DEFAULT_PAD_BRIGHTNESS = 1;

void setPadsColor(Location loc, uint8_t value) {
  // Validate input parameters
  if (value > MAX_PAD_VALUE) {
    value = MAX_PAD_VALUE;
  }
  
  if (loc == Location::Left) {
    for (uint8_t i = 0; i < tPxPad; i++) {
      NPKit::handleChange(pxPadLeft[i], value);
    }
    return;
  }
  
  // Handle Right location
  for (uint8_t i = 0; i < tPxPad; i++) {
    NPKit::handleChange(pxPadRight[i], value);
  }
}

bool begin() {
  try {
    VUmeters::begin();
    NPKit::begin();
    Leds::begin();
    changeDeck(deckBChannel);
    setPadsColor(Location::Left, DEFAULT_PAD_BRIGHTNESS);
    setPadsColor(Location::Right, DEFAULT_PAD_BRIGHTNESS);
    
    isInitialized = true;
    return true;
  } catch (...) {
    isInitialized = false;
    lastError = 1; // Initialization error
    return false;
  }
}

void cChange(uint8_t channel, uint8_t number, uint8_t value) {
  if (!isInitialized) {
    lastError = 2; // Not initialized error
    return;
  }
  
  // Validate MIDI value range
  if (value > 127) {
    value = 127;
  }
  
  switch (channel) {
  case 1: // NeoPixel control
    NPKit::handleChange(number, value);
    break;
    
  case 2: // VU meters
    VUmeters::setLevel(number, value);
    break;
    
  default:
    lastError = 3; // Unknown channel error
    break;
  }
}

void noteOn(uint8_t channel, uint8_t number, uint8_t value) {
  if (!isInitialized) {
    lastError = 2;
    return;
  }
  
  // Validate velocity
  if (value > 127) {
    value = 127;
  }
  
  Leds::setElement(number, HIGH);
}

void noteOff(uint8_t channel, uint8_t number, uint8_t value) {
  if (!isInitialized) {
    lastError = 2;
    return;
  }
  
  Leds::setElement(number, LOW);
}

void changeDeck(uint8_t deck) {
  if (!isInitialized) {
    lastError = 2;
    return;
  }
  
  uint8_t color = deckBColor;
  if (deck == deckCChannel) {
    color = deckCColor;
  }

  NPKit::handleChange(NP_DECK_SEL, color);
}

// Utility functions for testing and debugging
bool isReady() {
  return isInitialized;
}

uint8_t getLastError() {
  return lastError;
}

void clearError() {
  lastError = 0;
}
} // namespace MDCore
