#include "motorleg.h"
#include "svm.h"

SVM oneSVM(3,4,5,6,9,10);

void setup() {
  analogWriteResolution(8); // 8-bit analog (pwm) write resolution // values 0-255
  pinMode(20, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  // 3,4,5,6,9.10
  // matched resolution / frequency as per chart https://www.pjrc.com/teensy/td_pulse.html
}

double duty;

void loop() { // it's a time a to testa - do update loop that reads pot before each update, writes next oneSVM.set accordingly!
  for(int i = 0; i < 360; i++){
    readPot(&duty);
    oneSVM.set(radians(i), 0.1); // angle, max duty
    delayMicroseconds(750);
  }
  for(int i = 360; i > 0; i++){
    readPot(&duty);
    oneSVM.set(radians(i), 0.1);
    delayMicroseconds(750);
  }
}

void readPot(double* val){
  *val = analogRead(9);
  oneSVM.doubleMap(val, 0, 1024, 0, 1);
  
  int statusLedVal = analogRead(9);
  analogWrite(20, map(statusLedVal, 0, 255, 0, 255));
}

// now do phases based on mag direction & strength
// test, steady-state direction, can you feel hold? does motor explode? stay lo... 
