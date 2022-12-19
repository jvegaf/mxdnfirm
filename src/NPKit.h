#pragma once

#include "np_map.h"
#include "pin_map.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace NPKit {

void begin();
void clear();
void handleChange(uint8_t pos, uint8_t val);
void handleColorChange(uint8_t pos, uint32_t color);
}; // namespace NPKit
