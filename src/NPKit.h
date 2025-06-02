#pragma once

#include "np_map.h"
#include "pin_map.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>

enum class NPKitError : uint8_t {
  NONE = 0,
  INVALID_POSITION = 1,
  INVALID_COLOR = 2,
  INVALID_VALUE = 3,
  NOT_INITIALIZED = 4,
  HARDWARE_ERROR = 5
};

struct NPKitConfig {
  uint8_t brightness;
  bool autoShow;
  uint32_t defaultColor;
  
  NPKitConfig() 
    : brightness(32), autoShow(true), defaultColor(0) {}
};

namespace NPKit {
  // Color constants
  extern const uint32_t CLEAR_COL;
  extern const uint32_t BLUE_COL;
  extern const uint32_t GREEN_COL;
  extern const uint32_t RED_COL;
  extern const uint32_t YELLOW_COL;
  extern const uint32_t ORANGE_COL;
  extern const uint32_t PURPLE_COL;
  extern const uint32_t GRAY_COL;
  
  // Initialization and configuration
  bool begin();
  bool begin(const NPKitConfig& config);
  bool setConfig(const NPKitConfig& config);
  NPKitConfig getConfig();
  
  // Cleanup resources
  void cleanup();
  
  // Status methods
  bool isReady();
  NPKitError getLastError();
  void clearError();
  
  // Main functionality
  bool clear();
  bool show();
  bool handleChange(uint8_t pos, uint8_t val);
  bool handleColorChange(uint8_t pos, uint32_t color);
  
  // Advanced functionality
  bool setPixelColor(uint8_t pos, uint32_t color);
  bool setPixelColor(uint8_t pos, uint8_t r, uint8_t g, uint8_t b);
  uint32_t getPixelColor(uint8_t pos);
  bool setBrightness(uint8_t brightness);
  uint8_t getBrightness();
  
  // Utility methods
  uint8_t getPixelCount();
  bool isValidPosition(uint8_t pos);
  uint32_t colorFromRGB(uint8_t r, uint8_t g, uint8_t b);
  bool setAllPixels(uint32_t color);
  bool fadePixel(uint8_t pos, uint8_t steps);
  
  // Preset patterns
  bool setRainbow();
  bool setTestPattern();
  bool pulsePixel(uint8_t pos, uint32_t color, uint8_t intensity);
};
