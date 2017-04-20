#include "SPIDualLS7366R.hpp"
#include "ArduinoSwitch.hpp"

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
#define MIN_MOT_THRESHOLD 3000

enum Motor {HOR_MOTOR, VER_MOTOR};

bool lastClickV = false, lastClickH = false;

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

void InitMotorsPort()
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

void InitMotorsPosition()
{
  int test[5] = {25, 50, 75, 150, 0}, currentTestValue;
  struct SPICounterValue spiValue;
  int positionMotor = 0;
  bool switchFound;
  int switchPosition;
  enum Motor motor;

  // Init Port and SPI
  Serial.println("PORT AND SPI");
  InitMotorsPort();
  InitSPICounter();
  InitLimitSwitch(); 

  for (int i = 0 ; i<2; i++)
  {
    Serial.print("CYCLE");
    Serial.println(i);
    //Select motor
    motor = (enum Motor)i;

    //Set element depending on the motor
    int pwmId = (motor == HOR_MOTOR) ? PWMH : PWMV;
    int dirId = (motor == HOR_MOTOR) ? DIRH : DIRV;
    int brkId = (motor == HOR_MOTOR) ? BRKH : BRKV;
    int slaveId = (motor == HOR_MOTOR) ? SSH : SSV;
    int limitSwitch = (motor == HOR_MOTOR) ? LSWH : LSWV; 
    bool switchFound = false;

    for( int testId = 0; (testId < 9)&& !switchFound; testId++)
    {
      Serial.print("TEST");
      Serial.println(testId);
      // If the limit switch is already
      if (!digitalReadSwitchUnbounced(limitSwitch))
        break;

      currentTestValue = test[testId/2] * (testId%2?-1:1); 
            
      ReadSPICounter(CNTR, slaveId, &spiValue);
      
      positionMotor = spiValue.value;
      
      digitalWrite(dirId, (currentTestValue-positionMotor > 0));

      while (abs(currentTestValue-positionMotor) >= 1 && !switchFound)
      {
        analogWrite(pwmId,MIN_MOT_THRESHOLD);
          
        ReadSPICounter(CNTR, slaveId, &spiValue);       
        positionMotor = spiValue.value;

        if(!digitalReadSwitchUnbounced(limitSwitch) && !switchFound)
        {
          Serial.println("TROUVEE!!!");
          switchFound = true;
          switchPosition = positionMotor;
        }
      }
  
      analogWrite(pwmId, 0);
    }

    // go to switch position
    if (digitalReadSwitchUnbounced(limitSwitch))
    {
      currentTestValue = switchPosition; 
  
      ReadSPICounter(CNTR, slaveId, &spiValue);
    
      positionMotor = spiValue.value;
      
      digitalWrite(dirId, (currentTestValue-positionMotor > 0));
      
      while (abs(currentTestValue-positionMotor) >= 1 && digitalReadSwitchUnbounced(limitSwitch))
      {
        analogWrite(pwmId,MIN_MOT_THRESHOLD);
          
        ReadSPICounter(CNTR, slaveId, &spiValue);
        positionMotor = spiValue.value;
      }
      
      analogWrite(pwmId, 0);
    }
  }

  // Reset counter for motors
  InitSPICounter(); 
}

void setup() 
{

  Serial.begin(9600);
  while (!Serial);

  InitMotorsPosition();
}

void loop() 
{
  int desiredPosition = 0;
  char car;
  enum Motor motor;
  bool moveMotor = false;

  if (Serial.available() > 0)
  {
    car = Serial.read();

    switch(car)
    {
      case 'H':
      case 'h':
        motor = HOR_MOTOR;
        moveMotor = true;
        break;

      case 'V':
      case 'v':
        motor = VER_MOTOR;
        moveMotor = true;
        break;

      case 'R':
      case 'r':
        InitMotorsPosition();
        break;
    }

    if (moveMotor)
    {
      desiredPosition = Serial.parseInt();
      GoToPosition(desiredPosition, motor);
    }
    
    while (Serial.available() > 0)
    {
      Serial.read();
    }
  }

  bool clickH = !digitalReadSwitchUnbounced(LSWH), clickV = !digitalReadSwitchUnbounced(LSWV);

  if(lastClickH != clickH)
  {
    lastClickH = clickH;
    Serial.print("Limit switch H ");
    Serial.println(lastClickH?"clicked":"unclicked"); 
  }

  if(lastClickV != clickV)
  {
    lastClickV = clickV;
    Serial.print("Limit switch V ");
    Serial.println(lastClickV?"clicked":"unclicked"); 
  }
  
}
