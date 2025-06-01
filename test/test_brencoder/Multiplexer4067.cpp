#include "Multiplexer4067.h"
#include <iostream>

Multiplexer4067::Multiplexer4067(uint8_t pin_S0, uint8_t pin_S1, uint8_t pin_S2, uint8_t pin_S3) 
    : _pin_S0(pin_S0), _pin_S1(pin_S1), _pin_S2(pin_S2), _pin_S3(pin_S3) {
    std::cout << "Multiplexer4067 created with pins: " << (int)pin_S0 << ", " 
              << (int)pin_S1 << ", " << (int)pin_S2 << ", " << (int)pin_S3 << std::endl;
}

Multiplexer4067::Multiplexer4067(uint8_t pin_S0, uint8_t pin_S1, uint8_t pin_S2, uint8_t pin_S3, uint8_t analog_pin) 
    : _pin_S0(pin_S0), _pin_S1(pin_S1), _pin_S2(pin_S2), _pin_S3(pin_S3), _analog_pin(analog_pin) {
    std::cout << "Multiplexer4067 created with pins: " << (int)pin_S0 << ", " 
              << (int)pin_S1 << ", " << (int)pin_S2 << ", " << (int)pin_S3 
              << " analog: " << (int)analog_pin << std::endl;
}

void Multiplexer4067::analog_read_setup(uint8_t analog_pin) {
    _analog_pin = analog_pin;
}

int Multiplexer4067::analog_read(uint8_t channel) {
    // Mock implementation - return test values based on channel
    return 512 + (channel * 10);
}

void Multiplexer4067::digital_write(uint8_t channel, uint8_t value) {
    std::cout << "Mux digital write channel " << (int)channel << " value " << (int)value << std::endl;
}

int Multiplexer4067::digital_read(uint8_t channel) {
    // Mock implementation
    return LOW;
}

void Multiplexer4067::disable() {
    std::cout << "Multiplexer disabled" << std::endl;
}

void Multiplexer4067::begin() {
    std::cout << "Multiplexer4067 begin() called" << std::endl;
}

int Multiplexer4067::readChannel(uint8_t channel) {
    return analog_read(channel);
}
