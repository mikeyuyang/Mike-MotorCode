/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	COM_HDProtocol1.c
//
// TITLE:	This function realized the HengDa standerd communication protocol1.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 9,2014
//
// Description: HengDa standerd communication protocol realization.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		
// Created date:   	Nov 3,2018
// Version:
// Descriptions: 	First Version
//
//------------------------------------------------------------------------------------------------------
// Modified by: 		
// Modified date:		
// Version:				
// Descriptions: 		 
//
//------------------------------------------------------------------------------------------------------
//######################################################################################################
*/

/*
********************************************************************************************************
********************************************************************************************************
*/



/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include <stm32f4xx.h>
#include <string.h>
#include "includes.h"

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#include "UTL_CRC.h"
#include "MOV_Control.h"
#include "DIO_Control.h"
#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "UTL_USART.h"
//#include "UTL_OCTimer.h"
#include "UTL_EEPROM.h"
//#include "APP_Control.h"
#include "SYS_Application.h"
#define COM_HD_PROTOCOL_1_GLOBAL_
#include "COM_HDProtocol2.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define HDY_RXFRAME_LEN 10

#define HDY_TXFRAME_LEN 24
/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/
extern MOVE_CTRL  g_MoveControl[MOTOR_PORT_MAX];
/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/
typedef	enum tagFUNCTION_CODE
{
	FUNCTION_CODE_LOOP_BACK = 0,
	
	FUNCTION_CODE_FIX_POINT_MOVE = 0x01,
	FUNCTION_CODE_SET_SPD = 0x04,

	FUNCTION_CODE_LookStatus = 0x05,
	FUNCTION_CODE_LookZero = 0x06,
	FUNCTION_CODE_STOP = 0x53,

	FUNCTION_CODE_MAX
}FUNCTION_CODE;

/* Modbus error code */
typedef	enum tagANS_CODE
{
	ANS_CODE_SUCCESS = 0x00,
	ANS_CODE_FAILURE = 0x01,
	MB_ERR_MAX
}ANS_CODE;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/


//CDP_CONTROL_FRAME CdpControlBuf;
//ABA_ANS_FRAME AbaAnsBuf;
/*
*********************************************************************************************************
*    Local function prototypes
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*   Program Start 
*********************************************************************************************************
*/

/*
****************************************************************************
*                           MAIN FUNCTION 
*                         (ONLY FOR MAIN FILE)
*
* Description  : 
*
* Arguments    : 
*
* Notes        : 
****************************************************************************
*/

/*
*********************************************************************************************************
* 	Global function implementations
*********************************************************************************************************
*/

/*
**************************************************************************
* Description	: Process serial data frame
  	1) Input argument  : pQueue - Point of queue that wait for process.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov,9 2014
**************************************************************************
*/
VOID HDProtocolYLoop(SERIAL_PORT port)
{
//	CHAR buffer[64];
	USART_COM_HANDLE* pUartHandle;
	USART_BUF* pRxBuf;
	UINT16 cnt;
	UINT16 readLen;
	UINT8 *pFrame;
	
	/* Get uart handle */
	pUartHandle = GetUSARTPortHandler(port);


	/* Get start address of buffers */
	pRxBuf = &pUartHandle->m_Buf.RXBuffer;

	if(pRxBuf->m_count >= HDY_RXFRAME_LEN)
	{
		pRxBuf->m_RdIdx = 0x00;
		if(0x59 == pRxBuf->m_buffer[pRxBuf->m_RdIdx])
		{
			if(0x79 == pRxBuf->m_buffer[(pRxBuf->m_RdIdx + 1) % USART_BUF_SIZE])
			{
				/* Copy frame to queue */
				readLen = HDY_RXFRAME_LEN;
				pFrame = (UINT8*)&g_HDYAnsFrame[port];

				for(cnt = 0; cnt < readLen; cnt ++)
				{

					*pFrame = pRxBuf->m_buffer[pRxBuf->m_RdIdx];
					pFrame ++;
					pRxBuf->m_RdIdx ++;
					pRxBuf->m_RdIdx %= USART_BUF_SIZE;
				}
				//memcpy(s_SerialQueue[port].m_buffer, pRxBuf->m_buffer, pRxBuf->m_count);
				pRxBuf->m_count -= readLen;				
			}
			else
			{
				pRxBuf->m_RdIdx += 2;
				pRxBuf->m_RdIdx %= USART_BUF_SIZE;
				pRxBuf->m_count -= 2;
				return;
			}
		}
		else
		{
			pRxBuf->m_RdIdx += 1;
			pRxBuf->m_RdIdx %= USART_BUF_SIZE;
			pRxBuf->m_count -= 1;
			return;
		}

		if(TRUE == ProcessHDProtocolYFrame(&g_HDYAnsFrame[port]))
		{
			;
		}
		else
		pUartHandle->m_Status = USART_STATUS_RECEIVING;
	}
}

/*
**************************************************************************
* Description	: Process serial data frame
  	1) Input argument  : pQueue - Point of queue that wait for process.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov,9 2014
**************************************************************************
*/
BOOL ProcessHDProtocolYFrame(HDY_ANS_FRAME* pAns)
{
	FUNCTION_CODE functionCode;
	UINT8 cnt;
	UINT8* pFrameData;
	UINT8 chkSum = 0;

#if(ENABLE_CHECK_SUM)
	/* calc check sum */
	for(cnt = 2; cnt < 13; cnt ++)
	{
		chkSum += *(((UINT8*)pReq) + cnt);
	}
	/* Check sum validation */
	if(chkSum != pReq->m_ChkSum)
	{
		return FALSE;
	}
#endif
	/* Check lenth. */

	/* Check slave address. */


	/* Check function code. */
	functionCode = (FUNCTION_CODE)pAns->m_FunCode;

	switch(functionCode)
	{
		case FUNCTION_CODE_LOOP_BACK:
		{
			break;
		}
		case FUNCTION_CODE_SET_SPD:
		{
			UINT16 spd1,spd2;
			spd1 = pAns->m_Data[1];
			spd1 <<= 8;
			spd1 += pAns->m_Data[0];

			spd2 = pAns->m_Data[3];
			spd2 <<= 8;
			spd2 += pAns->m_Data[2];			
			

				SPD.m_MaxSpd1 = spd1;
				EEPROMSaveParam(eDataAxis1MaxSpd1);

				SPD.m_MaxSpd2 = spd2;
				EEPROMSaveParam(eDataAxis1MaxSpd2);
		
			break;
		}
		case FUNCTION_CODE_FIX_POINT_MOVE:
		{
			UINT16 pos1;
			INT16 pos2;
			pos1 = pAns->m_Data[1];
			pos1 <<= 8;
			pos1 += pAns->m_Data[0];

			pos2 = pAns->m_Data[3];
			pos2 <<= 8;
			pos2 += pAns->m_Data[2];			
			

				if((pos1 < SPD.m_NegLimitVal1))
				{
					pos1 = SPD.m_NegLimitVal1;
				}
				if((pos1 > SPD.m_PosLimitVal1))
				{
					pos1 = SPD.m_PosLimitVal1;
				}
				MovePosCtrlSetPoint(MOTOR_PORT_1,(INT32)pos1);
			


				if((pos2 < SPD.m_NegLimitVal2))
				{
					pos2 = SPD.m_NegLimitVal2;
				}
				if((pos2 > SPD.m_PosLimitVal2))
				{
					pos2 = SPD.m_PosLimitVal2;
				}
				MovePosCtrlSetPoint(MOTOR_PORT_2,(INT32)pos2);
			}			
			break;
		
		
		case FUNCTION_CODE_LookZero:
		{
				MoveSetCtrlMode(MOTOR_PORT_1,MOVE_CTRL_MODE_FIND_ORIG);
				MoveFindOrigStart(MOTOR_PORT_1);		
				MoveSetCtrlMode(MOTOR_PORT_2,MOVE_CTRL_MODE_FIND_ORIG);
				MoveFindOrigStart(MOTOR_PORT_2);
			;
		}break;
		
		case FUNCTION_CODE_STOP:
		{

				MoveStop(MOTOR_PORT_1);

				MoveStop(MOTOR_PORT_2);
				
			
		}break;
		default:
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*
**************************************************************************
* Description	: Send active delivery frame
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date		: Nov,9 2018
**************************************************************************
*/
VOID HDYActSend(SERIAL_PORT port)
{
	UINT8 chkSum=0, cnt;
	INT16 i16Temp;
	INT32 i16Tempx;
	g_HDYActFrame[port].m_Head[0] = 0x79;
	g_HDYActFrame[port].m_Head[1] = 0x59;
	g_HDYActFrame[port].m_Len = 0x12;
	
	

	//g_HD1ActFrame[port].m_LimitInfo = 0;
//	g_HD1ActFrame[port].m_FalutInfo = 0;
	


	/* calc check sum */


	GenMBCRC16((UCHAR*)&g_HDYActFrame[port],0X14);
	

	USARTSendnByte((SERIAL_PORT)port, (UCHAR*)&g_HDYActFrame[port], HDY_TXFRAME_LEN);
	chkSum =0;
}



/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


