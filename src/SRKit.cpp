#include "SRKit.h"

SRKit::SRKit(uint8_t clkPin, uint8_t dtPin, uint8_t lchPin, uint8_t nRegs)
{
    clockPin = clkPin;
    dataPin = dtPin;
    latchPin = lchPin;
    regsAmount = nRegs;
} 

void SRKit::begin()
{
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
    digitalWrite(clockPin, LOW);
    digitalWrite(latchPin, LOW);
    clear();
}

void SRKit::clear()
{
    uint8_t regs[regsAmount] = {};
    for (uint8_t i = 0; i < regsAmount; i++)
    {
        regs[i] = B00000000;
        sendState(regs);
    }
}
void SRKit::sendState(uint8_t* regs)
{
    digitalWrite(latchPin, LOW);
    for (uint8_t i = 0; i < regsAmount; i++)
    {
        shiftOut(this->dataPin, this->clockPin, MSBFIRST, regs[i]);
    }
    digitalWrite(latchPin, HIGH);
}
