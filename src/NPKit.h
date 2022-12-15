#pragma once

#include "np_map.h"
#include "pin_map.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

namespace NPKit {

uint32_t const CLEAR_COL = Adafruit_NeoPixel::Color(0, 0, 0);
uint32_t const BLUE_COL = Adafruit_NeoPixel::Color(0, 0, 255);
uint32_t const GREEN_COL = Adafruit_NeoPixel::Color(0, 255, 0);
uint32_t const RED_COL = Adafruit_NeoPixel::Color(255, 0, 0);
uint32_t const YELLOW_COL = Adafruit_NeoPixel::Color(255, 230, 0);
uint32_t const ORANGE_COL = Adafruit_NeoPixel::Color(255, 143, 0);
uint32_t const PURPLE_COL = Adafruit_NeoPixel::Color(243, 0, 255);
uint32_t const GRAY_COL = Adafruit_NeoPixel::Color(191, 201, 202);

void begin();
void clear();
void handleChange(uint8_t pos, uint8_t val);
void handleChange(uint8_t pos, uint32_t color);
}; // namespace NPKit
