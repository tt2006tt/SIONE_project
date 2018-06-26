#include "main.h"

#define APDS9960_i2c_TIMING           0xff410507
#define APDS9960_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define APDS9960_LONG_TIMEOUT         ((uint32_t)(10 * APDS9960_FLAG_TIMEOUT))
__IO uint32_t  APDS9960_Timeout = APDS9960_LONG_TIMEOUT;

void i2c_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /* sEE_I2C Peripheral Disable */
  I2C_Cmd(I2C2, DISABLE);
 
  /* sEE_I2C DeInit */
  I2C_DeInit(I2C2);

  /* sEE_I2C Periph clock disable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, DISABLE);
    
  /* GPIO configuration */  
  /* Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void i2c_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Configure the I2C clock source. The clock is derived from the HSI */
  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);//?
    
  /* sEE_I2C_SCL_GPIO_CLK and sEE_I2C_SDA_GPIO_CLK Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB| RCC_AHBPeriph_GPIOB, ENABLE);
  
  /* sEE_I2C Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  
  /* Connect PXx to I2C_SCL*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_1);
  
  /* Connect PXx to I2C_SDA*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_1);
  
  /* GPIO configuration */  
  /* Configure sEE_I2C pins: SCL */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Configure sEE_I2C pins: SDA */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void i2c_DeInit(void)
{
  i2c_LowLevel_DeInit(); 
}

void APDS9960_i2c_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  
  i2c_LowLevel_Init();
  
  /* I2C configuration */
  /* sEE_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = APDS9960_i2c_TIMING;
  
  /* Apply sEE_I2C configuration after enabling it */
  I2C_Init(I2C2, &I2C_InitStructure);
   
  /* sEE_I2C Peripheral Enable */
  I2C_Cmd(I2C2, ENABLE); 
}


uint32_t I2C_TIMEOUT_UserCallback(void)
{
	#ifdef debug
	Uart_Puts("err\n");
	#endif
	return(0);
}

bool wireWriteByte(uint8_t val)
{
  return(true);
}

bool wireWriteDataByte(uint8_t reg, uint8_t val)
{

   I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1, 2,I2C_SoftEnd_Mode, I2C_Generate_Start_Write);	
   APDS9960_Timeout = APDS9960_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
   {
     if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
   }

    I2C_SendData(I2C2, reg);
    APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  	
    while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
    {
      if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }	 
		
		I2C_SendData(I2C2, val);  
		APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
    while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
    {
      if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }	 
		
	  I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1,0,I2C_AutoEnd_Mode, I2C_Generate_Stop);
    I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
	
    return true;  			 
}

bool wireWriteDataBlock(uint8_t reg,uint8_t *val,unsigned int len)
{
   uint8_t i;
	
   I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1, 1+len,I2C_SoftEnd_Mode, I2C_Generate_Start_Write);	
   APDS9960_Timeout = APDS9960_LONG_TIMEOUT;
   while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
   {
     if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
   }

    I2C_SendData(I2C2, reg);
  
    APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
	
    while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
    {
      if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }	 
		 
		 
     for(i=0;i<(len-1);i++)
     {		 
			 I2C_SendData(I2C2, *(val+i));
  
			 APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
	
       while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
      {
       if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
      }	 
		 }

		 I2C_SendData(I2C2, *(val+i));
		 i++;		 
		 APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
     while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
     {
       if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
     }	 		
		
	  I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1,0,I2C_AutoEnd_Mode, I2C_Generate_Stop);
    I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);
	
  return true; 
}

bool wireReadDataByte(uint8_t reg, uint8_t *val)	
{
	uint8_t i = 0;

	I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  APDS9960_Timeout =APDS9960_LONG_TIMEOUT;
	
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }	  
	
  I2C_SendData(I2C2, (uint8_t)(reg));
  
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
	
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }
	
	
  I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1,1,I2C_AutoEnd_Mode,I2C_Generate_Start_Read);
	
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT; 
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET)
  {
   if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }   

	*val= I2C_ReceiveData(I2C2); 
	i++;
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }

  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);	
  return true;  
}

bool wireReadDataBlock(uint8_t reg,uint8_t *val,unsigned int len)
{
	uint8_t i = 0;
	I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  APDS9960_Timeout =APDS9960_LONG_TIMEOUT;
	
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TXIS) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }	  
	
  I2C_SendData(I2C2, (uint8_t)(reg));
  
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT;  
	
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_TC) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }
	
	
  I2C_TransferHandling(I2C2, APDS9960_I2C_ADDR<<1,len,I2C_AutoEnd_Mode,I2C_Generate_Start_Read);
	
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT; 
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET)
  {
   if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }   
	
	for(i=0;i<len-1;i++)
	{
	  val[i]= I2C_ReceiveData(I2C2); 
    APDS9960_Timeout = APDS9960_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C2, I2C_ISR_RXNE) == RESET)
    {
      if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
    }
	}	
	val[i]= I2C_ReceiveData(I2C2); 
	i++;
  APDS9960_Timeout = APDS9960_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(I2C2, I2C_ISR_STOPF) == RESET)
  {
    if((APDS9960_Timeout--) == 0) return I2C_TIMEOUT_UserCallback();
  }

  I2C_ClearFlag(I2C2, I2C_ICR_STOPCF);	
  return i; 
}
