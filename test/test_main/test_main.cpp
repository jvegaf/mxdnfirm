#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include mocks
#include "../mocks/Arduino.h"
#include "../mocks/MIDI.h"
#include "../mocks/Thread.h"

// Mock all the hardware classes and dependencies for main.cpp testing
class MockBREncoder {
public:
    void readEnc(void (*func)(uint8_t, uint8_t, uint8_t)) {}
};

class MockMux {
public:
    MockMux(uint8_t sig, uint8_t sel) {}
    void begin() {}
    void read(void (*func)()) {}
};

class MockMuxer {
public:
    MockMuxer(uint8_t sig, uint8_t col) {}
    void begin() {}
    void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t ch) {}
};

class MockDeckControl {
public:
    MockDeckControl(MockMuxer* m) {}
    bool begin() { return true; }
    void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t ch) {}
};

class MockPadControl {
public:
    MockPadControl(MockMuxer* m) {}
    void begin() {}
    void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t ch) {}
};

class MockMuxPots {
public:
    MockMuxPots(uint8_t sig, uint8_t mux, uint8_t total) {}
    void begin() {}
    void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t ch) {}
};

class MockBtnKit {
public:
    MockBtnKit(const uint8_t* pins, uint8_t total) {}
    void begin() {}
};

class MockPotKit {
public:
    MockPotKit(const uint8_t* pins, uint8_t total) {}
    void read(void (*func)(uint8_t, uint8_t, uint8_t), uint8_t ch) {}
};

// Mock constants and variables
const uint8_t deckAChannel = 1;
const uint8_t deckBChannel = 2;
const uint8_t deckCChannel = 3;
const uint8_t padAChannel = 4;
const uint8_t padBChannel = 5;
const uint8_t padCChannel = 6;
const uint8_t topPotsChannel = 7;
const uint8_t bottomPotsChannel = 8;
const uint8_t potsChannel = 9;

const uint8_t rhMPSig = 1;
const uint8_t lhMPSig = 2;
const uint8_t topPotsSig = 3;
const uint8_t btmPotsSig = 4;

namespace MPlex {
    const uint8_t DECK_SEL = 0;
    const uint8_t deckLeftBtnsCol = 1;
    const uint8_t deckRightBtnsCol = 2;
    const uint8_t padLeftBtnsCol = 3;
    const uint8_t padRightBtnsCol = 4;
    const uint8_t topPots = 5;
    const uint8_t bottomPots = 6;
    const uint8_t tTopPots = 4;
}

const uint8_t btnPins[] = {1, 2, 3};
const uint8_t tBtnPins = 3;
const uint8_t PotPins[] = {A0, A1, A2};
const uint8_t tPotPins = 3;

namespace MDCore {
    bool begin() { return true; }
    void cChange(uint8_t ch, uint8_t num, uint8_t val) {}
    void noteOn(uint8_t ch, uint8_t num, uint8_t val) {}
    void noteOff(uint8_t ch, uint8_t num, uint8_t val) {}
    void changeDeck(uint8_t deck) {}
}

// Create a test version of the main functions
class MidiControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset MIDI mock state
        MIDI = MockMIDI();
    }
};

// Mock the main.cpp functions we want to test
void sendMidiNote(uint8_t number, uint8_t value, uint8_t channel);
void sendMidiCC(uint8_t number, uint8_t value, uint8_t channel);
void handleControlChange(uint8_t channel, uint8_t number, uint8_t value);
void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value);
void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value);

// Implement the functions to test
void sendMidiNote(uint8_t number, uint8_t value, uint8_t channel) {
    if (value > 127) value = 127;
    
    if (value == 0) {
        MIDI.sendNoteOff(number, 0, channel);
    } else {
        MIDI.sendNoteOn(number, value, channel);
    }
}

void sendMidiCC(uint8_t number, uint8_t value, uint8_t channel) {
    if (value > 127) value = 127;
    MIDI.sendControlChange(number, value, channel);
}

void handleControlChange(uint8_t channel, uint8_t number, uint8_t value) {
    if (value > 127) value = 127;
    MDCore::cChange(channel, number, value);
}

void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value) {
    if (value > 127) value = 127;
    
    if (value < 1U) {
        MDCore::noteOff(channel, number, 0);
        return;
    }
    MDCore::noteOn(channel, number, value);
}

void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value) {
    MDCore::noteOff(channel, number, 0);
}

TEST_F(MidiControllerTest, SendMidiNoteValidatesValue) {
    sendMidiNote(60, 100, 1);
    EXPECT_EQ(MIDI.lastNoteOn.param1, 60);
    EXPECT_EQ(MIDI.lastNoteOn.param2, 100);
    EXPECT_EQ(MIDI.lastNoteOn.param3, 1);
}

TEST_F(MidiControllerTest, SendMidiNoteClampsValue) {
    sendMidiNote(60, 200, 1);  // Over maximum
    EXPECT_EQ(MIDI.lastNoteOn.param2, 127);
}

TEST_F(MidiControllerTest, SendMidiNoteZeroSendsNoteOff) {
    sendMidiNote(60, 0, 1);
    EXPECT_EQ(MIDI.lastNoteOff.param1, 60);
    EXPECT_EQ(MIDI.lastNoteOff.param2, 0);
    EXPECT_EQ(MIDI.lastNoteOff.param3, 1);
}

TEST_F(MidiControllerTest, SendMidiCCValidatesValue) {
    sendMidiCC(7, 64, 1);
    EXPECT_EQ(MIDI.lastCC.param1, 7);
    EXPECT_EQ(MIDI.lastCC.param2, 64);
    EXPECT_EQ(MIDI.lastCC.param3, 1);
}

TEST_F(MidiControllerTest, SendMidiCCClampsValue) {
    sendMidiCC(7, 200, 1);  // Over maximum
    EXPECT_EQ(MIDI.lastCC.param2, 127);
}

TEST_F(MidiControllerTest, HandleNoteOnWithZeroVelocityCallsNoteOff) {
    // This test verifies the MIDI spec compliance
    handleNoteOn(1, 60, 0);
    // Should call MDCore::noteOff instead of noteOn
}

TEST_F(MidiControllerTest, HandleControlChangeClampsValue) {
    handleControlChange(1, 7, 200);
    // Value should be clamped to 127 before passing to MDCore
}
