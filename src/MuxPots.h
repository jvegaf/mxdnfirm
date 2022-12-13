
#pragma once
#include <Arduino.h>
#include <Multiplexer4067.h>

class MuxPots {
private:
  const uint8_t *elements;
  const uint8_t t_elements;
  uint16_t *potCState; // current state
  uint16_t *potPState; // previous state
  uint32_t *pTime;
  uint32_t *timer;

  const uint16_t kThreshold = 8;
  const uint32_t kTimeout = 50;

  Multiplexer4067 *mplexPots;

public:
  MuxPots(const uint8_t *mux_pins, const uint8_t sig, const uint8_t *el,
          const uint8_t t_el)
      : elements(el), t_elements(t_el) {
    mplexPots = new Multiplexer4067(mux_pins[0], mux_pins[1], mux_pins[2],
                                    mux_pins[3], sig);
    pTime = new uint32_t[t_el]();
    timer = new uint32_t[t_el]();
    potCState = new uint16_t[t_el]();
    potPState = new uint16_t[t_el]();
  }

  void begin();
  void read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
};
