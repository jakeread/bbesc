// Space Vector Modulator
// Takes a Mag Vector and translates it into PWM values for three Motor Legs

#ifndef SVM_H
#define SVM_H

#include <Arduino.h>
#include "motorleg.h"

class SVM{

  public:
  SVM(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC); // constructor

  void init();
  void set(double theta, double duty); // 0-360, where do we want the mag vector to point, at what duty cycle?
  void killAll(); // shut'r down

  MotorLeg* MLA;
  MotorLeg* MLB;
  MotorLeg* MLC;
  
  void doubleMap(double* valPtr, double fromLo, double fromHi, double toLo, double toHi);

  private:
  double sqrt2;
  double Va;
  double Vb;
  double Vc;
  
  void crush(double* valPtr); // crunches between 0 - 1;
  
};

#endif
