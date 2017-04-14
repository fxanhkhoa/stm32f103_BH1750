#include "stm32f10x.h"
#include "stm32f10x_i2c.h"
#include "stm32f10x_gpio.h"
#include "usart_print.h"	
#include "stm32f10x_tim.h"
#include "Main.h"
#include <math.h>
#include "BH1750.h"


void sleep(long i);
void led_toggle(void);
float Distance(float x, float y);

uint32_t timer, time_pre =0, time_now = 0;


	
int main()
{
	//prvSetupHardware();
	/* Enable I2C and GPIO clocks */
    RCC_APB1PeriphClockCmd(BH1750_I2C_RCC_Periph, ENABLE);
    RCC_APB2PeriphClockCmd(BH1750_I2C_RCC_Port, ENABLE);
/***********************************************************************************************
*											GPIO_Init						
***********************************************************************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	/* Set Led */
		GPIO_InitTypeDef GPIO_InitStruct,GPIO_InitStructure,GPIO;
		GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	  GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);

/*************************************************************************************
*																	USART_Init
**************************************************************************************/
			
	USART_InitTypeDef USART;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	USART.USART_BaudRate = 38400;
	USART.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART.USART_StopBits = USART_StopBits_1;
	USART.USART_WordLength = USART_WordLength_8b;
	USART.USART_Parity = USART_Parity_No;
	USART.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	
	/*---- Configure USART1 ----*/
			USART_Init(USART1, &USART);
	/*---- Enable RXNE interrupt ----*/
			USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/*---- USART ENABLE ----*/
			USART_Cmd(USART1, ENABLE);
		/*------ TX-Pin PA9 & RX-Pin PA10 -----*/
			
			GPIO.GPIO_Pin = GPIO_Pin_9;
			GPIO.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, & GPIO);
			
			GPIO.GPIO_Pin = GPIO_Pin_10;
			GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO);
			
/************************************************************************************************
*															I2C_Init
************************************************************************************************/
		I2C_InitTypeDef I2C_InitStructure;
    //GPIO_InitTypeDef GPIO_InitStructure;

    /* Configure I2C pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin = BH1750_SCL | BH1750_SDA;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(BH1750_I2C_PORT, &GPIO_InitStructure);

    /* I2C configuration */
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = BH1750_I2C_Speed;

    /* Apply I2C configuration after enabling it */
    I2C_Init(BH1750_I2C, &I2C_InitStructure);
    /* I2C Peripheral Enable */
    I2C_Cmd(BH1750_I2C, ENABLE);
/**************************************************************************************************
*											Timer
**************************************************************************************************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

		TIM_TimeBaseInitTypeDef timerInitStructure; 
		timerInitStructure.TIM_Prescaler = 36000;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = 2-1;
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &timerInitStructure);
		TIM_Cmd(TIM4, ENABLE);
		TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
		
		NVIC_InitTypeDef NVIC_InitStructure;
		NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
		/*----- NVIC Timer interrupt -----*/
			NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);
			
/**************************************************************************************************
*											Check_Connection
**************************************************************************************************/
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
	uint16_t data;
	
	BH1750_Write_Bytes(BH1750_POWER_ON, (BH1750_ADDRESS << 1)); // set power on
	BH1750_Write_Bytes(BH1750_RESET, (BH1750_ADDRESS << 1));// reset BH1750
	BH1750_Write_Bytes(BH1750_CONTINUOUS_HIGH_RES_MODE, (BH1750_ADDRESS << 1)); // set mode 1 continous
	
	timer = 0;
	while (timer < 100);
	while (1)
	{
		
		data = BH1750_Read_data(BH1750_ADDRESS << 1);
		
		//GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		timer = 0;
		U_Print(USART1, data);
		U_Print_Char(USART1,"\n");
		while (timer < 100);
		led_toggle();
	}
}

void sleep(long i)
{
	for (long k = 0 ; k < i; k++);
}

extern "C" void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
				timer ++;
				time_now++;
    }
}

void led_toggle(void)
		{
				/* Read LED output (GPIOA PIN8) status */
				uint8_t led_bit = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
			 
				/* If LED output set, clear it */
				if(led_bit == (uint8_t)Bit_SET)
				{
						GPIO_ResetBits(GPIOC, GPIO_Pin_13);
				}
				/* If LED output clear, set it */
				else
				{
						GPIO_SetBits(GPIOC, GPIO_Pin_13);
				}
		}
		
float Distance(float x, float y)
{
	return sqrt(x*x + y*y);
}


