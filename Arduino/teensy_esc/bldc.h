#ifndef BLDC_H
#define BLDC_H

#include <Arduino.h>
#include "motorleg.h"
#include "encoder_as5047.h"
#include "config.h"


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
  void loop(uint16_t posNow);
  void commutate(uint8_t comPos);

  private:
  int _duty;
  int _dir; // -1 or 1
  int _freq;

  int _comloc;
  unsigned long _lastcom;

  int _modulo;
  int _posNow;
  int _comZone;
  int _comCommand;

  int comtable[6][3] = {
    {  1,-1, 0 }, //0
    {  1, 0,-1 }, //1
    {  0, 1,-1 }, //2
    { -1, 1, 0 }, //3
    { -1, 0, 1 }, //4
    {  0,-1, 1 }, //5
  };
  
};

#endif
