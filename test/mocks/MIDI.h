#pragma once

#include <stdint.h>
#include <functional>

#define MIDI_CHANNEL_OMNI 0

class MockMIDI {
public:
    typedef std::function<void(uint8_t, uint8_t, uint8_t)> CallbackFunction;
    
    void begin(uint8_t channel) {}
    void turnThruOff() {}
    bool read() { return false; }
    
    void sendNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
        lastNoteOn = {note, velocity, channel};
    }
    
    void sendNoteOff(uint8_t note, uint8_t velocity, uint8_t channel) {
        lastNoteOff = {note, velocity, channel};
    }
    
    void sendControlChange(uint8_t number, uint8_t value, uint8_t channel) {
        lastCC = {number, value, channel};
    }
    
    void setHandleControlChange(CallbackFunction func) {
        ccCallback = func;
    }
    
    void setHandleNoteOn(CallbackFunction func) {
        noteOnCallback = func;
    }
    
    void setHandleNoteOff(CallbackFunction func) {
        noteOffCallback = func;
    }
    
    // Test utilities
    struct MidiMessage {
        uint8_t param1, param2, param3;
    };
    
    MidiMessage lastNoteOn = {0, 0, 0};
    MidiMessage lastNoteOff = {0, 0, 0};
    MidiMessage lastCC = {0, 0, 0};
    
    // Simulate incoming MIDI
    void simulateNoteOn(uint8_t note, uint8_t velocity, uint8_t channel) {
        if (noteOnCallback) {
            noteOnCallback(channel, note, velocity);
        }
    }
      void simulateControlChange(uint8_t number, uint8_t value, uint8_t channel) {
        if (ccCallback) {
            ccCallback(channel, number, value);
        }
    }
    
    // Reset function for tests
    static void reset() {
        // Reset static state if needed
    }

private:
    CallbackFunction ccCallback;
    CallbackFunction noteOnCallback;
    CallbackFunction noteOffCallback;
};

extern MockMIDI MIDI;

#define MIDI_CREATE_DEFAULT_INSTANCE() MockMIDI MIDI;
