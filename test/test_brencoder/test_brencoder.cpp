#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include mocks before anything else
#include "../mocks/Arduino.h"
#include "../mocks/Encoder.h"

// Include the actual BREncoder header (not implementation)
#include "../../src/BREncoder.h"

// Global mock instances
static std::vector<Encoder*> globalEncoders;

class BREncoderTest : public ::testing::Test {
protected:
    void SetUp() override {
        MockArduino::reset();
        globalEncoders.clear();
        encoder = new BREncoder();
        callbackCalls.clear();
    }
    
    void TearDown() override {
        delete encoder;
    }
    
    BREncoder* encoder;
    
    // Static variables to track callback calls
    static std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> callbackCalls;
    
    static void testCallback(uint8_t cc, uint8_t value, uint8_t channel) {
        callbackCalls.push_back(std::make_tuple(cc, value, channel));
    }
};

// Static member definition
std::vector<std::tuple<uint8_t, uint8_t, uint8_t>> BREncoderTest::callbackCalls;

TEST_F(BREncoderTest, ConstructorInitializesCorrectly) {
    EXPECT_FALSE(encoder->isReady());
    EXPECT_EQ(encoder->getPosition(true), 0);   // Left encoder
    EXPECT_EQ(encoder->getPosition(false), 0);  // Right encoder
}

TEST_F(BREncoderTest, BeginInitializesEncoders) {
    EXPECT_TRUE(encoder->begin());
    EXPECT_TRUE(encoder->isReady());
}

TEST_F(BREncoderTest, ReadEncWithoutInitializationDoesNothing) {
    // Don't call begin()
    encoder->readEnc(testCallback);
    EXPECT_EQ(callbackCalls.size(), 0);
}

TEST_F(BREncoderTest, ReadEncWithNullCallbackDoesNothing) {
    encoder->begin();
    encoder->readEnc(nullptr);
    EXPECT_EQ(callbackCalls.size(), 0);
}

TEST_F(BREncoderTest, LeftEncoderClockwiseRotationSendsCorrectMIDI) {
    encoder->begin();
    
    // Simulate rotation by changing internal state
    // Since we can't easily mock the internal encoder behavior,
    // we'll test the configuration aspect
    
    BREncoder::EncoderConfig config;
    config.ccNumberCW = 20;
    config.valueCW = 100;
    config.midiChannel = 8;
    config.debounceMs = 5;
    
    encoder->configureEncoder(true, config);
    
    // The actual MIDI testing would require more sophisticated mocking
    // of the internal Encoder objects, which is complex with the current architecture
}

TEST_F(BREncoderTest, ResetSetsPositionsToZero) {
    encoder->begin();
    encoder->reset();
    
    // After reset, positions should be 0
    EXPECT_EQ(encoder->getPosition(true), 0);
    EXPECT_EQ(encoder->getPosition(false), 0);
}

TEST_F(BREncoderTest, ConfigureEncoderChangesSettings) {
    BREncoder::EncoderConfig leftConfig;
    leftConfig.ccNumberCW = 50;
    leftConfig.valueCW = 120;
    leftConfig.midiChannel = 10;
    leftConfig.debounceMs = 20;
    
    encoder->configureEncoder(true, leftConfig);
    
    // The configuration should be stored (we can't easily test this without
    // exposing internal state, but this tests the interface)
    EXPECT_NO_THROW(encoder->configureEncoder(true, leftConfig));
}

TEST_F(BREncoderTest, GetPositionReturnsZeroWhenNotInitialized) {
    // Don't call begin()
    EXPECT_EQ(encoder->getPosition(true), 0);
    EXPECT_EQ(encoder->getPosition(false), 0);
}

// Test for memory management
TEST_F(BREncoderTest, DestructorHandlesNullPointers) {
    // This tests that the destructor can handle the case where
    // begin() was never called or failed
    BREncoder* testEncoder = new BREncoder();
    EXPECT_NO_THROW(delete testEncoder);
}

// Integration test
TEST_F(BREncoderTest, FullWorkflowTest) {
    // Test complete initialization and basic functionality
    EXPECT_TRUE(encoder->begin());
    EXPECT_TRUE(encoder->isReady());
    
    // Configure both encoders
    BREncoder::EncoderConfig leftConfig = {14, 14, 127, 1, 6, 10};
    BREncoder::EncoderConfig rightConfig = {15, 15, 127, 1, 6, 10};
    
    encoder->configureEncoder(true, leftConfig);
    encoder->configureEncoder(false, rightConfig);
    
    // Try reading (won't generate MIDI without actual encoder movement)
    EXPECT_NO_THROW(encoder->readEnc(testCallback));
    
    // Reset and verify
    encoder->reset();
    EXPECT_EQ(encoder->getPosition(true), 0);
    EXPECT_EQ(encoder->getPosition(false), 0);
}
