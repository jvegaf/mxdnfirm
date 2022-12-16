#pragma once
#include <Shifter.h>
#include "enums.h"


class Led
{
  Shifter* controller;
  uint8_t position;

  Led(Shifter* ctrler, uint8_t pos) :
      controller(ctrler), position(pos)
  {}

  void setState(State state);
 
};
