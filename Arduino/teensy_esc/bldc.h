#ifndef BLDC_H
#define BLDC_H

#include <Arduino.h>
#include "motorleg.h"


class BLDC{

  public:
  BLDC(int pinHiA, int pinLoA, int pinHiB, int pinLoB, int pinHiC, int pinLoC); // constructor

  MotorLeg* MLA;
  MotorLeg* MLB;
  MotorLeg* MLC;
  
  void init();          // starts timers
  void duty(int duty);  // set rms pwm val
  void dir(bool dir);   // set direction for commutation
  void freq(int freq);    // set commutation freq (updates timer)
  void hz(int hz);
  void loop();
  void commutate();

  private:
  int _duty;
  int _dir; // -1 or 1
  int _freq;

  int _comloc;
  unsigned long _lastcom;

  int comtable[6][3] = {
    {  1,-1, 0 },
    {  1, 0,-1 },
    {  0, 1,-1 },
    { -1, 1, 0 },
    { -1, 0, 1 },
    {  0,-1, 1 },
  };
  
};

#endif
