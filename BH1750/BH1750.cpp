#include "BH1750.h"

void BH1750_Write_Bytes(u8 Register_address, u8 Device_address)
{
	/* While the bus is busy */
  while (I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/* Generate Start */
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	
	/* Check if started */
	while (!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send 7bit address with transmitter bit */
	I2C_Send7bitAddress(BH1750_I2C, Device_address, I2C_Direction_Transmitter); 
	
	/* Check if sent 7 bit address */
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
	
	/* Send Register address to write to */
	I2C_SendData(BH1750_I2C, Register_address);
	
	/* Check Sent */
	while (!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	
	/* Generate Stop */
	I2C_GenerateSTOP(BH1750_I2C, ENABLE);
}

uint16_t BH1750_Read_data(u8 Device_address)
{
	uint8_t data1,data2;
	/* While the bus is busy */
  while (I2C_GetFlagStatus(BH1750_I2C, I2C_FLAG_BUSY));
	
	/* Generate Start */
	I2C_GenerateSTART(BH1750_I2C, ENABLE);
	
	/* Check if started */
	while (!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_MODE_SELECT));
	
	/* Send 7 bit address with Receiver bit */
	I2C_Send7bitAddress(BH1750_I2C, Device_address, I2C_Direction_Receiver);
	
	/* Check if sent 7 bit address */
	while(!I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
	
	I2C_AcknowledgeConfig(BH1750_I2C, ENABLE);
	
	while (!(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)));
		/* Read a byte from the MPU9250 */
		data1 = I2C_ReceiveData(BH1750_I2C);
		
		/****** Mode Read NACK ******/
		/* Disable Acknowledgement */
		I2C_AcknowledgeConfig(BH1750_I2C, DISABLE);
		/* Send STOP Condition */
		I2C_GenerateSTOP(BH1750_I2C, ENABLE);
		/***************************/
		
		while (!(I2C_CheckEvent(BH1750_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED)));
		
			/* Read a byte from the MPU9250 */
			data2 = I2C_ReceiveData(BH1750_I2C);
		
		//I2C_GenerateSTOP(BH1750_I2C, ENABLE);
		
	return  ((data1 << 8) |data2) / 1.2;
}
