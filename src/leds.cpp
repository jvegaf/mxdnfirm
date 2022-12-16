#include "leds.h"

namespace Leds
{
Shifter fbLeft(FBL_SIG, FBL_LATCH, SRCLK, tLedsLeft);
Shifter fbRight(FBR_SIG, FBR_LATCH, SRCLK, tLedsRight);

Led fx3Left(&fbLeft, 0);
Led fx2Left(&fbLeft, 1);
Led fx1Left(&fbLeft, 2);
Led playLeft(&fbLeft, 3);
Led cueLeft(&fbLeft, 4);
Led loopLeft(&fbLeft, 5);
Led pCueB(&fbLeft, 6);
Led pCueA(&fbLeft, 7);
Led pCueC(&fbRight, 0);
Led cueRight(&fbRight, 1);
Led playRight(&fbRight, 2);
Led loopRight(&fbRight, 3);
Led fx3Right(&fbRight, 4);
Led fx2Right(&fbRight, 5);
Led fx1Right(&fbRight, 6);

Led fbElements[] {
  fx3Left,
  fx2Left,
  fx1Left,
  playLeft,
  cueLeft,
  loopLeft,
  pCueB,
  pCueA,
  pCueC,
  cueRight,
  playRight,
  loopRight,
  fx3Right,
  fx2Right,
  fx1Right,
};

void begin() {
  fbLeft.clear();
  fbRight.clear();
  fbLeft.write();
  fbRight.write();
}

void setElement(uint8_t pos, uint8_t state) {
  fbElements[pos].setState(state);
}
}
