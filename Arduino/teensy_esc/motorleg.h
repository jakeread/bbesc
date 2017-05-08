// single phase of our three-phase BLDC
// when a phase is set in -ves, it is a Source of electrons (transistor connects it to ground), 
// when High it is a Drain (transistor connects it to voltage)

#ifndef MOTORLEG_H
#define MOTORLEG_H

#include <Arduino.h>

class MotorLeg{
  
  public:
  MotorLeg(int pinHi, int pinLo); // constructor
  //~MotorLeg(); // destructor, should have, don't
  void set(double val); // val, -1 <-> 1 (duty cycle, direction)
  void kill();

  private:
  int _pinHi;
  int _pinLo;
  double _rangeHi;
  double _rangeLo;
  double _expFloat; // expanded float, i.e. from -1, 1 to 0, 255 (bitwise)
  int _bitVal; // bitwise aka Integer-value.. hardware timers are bits resolution

  void crush(double* valPtr);
  void doubleMap(double* valPtr, double fromLo, double fromHi, double toLo, double toHi);
};

#endif
