#ifndef SPI_DUAL_LS7366R_H_
#define SPI_DUAL_LS7366R_H_

#include <SPI.h>

#define SS1 10
#define SS2 4
#define SPI_SPEED 2000000
#define SPI_SHIFT MSBFIRST
#define SPI_MODE SPI_MODE0

enum SPICounterRegister { MDR0=1, MDR1, DTR, CNTR, OTR, STR};
enum SPICounterIrOp { CLR=0, RD, WR, LOAD};

struct SPICounterValue
{
  int32_t value;
  uint8_t data[4];
};

void ClearSPICounter(enum SPICounterRegister reg, uint8_t slaveSelect);
void ReadSPICounter(enum SPICounterRegister reg, uint8_t slaveSelect, struct SPICounterValue * spiValue);
void InitSPICounter();

#endif //SPI_DUAL_LS7366R_H_