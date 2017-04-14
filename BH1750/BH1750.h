#ifndef _BH1750_H_
#define _BH1750_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
	
#define BH1750_I2C	I2C2
#define BH1750_SCL	GPIO_Pin_10
#define BH1750_SDA	GPIO_Pin_11
#define BH1750_I2C_PORT		GPIOB
#define BH1750_I2C_RCC_Port         RCC_APB2Periph_GPIOB
#define BH1750_I2C_Speed            100000 // 100kHz standard mode
#define BH1750_I2C_RCC_Periph       RCC_APB1Periph_I2C2
	
	
#define BH1750_POWER_DOWN 	0x00
#define BH1750_POWER_ON 		0x01
#define BH1750_RESET 				0x07
#define BH1750_CONTINUOUS_HIGH_RES_MODE  	 0x10
#define BH1750_CONTINUOUS_HIGH_RES_MODE_2  0x11
#define BH1750_CONTINUOUS_LOW_RES_MODE 		 0x13
#define BH1750_ONE_TIME_HIGH_RES_MODE 		 0x20
#define BH1750_ONE_TIME_HIGH_RES_MODE_2 	 0x21
#define BH1750_ONE_TIME_LOW_RES_MODE 			 0x23

#define BH1750_ADDRESS 0x23
	
void BH1750_Write_Bytes(u8 Register_address, u8 Device_address);
uint16_t BH1750_Read_data(u8 Device_address);
	
#ifdef __cplusplus
}
#endif

#endif /* __BH1750_H */
