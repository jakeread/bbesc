// does straightforward BLDC commutation

#include "bldc.h"

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


void BLDC::loop(uint16_t posNow){
  /*
   * OK: Have a Duty-Cycle (eventually Torque) setting, and a direction setting
   * SO: get a rolling-averaged position estimate (noInterrupts? -> or rollingAverage doesn't use most-recent value, so if it's in the process of being written we don't f...)
   * Check comm table / simple math: where are we in comm table? encoder resolution / poles % 6 ?
   * Set PWM's according
   */

  /*
   * OFFSET and loop-back if over
   */
  _posNow = posNow;
  _posNow -= AS5047_OFFSET;
  if(_posNow < 0){
    _posNow += AS5047_RESOLUTION;
  }

  /*
   * Do Modulo: for splitting Encoder (0-AS5047_RESOLUTION Physical Period, into 0-BLDC_MODULO Electrical Period)
   */
  _modulo = _posNow % MOTOR_MODULO;

  /*
  Serial.print("Duty: ");
  Serial.print(_duty);
  Serial.print(" Dir: ");
  Serial.print(_dir);
  
  Serial.print(" posNow: ");
  Serial.print(_posNow);
  Serial.print(" Modulo: ");
  Serial.println(_modulo);
  */

  /* ENTER CONFIG
   *  Needs to update zone-values per pole-number
   */

  if(_modulo >= 0 && _modulo < 248){
    _comZone = 0;
  } else if (_modulo >= 248 && _modulo < 496){
    _comZone = 1;
  } else if (_modulo >= 496 && _modulo < 745){
    _comZone = 2;
  } else if (_modulo >= 745 && _modulo < 993){
    _comZone = 3;
  } else if (_modulo >= 993 && _modulo < 1241){
    _comZone = 4;
  } else if (_modulo >= 1241 && _modulo <= 1489){
    _comZone = 5;
  }

  /*
  Serial.print(" comZone: ");
  Serial.print(comZone);
  */
  
  if(_dir == 1){
    _comCommand = _comZone + 2; // there is some falsity in this nomenclature: offset is not measured correctly atm
    if(_comCommand == 6){
      _comCommand = 0;
    }
    if(_comCommand == 7){
      _comCommand = 1;
    }
  }
  
  if(_dir == 0){
    _comCommand = _comZone - 2;
    if(_comCommand == -1){
      _comCommand = 5;
    }
    if(_comCommand == -2){
      _comCommand = 4;
    }
  }

  /*
  Serial.print(" comCommand: ");
  Serial.print(comCommand);
  
  Serial.println(" ");
  */
  
  commutate(_comCommand);

}

void BLDC::commutate(uint8_t comPos){

  /*
  _comloc += _dir;
  if(_comloc > 5){
    _comloc = 0;
  }
  if(_comloc < 0){
    _comloc = 5;
  }
  */

  _comloc = comPos % 6;

  MLA->set(_duty, comtable[_comloc][0]);
  MLB->set(_duty, comtable[_comloc][1]);
  MLC->set(_duty, comtable[_comloc][2]);
  
}

