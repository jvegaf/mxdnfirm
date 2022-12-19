#include "NPKit.h"
#include <stdint.h>

namespace NPKit {
Adafruit_NeoPixel np(tPixels, NP_DATA, NEO_GRB + NEO_KHZ800);

uint32_t const CLEAR_COL = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t const BLUE_COL = Adafruit_NeoPixel::Color(0, 0, 255);
uint32_t const GREEN_COL = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t const RED_COL = Adafruit_NeoPixel::Color(255, 0, 0);
uint32_t const YELLOW_COL = Adafruit_NeoPixel::Color(255, 230, 0);
uint32_t const ORANGE_COL = Adafruit_NeoPixel::Color(255, 143, 0);
uint32_t const PURPLE_COL = Adafruit_NeoPixel::Color(243, 0, 255);
uint32_t const GRAY_COL = Adafruit_NeoPixel::Color(191, 201, 202);

const uint8_t BRIGHTNESS = 32;

// Range: -1 to 5 (-1 = no hotcue, 0 = Cue, 1 = FadeIn, 2 = FadeOut, 3 = Load, 4
// = Grid, 5 = Loop) mapped 0 to 6
uint32_t Colors[] = {CLEAR_COL,  // no hotcue
                     BLUE_COL,   // cue
                     ORANGE_COL, // fade out
                     YELLOW_COL, // load
                     GRAY_COL,   // grid
                     GREEN_COL,  // loop
                     RED_COL,    PURPLE_COL};

void begin() {
  np.setBrightness(BRIGHTNESS);
  np.begin();
  np.clear();
}

void handleChange(uint8_t pos, uint8_t val) {
  np.setPixelColor(pos, Colors[val]);
  np.show();
}

void clear() {
  np.clear();
  np.show();
}
} // namespace NPKit
