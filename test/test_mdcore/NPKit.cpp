#include "NPKit.h"
#include <stdint.h>
#include <cstdio>

namespace NPKit {

// Mock internal state
static NPKitConfig currentConfig;
static NPKitError lastError = NPKitError::NONE;
static bool initialized = false;
static uint32_t mockPixels[tPixels];

// Mock color constants (simple RGB encoding)
const uint32_t CLEAR_COL = 0x000000;
const uint32_t BLUE_COL = 0x0000FF;
const uint32_t GREEN_COL = 0x00FF00;
const uint32_t RED_COL = 0xFF0000;
const uint32_t YELLOW_COL = 0xFFE600;
const uint32_t ORANGE_COL = 0xFF8F00;
const uint32_t PURPLE_COL = 0xF300FF;
const uint32_t GRAY_COL = 0xBFC9CA;

// Simple color mapping
static const uint32_t Colors[] = {
  CLEAR_COL,  // 0
  BLUE_COL,   // 1
  ORANGE_COL, // 2
  YELLOW_COL, // 3
  GRAY_COL,   // 4
  GREEN_COL,  // 5
  RED_COL,    // 6
  PURPLE_COL  // 7
};

static const uint8_t NUM_COLORS = sizeof(Colors) / sizeof(Colors[0]);

bool begin() {
  return begin(NPKitConfig());
}

bool begin(const NPKitConfig& config) {
  lastError = NPKitError::NONE;
  currentConfig = config;
  
  // Mock initialization
  for (uint8_t i = 0; i < tPixels; i++) {
    mockPixels[i] = 0;
  }
  
  initialized = true;
  return true;
}

bool isValidPosition(uint8_t pos) {
  return (pos < tPixels);
}

bool clear() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  for (uint8_t i = 0; i < tPixels; i++) {
    mockPixels[i] = 0;
  }
  return true;
}

bool show() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  // Mock show - just return success
  return true;
}

bool handleChange(uint8_t pos, uint8_t val) {
  printf("DEBUG: handleChange called with pos=%d, val=%d\n", pos, val);
  printf("DEBUG: initialized=%d, tPixels=%d, NUM_COLORS=%d\n", initialized, tPixels, NUM_COLORS);
  
  if (!initialized) {
    printf("DEBUG: Not initialized error\n");
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    printf("DEBUG: Invalid position error - pos=%d >= tPixels=%d\n", pos, tPixels);
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  if (val >= NUM_COLORS) {
    printf("DEBUG: Invalid value error - val=%d >= NUM_COLORS=%d\n", val, NUM_COLORS);
    lastError = NPKitError::INVALID_VALUE;
    return false;
  }
  
  printf("DEBUG: About to set pixel %d to color 0x%06X\n", pos, Colors[val]);
  mockPixels[pos] = Colors[val];
  return true;
}

bool handleColorChange(uint8_t pos, uint32_t color) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  mockPixels[pos] = color;
  return true;
}

bool setPixelColor(uint8_t pos, uint32_t color) {
  return handleColorChange(pos, color);
}

bool setPixelColor(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
  uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  return setPixelColor(pos, color);
}

uint32_t getPixelColor(uint8_t pos) {
  if (!isValidPosition(pos)) {
    return 0;
  }
  return mockPixels[pos];
}

NPKitError getLastError() {
  return lastError;
}

bool isInitialized() {
  return initialized;
}

uint8_t getPixelCount() {
  return tPixels;
}

bool setBrightness(uint8_t brightness) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  currentConfig.brightness = brightness;
  return true;
}

uint8_t getBrightness() {
  return currentConfig.brightness;
}

bool setConfig(const NPKitConfig& config) {
  currentConfig = config;
  return true;
}

NPKitConfig getConfig() {
  return currentConfig;
}

uint32_t colorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

bool setPixelColorFromRGB(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
  return setPixelColor(pos, r, g, b);
}

bool setAllPixels(uint32_t color) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  for (uint8_t i = 0; i < tPixels; i++) {
    mockPixels[i] = color;
  }
  return true;
}

bool fadePixel(uint8_t pos, uint8_t fadeAmount) {
  // Mock implementation - just reduce brightness
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  uint32_t color = mockPixels[pos];
  uint8_t r = (color >> 16) & 0xFF;
  uint8_t g = (color >> 8) & 0xFF;
  uint8_t b = color & 0xFF;
  
  r = (r > fadeAmount) ? r - fadeAmount : 0;
  g = (g > fadeAmount) ? g - fadeAmount : 0;
  b = (b > fadeAmount) ? b - fadeAmount : 0;
  
  mockPixels[pos] = ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
  return true;
}

bool setRainbow() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  // Mock rainbow - just set different colors
  for (uint8_t i = 0; i < tPixels; i++) {
    mockPixels[i] = Colors[i % NUM_COLORS];
  }
  return true;
}

bool setTestPattern() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  // Mock test pattern
  for (uint8_t i = 0; i < tPixels; i++) {
    mockPixels[i] = (i % 2) ? RED_COL : BLUE_COL;
  }
  return true;
}

bool setColorFromValue(uint8_t pos, uint8_t value) {
  return handleChange(pos, value);
}

} // namespace NPKit
