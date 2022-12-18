#pragma once

#include <Arduino.h>

enum class DeckSelected : uint8_t { 
  DeckA = 0x02, 
  DeckB = 0x03, 
  DeckC = 0x04, 
};

enum class Location { Left, Right };
