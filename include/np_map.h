#pragma once

#include <Arduino.h>

//////////////////////////////////////
// NEOPIXELS

uint8_t const NP_PADL1 = 0;
uint8_t const NP_PADL2 = 1;
uint8_t const NP_PADL3 = 2;
uint8_t const NP_PADL4 = 3;
uint8_t const NP_PADL5 = 4;
uint8_t const NP_PADL6 = 5;
uint8_t const NP_PADL7 = 6;
uint8_t const NP_PADL8 = 7;
uint8_t const NP_RANGE_L = 8;
uint8_t const NP_SYNC_L = 9;
uint8_t const NP_PADR1 = 10;
uint8_t const NP_PADR2 = 11;
uint8_t const NP_PADR3 = 12;
uint8_t const NP_PADR4 = 13;
uint8_t const NP_PADR5 = 14;
uint8_t const NP_PADR6 = 15;
uint8_t const NP_PADR7 = 16;
uint8_t const NP_PADR8 = 17;
uint8_t const NP_DECK_SEL = 18;
uint8_t const NP_RANGE_R = 19;
uint8_t const NP_SYNC_R = 20;

const uint8_t pxPadLeft[] = {NP_PADL1, NP_PADL2, NP_PADL3, NP_PADL4,
                             NP_PADL5, NP_PADL6, NP_PADL7, NP_PADL8};

const uint8_t pxPadRight[] = {NP_PADR1, NP_PADR2, NP_PADR3, NP_PADR4,
                              NP_PADR5, NP_PADR6, NP_PADR7, NP_PADR8};

const uint8_t pxDeckLeft[] = {NP_RANGE_L, NP_SYNC_L};
const uint8_t pxDeckRight[] = {NP_RANGE_R, NP_SYNC_R};

uint8_t const tPxDeck = 2;

uint8_t const tPixels = 21;
