#pragma once

#include <Arduino.h>
#include <Shifter.h>
#include "sr_map.h"
#include "Led.h"
#include "enums.h"

namespace Leds
{
  void begin();
  void setElement(uint8_t pos, State state);
}
