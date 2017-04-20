#ifndef ARDUINO_SWITCH_H
#define ARDUINO_SWITCH_H

#include <Arduino.h>

#define LSWH 7
#define LSWV 6
#define BOUNCE_TRY 5

bool digitalReadSwitchUnbounced(int pin, int numTry);
bool digitalReadSwitchUnbounced(int pin);
void InitLimitSwitch();

#endif // ARDUINO_SWITCH_H
