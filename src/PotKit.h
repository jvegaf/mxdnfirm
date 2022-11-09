
#pragma once
#include "pin_map.h"
#include "pots_map.h"
#include <Arduino.h>
#include <Multiplexer4067.h>

class PotKit {
private:
public:
  void begin();
  void read(void (*scc_func)(byte, byte, byte));
};
