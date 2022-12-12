#pragma once

#include <Arduino.h>

namespace MPlex {

enum class LeftBtns : uint8_t {
  PCUE2,
  PCUE1,
  SHIFT,
  LOOP_LH,
  SYNC_LH,
  RNG_LH,
  PLAY_LH,
  CUE_LH,
  LPAD8,
  LPAD4,
  LPAD7,
  LPAD3,
  LPAD2,
  LPAD6,
  LPAD1,
  LPAD5,
};

enum class RightBtns : uint8_t {
  CUE_RH = 0,
  PLAY_RH = 1,
  RNG_RH = 2,
  LOOP_RH = 3,
  PCUE3 = 5,
  SYNC_RH = 6,
  DECK_SEL = 7,
  RPAD8 = 8,
  RPAD4 = 9,
  RPAD7 = 10,
  RPAD3 = 11,
  RPAD2 = 12,
  RPAD6 = 13,
  RPAD1 = 14,
  RPAD5 = 15
};

} // namespace MPlex
