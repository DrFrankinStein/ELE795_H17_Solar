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
}

void loop() 
{
  int pwm;
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
  }
}
