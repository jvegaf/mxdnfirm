
#pragma once
#include <Arduino.h>
#define ENCODER_DO_NOT_USE_INTERRUPTS
#include "pin_map.h"
#include <Encoder.h>

/**
 * @brief Class for browser rotary encoder management
 * Manages left and right encoders with debouncing and validation
 */
class BREncoder {
public:
  // Encoder configuration
  struct EncoderConfig {    uint8_t ccNumberCW;      // Control Change for clockwise rotation
    uint8_t ccNumberCCW;     // Control Change for counterclockwise rotation  
    uint8_t valueCW;         // MIDI value for clockwise rotation
    uint8_t valueCCW;        // MIDI value for counterclockwise rotation
    uint8_t midiChannel;     // MIDI channel
    uint16_t debounceMs;     // Debounce time in ms
  };

  BREncoder();
  ~BREncoder();
    /**
   * @brief Initializes the encoders
   * @return true if initialization was successful
   */
  bool begin();
    /**
   * @brief Reads encoder state and sends MIDI if there are changes
   * @param scc_func Callback function to send Control Change
   */
  void readEnc(void (*scc_func)(uint8_t, uint8_t, uint8_t));
    /**
   * @brief Configures parameters for a specific encoder
   * @param isLeft true for left encoder, false for right
   * @param config Encoder configuration
   */
  void configureEncoder(bool isLeft, const EncoderConfig& config);
    /**
   * @brief Resets encoder positions
   */
  void reset();
    /**
   * @brief Gets the current position of an encoder
   * @param isLeft true for left encoder, false for right
   * @return Current encoder position
   */
  long getPosition(bool isLeft) const;
    /**
   * @brief Verifies if the system is initialized
   */
  bool isReady() const { return initialized; }

private:
  Encoder* leftEncoder;
  Encoder* rightEncoder;
    // Previous states
  long oldLeft;
  long oldRight;
  
  // Timestamps para debouncing
  unsigned long lastLeftChange;
  unsigned long lastRightChange;
    // Configurations
  EncoderConfig leftConfig;
  EncoderConfig rightConfig;
  
  bool initialized;
    /**
   * @brief Processes changes in a specific encoder
   * @param isLeft true for left encoder
   * @param scc_func Callback function for MIDI
   */
  void processEncoder(bool isLeft, void (*scc_func)(uint8_t, uint8_t, uint8_t));
};
