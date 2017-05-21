#ifndef ENCODER_AS5047_H
#define ENCODER_AS5047_H

#define AS5047_SAMPLES 256 // keep lots of these around so that we can do filtering, derivatives
#define AS5047_RESOLUTION 16384 
#define AS5047_AVERAGING 8

#include <Arduino.h>
#include "t3spi.h"
#include "ringBuffer.h"

class AS5047 {

  public:
  AS5047();
  void init();
  void readNow(); // read 1 value
  void filter();
  T3SPI SPI_AS5047;

  volatile uint16_t reading;
  RingBuffer Readings;
  volatile float filtered;

  private:
  uint8_t readingPosition;
  
  volatile uint16_t readWord;
  volatile uint16_t noOpWord;
  volatile uint16_t readWords[2] = {};
  volatile uint16_t returnWords[2] = {};

  float avgSum;
};

#endif
