#pragma once

#include <Arduino.h>
#include <Multiplexer4067.h>
#include "base/pin_map.h"
#include "base/pots_muxmap.h"

class PotKit  
{
	private:

	public:
		void begin();
		void read(void (*scc_func)(uint8_t, uint8_t, uint8_t));

};
