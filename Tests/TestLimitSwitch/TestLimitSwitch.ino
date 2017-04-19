#include "ArduinoSwitch.hpp"

long lastTime;
int countH, countV;
bool lastValueH, lastValueV;

void setup() 
{
  Serial.begin(9600);
  while (!Serial);
  // put your setup code here, to run once:
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  countH = 0;
  countV = 0;
  lastValueH = digitalReadSwitchUnbounced(7,10);
  lastValueV = digitalReadSwitchUnbounced(6,10);
}

void loop() 
{
  bool valueH, valueV;

  valueH = digitalReadSwitchUnbounced(7,10);
  valueV = digitalReadSwitchUnbounced(6,10);
  
  if(valueH!=lastValueH)
  {
    countH++;
    lastValueH= valueH;
  }

  if(valueV!=lastValueV)
  {
    countV++;
    lastValueV= valueV;
  }
  
  Serial.print(countH);
  Serial.print(" ");
  Serial.println(countV);
}
