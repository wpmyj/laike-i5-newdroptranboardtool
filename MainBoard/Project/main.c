/* Library includes. */
#include "stm32f10x.h"
#include "stm32f10x_it.h"

/* Standard includes. */
#include <stdio.h>
#include "string.h"

/*  Project includes. */
#include "maindef.h"
#include "bsp.h"
#include "gpioredef.h"
#include "apiLib.h"

#define LOG_TAG    "main"
#include <elog.h>
#include <bsp.h>
//#include <stdio.h>
#include <stdlib.h>


/*        Global variable.            */
_ST_MAINMANAGER mMaininf;
__IO uint16_t ADCValue[5];

/*      config function.      */
void prvSetupHardware(void);



int main(void)
{
	prvSetupHardware();

	/* initialize EasyLogger */
	elog_init();
	/* set EasyLogger log format */
	elog_set_fmt(ELOG_LVL_ASSERT, ELOG_FMT_ALL);
	elog_set_fmt(ELOG_LVL_ERROR, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_WARN, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_INFO, ELOG_FMT_LVL | ELOG_FMT_TAG | ELOG_FMT_TIME);
	elog_set_fmt(ELOG_LVL_DEBUG, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
	elog_set_fmt(ELOG_LVL_VERBOSE, ELOG_FMT_ALL & ~(ELOG_FMT_FUNC | ELOG_FMT_T_INFO | ELOG_FMT_P_INFO));
	/* start EasyLogger */
	elog_start();
//	u32 iCont = 0;
	while(1)
	{
		/*    定时复位看门狗   */
		IWDG_ReloadCounter();
//		PBout(1) = mMaininf.mDrop.mDropPWMStatus;
// 		if(mMaininf.mDrop.mTime == 0)       //     屏蔽打印
// 		{
// //			mMaininf.mDrop.mDropADCVAL[0] = ADCValue[0];
// //			elog_a("elog", "%d\n", mMaininf.mDrop.mDropADCVAL[0]);
// //			log_e(" Time : %d   Drop ADC : %d",iCont/2,mMaininf.mDrop.mDropADCDiff);
// 			log_e("Drop ADC : %d",mMaininf.mDrop.mDropADCDiff);   //  yuan
// 			
// //			log_e("免碰撞 ADC : %d",mMaininf.mDrop.mDropADCDiff);
// 			mMaininf.mDrop.mTime = 1000;
// //			iCont ++;
// //			elog_a("elog", "(%s) has assert failed at %s:%ld.\n", ex, func, line);
// 		}
		GetDropADC(ADCValue[0]);
		DetectDropADC();
		ADCProcess();
		
		
//		PBout(1) = 1;
	}

//	return 0;
}



void prvSetupHardware(void)
{
	BSP_Init();
}



































