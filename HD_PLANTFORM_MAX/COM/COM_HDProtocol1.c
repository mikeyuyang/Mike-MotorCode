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
#include "COM_HDProtocol1.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define HD1_FRAME_LEN 14
#define FRAME_ID_CODE_ANS	0x90
#define FRAME_ID_CODE_ACT	0xEB


#define ENABLE_CHECK_SUM 1

#define HD1_REQ_FRAME_OFFSET_HEAD			0
#define HD1_REQ_FRAME_OFFSET_LEN			2
#define HD1_REQ_FRAME_OFFSET_FUNCTION_CODE	3
#define HD1_REQ_FRAME_OFFSET_DATA			4
#define HD1_REQ_FRAME_OFFSET_SERIAL_NUM		10
#define HD1_REQ_FRAME_OFFSET_CHECK_SUM		13


#define HD1_ANS_FRAME_OFFSET_HEAD			0
#define HD1_ANS_FRAME_OFFSET_LEN			2
#define HD1_ANS_FRAME_OFFSET_ID_CODE		3
#define HD1_ANS_FRAME_OFFSET_FUNCTION_CODE	4
#define HD1_ANS_FRAME_OFFSET_DATA			5
#define HD1_ANS_FRAME_OFFSET_SERIAL_NUM		10
#define HD1_ANS_FRAME_OFFSET_CHECK_SUM		13


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
	FUNCTION_CODE_SET_SPD = 0x04,
	FUNCTION_CODE_FIX_POINT_MOVE = 0x01,
	FUNCTION_CODE_POS_SECTOR_SCAN = 0x02,
	FUNCTION_CODE_PITCH_SECTOR_SCAN = 0x03,
	FUNCTION_CODE_Look_Status = 0x05,
	FUNCTION_CODE_Find_Zero = 0x06,
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
VOID HDProtocol1Loop(SERIAL_PORT port, UINT8 slvAdd)
{
//	CHAR buffer[64];
	USART_COM_HANDLE* pUartHandle;
	USART_BUF* pRxBuf;
	UINT16 cnt;
	UINT16 readLen;
	UINT8 *pFrame;
	
	/* Get uart handle */
	pUartHandle = GetUSARTPortHandler(port);
	/* Create the semaphore used by frame notify */
//	sprintf((char *)buffer, "ComPortSem%d", port);
//	pSem = &pUartHandle->m_Sem;
//	*pSem = xEventGroupCreate();

	/* Get start address of buffers */
	pRxBuf = &pUartHandle->m_Buf.RXBuffer;

	if(pRxBuf->m_count >= HD1_FRAME_LEN)
	{
		if(0x55 == pRxBuf->m_buffer[pRxBuf->m_RdIdx])
		{
			if(0xAA == pRxBuf->m_buffer[(pRxBuf->m_RdIdx + 1) % USART_BUF_SIZE])
			{
				/* Copy frame to queue */
				readLen = HD1_FRAME_LEN;
				pFrame = (UINT8*)&g_HD1ReqFrame[port];

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

		if(TRUE == ProcessHDProtocol1Frame(&g_HD1ReqFrame[port], &g_HD1AnsFrame[port], slvAdd))
		{
			UINT8 chkSum = 0;
			/* calc check sum */
			for(cnt = 2; cnt < 13; cnt ++)
			{
				chkSum += *(((UINT8*)&g_HD1AnsFrame[port]) + cnt);
			}
			/* Fill check sum */
			g_HD1AnsFrame[port].m_ChkSum = chkSum;
			USARTSendnByte((SERIAL_PORT)port, (UCHAR*)&g_HD1AnsFrame[port], HD1_FRAME_LEN);
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
BOOL ProcessHDProtocol1Frame(HD1_REQ_FRAME* pReq, HD1_ANS_FRAME* pAns, UINT8 slvAdd)
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
	if(0x0a != pReq->m_Len)
	{
		return FALSE;
	}

	/* Check slave address. */
	if(slvAdd != pReq->m_Data[6] && 0 != pReq->m_Data[6])
	{
		return FALSE;
	}

	/* Check function code. */
	functionCode = (FUNCTION_CODE)pReq->m_FunCode;
	if(functionCode >= FUNCTION_CODE_MAX)
	{
		return FALSE;
	}

	/* Fill common data */
	pAns->m_Head[0] = 0x55;
	pAns->m_Head[1] = 0xAA;
	pAns->m_Len = 0x0A;
	pAns->m_IdCode = FRAME_ID_CODE_ANS;
	pAns->m_FunCode = functionCode;
	pAns->m_Data[0] = 0;
	pAns->m_Data[5] = slvAdd;

	switch(functionCode)
	{
		case FUNCTION_CODE_LOOP_BACK:
		{
			break;
		}
		case FUNCTION_CODE_SET_SPD:
		{
			UINT16 spd1,spd2;
			spd1 = pReq->m_Data[1];
			spd1 <<= 8;
			spd1 += pReq->m_Data[0];

			spd2 = pReq->m_Data[3];
			spd2 <<= 8;
			spd2 += pReq->m_Data[2];			
			
			if(pReq->m_Data[4] & 0x01)
			{
				SPD.m_MaxSpd1 = spd1;
				EEPROMSaveParam(eDataAxis1MaxSpd1);
			}

			if(pReq->m_Data[4] & 0x02)
			{
				SPD.m_MaxSpd2 = spd2;
				EEPROMSaveParam(eDataAxis1MaxSpd2);
			}			
			break;
		}
		case FUNCTION_CODE_FIX_POINT_MOVE:
		{
			UINT16 pos1;
			INT16 pos2;
			pos1 = pReq->m_Data[1];
			pos1 <<= 8;
			pos1 += pReq->m_Data[0];

			pos2 = pReq->m_Data[3];
			pos2 <<= 8;
			pos2 += pReq->m_Data[2];			
			
			if(pReq->m_Data[4] & 0x01)
			{
				if((pos1 < SPD.m_NegLimitVal1))
				{
					pos1 = SPD.m_NegLimitVal1;
				}
				if((pos1 > SPD.m_PosLimitVal1))
				{
					pos1 = SPD.m_PosLimitVal1;
				}
				MovePosCtrlSetPoint(MOTOR_PORT_1,(INT32)pos1);
			}

			if(pReq->m_Data[4] & 0x02)
			{
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
		}
		case FUNCTION_CODE_POS_SECTOR_SCAN:
		{
			break;
		}
//		case FUNCTION_CODE_CAL_NORTH:
//		{
//			break;
//		}
//		case FUNCTION_CODE_LEVELING:
//		{
//			break;
//		}
		case FUNCTION_CODE_PITCH_SECTOR_SCAN:
		{
			break;
		}
		case FUNCTION_CODE_STOP:
		{
			if(pReq->m_Data[4] & 0x01)
			{
				MoveStop(MOTOR_PORT_1);
			}

			if(pReq->m_Data[4] & 0x02)
			{
				MoveStop(MOTOR_PORT_2);
			}
			
			break;
		}
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
VOID HD1ActSend(SERIAL_PORT port, UINT8 slvAdd)
{
	UINT8 chkSum=0, cnt;
	INT16 i16Temp;
	INT32 i16Tempx;
	g_HD1ActFrame[port].m_Head[0] = 0x55;
	g_HD1ActFrame[port].m_Head[1] = 0xAA;
	g_HD1ActFrame[port].m_Len = 0x0A;
	g_HD1ActFrame[port].m_IdCode = FRAME_ID_CODE_ACT;
	
	i16Tempx = (INT32)AVD.m_UserPosition1;
	if(i16Tempx < 0)
		i16Tempx = i16Tempx + 36000;
	else
		i16Tempx = i16Tempx;
	g_HD1ActFrame[port].m_PosAng[0] = (UINT8)(i16Tempx & 0x00FF);
	g_HD1ActFrame[port].m_PosAng[1] = (UINT8)((i16Tempx >> 8) & 0x00FF);
	i16Temp = (INT16)AVD.m_UserPosition2;
	g_HD1ActFrame[port].m_PitAng[0] = (UINT8)(i16Temp & 0x00FF);
	g_HD1ActFrame[port].m_PitAng[1] = (UINT8)((i16Temp >> 8) & 0x00FF);
	i16Temp = (INT16)AVD.m_DIUserStatus & 0x07;
	switch(i16Temp)
	{
		case 0x01:g_HD1ActFrame[port].m_LimitInfo =0x01;break;
		case 0x04:g_HD1ActFrame[port].m_LimitInfo =0x10;break;
		case 0x02:g_HD1ActFrame[port].m_LimitInfo =0x20;break;
		case 0x05:g_HD1ActFrame[port].m_LimitInfo =0x11;break;
		case 0x03:g_HD1ActFrame[port].m_LimitInfo =0x21;break;
		default:g_HD1ActFrame[port].m_LimitInfo =0x00;break;
	}
	//g_HD1ActFrame[port].m_LimitInfo = 0;
//	g_HD1ActFrame[port].m_FalutInfo = 0;
	
	g_HD1ActFrame[port].m_SerialNo = slvAdd;

	/* calc check sum */
	for(cnt = 2; cnt < 13; cnt ++)
	{
		chkSum += *(((UINT8*)&g_HD1ActFrame[port]) + cnt);
	}

	
	g_HD1ActFrame[port].m_ChkSum = chkSum;
	USARTSendnByte((SERIAL_PORT)port, (UCHAR*)&g_HD1ActFrame[port], HD1_FRAME_LEN);
	chkSum =0;
}



/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


