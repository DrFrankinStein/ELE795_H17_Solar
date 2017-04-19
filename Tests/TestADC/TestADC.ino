#define NN_CAPTOR 8
#define SW_CAPTOR 9
#define SE_CAPTOR 10


void setup() 
{
  Serial.begin(9600);
  while(!Serial);
  analogReadResolution(12);
}

void loop() 
{
  int nn, sw, se;
  // put your main code here, to run repeatedly:
  nn = analogRead(NN_CAPTOR);
  sw = analogRead(SW_CAPTOR);
  se = analogRead(SE_CAPTOR);

  Serial.print("NN=");
  Serial.print(nn);
  Serial.print(' ');

  Serial.print("SW=");
  Serial.print(sw);
  Serial.print(' ');

  Serial.print("SE=");
  Serial.print(se);
  Serial.print(' ');

  Serial.println();

  delay(5);
}
