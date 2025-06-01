#pragma once

#include "Arduino.h"

// Mock implementation of Multiplexer4067 library for testing
class Multiplexer4067 {
private:
    uint8_t _pin_S0, _pin_S1, _pin_S2, _pin_S3;
    uint8_t _analog_pin;
    
public:
    // Constructor with 4 parameters (original)
    Multiplexer4067(uint8_t pin_S0, uint8_t pin_S1, uint8_t pin_S2, uint8_t pin_S3);
    // Constructor with 5 parameters (for MuxPots compatibility)
    Multiplexer4067(uint8_t pin_S0, uint8_t pin_S1, uint8_t pin_S2, uint8_t pin_S3, uint8_t analog_pin);
    
    void analog_read_setup(uint8_t analog_pin);
    int analog_read(uint8_t channel);
    void digital_write(uint8_t channel, uint8_t value);
    int digital_read(uint8_t channel);
    void disable();
    
    // Methods used by MuxPots
    void begin();
    int readChannel(uint8_t channel);
};
