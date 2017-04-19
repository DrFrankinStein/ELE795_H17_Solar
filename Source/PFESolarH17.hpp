#ifndef PFE_SOLAR_H17_H
#define PFE_SOLAR_H17_H

#include "SPIDualLS7366R.hpp"

// Limit switches
#define LSWH 7
#define LSWV 6
#define BOUNCE_DELAY_MS 5

// ADC photodiode
#define ADC_NW A8
#define ADC_NE A9
#define ADC_SW A10
#define ADC_SE A11

// Counter motor encoder 
#define CNTH SS1 //10 
#define CNTV SS2 //4

// Motor
#define DIRH 12
#define DIRV 13
#define PWMH 3
#define PWMV 11
#define BRKH 9
#define BRKV 8

// Gear Ratio
#define GEAR_RATIO_H
#define GEAR_RATIO_V

// Communication
// TODO SerialUSB

#endif // PFE_SOLAR_H17_H