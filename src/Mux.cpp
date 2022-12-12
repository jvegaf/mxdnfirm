#include "Mux.h"

void Mux::setMuxChannel(uint8_t channel)
{
    digitalWrite(muxpins[0], bitRead(channel, 0));
    digitalWrite(muxpins[1], bitRead(channel, 1));
    digitalWrite(muxpins[2], bitRead(channel, 2));
    digitalWrite(muxpins[3], bitRead(channel, 3));
}

Mux::Mux(const uint8_t* m_pins, const uint8_t sig, const uint8_t pos )
:muxpins(m_pins),muxSIG(sig), position(pos) { }



void Mux::read(void (*func)())
{
    setMuxChannel(position);
    cState = digitalRead(muxSIG);
    
    if ((millis() - lastdebouncetime) > debouncedelay)
    {
        if (pState != cState)
        {
            lastdebouncetime = millis();

            if (cState == LOW)
            {
                func();
            }

            pState = cState;
        }
    }
}
