#pragma once
#include "Arduino.h"

#ifdef TEST_ARDUINO_IMPLEMENTATION
// Implementation for Arduino mock functions
#include <chrono>
#include <map>

static auto start_time = std::chrono::high_resolution_clock::now();
static unsigned long mock_millis_offset = 0;
static std::map<uint8_t, uint8_t> digital_values;
static std::map<uint8_t, int> analog_values;

MockSerial Serial;

// MockArduino implementations
void MockArduino::reset() {
    mock_millis_offset = 0;
    digital_values.clear();
    analog_values.clear();
    start_time = std::chrono::high_resolution_clock::now();
}

void MockArduino::setDigitalValue(uint8_t pin, uint8_t value) {
    digital_values[pin] = value;
}

void MockArduino::setAnalogValue(uint8_t pin, int value) {
    analog_values[pin] = value;
}

void MockArduino::addMillis(unsigned long ms) {
    mock_millis_offset += ms;
}

unsigned long MockArduino::getMillis() {
    return millis();
}

unsigned long millis() {
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time);
    return static_cast<unsigned long>(duration.count()) + mock_millis_offset;
}

void delay(unsigned long ms) {
    // For testing, we don't actually delay
    std::cout << "delay(" << ms << ")" << std::endl;
}

void digitalWrite(uint8_t pin, uint8_t val) {
    digital_values[pin] = val;
    std::cout << "digitalWrite(" << (int)pin << ", " << (int)val << ")" << std::endl;
}

int digitalRead(uint8_t pin) {
    auto it = digital_values.find(pin);
    if (it != digital_values.end()) {
        return it->second;
    }
    return LOW; // Default value
}

int analogRead(uint8_t pin) {
    auto it = analog_values.find(pin);
    if (it != analog_values.end()) {
        return it->second;
    }
    return 512; // Default value
}

void analogWrite(uint8_t pin, int val) {
    std::cout << "analogWrite(" << (int)pin << ", " << val << ")" << std::endl;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void pinMode(uint8_t pin, uint8_t mode) {
    // Mock implementation - just acknowledge the call
    std::cout << "pinMode(" << (int)pin << ", " << (int)mode << ")" << std::endl;
}

uint8_t bitRead(uint8_t value, uint8_t bit) {
    return (value >> bit) & 0x01;
}

void bitWrite(uint8_t& value, uint8_t bit, uint8_t bitvalue) {
    if (bitvalue) {
        value |= (1 << bit);
    } else {
        value &= ~(1 << bit);
    }
}

void bitSet(uint8_t& value, uint8_t bit) {
    value |= (1 << bit);
}

void bitClear(uint8_t& value, uint8_t bit) {
    value &= ~(1 << bit);
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
    std::cout << "shiftOut(" << (int)dataPin << ", " << (int)clockPin << ", ";
    std::cout << (bitOrder == MSBFIRST ? "MSBFIRST" : "LSBFIRST") << ", " << (int)val << ")" << std::endl;
}

// Adafruit_NeoPixel static members
uint16_t Adafruit_NeoPixel::numLEDs = 0;
uint8_t Adafruit_NeoPixel::pin = 0;
uint32_t* Adafruit_NeoPixel::pixelData = nullptr;
uint8_t Adafruit_NeoPixel::brightness = 0;
bool Adafruit_NeoPixel::isInitialized = false;
#endif // TEST_ARDUINO_IMPLEMENTATION
