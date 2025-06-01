
#pragma once
#include <Arduino.h>

enum class PotKitError : uint8_t {
  NONE = 0,
  INVALID_ELEMENTS = 1,
  INVALID_COUNT = 2,
  MEMORY_ALLOCATION = 3,
  INVALID_FUNCTION = 4,
  INVALID_INDEX = 5
};

struct PotKitConfig {
  uint16_t threshold;
  uint16_t timeout;
  uint16_t minAnalog;
  uint16_t maxAnalog;
  uint8_t minMidi;
  uint8_t maxMidi;
  
  PotKitConfig() 
    : threshold(8), timeout(50), minAnalog(0), maxAnalog(1023), 
      minMidi(0), maxMidi(127) {}
};

class PotKit {
private:
  const uint8_t *elements;
  const uint8_t t_elements;
  uint16_t *potCState; // current state
  uint16_t *potPState; // previous state
  uint8_t *lastCcValue;
  uint32_t *pTime;
  uint32_t *timer;
  
  PotKitConfig config;
  PotKitError lastError;
  bool initialized;

  bool validateIndex(uint8_t index) const;
  uint8_t mapToMidi(uint16_t analogValue) const;

public:
  PotKit(const uint8_t *el, const uint8_t t_el);
  ~PotKit();
  
  // Configuration methods
  bool begin();
  bool setConfig(const PotKitConfig& newConfig);
  PotKitConfig getConfig() const { return config; }
  
  // Status methods
  bool isReady() const { return initialized; }
  PotKitError getLastError() const { return lastError; }
  void clearError() { lastError = PotKitError::NONE; }
  
  // Main functionality
  bool read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
  
  // Utility methods
  uint8_t getElementCount() const { return t_elements; }
  uint16_t getCurrentValue(uint8_t index) const;
  uint8_t getCurrentMidiValue(uint8_t index) const;
  bool isElementActive(uint8_t index) const;
};
