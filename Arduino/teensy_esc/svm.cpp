// Space Vector Modulator
// &address of
// *value pointed to by this pointer

#include "motorleg.h"
#include "svm.h"

SVM::SVM(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC){

  MLA = new MotorLeg(pinHiA, pinLoA); // TODO: rename to phases U V M
  MLB = new MotorLeg(pinHiB, pinLoB);
  MLC = new MotorLeg(pinHiC, pinLoC);
  //MotorLeg MLA = new MotorLeg(pinHiA, pinLoA);

  sqrt2 = sqrt(2);

  Ia = 0;
  Ib = 0;
  Ic = 0;
  
}

void SVM::killAll(){
  
  MLA->kill();
  MLB->kill();
  MLC->kill();
  
}

void SVM::set(double theta, double duty){ // theta, in radians

  //crush(&duty); // ensures between 0-1 // passing AddressOf_Duty (&duty)

  Ia = duty*cos(theta);
  Ib = duty*cos(theta - (2*PI)/3);
  Ic = duty*cos(theta + (2*PI)/3);

  MLA->set(Ia); // sets values between -1, 1
  MLB->set(Ib);
  MLC->set(Ic);
  
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
