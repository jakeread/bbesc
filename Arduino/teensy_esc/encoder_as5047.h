#ifndef ENCODER_AS5047_H
#define ENCODER_AS5047_H

#include <Arduino.h>
#include "t3spi.h"
#include "ringBuffer.h"
#include "config.h"

class AS5047 {

  public:
  AS5047();
  void init();
  void readNow(); // read 1 value
  uint16_t mostRecent();
  float filtered();
  T3SPI SPI_AS5047;

  RingBuffer Readings;

  private:
  uint8_t readingPosition;
  volatile float _filtered;
  volatile uint16_t _reading;

  volatile uint16_t readWord;
  volatile uint16_t noOpWord;
  volatile uint16_t readWords[2] = {};
  volatile uint16_t returnWords[2] = {};

  float avgSum;
};

#endif
