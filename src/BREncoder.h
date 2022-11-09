
#pragma once
#include <Arduino.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "pin_map.h"
#include <Encoder.h>

class BREncoder {
private:
public:
  BREncoder();
  void readEnc(void (*scc_func)(byte, byte, byte));
};
