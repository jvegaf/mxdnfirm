#include "NPKit.h"
#include "pin_map.h"

namespace NPKit {
Adafruit_NeoPixel np(tPixels, NP_DATA, NEO_GRB + NEO_KHZ800);

const byte BRIGHTNESS = 32;

// Range: -1 to 5 (-1 = no hotcue, 0 = Cue, 1 = FadeIn, 2 = FadeOut, 3 = Load, 4
// = Grid, 5 = Loop) mapped 0 to 6
uint32_t HCCols[] = {
    CLEAR_COL,  // no hotcue
    BLUE_COL,   // cue
    ORANGE_COL, // fade in
    ORANGE_COL, // fade out
    YELLOW_COL, // load
    GRAY_COL,   // grid
    GREEN_COL   // loop
};

void begin() {
  np.setBrightness(BRIGHTNESS);
  np.begin();
  np.clear();
}

void handleChange(uint8_t pos, uint8_t val) {
  np.setPixelColor(pos, HCCols[val]);
  np.show();
}

void clear() {
  np.clear();
  np.show();
}
} // namespace NPKit
