
#pragma once
#include "pin_map.h"
#include <Adafruit_NeoPixel.h>
#include <Arduino.h>
#include <Npixel.h>

class NPKit {
private:
  Adafruit_NeoPixel np;

public:
  NPKit(byte dataPin, byte nPixels);
  void begin();
  void clear();
  void handleChange(Npixel npx);
};
