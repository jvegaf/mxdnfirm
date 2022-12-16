
#pragma once
#include "NPKit.h"
#include "enums.h"
#include "leds.h"
#include "midi_map.h"
#include "np_map.h"
#include "pin_map.h"
#include "sr_map.h"
#include "vumeters.h"
#include <Arduino.h>
#include <Shifter.h>

namespace MDCore {

void begin();
void noteOff(uint8_t channel, uint8_t number, uint8_t value);
void cChange(uint8_t channel, uint8_t number, uint8_t value);
void noteOn(uint8_t channel, uint8_t number, uint8_t value);
void changeDeck(DeckSelected deck);
} // namespace MDCore
