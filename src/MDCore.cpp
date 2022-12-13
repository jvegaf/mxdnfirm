#include "MDCore.h"

namespace MDCore {

byte vuValues[] = {
    B00000000, B00000001, B00000011, B00000111, B00001111,
    B00011111, B00111111, B01111111, B11111111,
};

byte regsVU[] = {0, 0, 0, 0, 0};
int fbRegs = 0;
byte noteSet[] = {0,         MONITOR_CUE_C, PLAY_DECK_B,   CUE_DECK_B,
                  FX2_BTN_3, FX2_BTN_2,     FX2_BTN_1,     0,
                  0,         CUE_DECK_A,    PLAY_DECK_A,   FX1_BTN_1,
                  FX1_BTN_2, FX1_BTN_3,     MONITOR_CUE_A, MONITOR_CUE_B};
byte nSetAmount = 16;
SRKit vuSR(SF_CLOCK, VU_SF_DATA, VU_LATCH, 5);
Shifter fbackSR(FB_SF_DATA, FB_LATCH, SF_CLOCK, 2);
const int npTotal = 21;
NPKit np(NP_SIG_LEFT, npTotal);

void begin() {
  vuSR.begin();
  np.begin();
  setInitialDeckB();
}

void cChange(byte channel, byte number, byte value) {
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

void noteOn(byte channel, byte number, byte value) {
  for (int i = 0; i < nSetAmount; i++) {
    if (noteSet[i] != number) {
      continue;
    }
    fbackSR.setPin(i, HIGH);
    fbackSR.write();
  }
}

void noteOff(byte channel, byte number, byte value) {
  for (int i = 0; i < nSetAmount; i++) {
    if (noteSet[i] != number) {
      continue;
    }
    fbackSR.setPin(i, LOW);
    fbackSR.write();
  }
}

void vuChange(byte number, byte value) {
  if (regsVU[number] != vuValues[value]) {
    regsVU[number] = vuValues[value];
    vuSR.sendState(regsVU);
  }
}

void npChange(byte position, byte value) { np.handleChange(position, value); }

void setInitialDeckB() { np.handleChange(NP_DECK_SEL, 1); }
} // namespace MDCore
