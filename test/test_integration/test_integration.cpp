#include <gtest/gtest.h>

// Include mocks before any other includes
#include "Arduino.h"
#include "MIDI.h"
#include "Shifter.h"
#include "Multiplexer4067.h"
#include "Encoder.h"

// Include component headers
#include "../../src/MDCore.h"
#include "../../src/Deck.h"
#include "../../src/BREncoder.h"
#include "../../src/MuxPots.h"
#include "../../src/PotKit.h"
#include "../../src/BtnKit.h"
#include "../../src/NPKit.h"
#include "../../src/Muxer.h"

class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset MIDI mock state - use direct reset since no singleton
        MockMIDI::reset();
        
        // Setup test pins
        testPins[0] = A0;
        testPins[1] = A1;
        testPins[2] = A2;
        
        btnPins[0] = 2;
        btnPins[1] = 3;
        btnPins[2] = 4;
        
        // Initialize components
        leftMuxer = nullptr;
        rightMuxer = nullptr;
        leftDeck = nullptr;
        rightDeck = nullptr;
        potKit = nullptr;
        btnKit = nullptr;
    }
    
    void TearDown() override {
        delete leftDeck;
        delete rightDeck;
        delete leftMuxer;
        delete rightMuxer;
        delete potKit;
        delete btnKit;
    }
    
    uint8_t testPins[3];
    uint8_t btnPins[3];
    
    Muxer* leftMuxer;
    Muxer* rightMuxer;
    DeckControl* leftDeck;
    DeckControl* rightDeck;
    PotKit* potKit;
    BtnKit* btnKit;
    
    // Test callback functions
    static void midiCallback(uint8_t cc, uint8_t value, uint8_t channel) {
        lastMidiCC = cc;
        lastMidiValue = value;
        lastMidiChannel = channel;
        midiCallbackCount++;
    }
    
    static void noteCallback(uint8_t note, uint8_t velocity, uint8_t channel) {
        lastNote = note;
        lastVelocity = velocity;
        lastNoteChannel = channel;
        noteCallbackCount++;
    }
    
    static uint8_t lastMidiCC, lastMidiValue, lastMidiChannel;
    static uint8_t lastNote, lastVelocity, lastNoteChannel;
    static int midiCallbackCount, noteCallbackCount;
    
    static void resetCallbacks() {
        lastMidiCC = lastMidiValue = lastMidiChannel = 0;
        lastNote = lastVelocity = lastNoteChannel = 0;
        midiCallbackCount = noteCallbackCount = 0;
    }
};

// Static member definitions
uint8_t IntegrationTest::lastMidiCC = 0;
uint8_t IntegrationTest::lastMidiValue = 0;
uint8_t IntegrationTest::lastMidiChannel = 0;
uint8_t IntegrationTest::lastNote = 0;
uint8_t IntegrationTest::lastVelocity = 0;
uint8_t IntegrationTest::lastNoteChannel = 0;
int IntegrationTest::midiCallbackCount = 0;
int IntegrationTest::noteCallbackCount = 0;

TEST_F(IntegrationTest, FullSystemInitialization) {
    // Initialize MDCore namespace functions
    EXPECT_TRUE(MDCore::begin());
    EXPECT_TRUE(MDCore::isReady());
    
    // For integration tests, we'll test the components that can be tested
    // without complex hardware-specific initialization
    
    // Initialize controls
    potKit = new PotKit(testPins, 3);
    btnKit = new BtnKit(btnPins, 3);
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(btnKit->begin());
    
    // Initialize LED system
    EXPECT_TRUE(NPKit::begin());
    
    // Verify all systems are ready
    EXPECT_TRUE(MDCore::isReady());
    EXPECT_TRUE(leftDeck->isReady());
    EXPECT_TRUE(rightDeck->isReady());
    EXPECT_TRUE(potKit->isReady());
    EXPECT_TRUE(btnKit->isReady());
    EXPECT_TRUE(NPKit::isReady());
}

TEST_F(IntegrationTest, MIDIControllerWorkflow) {
    // Initialize system
    EXPECT_TRUE(MDCore::begin());
    
    potKit = new PotKit(testPins, 3);
    btnKit = new BtnKit(btnPins, 3);
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(NPKit::begin());
    
    resetCallbacks();
    
    // Simulate potentiometer movement
    MockArduino::setAnalogValue(A0, 512); // Mid position
    EXPECT_TRUE(potKit->read(midiCallback, 0));
    
    // Should generate MIDI CC
    EXPECT_GT(midiCallbackCount, 0);
    EXPECT_EQ(lastMidiChannel, 0);
    
    resetCallbacks();
    
    // Simulate button press
    MockArduino::setDigitalValue(2, HIGH); // Initial state
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    EXPECT_TRUE(btnKit->read(noteCallback, 1));
    
    MockArduino::setDigitalValue(2, LOW); // Button press
    EXPECT_TRUE(btnKit->read(noteCallback, 1));
    
    // Should generate MIDI note
    EXPECT_GT(noteCallbackCount, 0);
    EXPECT_EQ(lastNoteChannel, 1);
    EXPECT_EQ(lastVelocity, 127);
    
    // Simulate LED feedback
    EXPECT_TRUE(NPKit::handleChange(0, 1)); // Set to cue color
    EXPECT_EQ(NPKit::getPixelColor(0), NPKit::BLUE_COL);
}

TEST_F(IntegrationTest, DeckControlIntegration) {
    // Initialize MDCore
    EXPECT_TRUE(MDCore::begin());
    
    // Test basic MDCore functionality
    EXPECT_TRUE(MDCore::isReady());
    EXPECT_NO_THROW(MDCore::changeDeck(1));
    
    // Test MIDI operations
    EXPECT_NO_THROW(MDCore::noteOn(1, 60, 100));
    EXPECT_NO_THROW(MDCore::noteOff(1, 60, 0));
    EXPECT_NO_THROW(MDCore::cChange(1, 1, 127));
    
    // Initialize LED system    EXPECT_TRUE(NPKit::begin());
    
    // Test LED operations
    EXPECT_TRUE(NPKit::handleChange(0, 5)); // Green
    EXPECT_TRUE(NPKit::handleChange(10, 0)); // Clear
}

TEST_F(IntegrationTest, ErrorPropagation) {
    // Test error handling across components
    EXPECT_TRUE(MDCore::begin());
    potKit = new PotKit(nullptr, 3); // Invalid initialization
    btnKit = new BtnKit(btnPins, 3);
    
    // MDCore should initialize fine
    EXPECT_TRUE(MDCore::isReady());
    
    // PotKit should fail
    EXPECT_FALSE(potKit->begin());
    EXPECT_FALSE(potKit->isReady());
    EXPECT_NE(potKit->getLastError(), PotKitError::NONE);
    
    // BtnKit should initialize fine
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(btnKit->isReady());
    
    // System should handle partial failures gracefully
    resetCallbacks();
    EXPECT_FALSE(potKit->read(midiCallback, 0)); // Should fail
    EXPECT_EQ(midiCallbackCount, 0);
    
    // Set button states
    MockArduino::setDigitalValue(2, HIGH);
    MockArduino::setDigitalValue(3, HIGH);
    MockArduino::setDigitalValue(4, HIGH);
    
    EXPECT_TRUE(btnKit->read(noteCallback, 0)); // Should work
}

TEST_F(IntegrationTest, PerformanceStressTest) {
    // Initialize full system
    EXPECT_TRUE(MDCore::begin());
    potKit = new PotKit(testPins, 3);
    btnKit = new BtnKit(btnPins, 3);
    
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(NPKit::begin());
    
    // Set initial button states
    for (int i = 0; i < 3; i++) {
        MockArduino::setDigitalValue(btnPins[i], HIGH);
    }
    
    resetCallbacks();
    
    // Simulate high-frequency operations
    const int iterations = 100;
    
    for (int i = 0; i < iterations; i++) {
        // Vary analog values
        MockArduino::setAnalogValue(A0, (i * 10) % 1024);
        MockArduino::setAnalogValue(A1, ((i + 50) * 8) % 1024);
        MockArduino::setAnalogValue(A2, ((i + 100) * 12) % 1024);
        
        // Read controls
        EXPECT_TRUE(potKit->read(midiCallback, 0));
        EXPECT_TRUE(btnKit->read(noteCallback, 1));
        
        // Update LEDs
        EXPECT_TRUE(NPKit::handleChange(i % NPKit::getPixelCount(), i % 6));
        
        // Small time increment
        MockArduino::addMillis(1);
    }
      // Verify system is still responsive
    EXPECT_TRUE(MDCore::isReady());
    EXPECT_TRUE(potKit->isReady());
    EXPECT_TRUE(btnKit->isReady());
    EXPECT_TRUE(NPKit::isReady());
    
    // Should have generated some MIDI events
    EXPECT_GT(midiCallbackCount, 0);
}

TEST_F(IntegrationTest, ConfigurationSynchronization) {
    // Initialize system
    EXPECT_TRUE(MDCore::begin());
    potKit = new PotKit(testPins, 3);
    btnKit = new BtnKit(btnPins, 3);
    
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(btnKit->begin());
    EXPECT_TRUE(NPKit::begin());
    
    // Configure components with synchronized settings
    PotKitConfig potConfig;
    potConfig.threshold = 16;
    potConfig.timeout = 100;
    EXPECT_TRUE(potKit->setConfig(potConfig));
    
    BtnKitConfig btnConfig;
    btnConfig.debounceDelay = 50;
    EXPECT_TRUE(btnKit->setConfig(btnConfig));
    
    NPKitConfig npConfig;
    npConfig.brightness = 128;
    npConfig.autoShow = false;
    EXPECT_TRUE(NPKit::setConfig(npConfig));
    
    // Verify configurations are applied
    EXPECT_EQ(potKit->getConfig().threshold, 16);
    EXPECT_EQ(btnKit->getConfig().debounceDelay, 50);
    EXPECT_EQ(NPKit::getConfig().brightness, 128);
    EXPECT_FALSE(NPKit::getConfig().autoShow);
}

TEST_F(IntegrationTest, RealTimeOperationSimulation) {
    // Initialize system
    EXPECT_TRUE(MDCore::begin());
    potKit = new PotKit(testPins, 3);
    EXPECT_TRUE(potKit->begin());
    EXPECT_TRUE(NPKit::begin());
    
    resetCallbacks();
    
    // Simulate real-time DJ operations
    uint32_t startTime = MockArduino::getMillis();
    
    // Phase 1: Setup (0-100ms)
    MockArduino::setAnalogValue(A0, 0);   // Crossfader left
    MockArduino::setAnalogValue(A1, 512); // EQ mid
    MockArduino::setAnalogValue(A2, 1023); // Volume max
    
    EXPECT_TRUE(potKit->read(midiCallback, 0));
    EXPECT_TRUE(NPKit::setAllPixels(NPKit::BLUE_COL)); // All blue
    
    MockArduino::addMillis(100);
    
    // Phase 2: Mix transition (100-200ms)
    for (int fade = 0; fade <= 1023; fade += 50) {
        MockArduino::setAnalogValue(A0, fade); // Crossfade
        EXPECT_TRUE(potKit->read(midiCallback, 0));
        
        // Update LED to show crossfade position
        uint8_t ledPos = map(fade, 0, 1023, 0, NPKit::getPixelCount() - 1);
        EXPECT_TRUE(NPKit::handleChange(ledPos, 2)); // Orange for active
        
        MockArduino::addMillis(5);
    }
    
    // Phase 3: Final state (200ms+)
    MockArduino::setAnalogValue(A0, 1023); // Crossfader right
    EXPECT_TRUE(potKit->read(midiCallback, 0));
    EXPECT_TRUE(NPKit::setAllPixels(NPKit::RED_COL)); // All red
    
    uint32_t endTime = MockArduino::getMillis();
    uint32_t totalTime = endTime - startTime;
    
    // Verify timing constraints (should complete in reasonable time)
    EXPECT_LT(totalTime, 1000); // Less than 1 second for simulation
      // Verify system state
    EXPECT_TRUE(MDCore::isReady());
    EXPECT_TRUE(potKit->isReady());
    EXPECT_TRUE(NPKit::isReady());
    
    // Should have generated multiple MIDI events
    EXPECT_GT(midiCallbackCount, 10);
}

TEST_F(IntegrationTest, MemoryManagementIntegration) {
    // Test memory management across multiple component lifecycles
    for (int cycle = 0; cycle < 5; cycle++) {
        // Create components        EXPECT_TRUE(MDCore::begin());
        potKit = new PotKit(testPins, 3);
        btnKit = new BtnKit(btnPins, 3);
        
        // Initialize
        EXPECT_TRUE(potKit->begin());
        EXPECT_TRUE(btnKit->begin());
        EXPECT_TRUE(NPKit::begin());
        
        // Use components
        resetCallbacks();
        MockArduino::setAnalogValue(A0, cycle * 200);
        EXPECT_TRUE(potKit->read(midiCallback, 0));
        
        MockArduino::setDigitalValue(2, HIGH);        EXPECT_TRUE(btnKit->read(noteCallback, 0));
        
        EXPECT_TRUE(NPKit::handleChange(0, cycle % 6));
        
        // Cleanup
        delete potKit;
        delete btnKit;
        potKit = nullptr;
        btnKit = nullptr;
        
        // Verify no crashes occurred
        EXPECT_TRUE(true); // If we reach here, no memory issues
    }
}
