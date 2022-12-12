#pragma once

#include <Arduino.h>
#include <cstdint>

namespace MPlex {

////////////////////////////////
///     MPLEX LEFT          ///
///////////////////////////////

uint8_t const PCUE2 = 0;
uint8_t const PCUE1 = 1;
uint8_t const SHIFT = 2;
uint8_t const LOOP_LH = 3;
uint8_t const SYNC_LH = 4;
uint8_t const RNG_LH = 5;
uint8_t const PLAY_LH = 6;
uint8_t const CUE_LH = 7;
uint8_t const LPAD8 = 8;
uint8_t const LPAD4 = 9;
uint8_t const LPAD7 = 10;
uint8_t const LPAD3 = 11;
uint8_t const LPAD2 = 12;
uint8_t const LPAD6 = 13;
uint8_t const LPAD1 = 14;
uint8_t const LPAD5 = 15;

const uint8_t leftBtns[] = {PCUE2,   PCUE1,  SHIFT, LOOP_LH, SYNC_LH, RNG_LH,
                            PLAY_LH, CUE_LH, LPAD8, LPAD4,   LPAD7,   LPAD3,
                            LPAD2,   LPAD6,  LPAD1, LPAD5};

////////////////////////////////
///     MPLEX RIGHT          ///
///////////////////////////////
uint8_t const CUE_RH = 0;
uint8_t const PLAY_RH = 1;
uint8_t const RNG_RH = 2;
uint8_t const LOOP_RH = 3;
uint8_t const PCUE3 = 5;
uint8_t const SYNC_RH = 6;
uint8_t const DECK_SEL = 7;
uint8_t const RPAD8 = 8;
uint8_t const RPAD4 = 9;
uint8_t const RPAD7 = 10;
uint8_t const RPAD3 = 11;
uint8_t const RPAD2 = 12;
uint8_t const RPAD6 = 13;
uint8_t const RPAD1 = 14;
uint8_t const RPAD5 = 15;

const uint8_t rightBtns[] = {CUE_RH,  PLAY_RH, RNG_RH, LOOP_RH, PCUE3,
                             SYNC_RH, RPAD8,   RPAD4,  RPAD7,   RPAD3,
                             RPAD2,   RPAD6,   RPAD1,  RPAD5};

const uint8_t t_rightBtns = 16;

uint8_t const PFXL_1 = 0;
uint8_t const PFXL_2 = 1;
uint8_t const PFXL_3 = 2;
uint8_t const GAIN_A = 3;
uint8_t const GAIN_B = 4;
uint8_t const GAIN_C = 5;
uint8_t const PFXR_2 = 6;
uint8_t const PFXR_3 = 7;
uint8_t const TREB_A = 8;
uint8_t const TREB_B = 9;
uint8_t const TREB_C = 10;

const uint8_t topPots[] = {PFXL_1, PFXL_2, PFXL_3, GAIN_A, GAIN_B, GAIN_C,
                           PFXR_2, PFXR_3, TREB_A, TREB_B, TREB_C};

uint8_t const t_topPots = 11;

uint8_t const FADR_C = 0;
uint8_t const FADR_A = 1;
uint8_t const TEMP_R = 2;
uint8_t const FLTR_B = 3;
uint8_t const VOL_HP = 4;
uint8_t const BASS_A = 5;
uint8_t const MIDL_A = 6;
uint8_t const BASS_C = 7;
uint8_t const MIDL_B = 8;
uint8_t const MIDL_C = 9;
uint8_t const BASS_B = 10;
uint8_t const FLTR_C = 11;
uint8_t const FLTR_A = 12;
uint8_t const TEMP_L = 13;
uint8_t const MIX_HP = 14;
uint8_t const FADR_B = 15;


const uint8_t bottomPots[] = {
  FADR_C,
  FADR_A,
  TEMP_R,
  FLTR_B,
  VOL_HP,
  BASS_A,
  MIDL_A,
  BASS_C,
  MIDL_B,
  MIDL_C,
  BASS_B,
  FLTR_C,
  FLTR_A,
  TEMP_L,
  MIX_HP,
  FADR_B
};

uint8_t const t_bottomPots = 16;


} // namespace MPlex
