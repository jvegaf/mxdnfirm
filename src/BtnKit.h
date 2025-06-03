#pragma once
#include <Arduino.h>

enum class BtnKitError : uint8_t {
  NONE = 0,
  INVALID_ELEMENTS = 1,
  INVALID_COUNT = 2,
  MEMORY_ALLOCATION = 3,
  INVALID_FUNCTION = 4,
  INVALID_INDEX = 5,
  NOT_INITIALIZED = 6
};

struct BtnKitConfig {
  uint32_t debounceDelay;
  bool invertLogic;
  uint8_t inputMode; // INPUT, INPUT_PULLUP, INPUT_PULLDOWN
  
  BtnKitConfig() 
    : debounceDelay(20), invertLogic(false), inputMode(INPUT_PULLUP) {}
};

class BtnKit {
private:
  const uint8_t *elements;
  const uint8_t t_elements;
  uint16_t *pState;
  uint16_t *cState;
  uint32_t *lastDebounceTime;
  
  // Static members for just pressed/released state
  static bool justPressedArray[64];
  static bool justReleasedArray[64];

  BtnKitConfig config;
  BtnKitError lastError;
  bool initialized;
  
  bool validateIndex(uint8_t index) const;
  bool isPressed(uint16_t state) const;

public:
  BtnKit(const uint8_t *elem, const uint8_t t_elem);
  ~BtnKit();
  
  // Configuration methods
  bool begin();
  bool setConfig(const BtnKitConfig& newConfig);
  BtnKitConfig getConfig() const { return config; }
  
  // Status methods
  bool isReady() const { return initialized; }
  BtnKitError getLastError() const { return lastError; }
  void clearError() { lastError = BtnKitError::NONE; }
  
  // Main functionality
  bool read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
  
  // Utility methods
  uint8_t getElementCount() const { return t_elements; }
  bool isButtonPressed(uint8_t index) const;
  bool isButtonJustPressed(uint8_t index) const;
  bool isButtonJustReleased(uint8_t index) const;
  uint32_t getLastPressTime(uint8_t index) const;
};
