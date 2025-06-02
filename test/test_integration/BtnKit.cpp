#include "BtnKit.h"
#include <stdint.h>

BtnKit::BtnKit(const uint8_t *elem, const uint8_t t_elem)
    : elements(elem), t_elements(t_elem), pState(nullptr), cState(nullptr), 
      lastDebounceTime(nullptr), lastError(BtnKitError::NONE), initialized(false) {
  
  // Validate inputs
  if (elem == nullptr) {
    lastError = BtnKitError::INVALID_ELEMENTS;
    return;
  }
  
  if (t_elem == 0 || t_elem > 64) { // Reasonable upper limit
    lastError = BtnKitError::INVALID_COUNT;
    return;
  }
  
  // Allocate memory
  pState = new uint16_t[t_elem]();
  if (pState == nullptr) {
    lastError = BtnKitError::MEMORY_ALLOCATION;
    return;
  }
  
  cState = new uint16_t[t_elem]();
  if (cState == nullptr) {
    lastError = BtnKitError::MEMORY_ALLOCATION;
    delete[] pState;
    pState = nullptr;
    return;
  }
  
  lastDebounceTime = new uint32_t[t_elem]();
  if (lastDebounceTime == nullptr) {
    lastError = BtnKitError::MEMORY_ALLOCATION;
    delete[] pState;
    delete[] cState;
    pState = nullptr;
    cState = nullptr;
    return;
  }
  
  // Initialize arrays
  for (uint8_t i = 0; i < t_elements; i++) {
    pState[i] = HIGH; // Default state for pullup
    cState[i] = HIGH;
    lastDebounceTime[i] = 0;
  }
}

BtnKit::~BtnKit() {
  delete[] pState;
  delete[] cState;
  delete[] lastDebounceTime;
}

bool BtnKit::begin() {
  if (lastError != BtnKitError::NONE) {
    return false;
  }
  
  // Initialize pins
  for (uint8_t i = 0; i < t_elements; i++) {
    pinMode(elements[i], config.inputMode);
    
    // Read initial state
    uint16_t initialState = digitalRead(elements[i]);
    pState[i] = initialState;
    cState[i] = initialState;
    lastDebounceTime[i] = millis();
  }
  
  initialized = true;
  return true;
}

bool BtnKit::setConfig(const BtnKitConfig& newConfig) {
  if (newConfig.debounceDelay > 1000) { // Max 1 second debounce
    lastError = BtnKitError::INVALID_ELEMENTS;
    return false;
  }
  
  if (newConfig.inputMode != INPUT && 
      newConfig.inputMode != INPUT_PULLUP) {
    lastError = BtnKitError::INVALID_ELEMENTS;
    return false;
  }
  
  config = newConfig;
  return true;
}

bool BtnKit::validateIndex(uint8_t index) const {
  return (index < t_elements);
}

bool BtnKit::isPressed(uint16_t state) const {
  if (config.invertLogic) {
    return (state == HIGH);
  } else {
    return (state == LOW); // Default for pullup
  }
}

bool BtnKit::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!initialized) {
    lastError = BtnKitError::NOT_INITIALIZED;
    return false;
  }
  
  if (func == nullptr) {
    lastError = BtnKitError::INVALID_FUNCTION;
    return false;
  }
  
  if (midiCh > 15) { // MIDI channels are 0-15
    lastError = BtnKitError::INVALID_ELEMENTS;
    return false;
  }
  
  // Read all current states first
  for (uint8_t i = 0; i < t_elements; i++) {
    cState[i] = digitalRead(elements[i]);
  }
  
  // Process changes with debouncing
  for (uint8_t i = 0; i < t_elements; i++) {
    uint32_t currentTime = millis();
    
    if ((currentTime - lastDebounceTime[i]) > config.debounceDelay) {
      if (cState[i] != pState[i]) {
        lastDebounceTime[i] = currentTime;
        
        if (isPressed(cState[i])) {
          func(i, 127, midiCh); // Note On
        } else {
          func(i, 0, midiCh);   // Note Off
        }
        
        pState[i] = cState[i];
      }
    }
  }
  
  return true;
}

bool BtnKit::isButtonPressed(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return false;
  }
  return isPressed(cState[index]);
}

bool BtnKit::isButtonJustPressed(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return false;
  }
  return isPressed(cState[index]) && !isPressed(pState[index]);
}

bool BtnKit::isButtonJustReleased(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return false;
  }
  return !isPressed(cState[index]) && isPressed(pState[index]);
}

uint32_t BtnKit::getLastPressTime(uint8_t index) const {
  if (!validateIndex(index) || !initialized) {
    return 0;
  }
  return lastDebounceTime[index];
}
