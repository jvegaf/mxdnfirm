
#pragma once
#include "pin_map.h"
#include <Arduino.h>
#include <Multiplexer4067.h>

/**
 * @brief Class for multiplexed potentiometer management
 * Manages reading of multiple potentiometers with filtering and debouncing
 */
class MuxPots {
public:  // Filtering and timing configuration
  struct FilterConfig {
    uint16_t threshold;        // Minimum change threshold
    uint16_t timeout;          // Timeout to consider movement finished
    uint16_t smoothingFactor;  // Smoothing factor (0-100)
  };
  /**
   * @brief Constructor
   * @param sig Multiplexer signal pin
   * @param el Array of elements/channels to read
   * @param t_el Total elements
   */
  MuxPots(const uint8_t sig, const uint8_t *el, const uint8_t t_el);
    /**
   * @brief Destructor - frees allocated memory
   */
  ~MuxPots();
  /**
   * @brief Initializes the multiplexer
   * @return true if initialization was successful
   */
  bool begin();
    /**
   * @brief Reads all potentiometers and sends MIDI changes
   * @param scc_func Callback function to send Control Change
   * @param midiCh MIDI channel to use
   */
  void read(void (*scc_func)(uint8_t, uint8_t, uint8_t), uint8_t midiCh);
    /**
   * @brief Configures filtering parameters
   * @param config New filtering configuration
   */
  void setFilterConfig(const FilterConfig& config);
    /**
   * @brief Gets the current value of a specific potentiometer
   * @param index Potentiometer index
   * @return MIDI value (0-127) or 0xFFFF if index is invalid
   */
  int getCurrentValue(uint8_t index) const;
    /**
   * @brief Resets all stored values
   */
  void reset();
    /**
   * @brief Verifies if the system is initialized
   */
  bool isReady() const { return initialized; }
    /**
   * @brief Calibrates a specific potentiometer
   * @param index Potentiometer index
   * @param minVal Minimum read value
   * @param maxVal Maximum read value
   */
  void calibratePot(uint8_t index, uint16_t minVal, uint16_t maxVal);

private:
  // Configuration
  const uint8_t *elements;
  const uint8_t tElements;
  FilterConfig filterConfig;
  
  // Dynamic arrays for state
  uint8_t *lastCcValue;      // Last MIDI value sent
  uint16_t *potCState;       // Current state
  uint16_t *potPState;       // Previous state
  uint16_t *smoothedValue;   // Smoothed value
  uint32_t *pTime;           // Last activity time
  uint32_t *timer;           // Calculated timer
  
  // Calibration
  uint16_t *minValues;       // Calibrated minimum values
  uint16_t *maxValues;       // Calibrated maximum values
  
  Multiplexer4067 *mplexPots;
  bool initialized;
  
  /**
   * @brief Clean up allocated resources
   * Used to free memory when allocation fails or during destruction
   */
  void clean();
    /**
   * @brief Applies smoothing to a value
   * @param index Potentiometer index
   * @param newValue New read value
   * @return Smoothed value
   */
  uint16_t applySmoothingFilter(uint8_t index, uint16_t newValue);
    /**
   * @brief Converts analog value to MIDI with calibration
   * @param index Potentiometer index
   * @param analogValue Read analog value
   * @return MIDI value (0-127)
   */
  uint8_t analogToMidi(uint8_t index, uint16_t analogValue) const;
    /**
   * @brief Validates that an index is in valid range
   * @param index Index to validate
   * @return true if the index is valid
   */
  bool isValidIndex(uint8_t index) const;
};
