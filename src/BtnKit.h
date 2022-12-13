#pragma once

#include <Arduino.h>
#include <stdint.h>

class BtnKit {
private:
  const uint8_t *elements;
  const uint8_t t_elements;
  uint16_t *pState;
  uint16_t *cState;

  uint32_t *lastDebounceTime;
  const uint32_t debouncedelay = 20;

public:
  BtnKit(const uint8_t *elem, const uint8_t t_elem)
      : elements(elem), t_elements(t_elem) {
    pState = new uint16_t[t_elem]();
    cState = new uint16_t[t_elem]();
    lastDebounceTime = new uint32_t[t_elem]();
  }

  void begin();
  void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
};
