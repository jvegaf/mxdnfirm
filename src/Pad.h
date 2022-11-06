#pragma once

#include <Arduino.h>
#include "FuncPad.h"
#include "base/md_defs.h"

class Pad  
{
	private:
		FuncPad* _funcPads;
	public:
		Pad(FuncPad* funcpads);
		void setNote(uint8_t note);
		void read();
		void setTo(uint8_t value);
		void setTo(uint8_t position, uint8_t value);
};
