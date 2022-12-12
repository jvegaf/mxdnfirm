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

const uint8_t leftBtns[] = {
  PCUE2,
  PCUE1,
  SHIFT,
  LOOP_LH,
  SYNC_LH,
  RNG_LH,
  PLAY_LH,
  CUE_LH,
  LPAD8,
  LPAD4,
  LPAD7,
  LPAD3,
  LPAD2,
  LPAD6,
  LPAD1,
  LPAD5
};

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

const uint8_t rightBtns[] = {
  CUE_RH,
  PLAY_RH,
  RNG_RH,
  LOOP_RH,
  PCUE3,
  SYNC_RH,
  RPAD8,
  RPAD4,
  RPAD7,
  RPAD3,
  RPAD2,
  RPAD6,
  RPAD1,
  RPAD5
};

} // namespace MPlex
