// single phase of three-phase BLDC
// these bridges are MOSFETS
// MOSFETS can't drive voltages higher than their gate voltage (tf? who knew)
// so, with our 3.3v signal - and a 2.0v threshold voltage, the maximum Voltage is 
// Vout = Vgate - Vthreshold

#include "motorleg.h"

MotorLeg::MotorLeg(int pinHi, int pinLo){

  _pinHi = pinHi;
  _pinLo = pinLo;

  _rangeHi = 1;
  _rangeLo = -1;

  _expFloat = 0;
  _bitVal = 0;
  
  //analogWriteFrequency(_pinHi, 187500); // pin, frequency
  //analogWriteFrequency(_pinLo, 187500);
}

void MotorLeg::set(double val){

  crush(&val);

  doubleMap(&val, -1, 1, -255, 255);
  _bitVal = (int)val; // casting _expFloat as int, to pass to analogWrite

  if(_bitVal == 0){ 
    this->kill(); // sets both 0
  } else if (_bitVal > 0){
    analogWrite(_pinHi, _bitVal);
    analogWrite(_pinLo, 0);
  } else if (_bitVal < 0){
    analogWrite(_pinHi, 0);
    analogWrite(_pinLo, abs(_bitVal));
  }
  
}

void MotorLeg::kill(){
  analogWrite(_pinHi, 0);
  analogWrite(_pinLo, 0);
}

void MotorLeg::crush(double* valPtr){
  if(*valPtr > 1){ *valPtr = 1; } // resolving valPtr pointer to value
  if(*valPtr < -1){ *valPtr = -1; }
}

void MotorLeg::doubleMap(double* valPtr, double fromLo, double fromHi, double toLo, double toHi){
  // maths
  *valPtr = (*valPtr-fromLo) * (toHi - toLo) / (fromHi - fromLo) + toLo;
}
