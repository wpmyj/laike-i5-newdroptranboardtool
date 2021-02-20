/****************************************************************************************************/
/*			@FileName		:	apiLib.h																																    	*/
/*			@Author			:	zhengbaiqiu																																		*/
/*			@Version		:	v1.0																																					*/
/*			@Date				:	2019.11.12  																																	*/
/****************************************************************************************************/

#ifndef	__APILIB_H__
#define	__APILIB_H__
#include "stm32f10x.h"
//#include <stdint.h>
#include "gpioredef.h"

#ifdef __cplusplus
extern "C" {
#endif

// #include "ff.h"	
// #include "diskio.h"
#include "maindef.h"

	
void api_InitParamsAtPowerOn(void);
	
u32 api_GetSystemTimePeriod(u32 StartTime);
u32 api_GetCurrentSystemTime(void);
void GetDropADC(u16 val);
void DetectDropADC(void);
void ADCProcess(void);
	
// void api_UART_ReceiveData(u8 vData);
// void api_ClearUARTReceiveFlag(void);
// void api_ClearReceivePackets(void);
// void api_UART_SendData(u16 vCont, u8 *pData);
// void api_SendCmd(u8 cmd);
// void api_ParsingVersionData(void);
// void api_GetVersion(char *pre);
// void api_SendBurnCmd(u32 PackLens);
// void api_SendBurnData(u32 OffsetAddr,u16 PackLens);
// void api_ClearFlashBurn(void);
// void api_ClearBurnFlag(void);
// u8 api_CheckWorkStatus(_enum_ZBQ_ModeStatus iSta);
// 	
// u8 api_CheckSDCardPositionFlag(_enum_ZBQ_SDPosition pFlag);
// void SD_PowerOn(void);
// void SD_PowerOff(void);
// void api_DetectSDCardPositionChange(u8 ucFlag);
// void api_SendSDCardInitResult(u8 ucFlag);
// void api_SDCardStatusDisplay(void);
// //void api_CreatePrintFile(u8 pType);
// void api_SearchPrintFolder(void);
// void api_SendPrintRespond(u8 Cmd,u8 Sta);
// void api_CreatePrintFile(void);
// void api_CreatNewPrintFileName(char *pOldName);
// void api_GetSubString(char *pStr,u8 Start,u8 Lens,char *pStrt);
// int api_StringTurnNumber(char *pStr,u8 Num);
// u8 api_CharTurnNumber(char Num);
// int api_Pow(u8 Base,u8 Index);
// void api_NumberTurnString(int mNum,char *Str);
// int api_StringCompareASCII(char* StrOne,char* StrTwo);
// void api_SearchBurnFile(void);
// void api_LedControl(u16 LedPos,u8 LedSta);
// void api_ClearLinkNode(List PreList);

	
	
#ifdef _cplusplus
extern }
#endif

#endif /*__APILIB_H__*/	

