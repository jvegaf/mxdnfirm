#pragma once
#include <Shifter.h>


class Led
{
  private:
  Shifter* controller;
  uint8_t position;

  public:
  Led(Shifter* ctrler, uint8_t pos) :
      controller(ctrler), position(pos)
  {}

  void setState(uint8_t state);
 
};
