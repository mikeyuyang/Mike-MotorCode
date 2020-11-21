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
#include "COM_JZSenor.h"


#include "COM_JZSenor.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define JZSenor_AnsFRAME_LEN				 17
#define JZSenor_ReqFRAME_LEN				 9

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


/* Modbus error code */


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
VOID JZSenorProtocol1Loop(SERIAL_PORT port)
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

	if(pRxBuf->m_count >= JZSenor_AnsFRAME_LEN)
	{
		if(0x4a == pRxBuf->m_buffer[pRxBuf->m_RdIdx])
		{
			if(0x59 == pRxBuf->m_buffer[(pRxBuf->m_RdIdx + 1) % USART_BUF_SIZE])
			{
				/* Copy frame to queue */
				readLen = JZSenor_AnsFRAME_LEN;
				pFrame = (UINT8*)&g_JZAnsFrame[port];

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
		
		

		if(TRUE == ProcessJZProtocol1Frame(&g_JZReqFrame[port], &g_JZAnsFrame[port]))
		{

			USARTSendnByte((SERIAL_PORT)port, (UCHAR*)&g_JZReqFrame[port], JZSenor_ReqFRAME_LEN);
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
BOOL ProcessJZProtocol1Frame(JZ_REQ_FRAME* pReq, JZ_ANS_FRAME* pAns)
{

	UINT8 cnt;
	UINT8* pFrameData;
	UINT8 chkSum = 0;
	union{unsigned char uchar[4]; float float32;}f32ToChar;

#if(ENABLE_CHECK_SUM)
	/* calc check sum */
	for(cnt = 0; cnt < 117; cnt ++)
	{
		chkSum += *(((UINT8*)pAns) + cnt);
	}
	/* Check sum validation */
	if(chkSum != pAns->m_ChkSum)
	{
		return FALSE;
	}
#endif
	/* Check lenth. */
	if(0x10 != pAns->m_Len[0])
	{
		return FALSE;
	}
	if(0x01 != pAns->m_FrameType)
	{
		return FALSE;
	}
	
	if(0x05 != pAns->m_FunctionType)
	{
		return FALSE;
	}

	
	pReq->m_Head[0] = 0x4a;
	pReq->m_Head[1] = 0x59;
	pReq->m_Len[0] = 0x09;
	pReq->m_Len[1] = 0x00;
	pReq->m_SenorType = 0x02;
	pReq->m_SenorAddress = 0x01;
	pReq->m_FrameType = 0x01;
	pReq->m_FunctionType = 0x05;
	pReq->m_ChkSum = 0xb5;
	
	pAns->m_SenorDataX[0] = f32ToChar.uchar[0];
	pAns->m_SenorDataX[1] = f32ToChar.uchar[1];
	pAns->m_SenorDataX[2] = f32ToChar.uchar[2];
	pAns->m_SenorDataX[3] = f32ToChar.uchar[3];
	CompassMsg->X_Data =  f32ToChar.float32;
	pAns->m_SenorDataY[0] = f32ToChar.uchar[0];
	pAns->m_SenorDataY[0] = f32ToChar.uchar[1];
	pAns->m_SenorDataY[0] = f32ToChar.uchar[2];
	pAns->m_SenorDataY[0] = f32ToChar.uchar[3];
	CompassMsg->Y_Data =  f32ToChar.float32;

	return TRUE;
}




/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


