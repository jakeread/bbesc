#include "motorleg.h"
#include "svm.h"
#include "bldc.h"
#include "encoder_as5047.h"

#include "config.h"

#define RUN_BLDC_DEBUG_LOOP     0
#define RUN_SVM_DEBUG_LOOP      1
#define RUN_ENCODER_LOOP        1
#define RUN_SVM_LOOP            0
#define RUN_BLDC_LOOP           0

#if RUN_BLDC_DEBUG_LOOP || RUN_SVM_DEBUG_LOOP
  IntervalTimer Debug_Timer;
#endif

#if RUN_ENCODER_LOOP
  AS5047 TAS5047;
  IntervalTimer AS5047_Timer;
#endif

#if RUN_SVM_LOOP || RUN_SVM_DEBUG_LOOP
  SVM TSVM(10, 9, 6, 5, 4, 3); // THESE IN A KERNEL : YES DO IT SOON
  //SVM TSVM(3,4,5,6,9,10);
  IntervalTimer SVM_Timer;
#endif

#if RUN_BLDC_LOOP
  BLDC TBLDC(10, 9, 6, 5, 4, 3);
  IntervalTimer BLDC_Timer;
#endif


void setup() {

  pinMode(17, INPUT); // V_SENSE_A
  pinMode(21, INPUT); // A_SENSE_A
  pinMode(18, INPUT); // V_SENSE_B
  pinMode(22, INPUT); // A_SENSE_B
  pinMode(19, INPUT); // V_SENSE_C
  pinMode(23, INPUT); // A_SENSE_C
  
  pinMode(13, OUTPUT);

  Serial.begin(115200);

  #if RUN_ENCODER_LOOP
    TAS5047.init();
    AS5047_Timer.begin(AS5047_Loop, 1000000/AS5047_LOOP_HZ); 
  #endif

  #if RUN_SVM_LOOP
    SVM_Timer.begin(SVM_Loop, 1000000/SVM_LOOP_HZ);
  #endif

  #if RUN_BLDC_LOOP
    BLDC_Timer.begin(BLDC_Loop, 1000000/BLDC_LOOP_HZ);       
  #endif

  #if RUN_SVM_DEBUG_LOOP || RUN_BLDC_DEBUG_LOOP
    Debug_Timer.begin(Debug_Loop, 1000000/DEBUG_LOOP_HZ); 
  #endif
}

// ------------------------------------------------------- AS5047 LOOP
#if RUN_ENCODER_LOOP
void AS5047_Loop(){
  TAS5047.readNow();
}
#endif
// ------------------------------------------------------- SVM Loop
#if RUN_SVM_LOOP
int controlInput = 0;

void SVM_Loop(){
  digitalWrite(13, !digitalRead(13));
  
  controlInput = 824;

  if (controlInput < 512) {
    TSVM.dir(-1);
    TSVM.duty(map(controlInput, 512, 0, 0, 255));
    //Serial.println(map(controlInput, 512, 0, 0, 255));
  } else if (controlInput >= 512) {
    TSVM.dir(1);
    TSVM.duty(map(controlInput, 512, 1023, 0, 255));
    //Serial.println(map(controlInput, 512, 1023, 0, 255));
  }
  TSVM.loop(TAS5047.filteredInt());
}
#endif
// ------------------------------------------------------- SVM Debug Loop
#if RUN_SVM_DEBUG_LOOP
int cycleCounter = 0;
int filteredKeeper = 0;
double thetaKeeper = 0;
int immediateReading = 0;

void Debug_Loop(){
  digitalWrite(13, !digitalRead(13));

  if(cycleCounter % 12 == 0){

    noInterrupts();
    filteredKeeper = TAS5047.filteredInt();
    thetaKeeper = TSVM.getTheta();
    immediateReading = TAS5047  .mostRecent();
    interrupts();
    
    Serial.print("F:\t");
    Serial.print(filteredKeeper);
    Serial.print("\tI:\t");
    Serial.print(immediateReading);
    Serial.print("\tFM:\t");
    Serial.print(filteredKeeper % MOTOR_MODULO);
    
    Serial.print("\tT:\t");
    Serial.print(thetaKeeper);

    Serial.print("\tAV: ");
    Serial.print(analogRead(17));
    Serial.print("\tAA: ");
    Serial.print(analogRead(21));
    Serial.print("\tBV: ");
    Serial.print(analogRead(18));
    Serial.print("\tBA: ");
    Serial.print(analogRead(22));
    Serial.print("\tCV: ");
    Serial.print(analogRead(19));
    Serial.print("\tCA: ");
    Serial.print(analogRead(23));
    /*
    Serial.print("\tsetValA:\t");
    Serial.print(TSVM.getSetVal(0));
    TSVM.MLA->report();
    Serial.print("\tsetValB:\t");
    Serial.print(TSVM.getSetVal(1));
    TSVM.MLB->report();
    Serial.print("\tsetValC:\t");
    Serial.print(TSVM.getSetVal(2));
    TSVM.MLC->report();
    */
    Serial.println("");

  }
 
  TSVM.commutate(PI/48);  // moves theta along
  TSVM.duty(158);         // sets duty
  TSVM.assert();          // flips switches
  
  cycleCounter ++;
  
  
  // u kno the modulo
  // do modulo, print vals
  // avg all 0-com-cycle vals for 0-point
  // this is 'where it is' when encoder,
  // ++ // -- 90deg (pi/2 rads) (or less, for safety, #define a PHASE_ADVANCE) from this for commutate
}
#endif

// -------------------------------------------------------- BLDC LOOP
#if RUN_BLDC_LOOP
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
#endif
// --------------------------------------------------------- FIN BLDC LOOP

// --------------------------------------------------------- DEBUG LOOP
#if RUN_BLDC_DEBUG_LOOP

BLDC TBLDC(10, 9, 6, 5, 4, 3);

int dblCounter = 0;
uint16_t comCounter = 5;
int comDivisor = 1;

void Debug_Loop() {

  digitalWrite(13, !digitalRead(13));

  TBLDC.duty(138);

  if (dblCounter % comDivisor == 0) {
    comCounter ++;
    Serial.println(""); // print 1st: i.e. at 'end' of last loop... so that spring has come out of motor
    Serial.print(comCounter % 6);
    TBLDC.commutate(comCounter % 6);
    Serial.print(", ");
    Serial.println(TAS5047.filteredInt());
  }

  dblCounter ++;

}
#endif

// --------------------------------------------------------- FIN DEBUG LOOP

// --------------------------------------------------------- HZ SVM UPDATE LOOP

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

