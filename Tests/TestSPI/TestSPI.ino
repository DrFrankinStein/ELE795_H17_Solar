#include "SPIDualLS7366R.hpp"

byte test;

void setup() 
{
  Serial.begin(9600);
  while(!Serial);

  InitSPICounter(); 
}

void loop() 
{
  struct SPICounterValue spiValue1, spiValue2;

  ReadSPICounter(CNTR, SS1, &spiValue1);
  ReadSPICounter(CNTR, SS2, &spiValue2);
  
  Serial.print(spiValue1.value);
  Serial.print(" ");
  Serial.println(spiValue2.value);
  
  delay(1000);
}
