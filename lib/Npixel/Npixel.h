#pragma once
#include <Arduino.h>
class Npixel {
private:
  byte _position;
  byte _value;

public:
  Npixel(byte position, byte value);
  byte position() const;
  byte value() const;
};
