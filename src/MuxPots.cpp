#include "MuxPots.h"

// Default configuration
const MuxPots::FilterConfig DEFAULT_FILTER_CONFIG = {
  .threshold = 8,
  .timeout = 50,
  .smoothingFactor = 15  // 15% smoothing
};

MuxPots::MuxPots(const uint8_t sig, const uint8_t *el, const uint8_t t_el)
    : elements(el), tElements(t_el), filterConfig(DEFAULT_FILTER_CONFIG), 
      lastCcValue(nullptr), potCState(nullptr), potPState(nullptr), 
      smoothedValue(nullptr), pTime(nullptr), timer(nullptr), 
      minValues(nullptr), maxValues(nullptr), mplexPots(nullptr), initialized(false) {  
  // Validate input parameters
  if (!el || t_el == 0) {
    return;
  }
  
  // Initialize multiplexer
  mplexPots = new Multiplexer4067(muxPins[0], muxPins[1], muxPins[2], muxPins[3], sig);
  if (mplexPots == nullptr) {
    return;
  }
  
  // Allocate memory for arrays
  pTime = new uint32_t[t_el]();
  if (pTime == nullptr) {
    clean();
    return;
  }
  
  timer = new uint32_t[t_el]();
  if (timer == nullptr) {
    clean();
    return;
  }
  
  potCState = new uint16_t[t_el]();
  if (potCState == nullptr) {
    clean();
    return;
  }
  
  potPState = new uint16_t[t_el]();
  if (potPState == nullptr) {
    clean();
    return;
  }
  
  smoothedValue = new uint16_t[t_el]();
  if (smoothedValue == nullptr) {
    clean();
    return;
  }
  
  lastCcValue = new uint8_t[t_el]();
  if (lastCcValue == nullptr) {
    clean();
    return;
  }
  
  minValues = new uint16_t[t_el]();
  if (minValues == nullptr) {
    clean();
    return;
  }
  
  maxValues = new uint16_t[t_el]();
  if (maxValues == nullptr) {
    clean();
    return;
  }
    
    // Initialize default calibration values
    for (uint8_t i = 0; i < t_el; i++) {
      minValues[i] = 0;
      maxValues[i] = 1023;
      lastCcValue[i] = 255; // Invalid value to force first send
    }
    initialized = false;
}

MuxPots::~MuxPots() {
  clean();
}

void MuxPots::clean() {
  delete mplexPots;
  delete[] pTime;
  delete[] timer;
  delete[] potCState;
  delete[] potPState;
  delete[] smoothedValue;
  delete[] lastCcValue;
  delete[] minValues;
  delete[] maxValues;
  
  mplexPots = nullptr;
  pTime = nullptr;
  timer = nullptr;
  potCState = nullptr;
  potPState = nullptr;
  smoothedValue = nullptr;
  lastCcValue = nullptr;
  minValues = nullptr;
  maxValues = nullptr;
}

bool MuxPots::begin() {
  if (!mplexPots) {
    return false;
  }
  
  mplexPots->begin();
  initialized = true;
  return true;
}

void MuxPots::read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  if (!initialized || !scc_func) {
    return;
  }
  
  unsigned long currentTime = millis();
    for (uint8_t i = 0; i < tElements; i++) {
    // Read current potentiometer value
    uint16_t rawValue = mplexPots->readChannel(elements[i]);
    
    // Apply smoothing filter
    uint16_t smoothedVal = applySmoothingFilter(i, rawValue);
    potCState[i] = smoothedVal;
    
    // Calculate variation from last reading
    uint16_t potVar = abs(potCState[i] - potPState[i]);
    
    if (potVar >= filterConfig.threshold) {
      pTime[i] = currentTime; // Update last activity time
    }
    
    timer[i] = currentTime - pTime[i];
    
    // If the potentiometer is still moving (within timeout)
    if (timer[i] < filterConfig.timeout) {
      uint8_t ccValue = analogToMidi(i, potCState[i]);
      
      // Send only if MIDI value has changed
      if (lastCcValue[i] != ccValue) {
        scc_func(i, ccValue, midiCh);
        potPState[i] = potCState[i];
        lastCcValue[i] = ccValue;
      }
    }
  }
}

void MuxPots::setFilterConfig(const FilterConfig& config) {
  filterConfig = config;
  
  // Validate ranges
  if (filterConfig.smoothingFactor > 100) {
    filterConfig.smoothingFactor = 100;
  }
}

int MuxPots::getCurrentValue(uint8_t index) const {
  if (!isValidIndex(index) || !initialized) {
    return 0xFFFF;  // Error value - un valor alto que no se confunda con un valor real
  }
  
  return analogToMidi(index, potCState[index]);
}

void MuxPots::reset() {
  if (!initialized) return;
  
  for (uint8_t i = 0; i < tElements; i++) {    potCState[i] = 0;
    potPState[i] = 0;
    smoothedValue[i] = 0;
    lastCcValue[i] = 255; // Invalid value
    pTime[i] = 0;
    timer[i] = 0;
  }
}

void MuxPots::calibratePot(uint8_t index, uint16_t minVal, uint16_t maxVal) {
  if (!isValidIndex(index)) return;
  
  // Validate that min < max
  if (minVal >= maxVal) {
    return;
  }
  
  minValues[index] = minVal;
  maxValues[index] = maxVal;
}

uint16_t MuxPots::applySmoothingFilter(uint8_t index, uint16_t newValue) {
  if (!isValidIndex(index)) return newValue;
  
  if (filterConfig.smoothingFactor == 0) {
    smoothedValue[index] = newValue;  } else {
    // Exponential moving average filter
    uint32_t factor = filterConfig.smoothingFactor;
    smoothedValue[index] = ((100 - factor) * smoothedValue[index] + factor * newValue) / 100;
  }
  
  return smoothedValue[index];
}

uint8_t MuxPots::analogToMidi(uint8_t index, uint16_t analogValue) const {
  if (!isValidIndex(index)) return 0;
  
  // Apply potentiometer-specific calibration
  uint16_t minVal = minValues[index];
  uint16_t maxVal = maxValues[index];
  
  // Clamp value within calibrated range
  if (analogValue < minVal) analogValue = minVal;
  if (analogValue > maxVal) analogValue = maxVal;
  
  // Map to MIDI range (0-127)
  return map(analogValue, minVal, maxVal, 0, 127);
}

bool MuxPots::isValidIndex(uint8_t index) const {
  return index < tElements;
}
