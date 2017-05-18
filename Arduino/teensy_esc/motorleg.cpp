// single phase of three-phase BLDC

#include "motorleg.h"

#define LEG_PWMFREQ 187500

MotorLeg::MotorLeg(int pinHi, int pinLo){

  _pinHi = pinHi;
  _pinLo = pinLo;

  analogWriteFrequency(_pinHi, 187500); // pin, frequency
  analogWriteFrequency(_pinLo, 187500);
}

/*
 * set function should be hella fast
 */

void MotorLeg::set(int duty, int dir){ // -255 -> 255 : value checks happen outside of this f'n!
  if(dir > 0){ // 'electric direction' i.e. open-to-lo or open-to-hi ... never both
    analogWrite(_pinHi, duty);
    analogWrite(_pinLo, 0);
  } else if(dir < 0){
    analogWrite(_pinHi, 0);
    analogWrite(_pinLo, duty);
  } else {
    analogWrite(_pinHi, 0);
    analogWrite(_pinLo, 0);
  }
}

void MotorLeg::kill(){
  analogWrite(_pinHi, 0);
  analogWrite(_pinLo, 0);
}
