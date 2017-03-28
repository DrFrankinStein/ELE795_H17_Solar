 // <, >, W, R, X, E, H, V, P, S, D
 
#define BUFFERSIZE 50
enum sens {CW=0,CCW=1};
char buffer[BUFFERSIZE], currentChar;
int numChar;
bool commandInput;

typedef struct axeData
{
  int position;
  int speed;
  enum sens direction;
} AxeData;

AxeData hor, ver;

void errorCommand()
{
  Serial.write("<E>");
  Serial.print(" Error ");
  Serial.write(buffer, numChar);
  Serial.println();
}

void resetCommand()
{
  Serial.print("<X> Reset ");
  Serial.write(buffer, numChar);
  Serial.println();

  Serial.print("Int size : ");
  Serial.println(sizeof(int));
  numChar=0;
  commandInput=false;
  hor.speed = 0;
  hor.position = 0;
  hor.direction = CW;
  ver.speed = 0;
  ver.position = 0;
  ver.direction = CW;
}

void readSettingCommand()
{
  /*Serial.print("Read ");
  Serial.write(buffer, numChar);
  Serial.println();*/
  bool readHor = false;
  bool readVer = false;

  switch(buffer[2])
  {
    case 'H':
    case 'h':
      readHor = true;
      break;
      
    case 'V':
    case 'v':
      readVer = true;
      break;
      
    default:
      errorCommand();
      return;  
  }

  Serial.print("Read ");
  
  if(readHor)
  {
    Serial.print("Hor Speed= ");
    Serial.print(hor.speed);
    Serial.print(" Dir= ");
    Serial.println(hor.direction);
  }

  if(readVer)
  {
    Serial.print("Ver Speed= ");
    Serial.print(ver.speed);
    Serial.print(" Dir= ");
    Serial.println(ver.direction);
  }
}

void writeSettingCommand()
{
  //Serial.print("Write ");
  //Serial.write(buffer, numChar);
  //Serial.println();

  bool modifyHor = false;
  bool modifyVer = false;
  bool modifyDir = false;
  int positionValue = 0;
  int speedValue = 0;
  enum sens directionValue;
  int value = 0, tmp = -1;
  
  //<whp1234>
  //<wvp1234>
  //<whs1234>
  //<wvs1234>
  //<whs1234+>
  //<whs1234->
  //<wvs1234+>
  //<wvs1234->

  if (numChar == 9 || numChar == 10)
  {
    switch(buffer[2])
    {
      case 'H':
      case 'h':
        modifyHor = true;
        break;
        
      case 'V':
      case 'v':
        modifyVer = true;
        break;
        
      default:
        errorCommand();
        return;  
    }

    switch(buffer[3])
    {
      /*case 'P':
      case 'p':
        if (numChar == 9)
        {
          for (int i = 0; i < 4; i++)
          {
            tmp = (int)(buffer[i+4]-'0');
            if (tmp < 0 || tmp > 9)
            {
              errorCommand();
              return;
            }
            positionValue = positionValue*10 + tmp;
          }

          if(modifyHor)
          {
            hor.position = positionValue;
          }

          if(modifyVer)
          {
            ver.position = positionValue;
          }
        }
        else
        {
          errorCommand();
          return;
        }
        break;*/
        
      case 'S':
      case 's':
          for (int i = 0; i < 4; i++)
          {
            tmp = (int)(buffer[i+4]-'0');
            if (tmp < 0 || tmp > 9)
            {
              errorCommand();
              return;
            }
            speedValue = speedValue*10 + tmp;
          }

          if (numChar == 10)
          {
            switch(buffer[8])
            {
              case '1':
                directionValue = CCW;
                modifyDir = true;
                break;

              case '0':
                directionValue = CW;
                modifyDir = true;
                break;
              
              default:
                errorCommand();
                return;  
            }
          }

          if(modifyHor)
          {
            hor.speed = speedValue;
            if (modifyDir)
              hor.direction = directionValue;
          }

          if(modifyVer)
          {
            ver.speed = speedValue;
            if (modifyDir)
              ver.direction = directionValue;
          }
                  
        break;
        
      default:
        errorCommand();
        return;  
    }

    
  }
  else
  {
    errorCommand();
    return;
  }
    
  Serial.print("Write ");

  if(modifyHor)
  {
    Serial.print("Hor Speed= ");
    Serial.print(hor.speed);
    Serial.print(" Dir= ");
    Serial.println(hor.direction);
  }

  if(modifyVer)
  {
    Serial.print("Ver Speed= ");
    Serial.print(ver.speed);
    Serial.print(" Dir= ");
    Serial.println(ver.direction);
  }
}

void decryptCommand()
{
  
  if(numChar > 2)
  {
    switch(buffer[1])
    {
      case 'W':
      case 'w':
        writeSettingCommand();
        break;
      case 'R':
      case 'r':
        readSettingCommand();
        break;
      case 'X':
      case 'x':
        resetCommand();
        break;
      default:
        errorCommand();
        break;  
    }
  }
  else
  {
    errorCommand();
  }
}

unsigned long previousMillis = 0;
const int ledPin =  LED_BUILTIN;
int ledState = LOW; 

void setup() 
{
  Serial.begin(9600);
  
  resetCommand();

  pinMode(ledPin, OUTPUT);
}

void loop() 
{
  unsigned long currentMillis = millis();

  if (hor.speed > 0 && (currentMillis - previousMillis >= hor.speed)) 
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) 
    {
      ledState = HIGH;
    } 
    else 
    {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

void serialEvent() 
{
  while (Serial.available()) 
  {
    currentChar = (char)Serial.read();
    
    switch(currentChar)
    {
      case '<':
        if (!commandInput)
        {
          commandInput = true;
          buffer[numChar] = currentChar;
          numChar++;
        }
        break;

      case '>':
        if (commandInput)
        {
          buffer[numChar] = currentChar;
          numChar++;
          //Serial.write(buffer,numChar);
          decryptCommand();
          commandInput = false;
          numChar = 0;
        }
        break;

      default:
        if (commandInput && (numChar < (BUFFERSIZE-1)))
        {
          buffer[numChar] = currentChar;
          numChar++;
        }
        break;
    }
  }
}
