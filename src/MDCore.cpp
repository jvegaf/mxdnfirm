#include "MDCore.h"
#include "base.h"
#include "midi_map.h"
#include <stdint.h>

namespace MDCore {

void setPadsColor(Location loc, uint8_t value) {
  if (loc == Location::Left) {
    for (uint8_t i = 0; i < tPxPad; i++) {
      NPKit::handleChange(pxPadLeft[i], value);
    }
    return;
  }
  for (uint8_t i = 0; i < tPxPad; i++) {
    NPKit::handleChange(pxPadRight[i], value);
  }
}

void begin() {
  VUmeters::begin();
  NPKit::begin();
  Leds::begin();
  changeDeck(deckBChannel);
  setPadsColor(Location::Left, 1);
  setPadsColor(Location::Right, 1);
}

void cChange(uint8_t channel, uint8_t number, uint8_t value) {
  switch (channel) {
  case 1: // np
    NPKit::handleChange(number, value);

    break;
  case 2: // VU
    VUmeters::setLevel(number, value);
    break;

  default:
    break;
  }
}

void noteOn(uint8_t channel, uint8_t number, uint8_t value) {
  Leds::setElement(number, HIGH);
}

void noteOff(uint8_t channel, uint8_t number, uint8_t value) {
  Leds::setElement(number, LOW);
}

void changeDeck(uint8_t deck) {
  uint8_t color = deckBColor;
  if (deck == deckCChannel) {
    color = deckCColor;
  }

  NPKit::handleChange(NP_DECK_SEL, color);
}
} // namespace MDCore
