#include "SPIDualLS7366R.hpp"

#define DIRA 12
#define DIRB 13
#define PWMA 3
#define PWMB 11
#define BRKA 9
#define BRKB 8  

#define DIRH DIRA
#define DIRV DIRB
#define PWMH PWMA
#define PWMV PWMB
#define BRKH BRKA
#define BRKV BRKB

#define SSH SS1
#define SSV SS2

#define GEAR_RATIO 30
#define ENCODER_STEP 10
#define FULL_ROT 300
#define SLOW_DOWN_THRESHOLD 50
#define MAX_MOT_THRESHOLD 4000
#define MIN_MOT_THRESHOLD 2000

enum Motor {HOR_MOTOR, VER_MOTOR};

void GoToPosition(int desiredPosition, enum Motor motor)
{
  struct SPICounterValue spiValue;
  int positionMotor = 0;

  int pwmId = (motor == HOR_MOTOR) ? PWMH : PWMV;
  int dirId = (motor == HOR_MOTOR) ? DIRH : DIRV;
  int brkId = (motor == HOR_MOTOR) ? BRKH : BRKV;
  int slaveId = (motor == HOR_MOTOR) ? SSH : SSV;
  
  ReadSPICounter(CNTR, slaveId, &spiValue);
  
  positionMotor = spiValue.value;

  digitalWrite(dirId, (desiredPosition-positionMotor > 0));
  
  while (abs(desiredPosition-positionMotor) >= 1)
  {
    if (abs(desiredPosition-positionMotor) <= SLOW_DOWN_THRESHOLD)
      analogWrite(pwmId,MIN_MOT_THRESHOLD + (MAX_MOT_THRESHOLD - MIN_MOT_THRESHOLD) * abs(desiredPosition-positionMotor) / SLOW_DOWN_THRESHOLD);
    else
      analogWrite(pwmId,MAX_MOT_THRESHOLD);
      
    ReadSPICounter(CNTR, slaveId, &spiValue);
    positionMotor = spiValue.value;
    Serial.println(positionMotor);
    delay(5);
  }

  analogWrite(pwmId, 0);
}

void InitMotor()
{
  pinMode(DIRH, OUTPUT);
  pinMode(DIRV, OUTPUT);
  pinMode(BRKH, OUTPUT);
  pinMode(BRKV, OUTPUT);
  digitalWrite(DIRH, HIGH);
  digitalWrite(DIRV, HIGH);
  analogWrite(PWMH, 0);
  analogWrite(PWMV, 0);
  digitalWrite(BRKH, LOW);
  digitalWrite(BRKV, LOW);
  analogWriteResolution(12);
}

/*void GoToPosition(int desiredPosition)
{
  struct SPICounterValue spiValue1;
  int positionMotor = 0;
  
  ReadSPICounter(CNTR, SS2, &spiValue1);
  positionMotor = spiValue1.value;

  digitalWrite(DIRB, (desiredPosition-positionMotor > 0));
  
  while (abs(desiredPosition-positionMotor) >=1)
  {
    if (abs(desiredPosition-positionMotor) <=50)
    {
      analogWrite(PWMB,2000 + (4000 - 2000) * abs(desiredPosition-positionMotor) / 50);
    }
    else
      analogWrite(PWMB,4000);
      
    ReadSPICounter(CNTR, SS2, &spiValue1);
    positionMotor = spiValue1.value;
    Serial.println(positionMotor);
    delay(5);
  }

  analogWrite(PWMB, 0);
}*/

void setup() 
{

  Serial.begin(9600);
  while (!Serial);

  InitMotor();
  InitSPICounter(); 
}

void loop() 
{
  int desiredPosition = 0;

  if (Serial.available() > 0)
  {
    desiredPosition = Serial.parseInt();
    GoToPosition(desiredPosition, HOR_MOTOR);
    
    while (Serial.available() > 0)
    {
      Serial.read();
    }
  }
}
