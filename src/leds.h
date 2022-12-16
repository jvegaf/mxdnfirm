#pragma once

#include <Arduino.h>
#include <Shifter.h>
#include "sr_map.h"
#include "Led.h"
#include "pin_map.h"

namespace Leds
{
  void begin();
  void setElement(uint8_t pos, uint8_t state);
}
