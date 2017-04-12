#include "SPIDualLS7366R.hpp"

void ClearSPICounter(enum SPICounterRegister reg, uint8_t slaveSelect)
{
  if(reg == DTR || reg == OTR)
    return;
  
  SPI.beginTransaction(SPISettings(SPI_SPEED,SPI_SHIFT, SPI_MODE));
  digitalWrite(slaveSelect, LOW);
  SPI.transfer(((CLR&0x03)<<6) | ((reg&0x07)<<3));
  digitalWrite(slaveSelect, HIGH);
  SPI.endTransaction();
}

void ReadSPICounter(enum SPICounterRegister reg, uint8_t slaveSelect, struct SPICounterValue * spiValue)
{
  if(reg == DTR)
    return;
  
  SPI.beginTransaction(SPISettings(SPI_SPEED,SPI_SHIFT, SPI_MODE));
  digitalWrite(slaveSelect, LOW);
  SPI.transfer(((RD&0x03)<<6) | ((reg&0x07)<<3));
  switch(reg)
  {
    case MDR0:
    case MDR1:
    case STR:
      spiValue->data[0] = SPI.transfer(0);
    break;

    case CNTR:
    case OTR:
    for (int8_t i=0; i<4; i++)
      spiValue->data[i] = SPI.transfer(0);
      break;

    default:
      break;
  }
  digitalWrite(slaveSelect, HIGH);
  SPI.endTransaction();

  spiValue->value = 0;
  for (int i=0; i<4; i++)
    spiValue->value |= (spiValue->data[3-i]<<(8*i));
}

void InitSPICounter()
{
  pinMode(SS1,OUTPUT);
  pinMode(SS2,OUTPUT);

  digitalWrite(SS1, HIGH);
  digitalWrite(SS2, HIGH);

  SPI.begin();
  
  for (uint8_t i = MDR0; i <=STR; i++)
  {
    ClearSPICounter((enum SPICounterRegister)i,SS1);
    ClearSPICounter((enum SPICounterRegister)i,SS2);
  } 
}