#include "ADCLightCaptor.hpp"

void InitADC()
{
	analogReadResolution(12);
}

int CalculateADCSolarCaptor(int captor)
{
	return analogRead(captor);
}

void CalculateADCDifference(int* vertical, int* horizontal)
{
	enum direction {NW, NE, SW, SE, DIR_MAX};
	int value[DIR_MAX];
	
	for(int i=0; i<DIR_MAX; i++)
	{
		switch(i)
		{
			case NW:
				value[i] = CalculateADCSolarCaptor(NW_CAPTOR);
				break;
			case NE:
				value[i] = CalculateADCSolarCaptor(NE_CAPTOR);
				break;
			case SW:
				value[i] = CalculateADCSolarCaptor(SW_CAPTOR);
				break;
			case SE:
				value[i] = CalculateADCSolarCaptor(SE_CAPTOR);
				break;
				
		}
	}
	
	*vertical = ((value[NW] + value[NE])-(value[SW] + value[SE]));
	*horizontal = ((value[NW] + value[SW])-(value[NE] + value[SE]));
}