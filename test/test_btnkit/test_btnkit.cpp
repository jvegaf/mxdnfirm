#include <gtest/gtest.h>
#include "../../src/BtnKit.h"
#include "../mocks/Arduino.h"

class BtnKitTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mock state
        MockArduino::reset();
        
        // Setup test pins
        testPins[0] = 2;
        testPins[1] = 3;
        testPins[2] = 4;
        
        btnKit = nullptr;
    }
    
    void TearDown() override {
        delete btnKit;
    }
    
    uint8_t testPins[3];
    BtnKit* btnKit;
    
    // Test callback function
    static void testCallback(uint8_t note, uint8_t velocity, uint8_t channel) {
        lastNote = note;
        lastVelocity = velocity;
        lastChannel = channel;
        callbackCount++;
    }
    
    static uint8_t lastNote;
    static uint8_t lastVelocity;
    static uint8_t lastChannel;
    static int callbackCount;
    
    static void resetCallback() {
        lastNote = 0;
        lastVelocity = 0;
        lastChannel = 0;
        callbackCount = 0;
    }
};

// Static member definitions
uint8_t BtnKitTest::lastNote = 0;
uint8_t BtnKitTest::lastVelocity = 0;
uint8_t BtnKitTest::lastChannel = 0;
int BtnKitTest::callbackCount = 0;

TEST_F(BtnKitTest, ConstructorValidInput) {
    btnKit = new BtnKit(testPins, 3);
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::NONE);
    EXPECT_EQ(btnKit->getElementCount(), 3);
    EXPECT_FALSE(btnKit->isReady()); // Not initialized yet
}

TEST_F(BtnKitTest, ConstructorNullPointer) {
    btnKit = new BtnKit(nullptr, 3);
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_ELEMENTS);
    EXPECT_FALSE(btnKit->isReady());
}

TEST_F(BtnKitTest, ConstructorZeroElements) {
    btnKit = new BtnKit(testPins, 0);
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_COUNT);
    EXPECT_FALSE(btnKit->isReady());
}

TEST_F(BtnKitTest, ConstructorTooManyElements) {
    btnKit = new BtnKit(testPins, 100);
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_COUNT);
    EXPECT_FALSE(btnKit->isReady());
}

TEST_F(BtnKitTest, BeginSuccess) {
    btnKit = new BtnKit(testPins, 3);
    
    // Set initial pin states to HIGH (default for pullup)
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(btnKit->isReady());
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::NONE);
}

TEST_F(BtnKitTest, BeginWithError) {
    btnKit = new BtnKit(nullptr, 3);
    EXPECT_FALSE(btnKit->begin());
    EXPECT_FALSE(btnKit->isReady());
}

TEST_F(BtnKitTest, ConfigurationManagement) {
    btnKit = new BtnKit(testPins, 3);
    
    BtnKitConfig config;
    config.debounceDelay = 50;
    config.invertLogic = true;
    config.inputMode = INPUT;
    
    EXPECT_TRUE(btnKit->setConfig(config));
    
    BtnKitConfig retrievedConfig = btnKit->getConfig();
    EXPECT_EQ(retrievedConfig.debounceDelay, 50);
    EXPECT_TRUE(retrievedConfig.invertLogic);
    EXPECT_EQ(retrievedConfig.inputMode, INPUT);
}

TEST_F(BtnKitTest, InvalidConfiguration) {
    btnKit = new BtnKit(testPins, 3);
    
    BtnKitConfig config;
    config.debounceDelay = 2000; // Too high
    EXPECT_FALSE(btnKit->setConfig(config));
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_ELEMENTS);
    
    config.debounceDelay = 20;
    config.inputMode = 99; // Invalid mode
    EXPECT_FALSE(btnKit->setConfig(config));
}

TEST_F(BtnKitTest, ReadWithoutInitialization) {
    btnKit = new BtnKit(testPins, 3);
    resetCallback();
    
    EXPECT_FALSE(btnKit->read(testCallback, 0));
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::NOT_INITIALIZED);
    EXPECT_EQ(callbackCount, 0);
}

TEST_F(BtnKitTest, ReadWithNullCallback) {
    btnKit = new BtnKit(testPins, 3);
    
    // Set initial states
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->begin());
    
    EXPECT_FALSE(btnKit->read(nullptr, 0));
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_FUNCTION);
}

TEST_F(BtnKitTest, ReadWithInvalidChannel) {
    btnKit = new BtnKit(testPins, 3);
    
    // Set initial states
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->begin());
    resetCallback();
    
    EXPECT_FALSE(btnKit->read(testCallback, 16)); // Invalid MIDI channel
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::INVALID_ELEMENTS);
}

TEST_F(BtnKitTest, ButtonPressDetection) {
    btnKit = new BtnKit(testPins, 3);
    
    // Set initial states to HIGH (unpressed for pullup)
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->begin());
    resetCallback();
    
    // Press button 0 (set to LOW)
    MockArduino::setDigitalValue(2, LOW);
    
    // Advance time to allow debounce processing
    MockArduino::addMillis(25); // More than debounce delay (20ms)
    
    // Now read to process the button press
    EXPECT_TRUE(btnKit->read(testCallback, 1));
    
    // Should trigger note on
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastNote, 0);
    EXPECT_EQ(lastVelocity, 127);
    EXPECT_EQ(lastChannel, 1);
    
    resetCallback();
    
    // Release button 0 (set to HIGH)
    MockArduino::setDigitalValue(2, HIGH);
    
    // Advance time to allow debounce processing
    MockArduino::addMillis(25); // More than debounce delay (20ms)
    
    // Now read to process the button release
    EXPECT_TRUE(btnKit->read(testCallback, 1));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);    
    // Should trigger note off
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastNote, 0);
    EXPECT_EQ(lastVelocity, 0);
    EXPECT_EQ(lastChannel, 1);
}

TEST_F(BtnKitTest, DebounceFiltering) {
    btnKit = new BtnKit(testPins, 1);
    
    BtnKitConfig config;
    config.debounceDelay = 100; // 100ms debounce
    EXPECT_TRUE(btnKit->setConfig(config));
    
    // Set initial state
    MockArduino::setDigitalValue(2, HIGH);
    EXPECT_TRUE(btnKit->begin());
    resetCallback();
    
    // Press button
    MockArduino::setDigitalValue(2, LOW);
    
    // Advance time by less than debounce delay
    MockArduino::addMillis(50);
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Should not trigger callback yet (still bouncing)
    EXPECT_EQ(callbackCount, 0);
    
    // Advance time to exceed debounce delay
    MockArduino::addMillis(60); // Total 110ms > 100ms debounce
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce    
    // Now should trigger callback after debounce period
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastNote, 0);
    EXPECT_EQ(lastVelocity, 127);
    
    resetCallback();
    
    // Quick release and press (should be filtered)
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::addMillis(10); // Only 10ms - less than debounce
    MockArduino::setDigitalValue(2, LOW);
    
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    EXPECT_EQ(callbackCount, 0); // Should be filtered due to debounce
    
    // Wait for full debounce period and read again
    MockArduino::addMillis(101); // Now exceed debounce delay
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    EXPECT_EQ(callbackCount, 1); // Should now register the press
}
}

TEST_F(BtnKitTest, InvertedLogicConfiguration) {
    btnKit = new BtnKit(testPins, 1);
    
    BtnKitConfig config;
    config.invertLogic = true; // HIGH = pressed
    EXPECT_TRUE(btnKit->setConfig(config));
    
    // Set initial state
    MockArduino::setDigitalValue(2, LOW);
    EXPECT_TRUE(btnKit->begin());
    resetCallback();
    
    // Press button (set to HIGH with inverted logic)
    MockArduino::setDigitalValue(2, HIGH);
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(50);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Should trigger note on
    EXPECT_EQ(callbackCount, 1);
    EXPECT_EQ(lastVelocity, 127);
}

TEST_F(BtnKitTest, ButtonStateQueries) {
    btnKit = new BtnKit(testPins, 3);
    
    // Set initial states
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, LOW);  // Pressed
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Check button states
    EXPECT_FALSE(btnKit->isButtonPressed(0)); // HIGH = not pressed for pullup
    EXPECT_TRUE(btnKit->isButtonPressed(1));  // LOW = pressed for pullup
    EXPECT_FALSE(btnKit->isButtonPressed(2));
    
    // Invalid index
    EXPECT_FALSE(btnKit->isButtonPressed(5));
}

TEST_F(BtnKitTest, JustPressedAndReleasedDetection) {
    btnKit = new BtnKit(testPins, 1);
    
    // Set initial state
    MockArduino::setDigitalValue(2, HIGH);
    EXPECT_TRUE(btnKit->begin());
    
    // Initial read
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    EXPECT_FALSE(btnKit->isButtonJustPressed(0));
    EXPECT_FALSE(btnKit->isButtonJustReleased(0));
    
    // Press button
    MockArduino::setDigitalValue(2, LOW);
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(50);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(150);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    EXPECT_TRUE(btnKit->isButtonJustPressed(0));
    EXPECT_FALSE(btnKit->isButtonJustReleased(0));
    
    // Read again without change
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    EXPECT_FALSE(btnKit->isButtonJustPressed(0)); // No longer "just" pressed
    EXPECT_FALSE(btnKit->isButtonJustReleased(0));
    
    // Release button
    MockArduino::setDigitalValue(2, HIGH);
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(50);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(50);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(20);
    EXPECT_FALSE(btnKit->isButtonJustPressed(0));
    EXPECT_TRUE(btnKit->isButtonJustReleased(0));
}

TEST_F(BtnKitTest, GetLastPressTime) {
    // Reset time at the beginning of the test
    MockArduino::reset();
    btnKit = new BtnKit(testPins, 1);
    
    // Set initial state
    MockArduino::setDigitalValue(2, HIGH);
    EXPECT_TRUE(btnKit->begin());
    
    uint32_t initialTime = btnKit->getLastPressTime(0);
    EXPECT_GT(initialTime, 0);
    
    // Wait and press button
    MockArduino::addMillis(100);
    MockArduino::setDigitalValue(2, LOW);
    EXPECT_TRUE(btnKit->read(testCallback, 0));
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(150);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    // Avanzar el tiempo para superar el debounce
    MockArduino::addMillis(101);
    
    uint32_t pressTime = btnKit->getLastPressTime(0);
    EXPECT_GT(pressTime, initialTime);
    
    // Invalid index
    EXPECT_EQ(btnKit->getLastPressTime(5), 0);
}

TEST_F(BtnKitTest, ErrorHandling) {
    btnKit = new BtnKit(testPins, 3);
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::NONE);
    
    // Trigger an error
    EXPECT_FALSE(btnKit->read(nullptr, 0));
    EXPECT_NE(btnKit->getLastError(), BtnKitError::NONE);
    
    // Clear error
    btnKit->clearError();
    EXPECT_EQ(btnKit->getLastError(), BtnKitError::NONE);
}

TEST_F(BtnKitTest, MemoryManagement) {
    // Test that destructor doesn't crash
    btnKit = new BtnKit(testPins, 3);
    delete btnKit;
    btnKit = nullptr;
    
    // Test multiple allocations
    for (int i = 0; i < 10; i++) {
        BtnKit* testKit = new BtnKit(testPins, 3);
        EXPECT_EQ(testKit->getLastError(), BtnKitError::NONE);
        delete testKit;
    }
}
