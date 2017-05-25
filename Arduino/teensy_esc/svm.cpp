// Space Vector Modulator
// &address of
// *value pointed to by this pointer

#include "motorleg.h"
#include "svm.h"
#include "trig.h"

SVM::SVM(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC){

  analogWriteResolution(LEG_PWMRES);
  // 8-bit analog (pwm) write resolution // values 0-255
  // & maching freq as per https://www.pjrc.com/teensy/td_pulse.html set in motorleg

  MLA = new MotorLeg(pinHiA, pinLoA); // TODO: rename to phases U V M
  MLB = new MotorLeg(pinHiB, pinLoB);
  MLC = new MotorLeg(pinHiC, pinLoC);

  _theta = 128;
  _duty = 0;
  _dir = 1;
}

void SVM::init(){
  // does loop-starting
}

void SVM::killAll(){
  
  MLA->kill();
  MLB->kill();
  MLC->kill();
  
}

void SVM::duty(int duty){
  if(duty >= 0 && duty <= 255){
    _duty = duty;
  } else if (duty > 255){
    _duty = 255;
  } else if (duty < 0){
    _duty = 0;
  }
}

void SVM::dir(int dir){
  if(dir < 0){
    _dir = -1;
  } else if (dir >= 0){
    _dir = 1;
  }
}

void SVM::theta(double theta){  // theta, in radians
                                // 360 electrical degrees resoltion is enough?
  if(theta >= 0 && theta <= 2*PI){
    _theta = theta;
  } else if (theta < 0){
    _theta = 0;
  } else if (theta > 2*PI){
    _theta = 2*PI;
  }
}

void SVM::assert(){
  MLA->setSVM(_duty*cos(_theta)); // sets values between 
  MLB->setSVM(_duty*cos(_theta - (TWO_PI)/3));
  MLC->setSVM(_duty*cos(_theta + (TWO_PI)/3));
}

void SVM::commutate(double rads){ // com in -> rads
  _theta += rads;
  if(_theta >= TWO_PI){
    _theta -= TWO_PI;
  } else if (_theta < 0){
    _theta += TWO_PI;
  }
  this->assert();
}

double SVM::getTheta(){
  return _theta;
}

void SVM::loop(uint32_t pos){
  _theta = (pos % MOTOR_MODULO) * POS_TO_THETA + _dir*PHASE_ADVANCE;
  if(_theta >= TWO_PI){
    _theta -= TWO_PI;
  } else if (_theta < 0){
    _theta += TWO_PI;
  }
  this->assert();
}


