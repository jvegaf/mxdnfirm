#include <gtest/gtest.h>
#include "../../src/PotKit.h"
#include "../mocks/Arduino.h"

class PotKitTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mock state
        MockArduino::reset();
        
        // Setup test pins
        testPins[0] = A0;
        testPins[1] = A1;
        testPins[2] = A2;
        
        potKit = nullptr;
    }
    
    void TearDown() override {
        delete potKit;
    }
    
    uint8_t testPins[3];
    PotKit* potKit;
    
    // Test callback function
    static void testCallback(uint8_t cc, uint8_t value, uint8_t channel) {
        lastCC = cc;
        lastValue = value;
        lastChannel = channel;
        callbackCount++;
    }
    
    static uint8_t lastCC;
    static uint8_t lastValue;
    static uint8_t lastChannel;
    static int callbackCount;
    
    static void resetCallback() {
        lastCC = 0;
        lastValue = 0;
        lastChannel = 0;
        callbackCount = 0;
    }
};

// Static member definitions
uint8_t PotKitTest::lastCC = 0;
uint8_t PotKitTest::lastValue = 0;
uint8_t PotKitTest::lastChannel = 0;
int PotKitTest::callbackCount = 0;

TEST_F(PotKitTest, ConstructorValidInput) {
    potKit = new PotKit(testPins, 3);
    EXPECT_EQ(potKit->getLastError(), PotKitError::NONE);
    EXPECT_EQ(potKit->getElementCount(), 3);
    EXPECT_FALSE(potKit->isReady()); // Not initialized yet
}

TEST_F(PotKitTest, ConstructorNullPointer) {
    potKit = new PotKit(nullptr, 3);
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_ELEMENTS);
    EXPECT_FALSE(potKit->isReady());
}

TEST_F(PotKitTest, ConstructorZeroElements) {
    potKit = new PotKit(testPins, 0);
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_COUNT);
    EXPECT_FALSE(potKit->isReady());
}

TEST_F(PotKitTest, ConstructorTooManyElements) {
    potKit = new PotKit(testPins, 50);
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_COUNT);
    EXPECT_FALSE(potKit->isReady());
}

TEST_F(PotKitTest, BeginSuccess) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(potKit->isReady());
    EXPECT_EQ(potKit->getLastError(), PotKitError::NONE);
}

TEST_F(PotKitTest, BeginWithError) {
    potKit = new PotKit(nullptr, 3);
    EXPECT_FALSE(potKit->begin());
    EXPECT_FALSE(potKit->isReady());
}

TEST_F(PotKitTest, ConfigurationManagement) {
    potKit = new PotKit(testPins, 3);
    
    PotKitConfig config;
    config.threshold = 16;
    config.timeout = 100;
    config.minAnalog = 10;
    config.maxAnalog = 1000;
    config.minMidi = 1;
    config.maxMidi = 126;
    
    EXPECT_TRUE(potKit->setConfig(config));
    
    PotKitConfig retrievedConfig = potKit->getConfig();
    EXPECT_EQ(retrievedConfig.threshold, 16);
    EXPECT_EQ(retrievedConfig.timeout, 100);
    EXPECT_EQ(retrievedConfig.minAnalog, 10);
    EXPECT_EQ(retrievedConfig.maxAnalog, 1000);
    EXPECT_EQ(retrievedConfig.minMidi, 1);
    EXPECT_EQ(retrievedConfig.maxMidi, 126);
}

TEST_F(PotKitTest, InvalidConfiguration) {
    potKit = new PotKit(testPins, 3);
    
    PotKitConfig config;
    config.threshold = 1000; // Too high
    EXPECT_FALSE(potKit->setConfig(config));
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_ELEMENTS);
    
    config.threshold = 8;
    config.minAnalog = 1000;
    config.maxAnalog = 100; // min > max
    EXPECT_FALSE(potKit->setConfig(config));
}

TEST_F(PotKitTest, ReadWithoutInitialization) {
    potKit = new PotKit(testPins, 3);
    resetCallback();
    
    EXPECT_FALSE(potKit->read(testCallback, 0));
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_ELEMENTS);
    EXPECT_EQ(callbackCount, 0);
}

TEST_F(PotKitTest, ReadWithNullCallback) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    
    EXPECT_FALSE(potKit->read(nullptr, 0));
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_FUNCTION);
}

TEST_F(PotKitTest, ReadWithInvalidChannel) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    resetCallback();
    
    EXPECT_FALSE(potKit->read(testCallback, 16)); // Invalid MIDI channel
    EXPECT_EQ(potKit->getLastError(), PotKitError::INVALID_ELEMENTS);
}

TEST_F(PotKitTest, ReadBasicFunctionality) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    resetCallback();
    
    // Set initial analog values
    MockArduino::setAnalogValue(A0, 512);
    MockArduino::setAnalogValue(A1, 0);
    MockArduino::setAnalogValue(A2, 1023);
    
    EXPECT_TRUE(potKit->read(testCallback, 1));
    
    // Should trigger callbacks for significant changes
    EXPECT_GT(callbackCount, 0);
    EXPECT_EQ(lastChannel, 1);
}

TEST_F(PotKitTest, ReadThresholdFiltering) {
    potKit = new PotKit(testPins, 1);
    EXPECT_TRUE(potKit->begin());
    
    PotKitConfig config;
    config.threshold = 50; // High threshold
    EXPECT_TRUE(potKit->setConfig(config));
    
    resetCallback();
    
    // Small change should not trigger callback
    MockArduino::setAnalogValue(A0, 512);
    EXPECT_TRUE(potKit->read(testCallback, 0));
    
    MockArduino::setAnalogValue(A0, 515); // Small change
    EXPECT_TRUE(potKit->read(testCallback, 0));
    
    // Should not have triggered callback due to threshold
    EXPECT_EQ(callbackCount, 0);
    
    // Large change should trigger callback
    MockArduino::setAnalogValue(A0, 600);
    EXPECT_TRUE(potKit->read(testCallback, 0));
    EXPECT_GT(callbackCount, 0);
}

TEST_F(PotKitTest, GetCurrentValues) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    
    MockArduino::setAnalogValue(A0, 512);
    MockArduino::setAnalogValue(A1, 256);
    MockArduino::setAnalogValue(A2, 768);
    
    // Read to update internal state
    EXPECT_TRUE(potKit->read(testCallback, 0));
    
    EXPECT_EQ(potKit->getCurrentValue(0), 512);
    EXPECT_EQ(potKit->getCurrentValue(1), 256);
    EXPECT_EQ(potKit->getCurrentValue(2), 768);
    
    // Test MIDI mapping
    EXPECT_EQ(potKit->getCurrentMidiValue(0), 63); // 512/1023 * 127 ≈ 63
    EXPECT_EQ(potKit->getCurrentMidiValue(1), 31); // 256/1023 * 127 ≈ 31
    EXPECT_EQ(potKit->getCurrentMidiValue(2), 95); // 768/1023 * 127 ≈ 95
}

TEST_F(PotKitTest, GetCurrentValuesInvalidIndex) {
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    
    EXPECT_EQ(potKit->getCurrentValue(5), 0); // Invalid index
    EXPECT_EQ(potKit->getCurrentMidiValue(10), 0); // Invalid index
}

TEST_F(PotKitTest, IsElementActive) {
    potKit = new PotKit(testPins, 1);
    EXPECT_TRUE(potKit->begin());
    
    PotKitConfig config;
    config.timeout = 100; // 100ms timeout
    EXPECT_TRUE(potKit->setConfig(config));
    
    MockArduino::setAnalogValue(A0, 512);
    EXPECT_TRUE(potKit->read(testCallback, 0));
    
    // Should be active immediately after change
    EXPECT_TRUE(potKit->isElementActive(0));
    
    // Simulate time passing
    MockArduino::addMillis(150); // More than timeout
    
    // Should no longer be active
    EXPECT_FALSE(potKit->isElementActive(0));
    
    // Invalid index
    EXPECT_FALSE(potKit->isElementActive(5));
}

TEST_F(PotKitTest, ErrorHandling) {
    potKit = new PotKit(testPins, 3);
    EXPECT_EQ(potKit->getLastError(), PotKitError::NONE);
    
    // Trigger an error
    EXPECT_FALSE(potKit->read(nullptr, 0));
    EXPECT_NE(potKit->getLastError(), PotKitError::NONE);
    
    // Clear error
    potKit->clearError();
    EXPECT_EQ(potKit->getLastError(), PotKitError::NONE);
}

TEST_F(PotKitTest, MemoryManagement) {
    // Test that destructor doesn't crash
    potKit = new PotKit(testPins, 3);
    delete potKit;
    potKit = nullptr;
    
    // Test multiple allocations
    for (int i = 0; i < 10; i++) {
        PotKit* testKit = new PotKit(testPins, 3);
        EXPECT_EQ(testKit->getLastError(), PotKitError::NONE);
        delete testKit;
    }
}
