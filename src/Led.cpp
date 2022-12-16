#include "Led.h"

Led::setState(State state) {
  controller->setPin(position, state);
  controller->write();
}
