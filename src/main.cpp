#include "BREncoder.h"
#include "BtnKit.h"
#include "MDCore.h"
#include "PotKit.h"
#include "pin_map.h"
#include "np_map.h"
#include "mpx_map.h"
#include "Muxer.h"
#include "Mux.h"
#include <Arduino.h>
#include <MIDI.h>
#include <Thread.h>
#include <ThreadController.h>
// Rev2 Version
//
const uint8_t DECK_B = 2;
const uint8_t DECK_C = 3;

MIDI_CREATE_DEFAULT_INSTANCE();
volatile uint8_t deckSelected = 2;
BREncoder enc;
BtnKit buttons;
PotKit pots;
MDCore mdCore;

Mux selector(mux_pins, lh_mp_data, MPlex::DECK_SEL);

ThreadController cpu;     // thread master, onde as outras vao ser adicionadas
Thread threadReadPots;    // thread para controlar os pots
Thread threadReadButtons; // thread para controlar os botoes

void changeDeck();
void handleControlChange(byte channel, byte number, byte value);
void handleNoteOn(byte channel, byte number, byte value);
void handleNoteOff(byte channel, byte number, byte value);
void midiSetup();
void threadsSetup();
void readButtons();
void readPots();
void readEncoder();
void sendMidiNote(byte number, byte value, byte channel);
void sendMidiCC(byte number, byte value, byte channel);

void setup() {
  midiSetup();
  buttons.begin();
  pots.begin();
  mdCore.begin();
  threadsSetup();
}

void loop() {
  cpu.run();
  MIDI.read();
  readEncoder();
}

void handleControlChange(byte channel, byte number, byte value) {
  mdCore.cChange(channel, number, value);
}

void handleNoteOn(byte channel, byte number, byte value) {
  if (value < 1U) {
    mdCore.noteOff(channel, number, value);
    return;
  }
  mdCore.noteOn(channel, number, value);
}
void handleNoteOff(byte channel, byte number, byte value) {
  mdCore.noteOff(channel, number, value);
}

void midiSetup() {
  MIDI.setHandleControlChange(handleControlChange);
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.turnThruOff();
}

void threadsSetup() {
  // pots
  threadReadPots.setInterval(10);
  threadReadPots.onRun(readPots);
  cpu.add(&threadReadPots);

  // buttons
  threadReadButtons.setInterval(20);
  threadReadButtons.onRun(readButtons);
  cpu.add(&threadReadButtons);
}

void changeDeck() {
  if (deckSelected == DECK_B) {
    deckSelected = DECK_C;
    mdCore.cChange(1, NP_DECK_SEL, 4);
  } else {
    deckSelected = DECK_B;
    mdCore.cChange(1, NP_DECK_SEL, 1);
  }
}

void readButtons() {
  buttons.read(sendMidiNote);
}

void readPots() { pots.read(sendMidiCC); }

void readEncoder() { enc.readEnc(sendMidiCC); }

void sendMidiNote(byte number, byte value, byte channel) {
  MIDI.sendNoteOn(number, value, channel);
}

void sendMidiCC(byte number, byte value, byte channel) {
  MIDI.sendControlChange(number, value, channel);
}
