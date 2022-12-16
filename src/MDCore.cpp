#include "MDCore.h"

namespace MDCore {

void setInitialDeckB() { NPKit::handleChange(NP_DECK_SEL, 1); }

void begin() {
  VUmeters::begin();
  NPKit::begin();
  Leds::begin();
  setInitialDeckB();
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
  Leds::setElement(number, State::On);
}

void noteOff(uint8_t channel, uint8_t number, uint8_t value) {
  Leds::setElement(number, State::Off);
}

} // namespace MDCore
