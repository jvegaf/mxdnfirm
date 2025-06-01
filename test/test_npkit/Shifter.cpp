#include "Shifter.h"
#include <iostream>

Shifter::Shifter(uint8_t sig_pin, uint8_t latch_pin, uint8_t clk_pin, uint8_t data_size) 
    : _sig_pin(sig_pin), _latch_pin(latch_pin), _clk_pin(clk_pin), _data_size(data_size), _register_state(0) {
    std::cout << "Shifter created with pins: " << (int)sig_pin << ", " 
              << (int)latch_pin << ", " << (int)clk_pin << " size: " << (int)data_size << std::endl;
}

void Shifter::update() {
    // Mock implementation
    std::cout << "Shifter update called" << std::endl;
}

void Shifter::setBit(uint8_t position, bool value) {
    if (position < 8) {
        if (value) {
            _register_state |= (1 << position);
        } else {
            _register_state &= ~(1 << position);
        }
    }
}

bool Shifter::getBit(uint8_t position) {
    if (position < 8) {
        return (_register_state >> position) & 1;
    }
    return false;
}

void Shifter::setPin(uint8_t position, uint8_t state) {
    setBit(position, state != 0);
}

void Shifter::write() {
    update();
}

void Shifter::clear() {
    _register_state = 0;
}
