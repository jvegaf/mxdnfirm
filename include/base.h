#pragma once

#include <Arduino.h>

enum class Colors : uint8_t {
  Clear,
  Blue,
  Orange,
  Yellow,
  Gray,
  Green,
  Red,
  Purple
};

uint8_t const deckBColor = static_cast<uint8_t>(Colors::Blue);
uint8_t const deckCColor = static_cast<uint8_t>(Colors::Orange);

enum class DeckSelected : uint8_t {
  DeckA = 0x02,
  DeckB = 0x03,
  DeckC = 0x04,
};

enum class Location { Left, Right };

struct ElemCollection {
  const uint8_t *elements;
  uint8_t const size;
};
