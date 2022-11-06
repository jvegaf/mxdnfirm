#pragma once

#include "NPKit.h"
#include <Shifter.h>
#include "base/pin_map.h"
#include "base/np_map.h"
#include "base/sr_fb_map.h"
#include "base/midi_map.h"
#include "MuxInput.h"
#include "FuncMode.h"
#include "FuncPad.h"
#include "ArduInput.h"
#include "base/sw_muxmap.h"
#include "FBLed.h"
#include "FBPixel.h"
#include "Pad.h"
#include "Funcs.h"
#include "FuncsBlind.h"


class FuncFactory
{
public:
    void begin(NPKit* npkit, void (*funcOn)(uint8_t, uint8_t, uint8_t), void (*funcOff)(uint8_t, uint8_t, uint8_t));

    FuncsBlind* getBlindFuncs() const { return this->_blindFuncs; }

    Funcs* getFuncs() const { return this->_funcs; }

    Pad* getLeftPad() const { return this->_leftPad; }

    Pad* getRightPad() const { return this->_rightPad; }

    FuncMode* getFuncModeLeft() const { return this->_funcModeLeft; }

    FuncMode* getFuncModeRight() const { return this->_funcModeRight; }
    

private:
    NPKit* _npkit;
    Shifter* _shfLeft;
	Shifter* _shfRight;
    FuncMode* _funcModeLeft; 
    FuncMode* _funcModeRight;
    Funcs* _funcs;
    FuncsBlind* _blindFuncs;
    Pad* _leftPad;
    Pad* _rightPad; 
    void (*fOn)(uint8_t, uint8_t, uint8_t); 
    void (*fOff)(uint8_t, uint8_t, uint8_t);

    Input** createInputs();
    OutputBase **createOutputs();

    Input **createInputPads(const uint8_t* mxPinBundle, uint8_t sigPin, const uint8_t* positions, void (*funcOn)(uint8_t, uint8_t, uint8_t), void (*funcOff)(uint8_t, uint8_t, uint8_t));

    OutputBase** createOutputPads(NPKit* npk, const uint8_t* positions);
    FuncMode* createLeftModeFunc();
    FuncMode* createRightModeFunc();
    Input** createBlindInputs();
    Func* createFuncs(Input **inAggr, OutputBase **outAggr, uint8_t midiCh, uint8_t t_funcs);
    FuncBase* createBlindFuncs(Input **inAggr, uint8_t midiCh, uint8_t t_funcs);
    FuncPad* createFuncPads(Input **inAggr, OutputBase **outAggr, uint8_t midiCh, uint8_t t_funcs);

    
};

