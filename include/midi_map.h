#pragma once

#include "sr_map.h"
#include <Arduino.h>
#include <stdint.h>

/////////////////////////////////////////
// OUTS

uint8_t const MONITOR_CUE_A = 39; // d#2
uint8_t const MONITOR_CUE_B = 40; // e2
uint8_t const MONITOR_CUE_C = 41; // f2

uint8_t const FX1_BTN_1 = 42; // f#2
uint8_t const FX1_BTN_2 = 43; // g2
uint8_t const FX1_BTN_3 = 44; // g#2
uint8_t const FX2_BTN_1 = 45; // a2
uint8_t const FX2_BTN_2 = 46; // a#2
uint8_t const FX2_BTN_3 = 47; // b2

uint8_t const PLAY_DECK_A = 48; // c3
uint8_t const PLAY_DECK_B = 49; // c#3     & DECK_C

uint8_t const CUE_DECK_A = 50; // d3
uint8_t const CUE_DECK_B = 51; // d#3     & DECK_C

// Control Changes

// channel 1

uint8_t const HOTCUE_1T_DK_A = 0;
uint8_t const HOTCUE_2T_DK_A = 1;
uint8_t const HOTCUE_3T_DK_A = 2;
uint8_t const HOTCUE_4T_DK_A = 3;
uint8_t const HOTCUE_5T_DK_A = 4;
uint8_t const HOTCUE_6T_DK_A = 5;
uint8_t const HOTCUE_7T_DK_A = 6;
uint8_t const HOTCUE_8T_DK_A = 7;

uint8_t const RANGE_SELECTOR_DK_A = 8;
uint8_t const TEMPO_MASTER_DK_A = 9;
uint8_t const SYNC_ON_DECK_A = 10;

// channel 2

uint8_t const HOTCUE_1T_DK_B = 7;
uint8_t const HOTCUE_2T_DK_B = 6;
uint8_t const HOTCUE_3T_DK_B = 5;
uint8_t const HOTCUE_4T_DK_B = 4;
uint8_t const HOTCUE_5T_DK_B = 3;
uint8_t const HOTCUE_6T_DK_B = 2;
uint8_t const HOTCUE_7T_DK_B = 1;
uint8_t const HOTCUE_8T_DK_B = 0;
uint8_t const DECK_B_C_SELECTION = 8;
uint8_t const RANGE_SELECTOR_DK_B = 9;
uint8_t const SYNC_ON_DECK_B = 10;
uint8_t const TEMPO_MASTER_DK_B = 11;

// channel 3

uint8_t const MASTER_OUT_L = 4;
uint8_t const MASTER_OUT_R = 3;
uint8_t const LEVEL_DECK_A = 2;
uint8_t const LEVEL_DECK_B = 1;
uint8_t const LEVEL_DECK_C = 0;

uint8_t const deckAChannel = 1;
uint8_t const deckBChannel = 2;
uint8_t const deckCChannel = 3;

uint8_t const padAChannel = 4;
uint8_t const padBChannel = 5;
uint8_t const padCChannel = 6;

uint8_t const potsChannel = 7;
uint8_t const topPotsChannel = 8;
uint8_t const bottomPotsChannel = 9;

uint8_t const mixerChannel = 10;
