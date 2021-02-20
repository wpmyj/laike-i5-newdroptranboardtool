/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "maindef.h"


 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
}



void TIM2_IRQHandler(void)        //1ms interrupt
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET)
	{
		/*        用于生成1ms滴答定时器          */
		mMaininf.mSystem.mSystemTick ++;
		if(mMaininf.mSystem.mSystemTick >= SYSTEMTIMEMAX) mMaininf.mSystem.mSystemTick = 0;
		
// 		/*        用于生成SD卡卡控制时序         */
// 		if(mMaininf.mSD.mSDTimeinf.mTimer1 > 0) mMaininf.mSD.mSDTimeinf.mTimer1 -- ;
// 		if(mMaininf.mSD.mSDTimeinf.mTimer2 > 0) mMaininf.mSD.mSDTimeinf.mTimer2 -- ;
// 		
// 		/*        用于生成UART超时               */
// 		if(mMaininf.mUart.mReceiveTimeOut > 0) mMaininf.mUart.mReceiveTimeOut -- ;
// 		
// 		/*        用于烧录超时                   */
// 		if(mMaininf.mBurn.mBurnStepTimeOut > 0) mMaininf.mBurn.mBurnStepTimeOut -- ;
// 		if(mMaininf.mBurn.mBurnAllTimeOut > 0) mMaininf.mBurn.mBurnAllTimeOut -- ;
// 		
// 		/*        用于打印超时                   */
// 		if(mMaininf.mPrint.mReceivePrintDataTimer > 0) mMaininf.mPrint.mReceivePrintDataTimer -- ;
// 		if(mMaininf.mPrint.mReceivePrintDataFlashTimer > 0) mMaininf.mPrint.mReceivePrintDataFlashTimer -- ;
// 		else GPIO_ResetBits(GPIOB,GPIO_Pin_7);         //打印红灯
// //		if(iiii > 0)iiii--;
		if(mMaininf.mDrop.mTime > 0) mMaininf.mDrop.mTime --;
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}


void TIM3_IRQHandler(void)        //0.2ms interrupt
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET)
	{
		if(mMaininf.mDrop.mDropTime > 0) mMaininf.mDrop.mDropTime --;
		if(mMaininf.mDrop.mDropPWMTime == 9)
		{
// 			mMaininf.mDrop.mDropPWMStatus = ~mMaininf.mDrop.mDropPWMStatus;
// 			mMaininf.mDrop.mDropPWMTime = 0;
			mMaininf.mDrop.mDropPWMStatus = 1;
		}
		else if(mMaininf.mDrop.mDropPWMTime >= 10)
		{
			mMaininf.mDrop.mDropPWMStatus = 0;
			mMaininf.mDrop.mDropPWMTime = 0;
		}
		mMaininf.mDrop.mDropPWMTime ++;
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
