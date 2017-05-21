#include "motorleg.h"
//#include "svm.h"
#include "bldc.h"
#include "encoder_as5047.h"

//SVM theSVM(3,4,5,6,9,10); // THESE IN A KERNEL : YES DO IT SOON
// GENERALLY: Need a much stronger structure... or thoughts on it
// and nomenclature for mag fields & vectors & approximations etc... offsets / directions / modulo etc / electrical position vs. encoder position yadda
// auto-finding offset, modulo, super critical
BLDC BLDC(3,4,5,6,9,10);
AS5047 AS5047;

IntervalTimer BLDC_Timer;
IntervalTimer Serial_Timer;

IntervalTimer Debug_Timer;

void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(115200);

  AS5047.init();
  
  BLDC_Timer.begin(BLDC_Loop, 25);        // 20kHz
  //Debug_Timer.begin(Debug_Loop, 500000);  // 10 Hz
  
  // do two pots
  // read'em
  // update timer freq. for PWM
  // update PWM size for phase voltages
  // w/ a BLDC object
}

void BLDC_Loop(){
  
  BLDC.duty(map(analogRead(8), 8, 255, 28, 255));

  AS5047.readNow();
  AS5047.filter();
  /*
  Serial.print("NOW:\t");
  Serial.print(AS5047.reading);
  Serial.print("\tFILT:\t");
  Serial.println(AS5047.filtered);
  */
  
  BLDC.loop((int)AS5047.filtered);
}

int comPos = 0;

void Debug_Loop(){

  Serial.print(""); // print 1st: i.e. at 'end' of last loop... so that spring has come out of motor
  Serial.print(comPos % 6);
  
  Serial.print(", ");
  AS5047.filter();
  Serial.println(AS5047.filtered);

  comPos ++;

  BLDC.duty(map(analogRead(8), 8, 255, 28, 255));
  BLDC.commutate(comPos % 6);
  
}

void Serial_Loop(){
  Serial.print("report: ");
  noInterrupts();
  Serial.println(AS5047.reading);
  interrupts();
}


void loop() { // it's a time a to testa - do update loop that reads pot before each update, writes next oneSVM.set accordingly!
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
