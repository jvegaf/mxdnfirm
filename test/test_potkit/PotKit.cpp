#include "PotKit.h"

PotKit::PotKit(const uint8_t *el, const uint8_t t_el)
    : elements(el), t_elements(t_el), lastError(PotKitError::NONE), initialized(false),
      pTime(nullptr), timer(nullptr), potCState(nullptr), potPState(nullptr), lastCcValue(nullptr) {
  // Validate inputs
  if (el == nullptr) {
    lastError = PotKitError::INVALID_ELEMENTS;
    return;
  }
  
  if (t_el == 0 || t_el > 32) { // Reasonable upper limit
    lastError = PotKitError::INVALID_COUNT;
    return;
  }
  
  // Allocate memory
  try {
    pTime = new uint32_t[t_el]();
    timer = new uint32_t[t_el]();
    potCState = new uint16_t[t_el]();
    potPState = new uint16_t[t_el]();
    lastCcValue = new uint8_t[t_el]();
  } catch (...) {
    lastError = PotKitError::MEMORY_ALLOCATION;
    // Clean up partial allocations
    delete[] pTime;
    delete[] timer;
    delete[] potCState;
    delete[] potPState;
    delete[] lastCcValue;
    pTime = nullptr;
    timer = nullptr;
    potCState = nullptr;
    potPState = nullptr;
    lastCcValue = nullptr;
    return;
  }
  
  // Initialize arrays to safe values
  for (uint8_t i = 0; i < t_elements; i++) {
    pTime[i] = 0;
    timer[i] = 0;
    potCState[i] = 0;
    potPState[i] = 0;
    lastCcValue[i] = 255; // Invalid MIDI value to force first update
  }
}

PotKit::~PotKit() {
  delete[] pTime;
  delete[] timer;
  delete[] potCState;
  delete[] potPState;
  delete[] lastCcValue;
}

bool PotKit::begin() {
  if (lastError != PotKitError::NONE) {
    return false;
  }
  
  // Initialize pins as inputs (if needed)
  for (uint8_t i = 0; i < t_elements; i++) {
    // Analog pins don't need pinMode setup for reading
    potPState[i] = analogRead(elements[i]);
    potCState[i] = potPState[i];
    lastCcValue[i] = mapToMidi(potCState[i]);
    pTime[i] = millis();
  }
  
  initialized = true;
  return true;
}

bool PotKit::setConfig(const PotKitConfig& newConfig) {
  if (newConfig.threshold > 512 || newConfig.timeout > 10000) {
    lastError = PotKitError::INVALID_ELEMENTS;
    return false;
  }
  
  if (newConfig.minAnalog >= newConfig.maxAnalog || 
      newConfig.minMidi >= newConfig.maxMidi) {
    lastError = PotKitError::INVALID_ELEMENTS;
    return false;
  }
  
  config = newConfig;
  return true;
}

bool PotKit::validateIndex(uint8_t index) const {
  if (index >= t_elements) {
    return false;
  }
  return true;
}

uint8_t PotKit::mapToMidi(uint16_t analogValue) const {
  // Constrain input to valid range
  if (analogValue < config.minAnalog) analogValue = config.minAnalog;
  if (analogValue > config.maxAnalog) analogValue = config.maxAnalog;
  
  return map(analogValue, config.minAnalog, config.maxAnalog, 
             config.minMidi, config.maxMidi);
}

bool PotKit::read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!initialized) {
    lastError = PotKitError::INVALID_ELEMENTS;
    return false;
  }
  
  if (scc_func == nullptr) {
    lastError = PotKitError::INVALID_FUNCTION;
    return false;
  }
  
  if (midiCh > 15) { // MIDI channels are 0-15
    lastError = PotKitError::INVALID_ELEMENTS;
    return false;
  }
  
  for (uint8_t i = 0; i < t_elements; i++) {
    potCState[i] = analogRead(elements[i]);
    uint16_t potVar = abs(potCState[i] - potPState[i]);

    if (potVar >= config.threshold) {
      pTime[i] = millis();
    }
    
    timer[i] = millis() - pTime[i];
    
    if (timer[i] < config.timeout) {
      uint8_t ccValue = mapToMidi(potCState[i]);

      if (lastCcValue[i] != ccValue) {
        scc_func(i, ccValue, midiCh);
        potPState[i] = potCState[i];
        lastCcValue[i] = ccValue;
      }
    }
  }
  
  return true;
}

uint16_t PotKit::getCurrentValue(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return 0;
  }
  return potCState[index];
}

uint8_t PotKit::getCurrentMidiValue(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return 0;
  }
  return lastCcValue[index];
}

bool PotKit::isElementActive(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return false;
  }
  return (millis() - pTime[index]) < config.timeout;
}
