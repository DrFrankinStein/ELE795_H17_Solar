#include "ArduinoSwitch.hpp"

bool digitalReadSwitchUnbounced(int pin, int numTry)
{
	long lastDelayTime, currentTime;
	int counter = 0;
	bool stabilized = false;
	bool pinValue = false, tmp = false;
	
	pinValue = digitalRead(pin);
	lastDelayTime = millis();
	
	while(!stabilized)
	{
		currentTime = millis();
		
		if(lastDelayTime != currentTime)
		{
			if(pinValue != (tmp=digitalRead(pin)))
			{
				pinValue = tmp;
				counter = 0;
			}
			else
			{
				counter++;
			}
			
			if(counter >= numTry)
			{
				stabilized = true;
			}
			
			lastDelayTime = millis();
		}
	}

	return pinValue;
}