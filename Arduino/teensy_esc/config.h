#ifndef CONFIG_H
#define CONFIG_H

#define AS5047_RESOLUTION 16384 
#define AS5047_SAMPLES 256    // keep lots of these around so that we can do filtering, derivatives
#define AS5047_AVERAGING 8

#define RBUF_LENGTH 256

#define AS5047_OFFSET 833     // MT5208: 6663 = 6858 - 195 (offset - 1/2 switch phase)
#define AS5047_DIR 1          // 0 if doesn't match mag / elec dir, 1 if does

#define LEG_PWMFREQ 187500

#define MOTOR_POLEPAIRS 11 // MT5208 = 7
#define MOTOR_MODULO AS5047_RESOLUTION / MOTOR_POLEPAIRS // MT5208 = 2340 = resolution / poles / 2

#define AS5047_LOOP_HZ  20000
#define SVM_LOOP_HZ     20000
#define DEBUG_LOOP_HZ   10

#endif
