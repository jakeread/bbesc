#include "motorleg.h"
#include "svm.h"
#include "bldc.h"
#include "encoder_as5047.h"

#include "config.h"

AS5047 TAS5047;
SVM TSVM(3, 4, 5, 6, 9, 10); // THESE IN A KERNEL : YES DO IT SOON
// GENERALLY: Need a much stronger structure... or thoughts on it
// and nomenclature for mag fields & vectors & approximations etc... offsets / directions / modulo etc / electrical position vs. encoder position yadda
// auto-finding offset, modulo, super critical
// BLDC BLDC(3, 4, 5, 6, 9, 10);

IntervalTimer AS5047_Timer;

//IntervalTimer BLDC_Timer;

//IntervalTimer HZ_Timer;

IntervalTimer Debug_Timer;


void setup() {
  pinMode(13, OUTPUT);

  Serial.begin(115200);

  TAS5047.init();
  AS5047_Timer.begin(AS5047_Loop, 1000000/AS5047_LOOP_HZ); 
  //BLDC_Timer.begin(BLDC_Loop, 25);       
  //HZ_Timer.begin(HZ_Loop_SVM, 25);
  Debug_Timer.begin(Debug_Loop, 1000000/DEBUG_LOOP_HZ); 
}

// ------------------------------------------------------- AS5047 LOOP

void AS5047_Loop(){
 TAS5047.readNow();
}

// ------------------------------------------------------- SVM Debug Loop

void Debug_Loop(){
  Serial.println(TAS5047.filtered());
}

// -------------------------------------------------------- BLDC LOOP
/*
int controlInput = 0;

void BLDC_Loop() {

  controlInput = analogRead(9);

  if (controlInput < 128) {
    BLDC.dir(0);
    BLDC.duty(map(controlInput, 128, 0, 0, 255));
  } else if (controlInput >= 128) {
    BLDC.dir(1);
    BLDC.duty(map(controlInput, 128, 255, 0, 255));
  }

  AS5047.readNow();
  AS5047.filter();

  BLDC.loop((int)AS5047.filtered);
}

*/
// --------------------------------------------------------- FIN BLDC LOOP

// --------------------------------------------------------- DEBUG LOOP
/*
int dblCounter = 0;
int comCounter = 0;
int comDivisor = 128;

void Debug_Loop() {

  AS5047.readNow();
  BLDC.duty(map(analogRead(9), 8, 1024, 28, 255));

  if (dblCounter % comDivisor == 0) {
    comCounter ++;
    Serial.print(""); // print 1st: i.e. at 'end' of last loop... so that spring has come out of motor
    Serial.print(comCounter % 6);
    Serial.print(", ");
    AS5047.filter();
    Serial.println(AS5047.filtered);
    BLDC.commutate(comCounter % 6);
  }

  dblCounter ++;

}
*/
// --------------------------------------------------------- FIN DEBUG LOOP

// --------------------------------------------------------- HZ BLDC UPDATE LOOP
/*
int comCounterHz = 0;

unsigned long comDivisorAdd = 0;
unsigned long nextFlip;

void HZ_Loop_BLDC() {
  
  BLDC.duty(map(analogRead(9), 0, 1024, 0, 255));

  if (micros() >= nextFlip) {
    BLDC.commutate(comCounterHz % 6);
    comCounterHz ++;
    comDivisorAdd = map(analogRead(8), 0, 1024, 1024, 132096);
    nextFlip = micros() + comDivisorAdd;
  }

}
*/
// --------------------------------------------------------- FIN HZ UPDATE LOOP
/*
unsigned long nextFlip;
unsigned long period;

void HZ_Loop_SVM() {
  
  if(micros() >= nextFlip){
    TSVM.duty(map(analogRead(9), 0, 1024, 0, 255));
    TSVM.commutate(0.05);
    period = map(analogRead(8), 0, 512, 0, 132096);
    nextFlip = micros() + period;
  }
}
*/
void loop() { 
}

