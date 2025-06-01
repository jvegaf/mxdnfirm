#include "Arduino.h"
#include <chrono>
#include <map>

static auto start_time = std::chrono::high_resolution_clock::now();
static unsigned long mock_millis_offset = 0;
static std::map<uint8_t, uint8_t> digital_values;
static std::map<uint8_t, int> analog_values;

MockSerial Serial;

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

// Adafruit_NeoPixel mock implementation
uint16_t Adafruit_NeoPixel::numLEDs = 0;
uint8_t Adafruit_NeoPixel::pin = 0;
uint32_t* Adafruit_NeoPixel::pixelData = nullptr;
uint8_t Adafruit_NeoPixel::brightness = 255;
bool Adafruit_NeoPixel::isInitialized = false;

Adafruit_NeoPixel::Adafruit_NeoPixel(uint16_t n, uint8_t p, uint8_t t) {
    numLEDs = n;
    pin = p;
    if (pixelData) delete[] pixelData;
    pixelData = new uint32_t[n]();
    for (uint16_t i = 0; i < n; i++) {
        pixelData[i] = 0;
    }
    isInitialized = false;
}

Adafruit_NeoPixel::~Adafruit_NeoPixel() {
    delete[] pixelData;
    pixelData = nullptr;
}

void Adafruit_NeoPixel::begin() {
    isInitialized = true;
}

void Adafruit_NeoPixel::show() {
    // Mock implementation - in real hardware this updates LEDs
}

void Adafruit_NeoPixel::clear() {
    if (pixelData) {
        for (uint16_t i = 0; i < numLEDs; i++) {
            pixelData[i] = 0;
        }
    }
}

void Adafruit_NeoPixel::setBrightness(uint8_t b) {
    brightness = b;
}

void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint32_t c) {
    if (n < numLEDs && pixelData) {
        pixelData[n] = c;
    }
}

void Adafruit_NeoPixel::setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
    setPixelColor(n, Color(r, g, b));
}

uint32_t Adafruit_NeoPixel::getPixelColor(uint16_t n) {
    if (n < numLEDs && pixelData) {
        return pixelData[n];
    }
    return 0;
}

uint32_t Adafruit_NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t Adafruit_NeoPixel::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w) {
    return ((uint32_t)w << 24) | ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}

uint32_t Adafruit_NeoPixel::getPixelData(uint16_t index) {
    if (index < numLEDs && pixelData) {
        return pixelData[index];
    }
    return 0;
}

void Adafruit_NeoPixel::reset() {
    if (pixelData) {
        for (uint16_t i = 0; i < numLEDs; i++) {
            pixelData[i] = 0;
        }
    }
    brightness = 255;
    isInitialized = false;
}

// MockArduino implementation
void MockArduino::reset() {
    digital_values.clear();
    analog_values.clear();
    mock_millis_offset = 0;
    start_time = std::chrono::high_resolution_clock::now();
    
    // Reset NeoPixel mock
    Adafruit_NeoPixel::reset();
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
