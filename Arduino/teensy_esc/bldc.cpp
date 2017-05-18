// does straightforward BLDC commutation

#include "bldc.h"
#define MAXCOMM 1000000

BLDC::BLDC(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC){

  analogWriteResolution(8);
  // 8-bit analog (pwm) write resolution // values 0-255
  // & maching freq as per https://www.pjrc.com/teensy/td_pulse.html set in motorleg
  
  MLA = new MotorLeg(pinHiA, pinLoA); // TODO: rename to phases U V M
  MLB = new MotorLeg(pinHiB, pinLoB);
  MLC = new MotorLeg(pinHiC, pinLoC);

  _duty = 125;
  _dir = 1;
  _freq = 50000; // expressed as microseconds delay between commutation cycles

  _comloc = 0;
  _lastcom = micros();
  
}

void BLDC::init(){
}

void BLDC::duty(int duty){
  _duty = duty;
}

void BLDC::dir(bool dir){
  _dir = dir;
}

void BLDC::freq(int freq){
  _freq = freq;
  // if as previous nothing
  // else update timer
}

void BLDC::hz(int hz){
  if(hz){ // catch 0's
    _freq = 1000000/hz;
  } else {
    _freq = MAXCOMM;
  }
}

void BLDC::loop(){
  if(micros() > _lastcom + _freq){
    _lastcom = micros();
    this->commutate();
  }
}

void BLDC::commutate(){
  
  _comloc += _dir;
  if(_comloc > 5){
    _comloc = 0;
  }
  if(_comloc < 0){
    _comloc = 5;
  }

  MLA->set(_duty, comtable[_comloc][0]);
  MLB->set(_duty, comtable[_comloc][1]);
  MLC->set(_duty, comtable[_comloc][2]);
  
}

