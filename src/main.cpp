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
// Rev2 Version
//

MIDI_CREATE_DEFAULT_INSTANCE();

uint8_t deckMidiChannels[] = {deckAChannel, deckBChannel};
uint8_t padMidiChannels[] = {padAChannel, padBChannel};

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
  selector.begin();
  buttons.begin();
  topPots.begin();
  bottomPots.begin();
  deckLeft.begin();
  deckRight.begin();
  padLeft.begin();
  padRight.begin();
  MDCore::begin();
  threadsSetup();
}

void loop() {
  cpu.run();
  MIDI.read();
  readEncoder();
}

void handleControlChange(uint8_t channel, uint8_t number, uint8_t value) {
  MDCore::cChange(channel, number, value);
}

void handleNoteOn(uint8_t channel, uint8_t number, uint8_t value) {
  if (value < 1U) {
    MDCore::noteOff(channel, number, value);
    return;
  }
  MDCore::noteOn(channel, number, value);
}
void handleNoteOff(uint8_t channel, uint8_t number, uint8_t value) {
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
  MIDI.sendNoteOn(number, value, channel);
}

void sendMidiCC(uint8_t number, uint8_t value, uint8_t channel) {
  MIDI.sendControlChange(number, value, channel);
}
