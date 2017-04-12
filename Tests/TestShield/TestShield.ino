#include "SPIDualLS7366R.hpp"

#define DIRA 12
#define DIRB 13
#define PWMA 3
#define PWMB 11
#define BRKA 9
#define BRKB 8  

#define SINUS_SEC 10
#define DELAY_HZ 200
#define DELAY_MS 5

float sinus[SINUS_SEC*DELAY_HZ];

void setup() 
{
  for (int i=0; i<(SINUS_SEC*DELAY_HZ); i++)
  {
    sinus[i] = 4095.0*sin(((float)i*PI*2.0)/(SINUS_SEC*DELAY_HZ));
  }
  
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(BRKA, OUTPUT);
  pinMode(BRKB, OUTPUT);
  digitalWrite(DIRA, HIGH);
  digitalWrite(DIRB, HIGH);
  analogWrite(PWMA, 0);
  analogWrite(PWMB, 0);
  digitalWrite(BRKA, LOW);
  digitalWrite(BRKB, LOW);
  analogWriteResolution(12);
  Serial.begin(9600);
  while (!Serial);
  /*for (int i=0; i<(SINUS_SEC*DELAY_HZ); i++)
  {
    Serial.println(sinus[i]);
  }*/

  InitSPICounter(); 
}

void loop() 
{
  /*int pwm;
  bool dir;
  
  for (int i=0; i<SINUS_SEC*DELAY_HZ; i++)
  {
    
    dir = sinus[i] > 0;
    pwm = (int)(dir ? sinus[i]: -sinus[i]);
    
    analogWrite(PWMA,pwm);
    analogWrite(PWMB,pwm);
    digitalWrite(DIRA, dir?HIGH:HIGH);
    digitalWrite(DIRB, dir?HIGH:HIGH);
    delay(DELAY_MS);
  }*/

  /*struct SPICounterValue spiValue1;
  bool dir = digitalRead(DIRA);

  ReadSPICounter(CNTR, SS1, &spiValue1);

  Serial.println(spiValue1.value);

  if ((spiValue1.value > 100 && dir== HIGH) || (spiValue1.value < 0 && dir== LOW))
  {
      dir = !digitalRead(DIRA);
      digitalWrite(DIRA, dir);
  }

  delay(100);*/

  struct SPICounterValue spiValue1;
  int positionMotor = 0, desiredPosition = 0;

  if (Serial.available() > 0)
  {
    desiredPosition = Serial.parseInt();
    ReadSPICounter(CNTR, SS1, &spiValue1);
    positionMotor = spiValue1.value;

    digitalWrite(DIRA, (desiredPosition-positionMotor > 0));
    
    while (abs(desiredPosition-positionMotor) >=1)
    {
      /*if (abs(desiredPosition-positionMotor) >=100)
      {
        analogWrite(PWMA,4000);
      }

      else if (abs(desiredPosition-positionMotor) >=20)
      {
        analogWrite(PWMA,3000);
      }

      else
      {
        analogWrite(PWMA,2000);
      }*/

      if (abs(desiredPosition-positionMotor) <=50)
      {
        analogWrite(PWMA,2000 + (4000 - 2000) * abs(desiredPosition-positionMotor) / 50);
      }
      else
        analogWrite(PWMA,4000);
        
      ReadSPICounter(CNTR, SS1, &spiValue1);
      positionMotor = spiValue1.value;
      Serial.println(positionMotor);
      delay(5);
    }

    analogWrite(PWMA, 0);

    while (Serial.available() > 0)
    {
      Serial.read();
    }

    Serial.println(positionMotor);
  }
}
