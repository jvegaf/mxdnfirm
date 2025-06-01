#pragma once

#include "Arduino.h"

// Mock implementation of Shifter library for testing
class Shifter {
private:
    uint8_t _sig_pin, _latch_pin, _clk_pin;
    uint8_t _data_size;
    uint8_t _register_state;
    
public:
    Shifter(uint8_t sig_pin, uint8_t latch_pin, uint8_t clk_pin, uint8_t data_size);
    void update();
    void setBit(uint8_t position, bool value);
    bool getBit(uint8_t position);
    
    // Additional methods used by Led.cpp
    void setPin(uint8_t position, uint8_t state);
    void write();
    void clear();
};
