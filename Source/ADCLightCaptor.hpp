#ifndef ADC_LIGHT_CAPTOR_H
#define ADC_LIGHT_CAPTOR_H

#include <Arduino.h>

#define NW_CAPTOR 8
#define NE_CAPTOR 8
#define SW_CAPTOR 9
#define SE_CAPTOR 10

void InitADC();
int CalculateADCSolarCaptor(int captor);
void CalculateADCDifference(int* vertical, int* horizontal);


#endif // ADC_LIGHT_CAPTOR_H