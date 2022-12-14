#include "BREncoder.h"
#include "BtnKit.h"
#include "MDCore.h"
#include "Mux.h"
#include "MuxPots.h"
#include "Muxer.h"
#include "PotKit.h"
#include "midi_map.h"
#include "mpx_map.h"
#include "np_map.h"
#include "pin_map.h"
#include <Arduino.h>
#include <MIDI.h>
#include <Thread.h>
#include <ThreadController.h>
// Rev2 Version
//
const uint8_t DECK_B = 2;
const uint8_t DECK_C = 3;

MIDI_CREATE_DEFAULT_INSTANCE();

volatile uint8_t deckSelected = DECK_B;
BREncoder enc;

Mux selector(mux_pins, lhMP_sig, MPlex::DECK_SEL);

Muxer leftButtons(mux_pins, lhMP_sig, MPlex::leftBtns, MPlex::t_leftBtns);
Muxer rightButtons(mux_pins, rhMP_sig, MPlex::rightBtns, MPlex::t_rightBtns);

MuxPots topPots(mux_pins, topPots_sig, MPlex::topPots, MPlex::t_topPots);
MuxPots bottomPots(mux_pins, btmPots_sig, MPlex::bottomPots,
                   MPlex::t_bottomPots);

BtnKit buttons(btnPins, t_btnPins);
PotKit pots(PotPins, t_potPins);

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
  topPots.begin();
  bottomPots.begin();
  MDCore::begin();
  threadsSetup();
}

void loop() {
  cpu.run();
  MIDI.read();
  readEncoder();
}

void handleControlChange(byte channel, byte number, byte value) {
  MDCore::cChange(channel, number, value);
}

void handleNoteOn(byte channel, byte number, byte value) {
  if (value < 1U) {
    MDCore::noteOff(channel, number, value);
    return;
  }
  MDCore::noteOn(channel, number, value);
}
void handleNoteOff(byte channel, byte number, byte value) {
  MDCore::noteOff(channel, number, value);
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
    MDCore::cChange(1, NP_DECK_SEL, 4);
  } else {
    deckSelected = DECK_B;
    MDCore::cChange(1, NP_DECK_SEL, 1);
  }
}

void readButtons() {
  selector.read(changeDeck);
  leftButtons.read(sendMidiNote, leftBtnsChannel);
  rightButtons.read(sendMidiNote, rightBtnsChannel);
  buttons.read(sendMidiNote, buttonsChannel);
}

void readPots() {
  topPots.read(sendMidiCC, topPotsChannel);
  bottomPots.read(sendMidiCC, bottomPotsChannel);
  pots.read(sendMidiCC, potsChannel);
}

void readEncoder() { enc.readEnc(sendMidiCC); }

void sendMidiNote(byte number, byte value, byte channel) {
  MIDI.sendNoteOn(number, value, channel);
}

void sendMidiCC(byte number, byte value, byte channel) {
  MIDI.sendControlChange(number, value, channel);
}
