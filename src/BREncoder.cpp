#include "BREncoder.h"

// Default configuration for encoders
const BREncoder::EncoderConfig DEFAULT_LEFT_CONFIG = {
  .ccNumberCW = 14,
  .ccNumberCCW = 14, 
  .valueCW = 127,
  .valueCCW = 1,
  .midiChannel = 6,
  .debounceMs = 10
};

const BREncoder::EncoderConfig DEFAULT_RIGHT_CONFIG = {
  .ccNumberCW = 15,
  .ccNumberCCW = 15,
  .valueCW = 127, 
  .valueCCW = 1,
  .midiChannel = 6,
  .debounceMs = 10
};

BREncoder::BREncoder() 
  : leftEncoder(nullptr)
  , rightEncoder(nullptr)
  , oldLeft(-999)
  , oldRight(-999) 
  , lastLeftChange(0)
  , lastRightChange(0)
  , leftConfig(DEFAULT_LEFT_CONFIG)
  , rightConfig(DEFAULT_RIGHT_CONFIG)
  , initialized(false) {
}

BREncoder::~BREncoder() {
  delete leftEncoder;
  delete rightEncoder;
}

bool BREncoder::begin() {
  try {
    leftEncoder = new Encoder(L_BROWSER_A, L_BROWSER_B);
    rightEncoder = new Encoder(R_BROWSER_A, R_BROWSER_B);
    
    if (!leftEncoder || !rightEncoder) {
      return false;
    }
      // Read initial positions
    oldLeft = leftEncoder->read();
    oldRight = rightEncoder->read();
    
    initialized = true;
    return true;
    
  } catch (...) {
    initialized = false;
    return false;
  }
}

void BREncoder::readEnc(void (*scc_func)(uint8_t, uint8_t, uint8_t)) {
  if (!initialized || !scc_func) {
    return;
  }
    processEncoder(true, scc_func);   // Left encoder
  processEncoder(false, scc_func);  // Right encoder
}

void BREncoder::processEncoder(bool isLeft, void (*scc_func)(uint8_t, uint8_t, uint8_t)) {
  Encoder* encoder = isLeft ? leftEncoder : rightEncoder;
  long& oldPosition = isLeft ? oldLeft : oldRight;
  unsigned long& lastChange = isLeft ? lastLeftChange : lastRightChange;
  const EncoderConfig& config = isLeft ? leftConfig : rightConfig;
  
  if (!encoder) return;
  
  long newPosition = encoder->read();
  unsigned long currentTime = millis();
    // Check if there is change and if debounce time has passed
  if (newPosition != oldPosition && 
      (currentTime - lastChange) >= config.debounceMs) {
    
    long delta = newPosition - oldPosition;
    
    if (delta > 0) {
      // Clockwise rotation
      scc_func(config.ccNumberCW, config.valueCW, config.midiChannel);
    } else if (delta < 0) {
      // Counterclockwise rotation  
      scc_func(config.ccNumberCCW, config.valueCCW, config.midiChannel);
    }
    
    oldPosition = newPosition;
    lastChange = currentTime;
  }
}

void BREncoder::configureEncoder(bool isLeft, const EncoderConfig& config) {
  if (isLeft) {
    leftConfig = config;
  } else {
    rightConfig = config;
  }
}

void BREncoder::reset() {
  if (!initialized) return;
  
  if (leftEncoder) {
    leftEncoder->write(0);
    oldLeft = 0;
  }
  
  if (rightEncoder) {
    rightEncoder->write(0);
    oldRight = 0;
  }
}

long BREncoder::getPosition(bool isLeft) const {
  if (!initialized) return 0;
  
  Encoder* encoder = isLeft ? leftEncoder : rightEncoder;
  return encoder ? encoder->read() : 0;
}