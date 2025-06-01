#include "Led.h"

void Led::setState(uint8_t state) {
  controller->setPin(position, state);
  controller->write();
}
