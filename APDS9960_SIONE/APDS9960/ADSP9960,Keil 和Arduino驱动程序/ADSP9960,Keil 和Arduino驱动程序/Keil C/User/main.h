/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    22-November-2013
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#define debug

#define bool     unsigned char
#define uchar    unsigned char
#define uint     unsigned int
#define true     1
#define false    0

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

void i2c_LowLevel_Init(void);
void i2c_DeInit(void);
void i2c_Init(void);

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "uart.h"
#include "ADSP9960_platform.h"
#include "SparkFun_ADSP9960.h"

#ifdef USE_STM320518_EVAL
 #include "stm320518_eval.h"
 #include "stm320518_eval_lcd.h"
#endif /* USE_STM320518_EVAL */


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
