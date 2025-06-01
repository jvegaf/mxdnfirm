#pragma once

#include <stdint.h>

// Mock implementation of Encoder library for testing
class Encoder {
private:
    static long position;
    
public:
    Encoder(uint8_t pinA, uint8_t pinB);
    long read();
    void write(long position);
    
    // Test helpers
    static void setPosition(long pos) { position = pos; }
    static void reset() { position = 0; }
};
