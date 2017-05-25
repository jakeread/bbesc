#ifndef CONFIG_H
#define CONFIG_H

#define TRUE                1
#define FALSE               0

// ENCODER SETUPS

#define AS5047_RESOLUTION   16384 
#define AS5047_SAMPLES      256    // keep lots of these around so that we can do filtering, derivatives
#define AS5047_AVERAGING    8

#define RBUF_LENGTH 256

#define AS5047_OFFSET_UP    219     // MT5208: 6663 = 6858 - 195 (offset - 1/2 switch phase)
#define AS5047_OFFSET_DOWN  1270
#define AS5047_REV          TRUE          // set TRUE to reverse values, 0 to retain original


// MOTOR SPECIFIC

#define MOTOR_POLEPAIRS     11 // MT5208 = 7
#define MOTOR_MODULO        1489 // AS5047_RESOLUTION / MOTOR_POLEPAIRS // // MT4108 = 1489 || MT5208 = 2340 = resolution / poles / 2

// DRIVER

#define LEG_PWMRES          8
#define LEG_PWMFREQ         187500
#define LEG_MINPWM          0


// LOOP SPEEDS

#define AS5047_LOOP_HZ      90000
#define SVM_LOOP_HZ         30000
#define DEBUG_LOOP_HZ       10

// SVM

#define PHASE_ADVANCE       1.45 // RADIANS: optimum is 90*, or PI/2, or 1.57
#define POS_TO_THETA        0.0042197349

#endif
