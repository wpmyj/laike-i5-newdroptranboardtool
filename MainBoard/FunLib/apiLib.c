/*        Standard includes.                */
#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"

// /*        freertos           */
// #include "FreeRTOS.h"
// #include "FreeRTOSConfig.h"
// #include "portmacro.h"
// #include "projdefs.h"
// #include "task.h"
// #include "queue.h"
// #include "event_groups.h"
// #include "semphr.h"

/*        FatFS             */
//#include "ff.h"

/*        Project            */
#include "maindef.h"
//#include "sd.h"
#include "apiLib.h"
//#include "TK_SDCardController.h"


/*  ϵͳ������ؿ��ƿ�                                              */
void api_InitParamsAtPowerOn(void);

/*  ʱ����ؿ��ƿ�                                                  */
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void GetDropADC(u16 val);
void DetectDropADC(void);
void ADCProcess(void);




/*--------------------------------------------------------------------------

        ϵͳ������ؿ��ƿ�

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* SD card power-on initialization variables              		   			   */
/*-----------------------------------------------------------------------*/


void api_InitParamsAtPowerOn(void)
{
//	u16 iCont;
	/*    ϵͳ�δ������        */
	mMaininf.mSystem.mSystemTick = 0;
	
	/*    ����ʱ��              */
	mMaininf.mSystem.mPowerOnStartTime = 0;
	
	mMaininf.mDrop.mDropADCVAL[0] = 0;
//	mMaininf.mDrop.mDropStatus = 0;
	mMaininf.mDrop.mDropTrigger = FALSE;
	mMaininf.mDrop.mDropTriggerCont = 0;
	mMaininf.mDrop.mDropTime = 0;
	mMaininf.mDrop.mDropPWMTime = 0;
	mMaininf.mDrop.mDropPWMStatus = 0;
	
	mMaininf.mDrop.mDropADCValue [0][0] = 65535;
	mMaininf.mDrop.mDropADCValue [0][1] = 65535;
	mMaininf.mDrop.mDropADCValue [1][0] = 0;
	mMaininf.mDrop.mDropADCValue [1][1] = 0;
//	mMaininf.mDrop.mDropADCAGV [0] =;
	mMaininf.mDrop.mTime = 0;
}



/*--------------------------------------------------------------------------

        ʱ����ؿ⺯��

---------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/* Get system time period         	      														   */
/*-----------------------------------------------------------------------*/


u32 api_GetSystemTimePeriod(u32 StartTime)
{
	u32 res;
	
	if(mMaininf.mSystem.mSystemTick >= StartTime)
	{
		res = mMaininf.mSystem.mSystemTick - StartTime;
	}
	else
	{
		res = SYSTEMTIMEMAX - StartTime + mMaininf.mSystem.mSystemTick;
	}
	
	return res;
}



/*-----------------------------------------------------------------------*/
/* Get the current system time          															   */
/*-----------------------------------------------------------------------*/


u32 api_GetCurrentSystemTime(void)
{
	return mMaininf.mSystem.mSystemTick;
}



/*-----------------------------------------------------------------------*/
/* get adc                            															   */
/*-----------------------------------------------------------------------*/



void GetDropADC(u16 val)
{
	u8 iCont;
	u16 ADCValue,ADCValueChange;
	
	/*   ��ȡ��Сֵ   */
	ADCValue = val;
	if(ADCValue < mMaininf.mDrop.mDropADCValue [0][1])
	{
		for(iCont = 0; iCont < 2 ;iCont ++)
		{
			if(ADCValue < mMaininf.mDrop.mDropADCValue[0][iCont])
			{
				ADCValueChange = ADCValue;
				ADCValue = mMaininf.mDrop.mDropADCValue[0][iCont];
				mMaininf.mDrop.mDropADCValue[0][iCont] = ADCValueChange;
			}
		}
	}
	
	/*   ��ȡ���ֵ   */
	ADCValue = val;
	if(ADCValue > mMaininf.mDrop.mDropADCValue [1][1])
	{
		for(iCont = 0; iCont < 2 ;iCont ++)
		{
			if(ADCValue > mMaininf.mDrop.mDropADCValue[1][iCont])
			{
				ADCValueChange = ADCValue;
				ADCValue = mMaininf.mDrop.mDropADCValue[1][iCont];
				mMaininf.mDrop.mDropADCValue[1][iCont] = ADCValueChange;
			}
		}
	}
}


/*-----------------------------------------------------------------------*/
/* check adc                            															   */
/*-----------------------------------------------------------------------*/



void DetectDropADC(void)
{
	if(mMaininf.mDrop.mDropTime == 0)
	{
		mMaininf.mDrop.mDropADCAGV [0] = (mMaininf.mDrop.mDropADCValue[0][0] + mMaininf.mDrop.mDropADCValue[0][1])>>1;
		mMaininf.mDrop.mDropADCAGV [1] = (mMaininf.mDrop.mDropADCValue[1][0] + mMaininf.mDrop.mDropADCValue[1][1])>>1;
		mMaininf.mDrop.mDropADCDiff = mMaininf.mDrop.mDropADCAGV [1] - mMaininf.mDrop.mDropADCAGV [0];
		
		
		//if(mMaininf.mDrop.mDropADCVAL[0] < 300)
// 		if(ADCValue[0] < 2048 + 300)
		if(mMaininf.mDrop.mDropADCDiff < 300)
		{
			if(++mMaininf.mDrop.mDropTriggerCont == 2)
			{
				mMaininf.mDrop.mDropTrigger = TRUE;
				mMaininf.mDrop.mDropTriggerCont = 1;
			}
		}
		else
		{
			mMaininf.mDrop.mDropTrigger = FALSE;
			mMaininf.mDrop.mDropTriggerCont = 0;
		}
		
		mMaininf.mDrop.mDropADCValue [0][0] = 65535;
		mMaininf.mDrop.mDropADCValue [0][1] = 65535;
		mMaininf.mDrop.mDropADCValue [1][0] = 0;
		mMaininf.mDrop.mDropADCValue [1][1] = 0;
		
		//mMaininf.mDrop.mDropTime = 50;
		mMaininf.mDrop.mDropTime = 10;        //     ����Ƶ��
	}
}


/*-----------------------------------------------------------------------*/
/* adc process                           															   */
/*-----------------------------------------------------------------------*/


void ADCProcess(void)
{
	if(mMaininf.mDrop.mDropTrigger == TRUE)
	{
		//PBout(1) = mMaininf.mDrop.mDropPWMStatus;
		//PBout(1) = 1;       //     ֮ǰ
		PBout(1) = 0;       //     ����֮��ߵ͵�ƽ��ת
	}
	else
	{
		//PBout(1) = 1;
		//PBout(1) = mMaininf.mDrop.mDropPWMStatus;   //   ֮ǰ
		PBout(1) = ~mMaininf.mDrop.mDropPWMStatus;    //   ����֮��ߵ͵�ƽ��ת
	}
}







