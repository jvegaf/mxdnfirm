#include "BtnKit.h"
#include <stdint.h>

void BtnKit::begin() {
  for (uint8_t i = 0; i < t_elements; i++) {
    pinMode(elements[i], INPUT_PULLUP);
  }
}

void BtnKit::read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh) {
  for (uint8_t i = 0; i < t_elements; i++) {
    cState[i] = digitalRead(elements[i]);
  }

  for (uint8_t i = 0; i < t_elements; i++) {

    if ((millis() - lastDebounceTime[i]) > debouncedelay) {

      if (cState[i] != pState[i]) {
        lastDebounceTime[i] = millis();

        if (cState[i] == LOW) {
          func(i, 127, midiCh); // envia NoteOn(nota, velocity, canal midi)
        } else {
          func(i, 0, midiCh);
        }
        pState[i] = cState[i];
      }
    }
  }
}
