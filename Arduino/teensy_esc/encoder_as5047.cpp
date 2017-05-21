// Does SPI Footwork, averaging

#include "encoder_as5047.h"

AS5047::AS5047(){
  // cnstructor
}

void AS5047::init(){
  
  SPI_AS5047.begin_MASTER(ALT_SCK, ALT_MOSI, ALT_MISO, CS4, CS_ActiveLOW);
  SPI_AS5047.setCTAR(CTAR_0, 16, SPI_MODE1, LSB_FIRST, SPI_CLOCK_DIV8);

  readWord = 0x3FFF | 0x4000 | 0x8000;
  noOpWord = 0x0000 | 0x4000 | 0x8000;

  readWords[0] = readWord;
  readWords[1] = noOpWord;

  readingPosition = 0;

  avgSum = 0;
  
}

void AS5047::readNow(){
  
  SPI_AS5047.txrx16(readWords, returnWords, 2, CTAR_0, CS4);

  reading = returnWords[1] << 2;
  reading /= 4;
  
  Readings.push(reading); // 1st is wake-up
}

void AS5047::filter(){
  for(int i = 0; i < AS5047_AVERAGING; i++){
    /*
    Serial.print("IT: ");
    Serial.print(-i);
    Serial.print(" ");
    Serial.print(Readings.get(-i));
    Serial.print(" ");
    */
    avgSum += Readings.get(-i); // ringbuffer past / over end?
  }
  filtered = avgSum / AS5047_AVERAGING;
  avgSum = 0;
}

