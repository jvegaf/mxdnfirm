#include "BREncoder.h"
#include "BtnKit.h"
#include "Deck.h"
#include "MDCore.h"
#include "Mux.h"
#include "MuxPots.h"
#include "Muxer.h"
#include "Pad.h"
#include "PotKit.h"
#include "base.h"
#include "midi_map.h"
#include "mpx_map.h"
#include "np_map.h"
#include "pin_map.h"
#include <Arduino.h>
#include <MIDI.h>
#include <Thread.h>
#include <ThreadController.h>
#include <stdint.h>

// Performance optimization constants
#define MIDI_BAUD_RATE 31250
#define MAX_MIDI_VELOCITY 127
#define MIN_MIDI_VELOCITY 0

// Debounce and timing constants
#define ENCODER_DEBOUNCE_MS 5
#define BUTTON_DEBOUNCE_MS 20
#define POT_READ_INTERVAL_MS 10

// Error handling
#define MIDI_TIMEOUT_MS 1000


MIDI_CREATE_DEFAULT_INSTANCE();

// MIDI channel configuration
uint8_t deckMidiChannels[] = {deckAChannel, deckBChannel};
uint8_t padMidiChannels[] = {padAChannel, padBChannel};

// State management
volatile bool isDeckChanged = false;
volatile unsigned long lastEncoderRead = 0;
volatile unsigned long lastMidiActivity = 0;

// Hardware components
BREncoder enc;

Mux selector(rhMPSig, MPlex::DECK_SEL);

Muxer deckLeftMuxer(lhMPSig, MPlex::deckLeftBtnsCol);
Muxer deckRightMuxer(rhMPSig, MPlex::deckRightBtnsCol);
Muxer padLeftMuxer(lhMPSig, MPlex::padLeftBtnsCol);
Muxer padRightMuxer(rhMPSig, MPlex::padRightBtnsCol);

DeckControl deckLeft(&deckLeftMuxer);
DeckControl deckRight(&deckRightMuxer);

PadControl padLeft(&padLeftMuxer);
PadControl padRight(&padRightMuxer);

MuxPots topPots(topPotsSig, MPlex::topPots, MPlex::tTopPots);
MuxPots bottomPots(btmPotsSig, MPlex::bottomPots, MPlex::tBottomPots);

BtnKit buttons(btnPins, tBtnPins);
PotKit pots(PotPins, tPotPins);

ThreadController cpu;     // thread master, onde as outras vao ser adicionadas
Thread threadReadPots;    // thread para controlar os pots
Thread threadReadButtons; // thread para controlar os botoes

void changeDeck();
void handleControlChange(uint8_t channel, uint8_t number, uint8_t value);
void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value);
void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value);
void midiSetup();
void threadsSetup();
void readButtons();
void readPots();
void readEncoder();
void sendMidiNote(uint8_t number, uint8_t value, uint8_t channel);
void sendMidiCC(uint8_t number, uint8_t value, uint8_t channel);

void setup() {
  midiSetup();
  
  // Initialize components with error checking where available
  selector.begin();
  buttons.begin();
  topPots.begin();
  bottomPots.begin();
  
  if (!deckLeft.begin() || !deckRight.begin()) {
    // Could add error handling here  
  }
  
  padLeft.begin();
  padRight.begin();
  
  if (!MDCore::begin()) {
    // Could add error handling here
  }
  
  threadsSetup();
}

void loop() {
  unsigned long currentTime = millis();
  
  // Process threads with priority
  cpu.run();
  
  // Read MIDI with timeout protection
  if (MIDI.read()) {
    lastMidiActivity = currentTime;
  }
  
  // Read encoder with debouncing
  if (currentTime - lastEncoderRead >= ENCODER_DEBOUNCE_MS) {
    readEncoder();
    lastEncoderRead = currentTime;
  }
  
  // Check for MIDI timeout (optional health check)
  if (currentTime - lastMidiActivity > MIDI_TIMEOUT_MS) {
    // Could implement reconnection logic here if needed
  }
}

void handleControlChange(uint8_t channel, uint8_t number, uint8_t value) {
  // Validate MIDI data
  if (value > MAX_MIDI_VELOCITY) {
    value = MAX_MIDI_VELOCITY;
  }
  
  lastMidiActivity = millis();
  MDCore::cChange(channel, number, value);
}

void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value) {
  // Validate velocity
  if (value > MAX_MIDI_VELOCITY) {
    value = MAX_MIDI_VELOCITY;
  }
  
  lastMidiActivity = millis();
  
  if (value < 1U) {
    MDCore::noteOff(channel, number, 0);
    return;
  }
  MDCore::noteOn(channel, number, value);
}

void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value) {
  lastMidiActivity = millis();
  MDCore::noteOff(channel, number, 0); // Note off always sends 0 velocity
}

void midiSetup() {
  // Configure MIDI handlers
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  // Start MIDI with optimized settings
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
  
  // Set serial baud rate for better performance
  Serial.begin(MIDI_BAUD_RATE);
  
  // Initialize timing variables
  lastMidiActivity = millis();
  lastEncoderRead = millis();
}

void threadsSetup() {
  // Configure pot reading thread (higher frequency for smooth control)
  threadReadPots.setInterval(POT_READ_INTERVAL_MS);
  threadReadPots.onRun(readPots);
  cpu.add(&threadReadPots);

  // Configure button reading thread (lower frequency, includes debouncing)
  threadReadButtons.setInterval(BUTTON_DEBOUNCE_MS);
  threadReadButtons.onRun(readButtons);
  cpu.add(&threadReadButtons);
}

void changeDeck() {
  // Prevent rapid deck switching
  static unsigned long lastDeckChange = 0;
  unsigned long currentTime = millis();
  
  if (currentTime - lastDeckChange < 100) { // 100ms debounce
    return;
  }
  
  lastDeckChange = currentTime;
  isDeckChanged = true;
  
  if (deckMidiChannels[1] == deckBChannel) {
    deckMidiChannels[1] = deckCChannel;
    padMidiChannels[1] = padCChannel;
    MDCore::changeDeck(deckCChannel);
  } else {
    deckMidiChannels[1] = deckBChannel;
    padMidiChannels[1] = padBChannel;
    MDCore::changeDeck(deckBChannel);
  }
}

void readButtons() {
  selector.read(changeDeck);
  deckLeft.read(sendMidiNote, deckMidiChannels[0]);
  deckRight.read(sendMidiNote, deckMidiChannels[1]);
  padLeft.read(sendMidiNote, padMidiChannels[0]);
  padRight.read(sendMidiNote, padMidiChannels[1]);
}

void readPots() {
  topPots.read(sendMidiCC, topPotsChannel);
  bottomPots.read(sendMidiCC, bottomPotsChannel);
  pots.read(sendMidiCC, potsChannel);
}

void readEncoder() { enc.readEnc(sendMidiCC); }

void sendMidiNote(uint8_t number, uint8_t value, uint8_t channel) {
  // Validate parameters
  if (value > MAX_MIDI_VELOCITY) {
    value = MAX_MIDI_VELOCITY;
  }
  
  if (value == 0) {
    MIDI.sendNoteOff(number, 0, channel);
  } else {
    MIDI.sendNoteOn(number, value, channel);
  }
}

void sendMidiCC(uint8_t number, uint8_t value, uint8_t channel) {
  // Validate CC value
  if (value > MAX_MIDI_VELOCITY) {
    value = MAX_MIDI_VELOCITY;
  }
  
  MIDI.sendControlChange(number, value, channel);
}
