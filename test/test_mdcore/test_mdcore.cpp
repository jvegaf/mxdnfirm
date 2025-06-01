#include <gtest/gtest.h>
#include <gmock/gmock.h>

// Include mocks before any other includes
#include "Arduino.h"
#include "MIDI.h"
#include "Shifter.h"
#include "Multiplexer4067.h"
#include "Encoder.h"

// Include the actual MDCore header
#include "../../src/MDCore.h"

class MDCoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset MIDI mock state - use direct reset since no singleton
        MockMIDI::reset();
    }
    
    void TearDown() override {
        // No cleanup needed for namespace functions
    }
};

TEST_F(MDCoreTest, ConstructorInitializesCorrectly) {
    // Test MDCore namespace initialization
    EXPECT_TRUE(MDCore::begin());
    EXPECT_TRUE(MDCore::isReady());
}

TEST_F(MDCoreTest, BeginInitializesComponents) {
    bool result = MDCore::begin();
    EXPECT_TRUE(result);
    EXPECT_TRUE(MDCore::isReady());
}

TEST_F(MDCoreTest, HandleMIDIInput) {
    EXPECT_TRUE(MDCore::begin());
    
    // Test MIDI handling - should not crash
    EXPECT_NO_THROW(MDCore::cChange(1, 60, 127));
    EXPECT_NO_THROW(MDCore::noteOn(1, 60, 100));
    EXPECT_NO_THROW(MDCore::noteOff(1, 60, 0));
}

TEST_F(MDCoreTest, ConfigurationMethods) {
    EXPECT_TRUE(MDCore::begin());
    
    // Test configuration methods - should not crash
    EXPECT_NO_THROW(MDCore::changeDeck(1));
    EXPECT_TRUE(MDCore::isReady());
}

TEST_F(MDCoreTest, ErrorHandling) {
    // Test error handling after initialization
    EXPECT_TRUE(MDCore::begin());
    EXPECT_NO_THROW(MDCore::cChange(1, 60, 127));
}
