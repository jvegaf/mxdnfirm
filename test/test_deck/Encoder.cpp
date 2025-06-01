#include "Encoder.h"
#include <iostream>

long Encoder::position = 0;

Encoder::Encoder(uint8_t pinA, uint8_t pinB) {
    std::cout << "Encoder created on pins " << (int)pinA << " and " << (int)pinB << std::endl;
}

long Encoder::read() {
    return position;
}

void Encoder::write(long pos) {
    position = pos;
}
