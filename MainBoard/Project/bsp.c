#include "stm32f10x.h"
#include <stdio.h>
#include "bsp.h"
//#include "spi.h"
#include "apiLib.h"
// #define LOG_TAG    "main"
// #include <elog.h>
// #include <bsp.h>
// #include <stdio.h>
// #include <stdlib.h>


//#define LOG_TAG    "main"

/*      Public Function      */
void BSP_Init(void);

/*      Private Function     */
void NVIC_Configure(void);
u8 HSI_SystemClock(void);
void HSE_SystemClock(void);
void IWDG_Configure(void);
void Systick_Configure(void);
//void Systick_Config_Init(void);
void TIM2_Configure(void);
void TIM3_Configure(void);
void UART2_Configure(void);
void UART3_Configure(void);
void GpioConfigInit(void);
void GpioPeriphClockConfig(void);
void ADC1_Configure(void);
void SDCard_HardwareConfig(void);
void Key_HardwareConfig(void);
void Led_HardwareConfig(void);
void DigitalTubew_HardwareConfig(void);
void ADC_HardwareConfig(void);




/*-----------------------------------------------------------------------*/
/* BSP��ʼ��                                                             */
/*-----------------------------------------------------------------------*/


void BSP_Init(void)
{
	/*    �жϷ���              */
	NVIC_Configure();
	
//	/*    ѡ���ⲿʱ��Դ        */
//	HSE_SystemClock();
	
	/*    ѡ���ڲ�ʱ��Դ        */
	HSI_SystemClock();
	
	/*    GPIO�˿ڳ�ʼ������    */
	GpioConfigInit();
	
	/*    ʹ�ܿ��Ź�            */
	IWDG_Configure();
	
	/*    ʹ��ϵͳ�δ�          */
	Systick_Configure();
	
	/*    ʹ��TIM2ʱ��          */
	TIM2_Configure();
	
	/*    ʹ��TIM3ʱ��          */
	TIM3_Configure();

	/*    ʹ��ADC1              */
	ADC1_Configure();
	
	UART2_Configure();
	/*    ʹ��UART3�ж�         */
//	UART3_Configure();
	
// 	/*    ʹ��SPI2����          */
// 	SPI2_Configure();
	
	/*    ��ʼ������            */
	api_InitParamsAtPowerOn();
}



/*-----------------------------------------------------------------------*/
/* �����ж����ȼ�����                                                    */
/*-----------------------------------------------------------------------*/


void NVIC_Configure(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);            //  16�������ȼ�    0��Ӧ���ȼ�
}



/*-----------------------------------------------------------------------*/
/* ʱ��ԴΪ�ڲ����پ���                                                  */
/* �ڲ����پ���Ϊ8M                                                      */
/*-----------------------------------------------------------------------*/


u8 HSI_SystemClock(void)
{
	u8 sta;
	RCC_HSICmd(ENABLE);
	sta=RCC_GetFlagStatus(RCC_FLAG_HSIRDY);
	if(sta==RESET)return 0;
	else
	{
		RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_9);         //PLL36MHz
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		sta=RCC_GetFlagStatus(RCC_FLAG_PLLRDY);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);                  //SYSCLK36MHz
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);                            //AHBʱ�ӵ���ϵͳʱ��                  36M
		RCC_PCLK1Config(RCC_HCLK_Div1);                             //����AHBʱ��APB1����AHBʱ�ӵ�һ��     36M
		RCC_PCLK2Config(RCC_HCLK_Div1);                             //����AHBʱ��APB2����AHBʱ��           36M
		return 1;
	}
		
}



/*-----------------------------------------------------------------------*/
/* ʱ��ԴΪ�ⲿ���پ���                                                  */
/* �ⲿ���پ���Ϊ8M                                                      */
/*-----------------------------------------------------------------------*/


void HSE_SystemClock(void)
{
	RCC_DeInit();
	
	RCC_HSEConfig(RCC_HSE_ON);
	HSEStartUpStatus=RCC_WaitForHSEStartUp();
	if(HSEStartUpStatus==0)
	{
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);               //            72M        
		RCC_PLLCmd(ENABLE);
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);         //AHBʱ�ӵ���ϵͳʱ��                  72M
		RCC_PCLK1Config(RCC_HCLK_Div2);          //����AHBʱ��APB1����AHBʱ�ӵ�һ��     36M
		RCC_PCLK2Config(RCC_HCLK_Div1);          //����AHBʱ��APB2����AHBʱ��           72M
	}
	
}



/*-----------------------------------------------------------------------*/
/* ���Ź�����                                                            */
/*-----------------------------------------------------------------------*/


void IWDG_Configure(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0xFFF);
	IWDG_Enable();
}



/*-----------------------------------------------------------------------*/
/* ϵͳ��ʱ��                                                            */
/*-----------------------------------------------------------------------*/


void Systick_Configure(void)                             //1ms
{
	if(SysTick_Config(SystemCoreClock/1000))
	while(1);
}



/*-----------------------------------------------------------------------*/
/* TIM2 ��ʱ������                                                       */
/*-----------------------------------------------------------------------*/


void TIM2_Configure(void)                                      //1ms interrupt
{
	TIM_TimeBaseInitTypeDef TIM_TimeBase_Soure;
	NVIC_InitTypeDef NVIC_Soure;
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBase_Soure.TIM_Period =99;
	TIM_TimeBase_Soure.TIM_Prescaler =(360-1);
	TIM_TimeBase_Soure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBase_Soure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBase_Soure);
	
	NVIC_Soure.NVIC_IRQChannel =TIM2_IRQn;
	NVIC_Soure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_Soure.NVIC_IRQChannelSubPriority =0;
	NVIC_Soure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_Soure);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
}



/*-----------------------------------------------------------------------*/
/* TIM3 ��ʱ������                                                       */
/*-----------------------------------------------------------------------*/


void TIM3_Configure(void)                                //TIME interupt          0.2ms 
{
	TIM_TimeBaseInitTypeDef TIM_TimeBase_Soure;
	NVIC_InitTypeDef NVIC_Soure;
	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBase_Soure.TIM_Period =9;
	TIM_TimeBase_Soure.TIM_Prescaler =(720-1);
	TIM_TimeBase_Soure.TIM_ClockDivision =TIM_CKD_DIV1;
	TIM_TimeBase_Soure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBase_Soure);
	
	NVIC_Soure.NVIC_IRQChannel =TIM3_IRQn;
	NVIC_Soure.NVIC_IRQChannelPreemptionPriority =3;
	NVIC_Soure.NVIC_IRQChannelSubPriority =0;
	NVIC_Soure.NVIC_IRQChannelCmd =ENABLE;
	NVIC_Init(&NVIC_Soure);
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM3,ENABLE);
	
}



// /*-----------------------------------------------------------------------*/
// /* UART3��������                                                         */
// /*-----------------------------------------------------------------------*/


// void UART3_Configure(void)
// {
// 	GPIO_InitTypeDef GPIO_Soure;
// 	USART_InitTypeDef USART_Soure;
// 	NVIC_InitTypeDef NVIC_Soure;
// 	
// //	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
// //	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
// 	
// 	USART_DeInit(USART3);
// 	
// 	GPIO_Soure.GPIO_Pin =GPIO_Pin_10;
// 	GPIO_Soure.GPIO_Mode =GPIO_Mode_AF_PP;
// 	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB,&GPIO_Soure);
// 	
// 	GPIO_Soure.GPIO_Pin=GPIO_Pin_11;
// 	GPIO_Soure.GPIO_Mode =GPIO_Mode_IPU;
// 	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB,&GPIO_Soure);
// 	
// 	USART_Soure.USART_BaudRate =115200;
// 	USART_Soure.USART_WordLength =USART_WordLength_8b;
// 	USART_Soure.USART_StopBits =USART_StopBits_1;
// 	USART_Soure.USART_Parity =USART_Parity_No;
// 	USART_Soure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
// 	USART_Soure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
// 	USART_Init(USART3,&USART_Soure);
// 	
// 	NVIC_Soure.NVIC_IRQChannel =USART3_IRQn;
// 	NVIC_Soure.NVIC_IRQChannelPreemptionPriority =1;
// 	NVIC_Soure.NVIC_IRQChannelSubPriority =0;
// 	NVIC_Soure.NVIC_IRQChannelCmd =ENABLE;
// 	NVIC_Init(&NVIC_Soure);
// 	
// 	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
// 	
// 	USART_Cmd(USART3,ENABLE);
// }



/*-----------------------------------------------------------------------*/
/* оƬ��������                                                          */
/*-----------------------------------------------------------------------*/


void GpioConfigInit(void)
{
	/*    IO�˿�����ʱ������    */
	GpioPeriphClockConfig();
	
// 	/*    SD���˿ڳ�ʼ��        */
// 	SDCard_HardwareConfig();
// 	
// 	/*    �����˿ڳ�ʼ��        */
// 	Key_HardwareConfig();
// 	
// 	/*    LED�˿ڳ�ʼ��         */
// 	Led_HardwareConfig();
	
	/*    �������ʾ              */
//	DigitalTubew_HardwareConfig();
	ADC_HardwareConfig();
}



/*-----------------------------------------------------------------------*/
/* ����ʱ����������                                                      */
/*-----------------------------------------------------------------------*/


void GpioPeriphClockConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | 
												 RCC_APB2Periph_AFIO | RCC_APB2Periph_TIM1 | RCC_APB2Periph_USART1, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4 | 
												 RCC_APB1Periph_TIM6 | RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 | 
												 RCC_APB1Periph_UART4 | RCC_APB1Periph_UART5 | RCC_APB1Periph_WWDG,ENABLE);
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_Soure.GPIO_Pin = GPIO_Pin_All;
	GPIO_Soure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Soure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Soure);
	GPIO_Init(GPIOB, &GPIO_Soure);
	GPIO_Init(GPIOC, &GPIO_Soure);
}



/*-----------------------------------------------------------------------*/
/*  ADC1ģ��ת������                                                      */
/*-----------------------------------------------------------------------*/


void ADC1_Configure(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	DMA_InitTypeDef DMA_Soure;
	ADC_InitTypeDef ADC_Soure;
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div4);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );
	
 	GPIO_Soure.GPIO_Pin=GPIO_Pin_0;          
 	GPIO_Soure.GPIO_Mode =GPIO_Mode_AIN;
 	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB,&GPIO_Soure);

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_DeInit(DMA1_Channel1);
	DMA_Soure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);
	DMA_Soure.DMA_MemoryBaseAddr = (uint32_t)&ADCValue;
	DMA_Soure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_Soure.DMA_BufferSize = 1;
	DMA_Soure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_Soure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	DMA_Soure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_Soure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_Soure.DMA_Mode = DMA_Mode_Circular;
	DMA_Soure.DMA_Priority = DMA_Priority_High;
	DMA_Soure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_Soure);

	/* Enable DMA1 channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_Soure.ADC_Mode = ADC_Mode_Independent;
	ADC_Soure.ADC_ScanConvMode = ENABLE;
	ADC_Soure.ADC_ContinuousConvMode = ENABLE;
	ADC_Soure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Soure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Soure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_Soure);

	/* ADC1 regular channel14 configuration */ 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */   
	ADC_ResetCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* Start ADC1 Software Conversion */ 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



/*-----------------------------------------------------------------------*/
/* SD�������������                                                      */
/*-----------------------------------------------------------------------*/


void SDCard_HardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
// 	GPIO_Soure.GPIO_Pin=GPIO_Pin_0;           /*  power  */
// 	GPIO_Soure.GPIO_Mode =GPIO_Mode_Out_PP;
// 	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA,&GPIO_Soure);
// 	
// 	GPIO_Soure.GPIO_Pin=GPIO_Pin_1;           /*  SD_Det */
// 	GPIO_Soure.GPIO_Mode =GPIO_Mode_IPU;
// 	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOA,&GPIO_Soure);
// 	
// 	GPIO_Soure.GPIO_Pin = GPIO_Pin_0 ;        /*  SD_CS  */
// 	GPIO_Soure.GPIO_Mode = GPIO_Mode_Out_PP;
// 	GPIO_Soure.GPIO_Speed = GPIO_Speed_50MHz;
// 	GPIO_Init(GPIOB,&GPIO_Soure);
	
	GPIO_Soure.GPIO_Pin=GPIO_Pin_1;           /*  power  */
	GPIO_Soure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Soure);
	
	GPIO_Soure.GPIO_Pin=GPIO_Pin_12;           /*  SD_Det */
	GPIO_Soure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_Soure);
	
	GPIO_Soure.GPIO_Pin = GPIO_Pin_6 ;        /*  SD_CS  */
	GPIO_Soure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Soure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Soure);
}



/*-----------------------------------------------------------------------*/
/* ������������                                                          */
/*-----------------------------------------------------------------------*/


void Key_HardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
	GPIO_Soure.GPIO_Pin=GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_Soure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_Soure);
}


/*-----------------------------------------------------------------------*/
/* LED����������                                                         */
/*-----------------------------------------------------------------------*/


void Led_HardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_Soure.GPIO_Pin=GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 |GPIO_Pin_9;
	GPIO_Soure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Soure);
	
//	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
}


/*-----------------------------------------------------------------------*/
/* �������������                                                        */
/*-----------------------------------------------------------------------*/



void DigitalTubew_HardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
	GPIO_Soure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 |GPIO_Pin_5 | GPIO_Pin_6 |GPIO_Pin_7;
	GPIO_Soure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Soure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_Soure);
}



/*-----------------------------------------------------------------------*/
/* �������������                                                        */
/*-----------------------------------------------------------------------*/


void ADC_HardwareConfig(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	
	GPIO_Soure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Soure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Soure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_Soure);
}





/*--------------------------------------------------------------------------

        ��ӡlog

---------------------------------------------------------------------------*/



/*-----------------------------------------------------------------------*/
/* UART2��������                                                         */
/*-----------------------------------------------------------------------*/


void UART2_Configure(void)
{
	GPIO_InitTypeDef GPIO_Soure;
	USART_InitTypeDef USART_Soure;
	
	/* USART3_TX */
	GPIO_Soure.GPIO_Pin =GPIO_Pin_2;
	GPIO_Soure.GPIO_Mode =GPIO_Mode_AF_PP;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Soure);
	
	/* USART3_TX */
	GPIO_Soure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Soure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_Soure.GPIO_Speed =GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_Soure);
	
	USART_Soure.USART_BaudRate =115200;
	USART_Soure.USART_WordLength =USART_WordLength_8b;
	USART_Soure.USART_StopBits =USART_StopBits_1;
	USART_Soure.USART_Parity =USART_Parity_No;
	USART_Soure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
	USART_Soure.USART_Mode =USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_Soure);
	
	USART_Cmd(USART2,ENABLE);
}


int fputc(int ch, FILE *f)
{
  USART_SendData(USART2, (uint8_t) ch);
  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}
  return ch;
}

int fgetc(FILE *f)
{
  
   while(!(USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == SET))
   {
   }
  
    
   return (USART_ReceiveData(USART2));
}

void assert_failed(u8* file, u32 line)
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* Infinite loop */
    printf("assert failed at %s:%d \n", file, line);
    while (1) {
    }
}

void delay(uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}


