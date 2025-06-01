#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include mocks before any other includes
#include "Arduino.h"
#include "MIDI.h"
#include "Shifter.h"
#include "Multiplexer4067.h"
#include "Encoder.h"

// Include headers for components
#include "../../src/Deck.h"
#include "../../include/pad_modes.h"

class DeckTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset MIDI mock state
        MockMIDI::reset();
        deck = nullptr;
    }
    
    void TearDown() override {
        delete deck;
    }
      DeckControl* deck;
};

TEST_F(DeckTest, ConstructorValidPointer) {
    // Create a mock muxer with test data
    uint8_t testElements[] = {0, 1, 2};
    ElemCollection testCol = {testElements, 3};
    Muxer* mockMuxer = new Muxer(A0, testCol);
    deck = new DeckControl(mockMuxer);
    EXPECT_NE(deck, nullptr);
    
    delete mockMuxer;
}

TEST_F(DeckTest, ConstructorWithNullPointer) {
    deck = new DeckControl(nullptr);
    EXPECT_NE(deck, nullptr);
    EXPECT_FALSE(deck->begin()); // Should fail with null muxer
}

TEST_F(DeckTest, BeginInitializesComponents) {
    uint8_t testElements[] = {0, 1, 2};
    ElemCollection testCol = {testElements, 3};
    Muxer* mockMuxer = new Muxer(A0, testCol);
    deck = new DeckControl(mockMuxer);
    bool result = deck->begin();
    EXPECT_TRUE(result);
    EXPECT_TRUE(deck->isReady());
    
    delete mockMuxer;
}

TEST_F(DeckTest, ReadWithCallback) {
    uint8_t testElements[] = {0, 1, 2};
    ElemCollection testCol = {testElements, 3};
    Muxer* mockMuxer = new Muxer(A0, testCol);
    deck = new DeckControl(mockMuxer);
    deck->begin();
    
    // Test callback - should not crash
    auto testCallback = [](uint8_t a, uint8_t b, uint8_t c) {};
    EXPECT_NO_THROW(deck->read(testCallback, 1));
    
    delete mockMuxer;
}

TEST_F(DeckTest, ReadWithoutInitialization) {
    uint8_t testElements[] = {0, 1, 2};
    ElemCollection testCol = {testElements, 3};
    Muxer* mockMuxer = new Muxer(A0, testCol);
    deck = new DeckControl(mockMuxer);
    
    // Don't call begin()
    auto testCallback = [](uint8_t a, uint8_t b, uint8_t c) {};
    EXPECT_NO_THROW(deck->read(testCallback, 1)); // Should not crash
    
    delete mockMuxer;
}

TEST_F(DeckTest, ReadWithNullCallback) {
    uint8_t testElements[] = {0, 1, 2};
    ElemCollection testCol = {testElements, 3};
    Muxer* mockMuxer = new Muxer(A0, testCol);
    deck = new DeckControl(mockMuxer);
    deck->begin();
    
    // Test with null callback
    EXPECT_NO_THROW(deck->read(nullptr, 1)); // Should not crash
    
    delete mockMuxer;
}
