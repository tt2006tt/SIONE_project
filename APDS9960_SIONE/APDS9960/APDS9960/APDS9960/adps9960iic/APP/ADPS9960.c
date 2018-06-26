#include "ADPS9960.h"

#define    DELAYTIME  100


void delay(unsigned int ms)
{
  unsigned int x , y;
  for(x = ms; x > 0; x--)           /*  通过一定周期循环进行延时*/
    for(y = 1000 ; y > 0 ; y--);
}


static void delay_us(uint16_t time)
{
    do
    {
            nop();nop();nop();nop();
           // nop();nop();nop();nop();
           // nop();nop();nop();nop();
           // nop();nop();nop();nop();
    }while(time--);
}

void Soft_I2C_Int(void)
{
    Set_I2C_SCL_In;//
    Set_I2C_SDA_In;
        
}

bool Soft_I2C_Start(void)
{
    
        //Set_I2C_SCL_In;
        //Set_I2C_SDA_In;
        
        //while(!SDA_READ);//return FALSE;        //SDA线为低电平则总线忙,退出
        //if(!SCL_READ)return FALSE;
    
        Set_I2C_SCL_Out;
        Set_I2C_SDA_Out_H;
        SDA_H;
        SCL_H;
        delay_us(DELAYTIME);
        SDA_L;
        delay_us(DELAYTIME);
        SCL_L;                    //钳位，表示总线忙
        delay_us(DELAYTIME);
        return TRUE;
}

void Soft_I2C_Stop(void)
{
        SCL_L;
        delay_us(DELAYTIME);
        Set_I2C_SDA_Out_L;
        delay_us(DELAYTIME);
        SCL_H;
        delay_us(DELAYTIME);
        SDA_H;
        delay_us(DELAYTIME);
        Set_I2C_SCL_In;
        Set_I2C_SDA_In;
}

void Soft_I2C_Ack(void)
{        
        SCL_L;
        delay_us(DELAYTIME);
        Set_I2C_SDA_Out_L;
        delay_us(DELAYTIME);
        SCL_H;
        delay_us(DELAYTIME);
        SCL_L;
        delay_us(DELAYTIME);
}


void Soft_I2C_NAck(void)
{        
        SCL_L;
        delay_us(DELAYTIME);
        Set_I2C_SDA_Out_H;
        delay_us(DELAYTIME);
        SCL_H;
        delay_us(DELAYTIME);
        SCL_L;
        delay_us(DELAYTIME);
}

bool Soft_I2C_Check_Ack(void)          //返回为:=1有ACK,=0无ACK
{
        SCL_L;
		delay_us(DELAYTIME);
        Set_I2C_SDA_In; 
        delay_us(DELAYTIME);
        SCL_H;
        delay_us(DELAYTIME);
        if(SDA_READ)
        {
           SCL_L;           
           return FALSE;
        }
        SCL_L;
        return TRUE;
}

void Soft_I2C_Send_8bit(u8 SendByte) //数据从高位到低位//
{
    u8 i=8;
    
    Set_I2C_SDA_Out_H; 
    while(i--)
    {
        SCL_L;
        delay_us(DELAYTIME/2);
        if(SendByte&0x80)
        {
            SDA_H;  
        }
        else 
        {
            SDA_L;   
        }
        SendByte<<=1;
        //delay_us(DELAYTIME/2);
        SCL_H;
        delay_us(DELAYTIME);
    }
    SCL_L;
}

u8 Soft_I2C_Read_8bit(void)  //数据从高位到低位//
{ 
    u8 i=8;
    u8 ReceiveByte=0;

    Set_I2C_SDA_In;                                
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      delay_us(DELAYTIME);
      SCL_H;
      delay_us(DELAYTIME);        
      if(SDA_READ)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}



/****************************************************************************
* 名称：I2C_WriteByte(uint8_t reg,uint8_t data)
* 功能：进行IIC的写操作。
* 入口参数： uint8_t addr 寄存器的地址
* uint8_t data 写入的数据 
* 出口参数：无
* 说明：对触摸屏的寄存器进行写入操作，变量都是16进制
****************************************************************************/
bool wireWriteDataByte(uint8_t reg,uint8_t data)
{
#if 0 //库函数操作
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); //等待空闲

	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C1, APDS9960_I2C_WADDR, I2C_Direction_Transmitter);//器件地址 -- 默认0xD0
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	I2C_SendData(I2C1, reg);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	I2C_SendData(I2C1, data);//发送数据
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	I2C_AcknowledgeConfig(I2C1,ENABLE);

	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
#else
	Soft_I2C_Start();
	Soft_I2C_Send_8bit(APDS9960_I2C_WADDR);//

	while(!Soft_I2C_Check_Ack());//checkACK
	
	Soft_I2C_Send_8bit(reg);
	while(!Soft_I2C_Check_Ack());//checkACK

	Soft_I2C_Send_8bit(data);
	while(!Soft_I2C_Check_Ack());//checkACK

	Soft_I2C_Stop();

	

#endif
        return TRUE;
}


/****************************************************************************
* 名称：uint8_t I2C_ReadByte(uint8_t addr)
* 功能：进行IIC的读操作。
* 入口参数： uint8_t addr 寄存器的地址 
* 出口参数： uint8_t i 保存读到的数据并作为参数返回
* 说明：IIC读操作要先写寄存器再进行读寄存器的操作，否则会失败
****************************************************************************/
bool wireReadDataByte(uint8_t addr,uint8_t *val)
{
#if 0 //库函数操作      
	  //I2C_AcknowledgeConfig(I2C1,ENABLE);

      while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

      I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

      I2C_Send7bitAddress(I2C1, APDS9960_I2C_WADDR, I2C_Direction_Transmitter);//器件地址 -- 默认0xD0
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

      I2C_SendData(I2C1, addr);//寄存器地址
      while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));



      I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

      I2C_Send7bitAddress(I2C1, APDS9960_I2C_RADDR, I2C_Direction_Receiver);//器件地址 -- 默认0xD1
      while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


      while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));

      *val =I2C_ReceiveData(I2C1);//读取数据

      I2C_AcknowledgeConfig(I2C1,DISABLE);

      I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
#else
	Soft_I2C_Start();

	Soft_I2C_Send_8bit(APDS9960_I2C_WADDR);//  write 
	while(!Soft_I2C_Check_Ack());//checkACK

	Soft_I2C_Send_8bit(addr);//
	while(!Soft_I2C_Check_Ack());//checkACK

	delay_us(200);                     //delay
	
	Soft_I2C_Start();
	Soft_I2C_Send_8bit(APDS9960_I2C_RADDR);//  read 
	while(!Soft_I2C_Check_Ack());//checkACK
	


	*val = Soft_I2C_Read_8bit();
	Soft_I2C_Ack();//ACK


	Soft_I2C_Stop();
#endif
    return TRUE;
}

int wireReadDataBlock(uint8_t reg, uint8_t *val,unsigned int len)
{
	unsigned char i = 0;
	unsigned int len1 = len;

#if 0
	I2C_AcknowledgeConfig(I2C1,ENABLE);

	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C1, APDS9960_I2C_WADDR, I2C_Direction_Transmitter);//器件地址 -- 默认0xD0
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

	I2C_SendData(I2C1, reg);//寄存器地址
	while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));



	I2C_GenerateSTART(I2C1, ENABLE);//开启I2C1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));/*EV5,主模式*/

	I2C_Send7bitAddress(I2C1, APDS9960_I2C_RADDR, I2C_Direction_Receiver);//器件地址 -- 默认0xD1
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));


	//while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	
	
	for(i = 0;i < len - 1;i++){
		//printf("%d\r\n",i);
		while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
		val[i] =I2C_ReceiveData(I2C1);//读取数据
		//delay_us(50);
		
	}
	i++;
	while (!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED));
	val[i] =I2C_ReceiveData(I2C1);//读取数据
	
	I2C_AcknowledgeConfig(I2C1,DISABLE);
	
	I2C_GenerateSTOP(I2C1, ENABLE);//关闭I2C1总线
	

#else
    
    
	Soft_I2C_Start();

	Soft_I2C_Send_8bit(APDS9960_I2C_WADDR);//  write 
	while(!Soft_I2C_Check_Ack());//checkACK

	Soft_I2C_Send_8bit(reg);//
	while(!Soft_I2C_Check_Ack());//checkACK

	delay_us(200);                     //delay
	
	Soft_I2C_Start();
	Soft_I2C_Send_8bit(APDS9960_I2C_RADDR);//  read 
	while(!Soft_I2C_Check_Ack());//checkACK

	
	for(i = 0;i < len - 1;i++ ){
		val[i] = Soft_I2C_Read_8bit();
		delay_us(10);
		Soft_I2C_Ack();//ACK
	}
	i ++;
	val[i] = Soft_I2C_Read_8bit();
	delay_us(10);
	Soft_I2C_NAck();//noACK
	delay_us(10);
	Soft_I2C_Stop();
#endif

    return i;
}


/* Members */
gesture_data_type gesture_data_;
int gesture_ud_delta_;
int gesture_lr_delta_;
int gesture_ud_count_;
int gesture_lr_count_;
int gesture_near_count_;
int gesture_far_count_;
int gesture_state_;
int gesture_motion_;

 



/**
 * @brief Configures I2C communications and initializes registers to defaults
 *
 * @return True if initialized successfully. False otherwise.
 */
bool APDS9960init()
{
    uint8_t id;

   
    Soft_I2C_Int();
     
    /* Read ID register and check against known values for APDS-9960 */
    if( !wireReadDataByte(APDS9960_ID, &id) ) {
        return FALSE;
    }
    if( !(id == APDS9960_ID_1 || id == APDS9960_ID_2) ) {
		printf("id FALSE\r\n");
        return FALSE;
    }
     
    /* Set ENABLE register to 0 (disable all features) */
    if( !setMode(ALL, OFF) ) {
		printf("setMode FALSE\r\n");
		return FALSE;
    }
    
    /* Set default values for ambient light and proximity registers */
    if( !wireWriteDataByte(APDS9960_ATIME, DEFAULT_ATIME) ) {
		printf("APDS9960_ATIME FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_WTIME, DEFAULT_WTIME) ) {
		printf("APDS9960_WTIME FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_PPULSE, DEFAULT_PROX_PPULSE) ) {
		printf("APDS9960_PPULSE FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_POFFSET_UR, DEFAULT_POFFSET_UR) ) {
		printf("APDS9960_POFFSET_UR FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_POFFSET_DL, DEFAULT_POFFSET_DL) ) {
		printf("APDS9960_POFFSET_DL FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG1, DEFAULT_CONFIG1) ) {
		printf("APDS9960_CONFIG1 FALSE\r\n");
        return FALSE;
    }
    if( !setLEDDrive(DEFAULT_LDRIVE) ) {
		printf("DEFAULT_LDRIVE FALSE\r\n");
        return FALSE;
    }
    if( !setProximityGain(DEFAULT_PGAIN) ) {
		printf("DEFAULT_PGAIN FALSE\r\n");
        return FALSE;
    }
    if( !setAmbientLightGain(DEFAULT_AGAIN) ) {
		printf("DEFAULT_AGAIN FALSE\r\n");
        return FALSE;
    }
    if( !setProxIntLowThresh(DEFAULT_PILT) ) {
		printf("DEFAULT_PILT FALSE\r\n");
        return FALSE;
    }
    if( !setProxIntHighThresh(DEFAULT_PIHT) ) {
		printf("DEFAULT_PIHT FALSE\r\n");
        return FALSE;
    }
    if( !setLightIntLowThreshold(DEFAULT_AILT) ) {
		printf("DEFAULT_AILT FALSE\r\n");
        return FALSE;
    }
    if( !setLightIntHighThreshold(DEFAULT_AIHT) ) {
		printf("DEFAULT_AIHT FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_PERS, DEFAULT_PERS) ) {
		printf("APDS9960_PERS FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG2, DEFAULT_CONFIG2) ) {
		printf("APDS9960_CONFIG2 FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_CONFIG3, DEFAULT_CONFIG3) ) {
		printf("APDS9960_CONFIG3 FALSE\r\n");
        return FALSE;
    }
    
    /* Set default values for gesture sense registers */
    if( !setGestureEnterThresh(DEFAULT_GPENTH) ) {
		printf("DEFAULT_GPENTH FALSE\r\n");
        return FALSE;
    }
    if( !setGestureExitThresh(DEFAULT_GEXTH) ) {
		printf("DEFAULT_GEXTH FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GCONF1, DEFAULT_GCONF1) ) {
		printf("APDS9960_GCONF1 FALSE\r\n");
        return FALSE;
    }
    if( !setGestureGain(DEFAULT_GGAIN) ) {
		printf("DEFAULT_GGAIN FALSE\r\n");
        return FALSE;
    }
    if( !setGestureLEDDrive(DEFAULT_GLDRIVE) ) {
		printf("DEFAULT_GLDRIVE FALSE\r\n");
        return FALSE;
    }
    if( !setGestureWaitTime(DEFAULT_GWTIME) ) {
		printf("DEFAULT_GWTIME FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_U, DEFAULT_GOFFSET) ) {
		printf("APDS9960_GOFFSET_U FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_D, DEFAULT_GOFFSET) ) {
		printf("APDS9960_GOFFSET_D FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_L, DEFAULT_GOFFSET) ) {
		printf("APDS9960_GOFFSET_L FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GOFFSET_R, DEFAULT_GOFFSET) ) {
		printf("APDS9960_GOFFSET_R FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GPULSE, DEFAULT_GPULSE) ) {
		printf("APDS9960_GPULSE FALSE\r\n");
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_GCONF3, DEFAULT_GCONF3) ) {
		printf("APDS9960_GCONF3 FALSE\r\n");
        return FALSE;
    }
    if( !setGestureIntEnable(DEFAULT_GIEN) ) {
		printf("DEFAULT_GIEN FALSE\r\n");
        return FALSE;
    }
    

    return TRUE;
}

/*******************************************************************************
 * Public methods for controlling the APDS-9960
 ******************************************************************************/

/**
 * @brief Reads and returns the contents of the ENABLE register
 *
 * @return Contents of the ENABLE register. 0xFF if error.
 */
uint8_t getMode()
{
    uint8_t enable_value;
    
    /* Read current ENABLE register */
    if( !wireReadDataByte(APDS9960_ENABLE, &enable_value) ) {
        return ERROR;
    }
    
    return enable_value;
}

/**
 * @brief Enables or disables a feature in the APDS-9960
 *
 * @param[in] mode which feature to enable
 * @param[in] enable ON (1) or OFF (0)
 * @return True if operation success. False otherwise.
 */
bool setMode(uint8_t mode, uint8_t enable)
{
    uint8_t reg_val;

    /* Read current ENABLE register */
    reg_val = getMode();
    if( reg_val == ERROR ) {
		printf("reg_val error\r\n");
        return FALSE;
    }
    
    /* Change bit(s) in ENABLE register */
    enable = enable & 0x01;
    if( mode >= 0 && mode <= 6 ) {
        if (enable) {
            reg_val |= (1 << mode);
        } else {
            reg_val &= ~(1 << mode);
        }
    } else if( mode == ALL ) {
        if (enable) {
            reg_val = 0x7F;
        } else {
            reg_val = 0x00;
        }
    }
        
    /* Write value back to ENABLE register */
    if( !wireWriteDataByte(APDS9960_ENABLE, reg_val) ) {
		printf("reg_val22 error\r\n");
        return FALSE;
    }
        
    return TRUE;
}

/**
 * @brief Starts the light (R/G/B/Ambient) sensor on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware interrupt on high or low light
 * @return True if sensor enabled correctly. False on error.
 */
bool enableLightSensor(bool interrupts)
{
    
    /* Set default gain, interrupts, enable power, and enable sensor */
    if( !setAmbientLightGain(DEFAULT_AGAIN) ) {
        return FALSE;
    }
    if( interrupts ) {
        if( !setAmbientLightIntEnable(1) ) {
            return FALSE;
        }
    } else {
        if( !setAmbientLightIntEnable(0) ) {
            return FALSE;
        }
    }
    if( !enablePower() ){
        return FALSE;
    }
    if( !setMode(AMBIENT_LIGHT, 1) ) {
        return FALSE;
    }
    
    return TRUE;

}

/**
 * @brief Ends the light sensor on the APDS-9960
 *
 * @return True if sensor disabled correctly. False on error.
 */
bool disableLightSensor()
{
    if( !setAmbientLightIntEnable(0) ) {
        return FALSE;
    }
    if( !setMode(AMBIENT_LIGHT, 0) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Starts the proximity sensor on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware external interrupt on proximity
 * @return True if sensor enabled correctly. False on error.
 */
bool enableProximitySensor(bool interrupts)
{
    /* Set default gain, LED, interrupts, enable power, and enable sensor */
    if( !setProximityGain(DEFAULT_PGAIN) ) {
        return FALSE;
    }
    if( !setLEDDrive(DEFAULT_LDRIVE) ) {
        return FALSE;
    }
    if( interrupts ) {
        if( !setProximityIntEnable(1) ) {
            return FALSE;
        }
    } else {
        if( !setProximityIntEnable(0) ) {
            return FALSE;
        }
    }
    if( !enablePower() ){
        return FALSE;
    }
    if( !setMode(PROXIMITY, 1) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Ends the proximity sensor on the APDS-9960
 *
 * @return True if sensor disabled correctly. False on error.
 */
bool disableProximitySensor()
{
	if( !setProximityIntEnable(0) ) {
		return FALSE;
	}
	if( !setMode(PROXIMITY, 0) ) {
		return FALSE;
	}

	return TRUE;
}

/**
 * @brief Starts the gesture recognition engine on the APDS-9960
 *
 * @param[in] interrupts true to enable hardware external interrupt on gesture
 * @return True if engine enabled correctly. False on error.
 */
bool enableGestureSensor(bool interrupts)
{
    
    /* Enable gesture mode
       Set ENABLE to 0 (power off)
       Set WTIME to 0xFF
       Set AUX to LED_BOOST_300
       Enable PON, WEN, PEN, GEN in ENABLE 
    */
    resetGestureParameters();
    if( !wireWriteDataByte(APDS9960_WTIME, 0xFF) ) {
        return FALSE;
    }
    if( !wireWriteDataByte(APDS9960_PPULSE, DEFAULT_GESTURE_PPULSE) ) {
        return FALSE;
    }
    if( !setLEDBoost(LED_BOOST_300) ) {
        return FALSE;
    }
    if( interrupts ) {
        if( !setGestureIntEnable(1) ) {
            return FALSE;
        }
    } else {
        if( !setGestureIntEnable(0) ) {
            return FALSE;
        }
    }
    if( !setGestureMode(1) ) {
        return FALSE;
    }
    if( !enablePower() ){
        return FALSE;
    }
    if( !setMode(WAIT, 1) ) {
        return FALSE;
    }
    if( !setMode(PROXIMITY, 1) ) {
        return FALSE;
    }
    if( !setMode(GESTURE, 1) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Ends the gesture recognition engine on the APDS-9960
 *
 * @return True if engine disabled correctly. False on error.
 */
bool disableGestureSensor()
{
    resetGestureParameters();
    if( !setGestureIntEnable(0) ) {
        return FALSE;
    }
    if( !setGestureMode(0) ) {
        return FALSE;
    }
    if( !setMode(GESTURE, 0) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Determines if there is a gesture available for reading
 *
 * @return True if gesture available. False otherwise.
 */
bool isGestureAvailable()
{
    uint8_t val;
    
    /* Read value from GSTATUS register */
    if( !wireReadDataByte(APDS9960_GSTATUS, &val) ) {
        return FALSE;
    }
    
    /* Shift and mask out GVALID bit */
    val &= APDS9960_GVALID;
    
    /* Return true/false based on GVALID bit */
    if( val == 1) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/**
 * @brief Processes a gesture event and returns best guessed gesture
 *
 * @return Number corresponding to gesture. -1 on error.
 */
uint8_t gstatus;
uint8_t fifo_level = 0;

int readGesture()
{
    
    uint8_t bytes_read = 0;
    uint8_t fifo_data[128];
    
    int motion;
    int i;
    
    /* Make sure that power and gesture is on and data is valid */
    if( !isGestureAvailable() || !(getMode() & 0x41) ) {
        return DIR_NONE;
    }
    
    /* Keep looping as long as gesture data is valid */
    while(1) {
    
        /* Wait some time to collect next batch of FIFO data */
        delay(30);
        
        /* Get the contents of the STATUS register. Is data still valid? */
        if( !wireReadDataByte(APDS9960_GSTATUS, &gstatus) ) {
            return ERROR;
        }
        
        /* If we have valid data, read in FIFO */
        if( (gstatus & (1<<0)) == APDS9960_GVALID ) {
        	//Uart_String_CRLF("we have valid data");
            /* Read the current FIFO level */
            if( !wireReadDataByte(APDS9960_GFLVL, &fifo_level) ) {
                return ERROR;
            }

#if DEBUG
            printf("FIFO Level: %d\r\n",fifo_level);
#endif

            /* If there's stuff in the FIFO, read it into our data block */
            if( fifo_level > 0) {
				
                bytes_read = wireReadDataBlock(  APDS9960_GFIFO_U, 
                                                fifo_data, 
                                                (fifo_level * 4) );
                if( bytes_read == FALSE ) {
                    return ERROR;
                }
#if DEBUG
				printf("FIFO len: %d",fifo_level * 4);
                printf("FIFO Dump: ");

                for ( i = 0; i < bytes_read; i++ ) {
                    printf("%d",fifo_data[i]);
                    printf(" ");
                }
                printf("\r\n");
#endif

                /* If at least 1 set of data, sort the data into U/D/L/R */
                if( bytes_read >= 4 ) {
                    for( i = 0; i < bytes_read; i += 4 ) {
                        gesture_data_.u_data[gesture_data_.index] = \
                                                            fifo_data[i + 0];
                        gesture_data_.d_data[gesture_data_.index] = \
                                                            fifo_data[i + 1];
                        gesture_data_.l_data[gesture_data_.index] = \
                                                            fifo_data[i + 2];
                        gesture_data_.r_data[gesture_data_.index] = \
                                                            fifo_data[i + 3];
                        gesture_data_.index++;
                        gesture_data_.total_gestures++;
                    }
                    
#if DEBUG
                printf("Up Data: ");
                for ( i = 0; i < gesture_data_.total_gestures; i++ ) {
                    printf("gesture_data_.u_data[%d] = %d",i,gesture_data_.u_data[i]);
                    printf(" ");
                }
                printf("\r\n");
#endif

                    /* Filter and process gesture data. Decode near/far state */
                    if( processGestureData() ) {
                        if( decodeGesture() ) {
                            //***TODO: U-Turn Gestures
#if DEBUG
                            printf("gesture_motion_%d\r\n",gesture_motion_);
#endif
                        }
                    }
                    
                    /* Reset data */
                    gesture_data_.index = 0;
                    gesture_data_.total_gestures = 0;
                }
            }
        } else {
    		//Uart_String_CRLF(" read it into our data block");
            /* Determine best guessed gesture and clean up */
            delay(30);
            decodeGesture();
            motion = gesture_motion_;
#if DEBUG
            printf("END: ");
            printf("gesture_motion_%d\r\n",gesture_motion_);
#endif
            resetGestureParameters();

            return motion;
        }
    }
}

/**
 * Turn the APDS-9960 on
 *
 * @return True if operation successful. False otherwise.
 */
bool enablePower()
{
    if( !setMode(POWER, 1) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * Turn the APDS-9960 off
 *
 * @return True if operation successful. False otherwise.
 */
bool disablePower()
{
    if( !setMode(POWER, 0) ) {
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************************
 * Ambient light and color sensor controls
 ******************************************************************************/

/**
 * @brief Reads the ambient (clear) light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool readAmbientLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !wireReadDataByte(APDS9960_CDATAL, &val_byte) ) {
        return FALSE;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !wireReadDataByte(APDS9960_CDATAH, &val_byte) ) {
        return FALSE;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return TRUE;
}

/**
 * @brief Reads the red light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool readRedLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !wireReadDataByte(APDS9960_RDATAL, &val_byte) ) {
        return FALSE;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !wireReadDataByte(APDS9960_RDATAH, &val_byte) ) {
        return FALSE;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return TRUE;
}
 
/**
 * @brief Reads the green light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool readGreenLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !wireReadDataByte(APDS9960_GDATAL, &val_byte) ) {
        return FALSE;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !wireReadDataByte(APDS9960_GDATAH, &val_byte) ) {
        return FALSE;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return TRUE;
}

/**
 * @brief Reads the red light level as a 16-bit value
 *
 * @param[out] val value of the light sensor.
 * @return True if operation successful. False otherwise.
 */
bool readBlueLight(uint16_t *val)
{
    uint8_t val_byte;
    *val = 0;
    
    /* Read value from clear channel, low byte register */
    if( !wireReadDataByte(APDS9960_BDATAL, &val_byte) ) {
        return FALSE;
    }
    *val = val_byte;
    
    /* Read value from clear channel, high byte register */
    if( !wireReadDataByte(APDS9960_BDATAH, &val_byte) ) {
        return FALSE;
    }
    *val = *val + ((uint16_t)val_byte << 8);
    
    return TRUE;
}

/*******************************************************************************
 * Proximity sensor controls
 ******************************************************************************/

/**
 * @brief Reads the proximity level as an 8-bit value
 *
 * @param[out] val value of the proximity sensor.
 * @return True if operation successful. False otherwise.
 */
bool readProximity(uint8_t *val)
{
    *val = 0;
    
    /* Read value from proximity data register */
    if( !wireReadDataByte(APDS9960_PDATA, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************************
 * High-level gesture controls
 ******************************************************************************/

/**
 * @brief Resets all the parameters in the gesture data member
 */
void resetGestureParameters()
{
    gesture_data_.index = 0;
    gesture_data_.total_gestures = 0;
    
    gesture_ud_delta_ = 0;
    gesture_lr_delta_ = 0;
    
    gesture_ud_count_ = 0;
    gesture_lr_count_ = 0;
    
    gesture_near_count_ = 0;
    gesture_far_count_ = 0;
    
    gesture_state_ = 0;
    gesture_motion_ = DIR_NONE;
}

/**
 * @brief Processes the raw gesture data to determine swipe direction
 *
 * @return True if near or far state seen. False otherwise.
 */
bool processGestureData()
{
    uint8_t u_first = 0;
    uint8_t d_first = 0;
    uint8_t l_first = 0;
    uint8_t r_first = 0;
    uint8_t u_last = 0;
    uint8_t d_last = 0;
    uint8_t l_last = 0;
    uint8_t r_last = 0;
    int ud_ratio_first;
    int lr_ratio_first;
    int ud_ratio_last;
    int lr_ratio_last;
    int ud_delta;
    int lr_delta;
    int i;

    /* If we have less than 4 total gestures, that's not enough */
    if( gesture_data_.total_gestures <= 4 ) {
        return FALSE;
    }
    
    /* Check to make sure our data isn't out of bounds */
    if( (gesture_data_.total_gestures <= 32) && \
        (gesture_data_.total_gestures > 0) ) {
        
        /* Find the first value in U/D/L/R above the threshold */
        for( i = 0; i < gesture_data_.total_gestures; i++ ) {
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_first = gesture_data_.u_data[i];
                d_first = gesture_data_.d_data[i];
                l_first = gesture_data_.l_data[i];
                r_first = gesture_data_.r_data[i];
                break;
            }
        }
        
        /* If one of the _first values is 0, then there is no good data */
        if( (u_first == 0) || (d_first == 0) || \
            (l_first == 0) || (r_first == 0) ) {
            
            return FALSE;
        }
        /* Find the last value in U/D/L/R above the threshold */
        for( i = gesture_data_.total_gestures - 1; i >= 0; i-- ) {
#if DEBUG
            printf("Finding last: ");
            printf("U:");
            printf("%d",gesture_data_.u_data[i]);
            printf(" D:");
            printf("%d",gesture_data_.d_data[i]);
            printf(" L:");
           	printf("%d",gesture_data_.l_data[i]);
           	printf(" R:");
            printf("%d",gesture_data_.r_data[i]);
			printf("\r\n");
#endif
            if( (gesture_data_.u_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.d_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.l_data[i] > GESTURE_THRESHOLD_OUT) &&
                (gesture_data_.r_data[i] > GESTURE_THRESHOLD_OUT) ) {
                
                u_last = gesture_data_.u_data[i];
                d_last = gesture_data_.d_data[i];
                l_last = gesture_data_.l_data[i];
                r_last = gesture_data_.r_data[i];
                break;
            }
        }
    }
    
    /* Calculate the first vs. last ratio of up/down and left/right */
    ud_ratio_first = ((u_first - d_first) * 100) / (u_first + d_first);
    lr_ratio_first = ((l_first - r_first) * 100) / (l_first + r_first);
    ud_ratio_last = ((u_last - d_last) * 100) / (u_last + d_last);
    lr_ratio_last = ((l_last - r_last) * 100) / (l_last + r_last);
       
#if DEBUG
    printf("Last Values: ");
    printf("U:");
    printf("%d",u_last);
    printf(" D:");
    printf("%d",d_last);
    printf(" L:");
    printf("%d",l_last);
    printf(" R:");
    printf("%d",r_last);
	printf("\r\n");

    printf("Ratios: ");
    printf("UD Fi: ");
    printf("%d",ud_ratio_first);
    printf(" UD La: ");
    printf("%d",ud_ratio_last);
    printf(" LR Fi: ");
    printf("%d",lr_ratio_first);
    printf(" LR La: ");
    printf("%d",lr_ratio_last);
	printf("\r\n");
#endif
       
    /* Determine the difference between the first and last ratios */
    ud_delta = ud_ratio_last - ud_ratio_first;
    lr_delta = lr_ratio_last - lr_ratio_first;
    
#if DEBUG
    printf("Deltas: ");
    printf("UD: %d",ud_delta);
    //Serial.print(ud_delta);
    printf(" LR: %d\r\n",lr_delta);
    //Serial.println(lr_delta);
#endif

    /* Accumulate the UD and LR delta values */
    gesture_ud_delta_ += ud_delta;
    gesture_lr_delta_ += lr_delta;
    
#if DEBUG
    printf("Accumulations: ");
    printf("UD: %d",gesture_ud_delta_);
    //Serial.print(gesture_ud_delta_);
    printf(" LR: %d\r\n",gesture_lr_delta_);
    //Serial.println(gesture_lr_delta_);
#endif
    
    /* Determine U/D gesture */
    if( gesture_ud_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = 1;
    } else if( gesture_ud_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_ud_count_ = -1;
    } else {
        gesture_ud_count_ = 0;
    }
    
    /* Determine L/R gesture */
    if( gesture_lr_delta_ >= GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = 1;
    } else if( gesture_lr_delta_ <= -GESTURE_SENSITIVITY_1 ) {
        gesture_lr_count_ = -1;
    } else {
        gesture_lr_count_ = 0;
    }
    
    /* Determine Near/Far gesture */
    if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 0) ) {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
            
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            } else if( (ud_delta != 0) || (lr_delta != 0) ) {
                gesture_far_count_++;
            }
            
            if( (gesture_near_count_ >= 10) && (gesture_far_count_ >= 2) ) {
                if( (ud_delta == 0) && (lr_delta == 0) ) {
                    gesture_state_ = NEAR_STATE;
                } else if( (ud_delta != 0) && (lr_delta != 0) ) {
                    gesture_state_ = FAR_STATE;
                }
                return TRUE;
            }
        }
    } else {
        if( (abs(ud_delta) < GESTURE_SENSITIVITY_2) && \
            (abs(lr_delta) < GESTURE_SENSITIVITY_2) ) {
                
            if( (ud_delta == 0) && (lr_delta == 0) ) {
                gesture_near_count_++;
            }
            
            if( gesture_near_count_ >= 10 ) {
                gesture_ud_count_ = 0;
                gesture_lr_count_ = 0;
                gesture_ud_delta_ = 0;
                gesture_lr_delta_ = 0;
            }
        }
    }
    
#if DEBUG
    printf("UD_CT: %d",gesture_ud_count_);
    //Serial.print(gesture_ud_count_);
    printf(" LR_CT: %d",gesture_lr_count_);
    //Serial.print(gesture_lr_count_);
    printf(" NEAR_CT: ");
    //Serial.print(gesture_near_count_);
    printf(" FAR_CT: ");
    //Serial.println(gesture_far_count_);
    printf("----------");
	printf("\r\n");
#endif
    
    return FALSE;
}

/**
 * @brief Determines swipe direction or near/far state
 *
 * @return True if near/far event. False otherwise.
 */
bool decodeGesture()
{
    /* Return if near or far event is detected */
    if( gesture_state_ == NEAR_STATE ) {
        gesture_motion_ = DIR_NEAR;
        return TRUE;
    } else if ( gesture_state_ == FAR_STATE ) {
        gesture_motion_ = DIR_FAR;
        return TRUE;
    }
    
    /* Determine swipe direction */
    if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_UP;
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 0) ) {
        gesture_motion_ = DIR_DOWN;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == 1) ) {
        gesture_motion_ = DIR_RIGHT;
    } else if( (gesture_ud_count_ == 0) && (gesture_lr_count_ == -1) ) {
        gesture_motion_ = DIR_LEFT;
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == -1) && (gesture_lr_count_ == -1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_UP;
        } else {
            gesture_motion_ = DIR_LEFT;
        }
    } else if( (gesture_ud_count_ == 1) && (gesture_lr_count_ == 1) ) {
        if( abs(gesture_ud_delta_) > abs(gesture_lr_delta_) ) {
            gesture_motion_ = DIR_DOWN;
        } else {
            gesture_motion_ = DIR_RIGHT;
        }
    } else {
        return FALSE;
    }
    
    return TRUE;
}

/*******************************************************************************
 * Getters and setters for register values
 ******************************************************************************/

/**
 * @brief Returns the lower threshold for proximity detection
 *
 * @return lower threshold
 */
uint8_t getProxIntLowThresh()
{
    uint8_t val;
    
    /* Read value from PILT register */
    if( !wireReadDataByte(APDS9960_PILT, &val) ) {
        val = 0;
    }
    
    return val;
}

/**
 * @brief Sets the lower threshold for proximity detection
 *
 * @param[in] threshold the lower proximity threshold
 * @return True if operation successful. False otherwise.
 */
bool setProxIntLowThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_PILT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Returns the high threshold for proximity detection
 *
 * @return high threshold
 */
uint8_t getProxIntHighThresh()
{
    uint8_t val;
    
    /* Read value from PIHT register */
    if( !wireReadDataByte(APDS9960_PIHT, &val) ) {
        val = 0;
    }
    
    return val;
}

/**
 * @brief Sets the high threshold for proximity detection
 *
 * @param[in] threshold the high proximity threshold
 * @return True if operation successful. False otherwise.
 */
bool setProxIntHighThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_PIHT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Returns LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the value of the LED drive strength. 0xFF on failure.
 */
uint8_t getLEDDrive()
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out LED drive bits */
    val = (val >> 6) & 0x03;
    
    return val;
}

/**
 * @brief Sets the LED drive strength for proximity and ALS
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value (0-3) for the LED drive strength
 * @return True if operation successful. False otherwise.
 */
bool setLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 6;
    val &= 0x3F;
    val |= drive;
    
    /* Write register value back into CONTROL register */
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Returns receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the value of the proximity gain. 0xFF on failure.
 */
uint8_t getProximityGain()
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out PDRIVE bits */
    val = (val >> 2) & 0x03;
    
    return val;
}

/**
 * @brief Sets the receiver gain for proximity detection
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] drive the value (0-3) for the gain
 * @return True if operation successful. False otherwise.
 */
bool setProximityGain(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 2;
    val &= 0xF3;
    val |= drive;
    
    /* Write register value back into CONTROL register */
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Returns receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @return the value of the ALS gain. 0xFF on failure.
 */
uint8_t getAmbientLightGain()
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out ADRIVE bits */
    val &= 0x03;
    
    return val;
}

/**
 * @brief Sets the receiver gain for the ambient light sensor (ALS)
 *
 * Value    Gain
 *   0        1x
 *   1        4x
 *   2       16x
 *   3       64x
 *
 * @param[in] drive the value (0-3) for the gain
 * @return True if operation successful. False otherwise.
 */
bool setAmbientLightGain(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from CONTROL register */
    if( !wireReadDataByte(APDS9960_CONTROL, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    val &= 0xFC;
    val |= drive;
    
    /* Write register value back into CONTROL register */
    if( !wireWriteDataByte(APDS9960_CONTROL, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Get the current LED boost value
 * 
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @return The LED boost value. 0xFF on failure.
 */
uint8_t getLEDBoost()
{
    uint8_t val;
    
    /* Read value from CONFIG2 register */
    if( !wireReadDataByte(APDS9960_CONFIG2, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out LED_BOOST bits */
    val = (val >> 4) & 0x03;
    
    return val;
}

/**
 * @brief Sets the LED current boost value
 *
 * Value  Boost Current
 *   0        100%
 *   1        150%
 *   2        200%
 *   3        300%
 *
 * @param[in] drive the value (0-3) for current boost (100-300%)
 * @return True if operation successful. False otherwise.
 */
bool setLEDBoost(uint8_t boost)
{
    uint8_t val;
    
    /* Read value from CONFIG2 register */
    if( !wireReadDataByte(APDS9960_CONFIG2, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    boost &= 0x03;
    boost = boost << 4;
    val &= 0xCF;
    val |= boost;
    
    /* Write register value back into CONFIG2 register */
    if( !wireWriteDataByte(APDS9960_CONFIG2, val) ) {
        return FALSE;
    }
    
    return TRUE;
}    
   
/**
 * @brief Gets proximity gain compensation enable
 *
 * @return 1 if compensation is enabled. 0 if not. 0xFF on error.
 */
uint8_t getProxGainCompEnable()
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !wireReadDataByte(APDS9960_CONFIG3, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out PCMP bits */
    val = (val >> 5) & 0x01;
    
    return val;
}

/**
 * @brief Sets the proximity gain compensation enable
 *
 * @param[in] enable 1 to enable compensation. 0 to disable compensation.
 * @return True if operation successful. False otherwise.
 */
 bool setProxGainCompEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !wireReadDataByte(APDS9960_CONFIG3, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable = enable << 5;
    val &= 0xDF;
    val |= enable;
    
    /* Write register value back into CONFIG3 register */
    if( !wireWriteDataByte(APDS9960_CONFIG3, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the current mask for enabled/disabled proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @return Current proximity mask for photodiodes. 0xFF on error.
 */
uint8_t getProxPhotoMask()
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !wireReadDataByte(APDS9960_CONFIG3, &val) ) {
        return ERROR;
    }
    
    /* Mask out photodiode enable mask bits */
    val &= 0x0F;
    
    return val;
}

/**
 * @brief Sets the mask for enabling/disabling proximity photodiodes
 *
 * 1 = disabled, 0 = enabled
 * Bit    Photodiode
 *  3       UP
 *  2       DOWN
 *  1       LEFT
 *  0       RIGHT
 *
 * @param[in] mask 4-bit mask value
 * @return True if operation successful. False otherwise.
 */
bool setProxPhotoMask(uint8_t mask)
{
    uint8_t val;
    
    /* Read value from CONFIG3 register */
    if( !wireReadDataByte(APDS9960_CONFIG3, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    mask &= 0x0F;
    val &= 0xF0;
    val |= mask;
    
    /* Write register value back into CONFIG3 register */
    if( !wireWriteDataByte(APDS9960_CONFIG3, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the entry proximity threshold for gesture sensing
 *
 * @return Current entry proximity threshold.
 */
uint8_t getGestureEnterThresh()
{
    uint8_t val;
    
    /* Read value from GPENTH register */
    if( !wireReadDataByte(APDS9960_GPENTH, &val) ) {
        val = 0;
    }
    
    return val;
}

/**
 * @brief Sets the entry proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to start gesture mode
 * @return True if operation successful. False otherwise.
 */
bool setGestureEnterThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_GPENTH, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the exit proximity threshold for gesture sensing
 *
 * @return Current exit proximity threshold.
 */
uint8_t getGestureExitThresh()
{
    uint8_t val;
    
    /* Read value from GEXTH register */
    if( !wireReadDataByte(APDS9960_GEXTH, &val) ) {
        val = 0;
    }
    
    return val;
}

/**
 * @brief Sets the exit proximity threshold for gesture sensing
 *
 * @param[in] threshold proximity value needed to end gesture mode
 * @return True if operation successful. False otherwise.
 */
bool setGestureExitThresh(uint8_t threshold)
{
    if( !wireWriteDataByte(APDS9960_GEXTH, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @return the current photodiode gain. 0xFF on error.
 */
uint8_t getGestureGain()
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out GGAIN bits */
    val = (val >> 5) & 0x03;
    
    return val;
}

/**
 * @brief Sets the gain of the photodiode during gesture mode
 *
 * Value    Gain
 *   0       1x
 *   1       2x
 *   2       4x
 *   3       8x
 *
 * @param[in] gain the value for the photodiode gain
 * @return True if operation successful. False otherwise.
 */
bool setGestureGain(uint8_t gain)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    gain &= 0x03;
    gain = gain << 5;
    val &= 0x9F;
    val |= gain;
    
    /* Write register value back into GCONF2 register */
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the drive current of the LED during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @return the LED drive current value. 0xFF on error.
 */
uint8_t getGestureLEDDrive()
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out GLDRIVE bits */
    val = (val >> 3) & 0x03;
    
    return val;
}

/**
 * @brief Sets the LED drive current during gesture mode
 *
 * Value    LED Current
 *   0        100 mA
 *   1         50 mA
 *   2         25 mA
 *   3         12.5 mA
 *
 * @param[in] drive the value for the LED drive current
 * @return True if operation successful. False otherwise.
 */
bool setGestureLEDDrive(uint8_t drive)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    drive &= 0x03;
    drive = drive << 3;
    val &= 0xE7;
    val |= drive;
    
    /* Write register value back into GCONF2 register */
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @return the current wait time between gestures. 0xFF on error.
 */
uint8_t getGestureWaitTime()
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return ERROR;
    }
    
    /* Mask out GWTIME bits */
    val &= 0x07;
    
    return val;
}

/**
 * @brief Sets the time in low power mode between gesture detections
 *
 * Value    Wait time
 *   0          0 ms
 *   1          2.8 ms
 *   2          5.6 ms
 *   3          8.4 ms
 *   4         14.0 ms
 *   5         22.4 ms
 *   6         30.8 ms
 *   7         39.2 ms
 *
 * @param[in] the value for the wait time
 * @return True if operation successful. False otherwise.
 */
bool setGestureWaitTime(uint8_t time)
{
    uint8_t val;
    
    /* Read value from GCONF2 register */
    if( !wireReadDataByte(APDS9960_GCONF2, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    time &= 0x07;
    val &= 0xF8;
    val |= time;
    
    /* Write register value back into GCONF2 register */
    if( !wireWriteDataByte(APDS9960_GCONF2, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the low threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool getLightIntLowThreshold(uint16_t *threshold)
{
    uint8_t val_byte;
    *threshold = 0;
    
    /* Read value from ambient light low threshold, low byte register */
    if( !wireReadDataByte(APDS9960_AILTL, &val_byte) ) {
        return FALSE;
    }
    *threshold = val_byte;
    
    /* Read value from ambient light low threshold, high byte register */
    if( !wireReadDataByte(APDS9960_AILTH, &val_byte) ) {
        return FALSE;
    }
    *threshold = *threshold + ((uint16_t)val_byte << 8);
    
    return TRUE;
}

/**
 * @brief Sets the low threshold for ambient light interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool setLightIntLowThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    /* Break 16-bit threshold into 2 8-bit values */
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    /* Write low byte */
    if( !wireWriteDataByte(APDS9960_AILTL, val_low) ) {
        return FALSE;
    }
    
    /* Write high byte */
    if( !wireWriteDataByte(APDS9960_AILTH, val_high) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the high threshold for ambient light interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool getLightIntHighThreshold(uint16_t *threshold)
{
    uint8_t val_byte;
    *threshold = 0;
    
    /* Read value from ambient light high threshold, low byte register */
    if( !wireReadDataByte(APDS9960_AIHTL, &val_byte) ) {
        return FALSE;
    }
    *threshold = val_byte;
    
    /* Read value from ambient light high threshold, high byte register */
    if( !wireReadDataByte(APDS9960_AIHTH, &val_byte) ) {
        return FALSE;
    }
    *threshold = *threshold + ((uint16_t)val_byte << 8);
    
    return TRUE;
}

/**
 * @brief Sets the high threshold for ambient light interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool setLightIntHighThreshold(uint16_t threshold)
{
    uint8_t val_low;
    uint8_t val_high;
    
    /* Break 16-bit threshold into 2 8-bit values */
    val_low = threshold & 0x00FF;
    val_high = (threshold & 0xFF00) >> 8;
    
    /* Write low byte */
    if( !wireWriteDataByte(APDS9960_AIHTL, val_low) ) {
        return FALSE;
    }
    
    /* Write high byte */
    if( !wireWriteDataByte(APDS9960_AIHTH, val_high) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets the low threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool getProximityIntLowThreshold(uint8_t *threshold)
{
    *threshold = 0;
    
    /* Read value from proximity low threshold register */
    if( !wireReadDataByte(APDS9960_PILT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Sets the low threshold for proximity interrupts
 *
 * @param[in] threshold low threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool setProximityIntLowThreshold(uint8_t threshold)
{
    
    /* Write threshold value to register */
    if( !wireWriteDataByte(APDS9960_PILT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}
    
/**
 * @brief Gets the high threshold for proximity interrupts
 *
 * @param[out] threshold current low threshold stored on the APDS-9960
 * @return True if operation successful. False otherwise.
 */
bool getProximityIntHighThreshold(uint8_t *threshold)
{
    *threshold = 0;
    
    /* Read value from proximity low threshold register */
    if( !wireReadDataByte(APDS9960_PIHT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Sets the high threshold for proximity interrupts
 *
 * @param[in] threshold high threshold value for interrupt to trigger
 * @return True if operation successful. False otherwise.
 */
bool setProximityIntHighThreshold(uint8_t threshold)
{
    
    /* Write threshold value to register */
    if( !wireWriteDataByte(APDS9960_PIHT, threshold) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets if ambient light interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getAmbientLightIntEnable()
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !wireReadDataByte(APDS9960_ENABLE, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out AIEN bit */
    val = (val >> 4) & 0x01;
    
    return val;
}

/**
 * @brief Turns ambient light interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool setAmbientLightIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !wireReadDataByte(APDS9960_ENABLE, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable = enable << 4;
    val &= 0xEF;
    val |= enable;
    
    /* Write register value back into ENABLE register */
    if( !wireWriteDataByte(APDS9960_ENABLE, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets if proximity interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getProximityIntEnable()
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !wireReadDataByte(APDS9960_ENABLE, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out PIEN bit */
    val = (val >> 5) & 0x01;
    
    return val;
}

/**
 * @brief Turns proximity interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool setProximityIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from ENABLE register */
    if( !wireReadDataByte(APDS9960_ENABLE, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable = enable << 5;
    val &= 0xDF;
    val |= enable;
    
    /* Write register value back into ENABLE register */
    if( !wireWriteDataByte(APDS9960_ENABLE, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Gets if gesture interrupts are enabled or not
 *
 * @return 1 if interrupts are enabled, 0 if not. 0xFF on error.
 */
uint8_t getGestureIntEnable()
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !wireReadDataByte(APDS9960_GCONF4, &val) ) {
        return ERROR;
    }
    
    /* Shift and mask out GIEN bit */
    val = (val >> 1) & 0x01;
    
    return val;
}

/**
 * @brief Turns gesture-related interrupts on or off
 *
 * @param[in] enable 1 to enable interrupts, 0 to turn them off
 * @return True if operation successful. False otherwise.
 */
bool setGestureIntEnable(uint8_t enable)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !wireReadDataByte(APDS9960_GCONF4, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    enable &= 0x01;
    enable = enable << 1;
    val &= 0xFD;
    val |= enable;
    
    /* Write register value back into GCONF4 register */
    if( !wireWriteDataByte(APDS9960_GCONF4, val) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Clears the ambient light interrupt
 *
 * @return True if operation completed successfully. False otherwise.
 */
bool clearAmbientLightInt()
{
    uint8_t throwaway;
    if( !wireReadDataByte(APDS9960_AICLEAR, &throwaway) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Clears the proximity interrupt
 *
 * @return True if operation completed successfully. False otherwise.
 */
bool clearProximityInt()
{
    uint8_t throwaway;
    if( !wireReadDataByte(APDS9960_PICLEAR, &throwaway) ) {
        return FALSE;
    }
    
    return TRUE;
}

/**
 * @brief Tells if the gesture state machine is currently running
 *
 * @return 1 if gesture state machine is running, 0 if not. 0xFF on error.
 */
uint8_t getGestureMode()
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !wireReadDataByte(APDS9960_GCONF4, &val) ) {
        return ERROR;
    }
    
    /* Mask out GMODE bit */
    val &= 0X01;
    
    return val;
}

/**
 * @brief Tells the state machine to either enter or exit gesture state machine
 *
 * @param[in] mode 1 to enter gesture state machine, 0 to exit.
 * @return True if operation successful. False otherwise.
 */
bool setGestureMode(uint8_t mode)
{
    uint8_t val;
    
    /* Read value from GCONF4 register */
    if( !wireReadDataByte(APDS9960_GCONF4, &val) ) {
        return FALSE;
    }
    
    /* Set bits in register to given value */
    mode &= 0x01;
    val &= 0xFE;
    val |= mode;
    
    /* Write register value back into GCONF4 register */
    if( !wireWriteDataByte(APDS9960_GCONF4, val) ) {
        return FALSE;
    }
    
    return TRUE;
}



