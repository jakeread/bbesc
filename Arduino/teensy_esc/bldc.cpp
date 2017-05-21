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

void BLDC::loop(uint16_t posNow){
  /*
   * OK: Have a Duty-Cycle (eventually Torque) setting, and a direction setting
   * SO: get a rolling-averaged position estimate (noInterrupts? -> or rollingAverage doesn't use most-recent value, so if it's in the process of being written we don't f...)
   * Check comm table / simple math: where are we in comm table? encoder resolution / poles % 6 ?
   * Set PWM's according
   */

  posNow -= BLDC_OFFSET;
  if(posNow < 0){
    posNow += AS5047_RESOLUTION;
  }
  
  int modulod = posNow % BLDC_MODULO;

  /*
  Serial.print("Duty: ");
  Serial.print(_duty);
  Serial.print(" Dir: ");
  Serial.print(_dir);
  Serial.print(" Filt: ");
  Serial.print(posNow);
  Serial.print(" Modulo: ");
  Serial.print(modulod);
  */
  
  uint8_t comCommand;
  uint8_t comZone;

  if(modulod >= 0 && modulod < 390){
    comZone = 0;
  } else if (modulod >= 390 && modulod < 780){
    comZone = 1;
  } else if (modulod >= 780 && modulod < 1170){
    comZone = 2;
  } else if (modulod >= 1170 && modulod < 1560){
    comZone = 3;
  } else if (modulod >= 1560 && modulod < 1950){
    comZone = 4;
  } else if (modulod >= 1950 && modulod <= 2340){
    comZone = 5;
  }

  /*
  Serial.print(" comZone: ");
  Serial.print(comZone);
  */
  
  if(_dir == 1){
    comCommand = comZone + 2; // there is some falsity in this nomenclature: offset is not measured correctly atm
    if(comCommand < 0){
      comCommand = 5;
    }
    if(comCommand == 6){
      comCommand = 0;
    }
    if(comCommand == 7){
      comCommand = 1;
    }
    if(comCommand == 8){
      comCommand = 2;
    }
  }
  
  if(_dir == 0){
    comCommand = comZone - 2;
    if(comCommand == -1){
      comCommand = 5;
    }
    if(comCommand == -2){
      comCommand = 4;
    }
    if(comCommand > 5){
      comCommand = 0;
    }
  }

  /*
  Serial.print(" comCommand: ");
  Serial.print(comCommand);
  
  Serial.println(" ");
  */
  
  commutate(comCommand);

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

