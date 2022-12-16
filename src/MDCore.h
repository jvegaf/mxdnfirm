
#pragma once
#include "NPKit.h"
#include "midi_map.h"
#include "np_map.h"
#include "pin_map.h"
#include "sr_map.h"
#include "leds.h"
#include "enums.h"
#include "vumeters.h"
#include <Arduino.h>
#include <Shifter.h>

namespace MDCore {

void begin();
void noteOff(byte channel, byte number, byte value);
void cChange(byte channel, byte number, byte value);
void noteOn(byte channel, byte number, byte value);

} // namespace MDCore
