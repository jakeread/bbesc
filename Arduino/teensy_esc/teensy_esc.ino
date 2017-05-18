#include "motorleg.h"
//#include "svm.h"
#include "bldc.h"

//SVM theSVM(3,4,5,6,9,10);
BLDC theBLDC(3,4,5,6,9,10);
IntervalTimer BLDCTimer;

void setup() {
  pinMode(13, OUTPUT);
  BLDCTimer.begin(BLDC, 100);
  // do two pots
  // read'em
  // update timer freq. for PWM
  // update PWM size for phase voltages
  // w/ a BLDC object
}

void BLDC(){
  // BLDC loop, every 100us -> 10kHz
  theBLDC.loop();
}

void loop() { // it's a time a to testa - do update loop that reads pot before each update, writes next oneSVM.set accordingly!
  theBLDC.hz(map(analogRead(9), 0, 255, 8, 1024));
  theBLDC.duty(map(analogRead(8), 0, 255, 28, 255));
  delay(50);
}

/*
void readPot(double* val){
  *val = analogRead(9);  
  int statusLedVal = analogRead(9);
  analogWrite(20, map(statusLedVal, 0, 255, 0, 255));
}
*/

// now do phases based on mag direction & strength
// test, steady-state direction, can you feel hold? does motor explode? stay lo... 
