#include "Muxer.h"

void Muxer::setMuxChannel(uint8_t channel) {
  digitalWrite(muxPins[0], bitRead(channel, 0));
  digitalWrite(muxPins[1], bitRead(channel, 1));
  digitalWrite(muxPins[2], bitRead(channel, 2));
  digitalWrite(muxPins[3], bitRead(channel, 3));
}

void Muxer::begin() {
  for (uint8_t i = 0; i < tMuxPins; i++) {
    pinMode(muxPins[i], OUTPUT);
    digitalWrite(muxPins[i], LOW);
  }
  pinMode(sigPin, INPUT_PULLUP);
}

void Muxer::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  for (uint8_t i = 0; i <= tElements; i++) {

    setMuxChannel(elements[i]);

    cState[i] = digitalRead(sigPin);
    if ((millis() - lastdebouncetime[i]) > debouncedelay) {
      if (pState[i] != cState[i]) {
        lastdebouncetime[i] = millis();

        if (cState[i] == LOW) {
          func(i, 127U, midiCh);
        } else {
          func(i, 0, midiCh);
        }

        pState[i] = cState[i];
      }
    }
  }
}
