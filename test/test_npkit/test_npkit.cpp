#include <gtest/gtest.h>
#include "../../src/NPKit.h"
#include "../mocks/Arduino.h"

class NPKitTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Reset mock state
        MockArduino::reset();
        Adafruit_NeoPixel::reset();
        
        // Clean up NPKit state from previous tests
        NPKit::cleanup();
        NPKit::clearError();
    }
    
    void TearDown() override {
        // Clean up after test
        NPKit::cleanup();
    }
};

TEST_F(NPKitTest, BasicInitialization) {
    EXPECT_TRUE(NPKit::begin());
    EXPECT_TRUE(NPKit::isReady());
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NONE);
    EXPECT_TRUE(Adafruit_NeoPixel::getInitialized());
}

TEST_F(NPKitTest, InitializationWithConfig) {
    NPKitConfig config;
    config.brightness = 64;
    config.autoShow = false;
    config.defaultColor = 0xFF0000; // Red
    
    EXPECT_TRUE(NPKit::begin(config));
    EXPECT_TRUE(NPKit::isReady());
    
    NPKitConfig retrievedConfig = NPKit::getConfig();
    EXPECT_EQ(retrievedConfig.brightness, 64);
    EXPECT_FALSE(retrievedConfig.autoShow);
    EXPECT_EQ(retrievedConfig.defaultColor, 0xFF0000);
    
    EXPECT_EQ(Adafruit_NeoPixel::getBrightness(), 64);
}

TEST_F(NPKitTest, ConfigurationManagement) {
    EXPECT_TRUE(NPKit::begin());
    
    NPKitConfig config;
    config.brightness = 128;
    config.autoShow = false;
    
    EXPECT_TRUE(NPKit::setConfig(config));
    
    NPKitConfig retrievedConfig = NPKit::getConfig();
    EXPECT_EQ(retrievedConfig.brightness, 128);
    EXPECT_FALSE(retrievedConfig.autoShow);
}

TEST_F(NPKitTest, InvalidConfiguration) {
    EXPECT_TRUE(NPKit::begin());
    
    NPKitConfig config;
    config.brightness = 300; // Invalid brightness > 255
    
    EXPECT_FALSE(NPKit::setConfig(config));
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_VALUE);
}

TEST_F(NPKitTest, BasicPixelOperations) {
    EXPECT_TRUE(NPKit::begin());
    
    // Test setting individual pixel color
    EXPECT_TRUE(NPKit::setPixelColor(0, 255, 0, 0)); // Red
    EXPECT_TRUE(NPKit::setPixelColor(1, 0, 255, 0)); // Green
    EXPECT_TRUE(NPKit::setPixelColor(2, 0, 0, 255)); // Blue
    
    // Verify colors were set
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(0), Adafruit_NeoPixel::Color(255, 0, 0));
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(1), Adafruit_NeoPixel::Color(0, 255, 0));
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(2), Adafruit_NeoPixel::Color(0, 0, 255));
}

TEST_F(NPKitTest, PixelOperationsWithoutInitialization) {
    // Try operations without initialization
    EXPECT_FALSE(NPKit::setPixelColor(0, 255, 0, 0));
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NOT_INITIALIZED);
    
    EXPECT_FALSE(NPKit::clear());
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NOT_INITIALIZED);
    
    EXPECT_FALSE(NPKit::show());
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NOT_INITIALIZED);
}

TEST_F(NPKitTest, InvalidPositions) {
    EXPECT_TRUE(NPKit::begin());
    
    // Test with invalid positions
    EXPECT_FALSE(NPKit::setPixelColor(100, 255, 0, 0)); // Position > tPixels
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_POSITION);
    
    EXPECT_FALSE(NPKit::handleChange(200, 1));
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_POSITION);
}

TEST_F(NPKitTest, HandleChangeFunction) {
    EXPECT_TRUE(NPKit::begin());
    
    // Test different cue values
    EXPECT_TRUE(NPKit::handleChange(0, 0)); // Clear
    EXPECT_TRUE(NPKit::handleChange(1, 1)); // Blue (cue)
    EXPECT_TRUE(NPKit::handleChange(2, 2)); // Orange (fade out)
    EXPECT_TRUE(NPKit::handleChange(3, 3)); // Yellow (load)
    EXPECT_TRUE(NPKit::handleChange(4, 4)); // Gray (grid)
    EXPECT_TRUE(NPKit::handleChange(5, 5)); // Green (loop)
    
    // Verify some colors
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(0), NPKit::CLEAR_COL);
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(1), NPKit::BLUE_COL);
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(2), NPKit::ORANGE_COL);
}

TEST_F(NPKitTest, HandleChangeInvalidValue) {
    EXPECT_TRUE(NPKit::begin());
    
    // Test with invalid cue value
    EXPECT_FALSE(NPKit::handleChange(0, 10)); // Value too high
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_VALUE);
}

TEST_F(NPKitTest, HandleColorChange) {
    EXPECT_TRUE(NPKit::begin());
    
    uint32_t customColor = NPKit::colorFromRGB(128, 64, 192);
    EXPECT_TRUE(NPKit::handleColorChange(0, customColor));
    
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(0), customColor);
}

TEST_F(NPKitTest, ClearFunction) {
    EXPECT_TRUE(NPKit::begin());
    
    // Set some pixels first
    EXPECT_TRUE(NPKit::setPixelColor(0, 255, 0, 0));
    EXPECT_TRUE(NPKit::setPixelColor(1, 0, 255, 0));
    
    // Clear all pixels
    EXPECT_TRUE(NPKit::clear());
    
    // Verify all pixels are cleared
    for (uint8_t i = 0; i < NPKit::getPixelCount(); i++) {
        EXPECT_EQ(Adafruit_NeoPixel::getPixelData(i), 0);
    }
}

TEST_F(NPKitTest, BrightnessControl) {
    EXPECT_TRUE(NPKit::begin());
    
    EXPECT_TRUE(NPKit::setBrightness(100));
    EXPECT_EQ(NPKit::getBrightness(), 100);
    EXPECT_EQ(Adafruit_NeoPixel::getBrightness(), 100);
    
    EXPECT_TRUE(NPKit::setBrightness(255));
    EXPECT_EQ(NPKit::getBrightness(), 255);
}

TEST_F(NPKitTest, UtilityFunctions) {
    EXPECT_TRUE(NPKit::begin());
    
    // Test pixel count
    EXPECT_EQ(NPKit::getPixelCount(), 21); // tPixels from np_map.h
    
    // Test position validation
    EXPECT_TRUE(NPKit::isValidPosition(0));
    EXPECT_TRUE(NPKit::isValidPosition(20));
    EXPECT_FALSE(NPKit::isValidPosition(21));
    EXPECT_FALSE(NPKit::isValidPosition(100));
    
    // Test color creation
    uint32_t color = NPKit::colorFromRGB(255, 128, 64);
    EXPECT_EQ(color, Adafruit_NeoPixel::Color(255, 128, 64));
}

TEST_F(NPKitTest, SetAllPixels) {
    EXPECT_TRUE(NPKit::begin());
    
    uint32_t testColor = NPKit::RED_COL;
    EXPECT_TRUE(NPKit::setAllPixels(testColor));
    
    // Verify all pixels are set to the test color
    for (uint8_t i = 0; i < NPKit::getPixelCount(); i++) {
        EXPECT_EQ(Adafruit_NeoPixel::getPixelData(i), testColor);
    }
}

TEST_F(NPKitTest, FadePixel) {
    EXPECT_TRUE(NPKit::begin());
    
    // Set a bright pixel
    EXPECT_TRUE(NPKit::setPixelColor(0, 255, 255, 255));
    
    // Fade it
    EXPECT_TRUE(NPKit::fadePixel(0, 50));
    
    // Verify the pixel was dimmed
    uint32_t fadedColor = Adafruit_NeoPixel::getPixelData(0);
    EXPECT_EQ(fadedColor, Adafruit_NeoPixel::Color(205, 205, 205));
    
    // Test invalid position
    EXPECT_FALSE(NPKit::fadePixel(100, 10));
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_POSITION);
}

TEST_F(NPKitTest, RainbowPattern) {
    EXPECT_TRUE(NPKit::begin());
    
    EXPECT_TRUE(NPKit::setRainbow());
    
    // Verify different pixels have different colors
    uint32_t color0 = Adafruit_NeoPixel::getPixelData(0);
    uint32_t color10 = Adafruit_NeoPixel::getPixelData(10);
    uint32_t color20 = Adafruit_NeoPixel::getPixelData(20);
    
    EXPECT_NE(color0, color10);
    EXPECT_NE(color10, color20);
    EXPECT_NE(color0, color20);
}

TEST_F(NPKitTest, TestPattern) {
    EXPECT_TRUE(NPKit::begin());
    
    EXPECT_TRUE(NPKit::setTestPattern());
    
    // Verify pattern repeats
    uint32_t color0 = Adafruit_NeoPixel::getPixelData(0);
    uint32_t color4 = Adafruit_NeoPixel::getPixelData(4); // Should be same as 0
    
    EXPECT_EQ(color0, color4); // Test pattern should repeat every 4 pixels
}

TEST_F(NPKitTest, PulsePixel) {
    EXPECT_TRUE(NPKit::begin());
    
    uint32_t baseColor = NPKit::RED_COL;
    
    // Test different intensities
    EXPECT_TRUE(NPKit::pulsePixel(0, baseColor, 255)); // Full intensity
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(0), baseColor);
    
    EXPECT_TRUE(NPKit::pulsePixel(1, baseColor, 128)); // Half intensity
    uint32_t halfIntensity = Adafruit_NeoPixel::getPixelData(1);
    EXPECT_NE(halfIntensity, baseColor);
    
    EXPECT_TRUE(NPKit::pulsePixel(2, baseColor, 0)); // No intensity
    EXPECT_EQ(Adafruit_NeoPixel::getPixelData(2), 0);
    
    // Test invalid position
    EXPECT_FALSE(NPKit::pulsePixel(100, baseColor, 255));
    EXPECT_EQ(NPKit::getLastError(), NPKitError::INVALID_POSITION);
}

TEST_F(NPKitTest, GetPixelColor) {
    EXPECT_TRUE(NPKit::begin());
    
    uint32_t testColor = NPKit::YELLOW_COL;
    EXPECT_TRUE(NPKit::setPixelColor(5, testColor));
    
    EXPECT_EQ(NPKit::getPixelColor(5), testColor);
    
    // Test invalid position
    EXPECT_EQ(NPKit::getPixelColor(100), 0);
}

TEST_F(NPKitTest, ErrorHandling) {
    EXPECT_TRUE(NPKit::begin());
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NONE);
    
    // Trigger an error
    EXPECT_FALSE(NPKit::setPixelColor(100, 255, 0, 0));
    EXPECT_NE(NPKit::getLastError(), NPKitError::NONE);
    
    // Clear error
    NPKit::clearError();
    EXPECT_EQ(NPKit::getLastError(), NPKitError::NONE);
}

TEST_F(NPKitTest, AutoShowConfiguration) {
    NPKitConfig config;
    config.autoShow = false;
    
    EXPECT_TRUE(NPKit::begin(config));
    
    // With autoShow disabled, manual show should be required
    EXPECT_TRUE(NPKit::setPixelColor(0, 255, 0, 0));
    EXPECT_TRUE(NPKit::show()); // Manual show
    
    // Test that clear still works
    EXPECT_TRUE(NPKit::clear());
}

TEST_F(NPKitTest, ColorConstants) {
    // Test that color constants are properly defined
    EXPECT_EQ(NPKit::CLEAR_COL, Adafruit_NeoPixel::Color(0, 0, 0));
    EXPECT_EQ(NPKit::RED_COL, Adafruit_NeoPixel::Color(255, 0, 0));
    EXPECT_EQ(NPKit::GREEN_COL, Adafruit_NeoPixel::Color(0, 255, 0));
    EXPECT_EQ(NPKit::BLUE_COL, Adafruit_NeoPixel::Color(0, 0, 255));
    EXPECT_EQ(NPKit::YELLOW_COL, Adafruit_NeoPixel::Color(255, 230, 0));
    EXPECT_EQ(NPKit::ORANGE_COL, Adafruit_NeoPixel::Color(255, 143, 0));
    EXPECT_EQ(NPKit::PURPLE_COL, Adafruit_NeoPixel::Color(243, 0, 255));
    EXPECT_EQ(NPKit::GRAY_COL, Adafruit_NeoPixel::Color(191, 201, 202));
}
