#include "MDCore.h"
#include "NPKit.h"
#include "vumeters.h"

namespace MDCore {

void setInitialDeckB() { NPKit::handleChange(NP_DECK_SEL, 1); }

void begin() {
  VUmeters::begin();
  NPKit::begin();
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
  for (int i = 0; i < nSetAmount; i++) {
    if (noteSet[i] != number) {
      continue;
    }
    fbackSR.setPin(i, HIGH);
    fbackSR.write();
  }
}

void noteOff(uint8_t channel, uint8_t number, uint8_t value) {
  for (int i = 0; i < nSetAmount; i++) {
    if (noteSet[i] != number) {
      continue;
    }
    fbackSR.setPin(i, LOW);
    fbackSR.write();
  }
}

} // namespace MDCore
