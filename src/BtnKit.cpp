#include "BtnKit.h"

void BtnKit::begin() {
  for (uint8_t i = 0; i < t_elements; i++) {
    pinMode(elements[i], INPUT_PULLUP);
  }
}

void BtnKit::read(void (*func)(uint8_t, uint8_t, uint8_t)) {
  for (uint8_t i = 0; i < t_elements; i++) {
    cState[i] = digitalRead(elements[i]);
  }

  for (uint8_t i = 0; i < t_elements; i++) {

    if ((millis() - lastDebounceTime) > debounceDelay) {

      if (buttonCState[i] != buttonPState[i]) {
        lastDebounceTime = millis();

        if (buttonCState[i] == LOW) {
          func(i, 127, 10); // envia NoteOn(nota, velocity, canal midi)
        } else {
          func(i, 0, 10);
        }
        buttonPState[i] = buttonCState[i];
      }
    }
  }
}
