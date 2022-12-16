#include "Mux.h"

void Mux::setMuxChannel(uint8_t channel) {
  digitalWrite(muxPins[0], bitRead(channel, 0));
  digitalWrite(muxPins[1], bitRead(channel, 1));
  digitalWrite(muxPins[2], bitRead(channel, 2));
  digitalWrite(muxPins[3], bitRead(channel, 3));
}

Mux::Mux(const uint8_t sig, const uint8_t pos) : muxSIG(sig), position(pos) {}

void Mux::read(void (*func)()) {
  setMuxChannel(position);
  cState = digitalRead(muxSIG);

  if ((millis() - lastdebouncetime) > debouncedelay) {
    if (pState != cState) {
      lastdebouncetime = millis();

      if (cState == LOW) {
        func();
      }

      pState = cState;
    }
  }
}
