#pragma once

#include <Arduino.h>

namespace Leds {}

////////////////////////////////////
// REGISTERS

// VU
uint8_t const VU_MASTER_RIGHT = 0;
uint8_t const VU_MASTER_LEFT = 1;
uint8_t const VU_LINE_3 = 2;
uint8_t const VU_LINE_2 = 3;
uint8_t const VU_LINE_1 = 4;

// FB

uint8_t const FB_RIGHT = 0;
uint8_t const FB_LEFT = 1;

////////////////////////////////////
// MASTER RIGHT
uint8_t const VUMASR_0 = 0;
uint8_t const VUMASR_1 = 1;
uint8_t const VUMASR_2 = 2;
uint8_t const VUMASR_3 = 3;
uint8_t const VUMASR_4 = 4;
uint8_t const VUMASR_5 = 5;
uint8_t const VUMASR_6 = 6;
uint8_t const VUMASR_7 = 7;

////////////////////////////////////
// MASTER LEFT
uint8_t const VUMASL_0 = 8;
uint8_t const VUMASL_1 = 9;
uint8_t const VUMASL_2 = 10;
uint8_t const VUMASL_3 = 11;
uint8_t const VUMASL_4 = 12;
uint8_t const VUMASL_5 = 13;
uint8_t const VUMASL_6 = 14;
uint8_t const VUMASL_7 = 15;

////////////////////////////////////
// VUMETER LINE 1
uint8_t const VUL1_0 = 16;
uint8_t const VUL1_1 = 17;
uint8_t const VUL1_2 = 18;
uint8_t const VUL1_3 = 19;
uint8_t const VUL1_4 = 20;
uint8_t const VUL1_5 = 21;
uint8_t const VUL1_6 = 22;
uint8_t const VUL1_7 = 23;

////////////////////////////////////
// VUMETER LINE 2
uint8_t const VUL2_0 = 24;
uint8_t const VUL2_1 = 25;
uint8_t const VUL2_2 = 26;
uint8_t const VUL2_3 = 27;
uint8_t const VUL2_4 = 28;
uint8_t const VUL2_5 = 29;
uint8_t const VUL2_6 = 30;
uint8_t const VUL2_7 = 31;

////////////////////////////////////
// VUMETER LINE 3
uint8_t const VUL3_0 = 32;
uint8_t const VUL3_1 = 33;
uint8_t const VUL3_2 = 34;
uint8_t const VUL3_3 = 35;
uint8_t const VUL3_4 = 36;
uint8_t const VUL3_5 = 37;
uint8_t const VUL3_6 = 38;
uint8_t const VUL3_7 = 39;

///////////////////////////////////
// FB RIGHT
uint8_t const HP_C = 0;
uint8_t const PLAY_R = 1;
uint8_t const CUE_R = 2;
uint8_t const LOOP_R = 3;
uint8_t const FX3_R = 4;
uint8_t const FX2_R = 5;
uint8_t const FX1_R = 6;

////////////////////////////////////
// FB LEFT
uint8_t const FX3_L = 0;
uint8_t const FX2_L = 1;
uint8_t const FX1_L = 2;
uint8_t const PLAY_L = 3;
uint8_t const CUE_L = 4;
uint8_t const LOOP_L = 5;
uint8_t const HP_A = 6;
uint8_t const HP_B = 7;
