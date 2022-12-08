#include "Selector.h"
#include "pin_map.h"

namespace Selector {

int buttonCState = 0;
int buttonPState = 0;
/////////////////////////////////////////////
// debounce
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 5;

void begin() { pinMode(DECK_SEL, INPUT_PULLUP); }

void read(void((*cb)())) {
  buttonCState = digitalRead(DECK_SEL);

  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (buttonCState != buttonPState) {
      lastDebounceTime = millis();

      if (buttonCState == LOW) {
        cb(); // envia NoteOn(nota, velocity, canal midi)
      }
      buttonPState = buttonCState;
    }
  }
}

} // namespace Selector
