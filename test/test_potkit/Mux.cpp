#include "Mux.h"

void Mux::setMuxChannel(uint8_t channel) {
  digitalWrite(muxPins[0], bitRead(channel, 0));
  digitalWrite(muxPins[1], bitRead(channel, 1));
  digitalWrite(muxPins[2], bitRead(channel, 2));
  digitalWrite(muxPins[3], bitRead(channel, 3));
}

void Mux::begin() {
  for (uint8_t i = 0; i < tMuxPins; i++) {
    pinMode(muxPins[i], OUTPUT);
    digitalWrite(muxPins[i], LOW);
  }
  pinMode(sigPin, INPUT_PULLUP);
}

void Mux::read(void (*func)()) {
  setMuxChannel(position);
  cState = digitalRead(sigPin);

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
