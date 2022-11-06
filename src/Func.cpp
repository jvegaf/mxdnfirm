#include "Func.h"  

Func::Func(OutputBase* output)
{
	_output = output;
}

Func::Func(Input* input, OutputBase* output, uint8_t midiCh, uint8_t note)
	: FuncBase(input, midiCh, note)
{
	_output	= output;
}

void Func::read() {
	FuncBase::read();
}

void Func::setTo(uint8_t value) {
	_output->setTo(value);
}
