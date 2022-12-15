#include "MDCore.h"

namespace MDCore {

void begin() {
  np.begin();
  setInitialDeckB();
}

void cChange(uint8_t channel, uint8_t number, uint8_t value) {
  switch (channel) {
  case 1: // np
    npChange(number, value);
    break;
  case 2: // VU
    vuChange(number, value);
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

void vuChange(uint8_t number, uint8_t value) {
  if (regsVU[number] != vuValues[value]) {
    regsVU[number] = vuValues[value];
    vuSR.sendState(regsVU);
  }
}

void npChange(uint8_t position, uint8_t value) {
  np.handleChange(position, value);
}

void setInitialDeckB() { np.handleChange(NP_DECK_SEL, 1); }
} // namespace MDCore
