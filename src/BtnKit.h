
#pragma once
#include "pin_map.h"
#include <Arduino.h>
#include <MIDI.h>

class BtnKit {
private:
public:
  void begin();
  void read(void (*func)(byte, byte, byte));
};
