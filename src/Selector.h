#pragma once

#include "pin_map.h"
#include <Arduino.h>

namespace Selector {

void begin();
void read(void((*cb)()));

} // namespace Selector
