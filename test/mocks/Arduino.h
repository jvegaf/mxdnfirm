#pragma once

#include <stdint.h>
#include <iostream>

// Mock Arduino types and constants
#define HIGH 0x1
#define LOW  0x0

// Input mode constants
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3

// Binary literals
#define B00000000 0
#define B00000001 1
#define B00000010 2
#define B00000100 4
#define B00001000 8
#define B00010000 16
#define B00100000 32
#define B01000000 64
#define B10000000 128
#define B11111111 255

// Shift constants
#define MSBFIRST 1
#define LSBFIRST 0

// Bit manipulation functions
void pinMode(uint8_t pin, uint8_t mode);
uint8_t bitRead(uint8_t value, uint8_t bit);
void bitWrite(uint8_t& value, uint8_t bit, uint8_t bitvalue);
void bitSet(uint8_t& value, uint8_t bit);
void bitClear(uint8_t& value, uint8_t bit);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

// Analog pin constants
#define A0 14
#define A1 15
#define A2 16
#define A3 17
#define A4 18
#define A5 19
#define A6 20
#define A7 21
#define A8 22
#define A9 23
#define A10 24
#define A11 25
#define A12 26
#define A13 27
#define A14 28
#define A15 29

typedef bool boolean;
typedef uint8_t byte;

// Mock functions
unsigned long millis();
void delay(unsigned long ms);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
int analogRead(uint8_t pin);
void analogWrite(uint8_t pin, int val);
long map(long x, long in_min, long in_max, long out_min, long out_max);

// Mock Serial for testing
class MockSerial {
public:
    void begin(long baud) { std::cout << "Serial.begin(" << baud << ")" << std::endl; }
    void print(const char* str) { std::cout << str; }
    void println(const char* str) { std::cout << str << std::endl; }
};

extern MockSerial Serial;

// MockArduino class for controlling test state
class MockArduino {
public:
    static void reset();
    static void setDigitalValue(uint8_t pin, uint8_t value);
    static void setAnalogValue(uint8_t pin, int value);
    static void addMillis(unsigned long ms);
    static unsigned long getMillis();
};

// Mock for Adafruit_NeoPixel
class Adafruit_NeoPixel {
private:
    static uint16_t numLEDs;
    static uint8_t pin;
    static uint32_t* pixelData;
    static uint8_t brightness;
    static bool isInitialized;

public:
    // Constants matching real library
    static const uint8_t NEO_GRB = 0x01;
    static const uint8_t NEO_KHZ800 = 0x02;
    
    Adafruit_NeoPixel(uint16_t n, uint8_t p, uint8_t t);
    ~Adafruit_NeoPixel();
    
    void begin();
    void show();
    void clear();
    void setBrightness(uint8_t brightness);
    void setPixelColor(uint16_t n, uint32_t c);
    void setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b);
    uint32_t getPixelColor(uint16_t n);
    
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
    
    // Test helpers
    static uint32_t getPixelData(uint16_t index);
    static void reset();
    static bool getInitialized() { return isInitialized; }
    static uint8_t getBrightness() { return brightness; }
};
