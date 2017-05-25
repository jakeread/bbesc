// Space Vector Modulator
// Takes a Mag Vector and translates it into PWM values for three Motor Legs

#ifndef SVM_H
#define SVM_H

#include <Arduino.h>
#include "motorleg.h"
#include "config.h"

class SVM{

  public:
  SVM(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC); // constructor

  void init();
  void killAll(); // shut'r down

  void duty(int duty); // duty cycle (sets pwm)
  void theta(double theta); // 0-360, where do we want the mag vector to point, at what duty cycle?
  void assert();
  void commutate(double rads); // steps-from-current-angle to take, set


  MotorLeg* MLA;
  MotorLeg* MLB;
  MotorLeg* MLC;
  
  private:
  double sqrt2;
  int _duty;
  double _theta;
  
  
};

#endif
