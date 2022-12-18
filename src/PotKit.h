
#pragma once
#include <Arduino.h>

class PotKit {
private:
  const uint8_t *elements;
  const uint8_t t_elements;
  uint16_t *potCState; // current state
  uint16_t *potPState; // previous state
  uint8_t *lastCcValue;
  uint32_t *pTime;
  uint32_t *timer;

  const uint16_t kThreshold = 8;
  const uint16_t kTimeout = 50;

public:
  PotKit(const uint8_t *el, const uint8_t t_el)
      : elements(el), t_elements(t_el) {
    pTime = new uint32_t[t_el]();
    timer = new uint32_t[t_el]();
    potCState = new uint16_t[t_el]();
    potPState = new uint16_t[t_el]();
    lastCcValue = new uint8_t[t_el]();
  }

  void read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
};
