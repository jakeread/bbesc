// Space Vector Modulator
// &address of
// *value pointed to by this pointer

#include "motorleg.h"
#include "svm.h"

SVM::SVM(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC){

  analogWriteResolution(8);
  // 8-bit analog (pwm) write resolution // values 0-255
  // & maching freq as per https://www.pjrc.com/teensy/td_pulse.html set in motorleg

  MLA = new MotorLeg(pinHiA, pinLoA); // TODO: rename to phases U V M
  MLB = new MotorLeg(pinHiB, pinLoB);
  MLC = new MotorLeg(pinHiC, pinLoC);
  
  Va = 0;
  Vb = 0;
  Vc = 0;
  
}

void SVM::init(){
  // does loop-starting
}

void SVM::killAll(){
  
  MLA->kill();
  MLB->kill();
  MLC->kill();
  
}

void SVM::set(double theta, double duty){ // theta, in radians

  //crush(&duty); // ensures between 0-1 // passing AddressOf_Duty (&duty)
  
  //needs much work to become true svm

  Va = duty*cos(theta);
  Vb = duty*cos(theta - (2*PI)/3);
  Vc = duty*cos(theta + (2*PI)/3);

  //MLA->set(Va); // sets values between -1, 1
  //MLB->set(Vb);
  //MLC->set(Vc);
  
  // take theta
  // does math
  // sets MLA, MLB, MLC
  
}

void SVM::crush(double* valPtr){
  if(*valPtr > 1){ *valPtr = 1; } // resolving valPtr pointer to value
  if(*valPtr < 0){ *valPtr = 0; }
}

void SVM::doubleMap(double* valPtr, double fromLo, double fromHi, double toLo, double toHi){
  // maths
  *valPtr = (*valPtr-fromLo) * (toHi - toLo) / (fromHi - fromLo) + toLo;
}
