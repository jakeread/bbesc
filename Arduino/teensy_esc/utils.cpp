// Utils for Teensy ESC
// Todo: get into kernel, how to kernel?

#include "utils.h"

Utils::Utils(){
  // ain't no thang
}

void Utils::doubleMap(double* valPtr, double fromLo, double fromHi, double toLo, double toHi){
  // maths
  *valPtr = (*valPtr-fromLo) * (toHi - toLo) / (fromHi - fromLo) + toLo;
}

