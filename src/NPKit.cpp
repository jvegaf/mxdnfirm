#include "NPKit.h"

namespace NPKit {

// Internal state
static Adafruit_NeoPixel np(tPixels, NP_DATA, NEO_GRB + NEO_KHZ800);
static NPKitConfig currentConfig;
static NPKitError lastError = NPKitError::NONE;
static bool initialized = false;

// Color constants
const uint32_t CLEAR_COL = Adafruit_NeoPixel::Color(0, 0, 0);
const uint32_t BLUE_COL = Adafruit_NeoPixel::Color(0, 0, 255);
const uint32_t GREEN_COL = Adafruit_NeoPixel::Color(0, 255, 0);
const uint32_t RED_COL = Adafruit_NeoPixel::Color(255, 0, 0);
const uint32_t YELLOW_COL = Adafruit_NeoPixel::Color(255, 230, 0);
const uint32_t ORANGE_COL = Adafruit_NeoPixel::Color(255, 143, 0);
const uint32_t PURPLE_COL = Adafruit_NeoPixel::Color(243, 0, 255);
const uint32_t GRAY_COL = Adafruit_NeoPixel::Color(191, 201, 202);

// Color mapping for different cue types
// Range: -1 to 5 (-1 = no hotcue, 0 = Cue, 1 = FadeIn, 2 = FadeOut, 3 = Load, 4 = Grid, 5 = Loop)
static const uint32_t Colors[] = {
  CLEAR_COL,  // 0: no hotcue
  BLUE_COL,   // 1: cue
  ORANGE_COL, // 2: fade out
  YELLOW_COL, // 3: load
  GRAY_COL,   // 4: grid
  GREEN_COL,  // 5: loop
  RED_COL,    // 6: additional
  PURPLE_COL  // 7: additional
};

static const uint8_t NUM_COLORS = sizeof(Colors) / sizeof(Colors[0]);

bool begin() {
  return begin(NPKitConfig());
}

bool begin(const NPKitConfig& config) {
  lastError = NPKitError::NONE;
  currentConfig = config;
  
  try {
    // Initialize NeoPixel safely
    np.setBrightness(currentConfig.brightness);
    np.begin();
    
    
    np.clear();
      
    if (currentConfig.autoShow) {
      np.show();
    }
    
    
    initialized = true;
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    initialized = false;
    return false;
  }
}

bool setConfig(const NPKitConfig& config) {
  if (config.brightness > 255) {
    lastError = NPKitError::INVALID_VALUE;
    return false;
  }
  
  currentConfig = config;
  
  if (initialized) {
    np.setBrightness(currentConfig.brightness);
  }
  
  return true;
}

NPKitConfig getConfig() {
  return currentConfig;
}

bool isReady() {
  return initialized;
}

NPKitError getLastError() {
  return lastError;
}

void clearError() {
  lastError = NPKitError::NONE;
}

bool isValidPosition(uint8_t pos) {
  return (pos < tPixels);
}

bool clear() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  try {
    np.clear();
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool show() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  try {
    np.show();
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool handleChange(uint8_t pos, uint8_t val) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  if (val >= NUM_COLORS) {
    lastError = NPKitError::INVALID_VALUE;
    return false;
  }
  
  return setPixelColor(pos, Colors[val]);
}

bool handleColorChange(uint8_t pos, uint32_t color) {
  return setPixelColor(pos, color);
}

bool setPixelColor(uint8_t pos, uint32_t color) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  try {
    np.setPixelColor(pos, color);
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool setPixelColor(uint8_t pos, uint8_t r, uint8_t g, uint8_t b) {
  return setPixelColor(pos, Adafruit_NeoPixel::Color(r, g, b));
}

uint32_t getPixelColor(uint8_t pos) {
  if (!initialized || !isValidPosition(pos)) {
    return 0;
  }
  
  return np.getPixelColor(pos);
}

bool setBrightness(uint8_t brightness) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  currentConfig.brightness = brightness;
  
  try {
    np.setBrightness(brightness);
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

uint8_t getBrightness() {
  return currentConfig.brightness;
}

uint8_t getPixelCount() {
  return tPixels;
}

uint32_t colorFromRGB(uint8_t r, uint8_t g, uint8_t b) {
  return Adafruit_NeoPixel::Color(r, g, b);
}

bool setAllPixels(uint32_t color) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  try {
    for (uint8_t i = 0; i < tPixels; i++) {
      np.setPixelColor(i, color);
    }
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool fadePixel(uint8_t pos, uint8_t steps) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  uint32_t currentColor = np.getPixelColor(pos);
  
  // Extract RGB components
  uint8_t r = (currentColor >> 16) & 0xFF;
  uint8_t g = (currentColor >> 8) & 0xFF;
  uint8_t b = currentColor & 0xFF;
  
  // Fade by reducing each component
  r = (r > steps) ? r - steps : 0;
  g = (g > steps) ? g - steps : 0;
  b = (b > steps) ? b - steps : 0;
  
  return setPixelColor(pos, r, g, b);
}

bool setRainbow() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  try {
    for (uint8_t i = 0; i < tPixels; i++) {
      // Create rainbow effect
      uint8_t hue = (255 * i) / tPixels;
      // Simple HSV to RGB conversion for rainbow
      uint8_t r, g, b;
      
      if (hue < 85) {
        r = hue * 3;
        g = 255 - hue * 3;
        b = 0;
      } else if (hue < 170) {
        hue -= 85;
        r = 255 - hue * 3;
        g = 0;
        b = hue * 3;
      } else {
        hue -= 170;
        r = 0;
        g = hue * 3;
        b = 255 - hue * 3;
      }
      
      np.setPixelColor(i, Adafruit_NeoPixel::Color(r, g, b));
    }
    
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool setTestPattern() {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  try {
    const uint32_t testColors[] = {RED_COL, GREEN_COL, BLUE_COL, YELLOW_COL};
    const uint8_t numTestColors = sizeof(testColors) / sizeof(testColors[0]);
    
    for (uint8_t i = 0; i < tPixels; i++) {
      np.setPixelColor(i, testColors[i % numTestColors]);
    }
    
    if (currentConfig.autoShow) {
      np.show();
    }
    return true;
  } catch (...) {
    lastError = NPKitError::HARDWARE_ERROR;
    return false;
  }
}

bool pulsePixel(uint8_t pos, uint32_t color, uint8_t intensity) {
  if (!initialized) {
    lastError = NPKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (!isValidPosition(pos)) {
    lastError = NPKitError::INVALID_POSITION;
    return false;
  }
  
  // Extract RGB components
  uint8_t r = ((color >> 16) & 0xFF) * intensity / 255;
  uint8_t g = ((color >> 8) & 0xFF) * intensity / 255;
  uint8_t b = (color & 0xFF) * intensity / 255;
  
  return setPixelColor(pos, r, g, b);
}

// Añadimos la función cleanup
void cleanup() {
  if (initialized) {
    clear(); // Usamos clear() en lugar de clearPixels()
    show();
    initialized = false;
  }
  lastError = NPKitError::NONE;
}

} // namespace NPKit
