#ifndef _ADSP9960_PLATFOR_H
#define _ADSP9960_PLATFOR_H

void i2c_LowLevel_DeInit(void);
void APDS9960_i2c_Init(void);

uint32_t I2C_TIMEOUT_UserCallback(void);
bool wireWriteByte(uint8_t val);
bool wireWriteDataByte(uint8_t reg, uint8_t val);
bool wireWriteDataBlock(uint8_t reg,uint8_t *val,unsigned int len);
bool wireReadDataByte(uint8_t reg, uint8_t *val);
bool wireReadDataBlock(uint8_t reg,uint8_t *val,unsigned int len);

#endif
