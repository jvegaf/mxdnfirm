
#pragma once
#include "NPKit.h"
#include "base.h"
#include "leds.h"
#include "midi_map.h"
#include "np_map.h"
#include "pin_map.h"
#include "sr_map.h"
#include "vumeters.h"
#include <Arduino.h>
#include <Shifter.h>

namespace MDCore {

// Core functionality
bool begin();  // Changed to return bool for error checking
void noteOff(uint8_t channel, uint8_t number, uint8_t value);
void cChange(uint8_t channel, uint8_t number, uint8_t value);
void noteOn(uint8_t channel, uint8_t number, uint8_t value);
void changeDeck(uint8_t deck);

// Utility functions for testing and debugging
bool isReady();
uint8_t getLastError();
void clearError();

} // namespace MDCore
