/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	COM_ModbusRTU.c
//
// TITLE:	This function realized the Modbus communication protocol.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 9,2014
//
// Description: Modbus communication protocol realization.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Nov 9,2014
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
#define COM_MODBUS_RTU_GLOBAL_
#include "COM_ModbusRTU.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define ENABLE_CRC 0

#define MB_REQ_FRAME_OFFSET_SLAVE_ADD		0
#define MB_REQ_FRAME_OFFSET_FUNCTION_CODE	1
#define MB_REQ_FRAME_OFFSET_START_ADD		2
#define MB_REQ_FRAME_OFFSET_N_REG			4
#define MB_REQ_FRAME_OFFSET_BYTE_CNT		6
#define MB_REQ_FRAME_OFFSET_REG_VALUE		7


#define MB_ANS_FRAME_OFFSET_FUNCTION_CODE	1
#define MB_ANS_FRAME_OFFSET_BYTE_CNT		2
#define MB_ANS_FRAME_OFFSET_ERR_CODE		2
#define MB_ANS_FRAME_OFFSET_REG_VALUE		3

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
//----------------Modbus public command set-----------------------//
	FUNCTION_CODE_READ_COILS = 0x01,
	FUNCTION_CODE_READ_DISCRETE_INPUTS = 0x02,
	FUNCTION_CODE_READ_HOLDING_REG = 0x03,
	FUNCTION_CODE_READ_INPUT_REG = 0x04,
	FUNCTION_CODE_WRITE_SINGLE_COIL = 0x05,
	FUNCTION_CODE_WRITE_SINGLE_REG = 0x06,
	FUNCTION_CODE_READ_EXCEPTION_STATUS = 0x07,
	FUNCTION_CODE_DIAGNOSTIC = 0x08,
	FUNCTION_CODE_GET_COM_EVENT_CNT = 0x0B,
	FUNCTION_CODE_GET_COM_EVENT_LOG = 0x0C,
	FUNCTION_CODE_WRITE_MULTIPLE_COILS = 0x0F,
	FUNCTION_CODE_WRITE_MULTIPLE_REGS = 0x10,
	FUNCTION_CODE_REPORT_SERVER_ID = 0x11,
	FUNCTION_CODE_READ_FILE_RECORD = 0x14,
	FUNCTION_CODE_WRITE_FILE_RECORD = 0x15,
	FUNCTION_CODE_MASK_WRITE_REG = 0x16,
	FUNCTION_CODE_READ_WRITE_MULTIPLE_REGS = 0x17,
	FUNCTION_CODE_READ_FIFO_QUEUE = 0x18,
	FUNCTION_CODE_READ_DEVICE_IDENTIFICATION = 0x2B,
//----------------User extended commands----------------------------//

	FUNCTION_CODE_FIND_ORIG = 0x40,
	FUNCTION_CODE_GET_ANGLE = 0x41,
	FUNCTION_CODE_SET_CURRENT_AS_ORG = 0x42,
	FUNCTION_CODE_RELATIVE_MOVE = 0x43,
	FUNCTION_CODE_ABSOLUTE_MOVE = 0x44,
	FUNCTION_CODE_STOP_MOVE = 0x45,
	FUNCTION_CODE_SET_SPEED = 0x46,
	FUNCTION_CODE_SET_ACCEL = 0x47,
	FUNCTION_CODE_SET_TEST_SERIAL = 0x48,
	FUNCTION_CODE_START_TEST_SERIAL = 0x49,
	FUNCTION_CODE_STOP_TEST_SERIAL = 0x4A,
	FUNCTION_CODE_SET_MODEL = 0x4C,

	FUNCTION_CODE_MAX
}FUNCTION_CODE;

/* Modbus error code */
typedef	enum tagMB_ERR_CODE
{
	MB_ERR_ILLEGAL_FUNCTION = 0x01,
	MB_ERR_ILLEGAL_DATA_ADDRESS = 0x02,
	MB_ERR_ILLEGAL_DATA_VALUE = 0x03,
	MB_ERR_SLAVE_DEVICE_FAILURE = 0x04,
	MB_ERR_ACKNOWLEDGE = 0x05,
	MB_ERR_SLAVE_DEVICE_BUSY = 0x06,
	MB_ERR_MEMORY_PARITY_ERROR = 0x08,
	MB_ERR_GATEWAY_PATH_UNAVAILABLE = 0x0a,
	MB_ERR_GATEWAY_TARGET_DEVICE_FAILED_TO_RESPOND = 0x0b,
	MB_ERR_WORK_MODE_ERROR = 0x0c,
	MB_ERR_MAX
}MB_ERR_CODE;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
SERIAL_QUEUE 	s_SerialQueue[SERIAL_PORT_MAX];


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
VOID ModbusRtuLoop(SERIAL_PORT port, UINT8 slvAdd)
{
//	CHAR buffer[64];
	USART_COM_HANDLE* pUartHandle;
	EventGroupHandle_t* pSem;
	USART_BUF* pRxBuf;
	UINT16 crcBuf;
	EventBits_t uxBits;
	UINT8 err;
	UINT16 cnt;
	UINT16 readLen;
	
	/* Get uart handle */
	pUartHandle = GetUSARTPortHandler(port);
	/* Create the semaphore used by frame notify */
//	sprintf((char *)buffer, "ComPortSem%d", port);
//	pSem = &pUartHandle->m_Sem;
//	*pSem = xEventGroupCreate();

	/* Get start address of buffers */
	pRxBuf = &pUartHandle->m_Buf.RXBuffer;

	if(IsComCtrlTimerElapsed(port) && (pUartHandle->m_Status == USART_STATUS_RECEIVING))
	{
		StopComCtrlTimer(port);
		
		//uxBits = xEventGroupWaitBits(*pSem, BIT0, pdTRUE, pdFALSE, portMAX_DELAY);

		/* Copy frame to queue */
		readLen = pRxBuf->m_count;
		for(cnt = 0; cnt < readLen; cnt ++)
		{
			s_SerialQueue[port].m_buffer[cnt] = pRxBuf->m_buffer[pRxBuf->m_RdIdx];
			pRxBuf->m_RdIdx ++;
			pRxBuf->m_RdIdx %= USART_BUF_SIZE;
		}
		//memcpy(s_SerialQueue[port].m_buffer, pRxBuf->m_buffer, pRxBuf->m_count);
		s_SerialQueue[port].m_count = readLen;
		pRxBuf->m_count -= readLen;

		ProcessModbusRtuFrame(&s_SerialQueue[port], slvAdd);

		/* Send response */
		if(s_SerialQueue[port].m_count)
		{
//#if(ENABLE_CRC)
			crcBuf = GenMBCRC16( s_SerialQueue[port].m_buffer, s_SerialQueue[port].m_count);
			s_SerialQueue[port].m_buffer[s_SerialQueue[port].m_count ++] = (UINT8)(crcBuf & 0xff);
			s_SerialQueue[port].m_buffer[s_SerialQueue[port].m_count ++] = (UINT8)(crcBuf >> 8);
//#endif
			USARTSendnByte((SERIAL_PORT)port, s_SerialQueue[port].m_buffer, s_SerialQueue[port].m_count);
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
VOID ProcessModbusRtuFrame(SERIAL_QUEUE* pQueue, UINT8 slvAdd)
{
	FUNCTION_CODE functionCode;
	UINT16 u16CalcTemp = 0;
	UINT8* pSerial;
	UINT8 SerialNum, cnt;
	UINT16 startAdd, nReg, nByte;
	UINT8* pFrameData;
	UINT16* pActData;
	INT32 i32CalcTemp;

#if(ENABLE_CRC)
	/* Check CRC */
	/* Calculate the crc value. */
	//	if(crcBuf != crcChkBuf)
	if(0 != GenMBCRC16( pQueue->m_buffer, pQueue->m_count))
	{
		/* CRC Error */
		pQueue->m_count = 0;
		return;
	}
	pQueue->m_count -= 2;
#endif

	/* Check slave address. */
	if(pQueue->m_buffer[MB_REQ_FRAME_OFFSET_SLAVE_ADD] != slvAdd)
	{
		/* Slave address error */
		pQueue->m_count = 0;
		return;
	}
	/* Check function code. */
	functionCode = (FUNCTION_CODE)pQueue->m_buffer[MB_REQ_FRAME_OFFSET_FUNCTION_CODE];
	if(functionCode >= FUNCTION_CODE_MAX)
	{
		/* Frame function code error */
		pQueue->m_buffer[1] |= 0x80;
		pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
		pQueue->m_count = 3;
		return;
	}

	switch(functionCode)
	{
		case FUNCTION_CODE_LOOP_BACK:
		{
			/* Send back date frame directly. */
			break;
		}
		case FUNCTION_CODE_READ_COILS:
		case FUNCTION_CODE_READ_DISCRETE_INPUTS:
		{
			/* Unsupported function code */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
			break;
		}
		case FUNCTION_CODE_READ_HOLDING_REG:
		{
			/* Get start address */
			startAdd = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD + 1];

			/* Get read number of register */
			nReg = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG];
			nReg <<= 8;
			nReg += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG + 1];
			/* Check read number */
			if(1 > nReg || nReg > 125)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
				return;
			}
			/* Check address */
			if((startAdd + nReg) > eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Write Byte count */
			pQueue->m_buffer[MB_ANS_FRAME_OFFSET_BYTE_CNT] = nReg * 2;
			/* Copy data to frame */
			pFrameData = &pQueue->m_buffer[MB_ANS_FRAME_OFFSET_REG_VALUE];
			
			for(cnt = 0; cnt < nReg; cnt ++)
			{
				pActData = (UINT16 *)GetSPDataPoint(startAdd + cnt);
				*pFrameData++ = (UINT8)(*pActData >> 8);
				*pFrameData++ = (UINT8)(*pActData & 0x00ff);
			}
			pQueue->m_count = 3 + 2 * nReg;
			break;
		}
		case FUNCTION_CODE_READ_INPUT_REG:
		{
			/* Get start address */
			startAdd = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD + 1];

			/* Get read number of register */
			nReg = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG];
			nReg <<= 8;
			nReg += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG + 1];
			/* Check read number */
			if(1 > nReg || nReg > 125)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
				return;
			}
			/* Check address */
			if((startAdd + nReg) > eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Write Byte count */
			pQueue->m_buffer[MB_ANS_FRAME_OFFSET_BYTE_CNT] = nReg * 2;
			/* Copy data to frame */
			pFrameData = &pQueue->m_buffer[MB_ANS_FRAME_OFFSET_REG_VALUE];
			for(cnt = 0; cnt < nReg; cnt ++)
			{
				pActData = (UINT16 *)GetAVDataPoint(startAdd + cnt);
				*pFrameData++ = (UINT8)(*pActData >> 8);
				*pFrameData++ = (UINT8)(*pActData & 0x00ff);
			}
			pQueue->m_count = 3 + 2 * nReg;
			break;
		}
		case FUNCTION_CODE_WRITE_SINGLE_COIL:
		{
			/* Unsupported function code */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
			break;

		}
		case FUNCTION_CODE_WRITE_SINGLE_REG:
		{
			/* Get start address */
			startAdd = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD + 1];

			/* Check address */
			if(startAdd >= eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Copy data to SP Value */
			pActData = (UINT16 *)GetSPDataPoint(startAdd);

			*pActData = pQueue->m_buffer[4];
			*pActData <<= 8;
			*pActData += pQueue->m_buffer[5];

			break;
		}
		case FUNCTION_CODE_READ_EXCEPTION_STATUS:
		case FUNCTION_CODE_DIAGNOSTIC:
		case FUNCTION_CODE_GET_COM_EVENT_CNT:
		case FUNCTION_CODE_GET_COM_EVENT_LOG:
		case FUNCTION_CODE_WRITE_MULTIPLE_COILS:
		{
			/* Unsupported function code */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
			break;
		}
		case FUNCTION_CODE_WRITE_MULTIPLE_REGS:
		{
			/* Get start address */
			startAdd = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD + 1];

			/* Get read number of register */
			nReg = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG];
			nReg <<= 8;
			nReg += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG + 1];
			nByte = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_BYTE_CNT];
			/* Check read number */
			if(1 > nReg || nReg > 125)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
				return;
			}
			/* Check address */
			if((startAdd + nReg) > eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Copy data From frame */
			pFrameData = &pQueue->m_buffer[MB_REQ_FRAME_OFFSET_REG_VALUE];
			for(cnt = 0; cnt < nReg; cnt ++)
			{
				pActData = (UINT16 *)GetSPDataPoint(startAdd + cnt);
				*pActData = *pFrameData++;
				*pActData <<= 8;
				*pActData += *pFrameData++;
			}
			pQueue->m_count = 6;	// Return with start address and quantity of register.
			break;
		}
		case FUNCTION_CODE_REPORT_SERVER_ID:
		case FUNCTION_CODE_READ_FILE_RECORD:
		case FUNCTION_CODE_WRITE_FILE_RECORD:
		case FUNCTION_CODE_MASK_WRITE_REG:
		{
			/* Unsupported function code. */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
			break;
		}
		case FUNCTION_CODE_READ_WRITE_MULTIPLE_REGS:
		{
			/* Process write firstly */
			/* Get start address */
			startAdd = pQueue->m_buffer[6];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[7];
			/* Get read number of register */
			nReg = pQueue->m_buffer[8];
			nReg <<= 8;
			nReg += pQueue->m_buffer[9];
			nByte = pQueue->m_buffer[10];
			/* Check read number */
			if(1 > nReg || nReg > 125)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
				return;
			}
			/* Check address */
			if((startAdd + nReg) > eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Copy data From frame */
			pFrameData = &pQueue->m_buffer[MB_REQ_FRAME_OFFSET_REG_VALUE];
			for(cnt = 0; cnt < nReg; cnt ++)
			{
				pActData = (UINT16 *)GetSPDataPoint(startAdd + cnt);
				*pActData = *pFrameData++;
				*pActData <<= 8;
				*pActData += *pFrameData++;
			}

			/* Process read */
			/* Get start address */
			startAdd = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD];
			startAdd <<= 8;
			startAdd += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_START_ADD + 1];

			/* Get read number of register */
			nReg = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG];
			nReg <<= 8;
			nReg += pQueue->m_buffer[MB_REQ_FRAME_OFFSET_N_REG + 1];
			nByte = pQueue->m_buffer[MB_REQ_FRAME_OFFSET_BYTE_CNT];
			/* Check read number */
			if(1 > nReg || nReg > 125)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
				return;
			}
			/* Check address */
			if((startAdd + nReg) > eDataSPMax)
			{
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_FUNCTION_CODE] |= 0x80;
				pQueue->m_buffer[MB_ANS_FRAME_OFFSET_ERR_CODE] = MB_ERR_ILLEGAL_DATA_ADDRESS;
				pQueue->m_count = 3;
				return;
			}
			/* Write Byte count */
			pQueue->m_buffer[MB_ANS_FRAME_OFFSET_BYTE_CNT] = nReg * 2;
			/* Copy data to frame */
			pFrameData = &pQueue->m_buffer[MB_ANS_FRAME_OFFSET_REG_VALUE];
			for(cnt = 0; cnt < nReg; cnt ++)
			{
				pActData = (UINT16 *)GetSPDataPoint(startAdd + cnt);
				*pFrameData++ = (UINT8)(*pActData >> 8);
				*pFrameData++ = (UINT8)(*pActData & 0x00ff);
			}
			pQueue->m_count = 3 + 2 * nReg;
			break;
		}
		case FUNCTION_CODE_READ_FIFO_QUEUE:
		case FUNCTION_CODE_READ_DEVICE_IDENTIFICATION:
		{
			/* Unsupported function code. */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
			break;
		}
		case FUNCTION_CODE_FIND_ORIG:
		{
			if(MOTOR_PORT_1 == pQueue->m_buffer[2])
			{
				MoveFindOrigStart(MOTOR_PORT_1);
			}
			else if(MOTOR_PORT_2 == pQueue->m_buffer[2])
			{
				MoveFindOrigStart(MOTOR_PORT_2);
			}
			break;
		}
		
		case FUNCTION_CODE_GET_ANGLE:
		{
			/* Copy data */
			pQueue->m_buffer[2] = (AVD.m_UserPosition1 >> 24) & 0xFF;
			pQueue->m_buffer[3] = (AVD.m_UserPosition1 >> 16) & 0xFF;
			pQueue->m_buffer[4] = (AVD.m_UserPosition1 >> 8) & 0xFF;
			pQueue->m_buffer[5] = AVD.m_UserPosition1 & 0xFF;
			
			pQueue->m_buffer[6] = (AVD.m_UserPosition2 >> 24) & 0xFF;
			pQueue->m_buffer[7] = (AVD.m_UserPosition2 >> 16) & 0xFF;
			pQueue->m_buffer[8] = (AVD.m_UserPosition2 >> 8) & 0xFF;
			pQueue->m_buffer[9] = AVD.m_UserPosition2 & 0xFF;
			pQueue->m_count = 10;
			break;
		}
		case FUNCTION_CODE_SET_CURRENT_AS_ORG:
		{
			if(MOTOR_PORT_1 == pQueue->m_buffer[2])
			{
				if(SPD.m_PosSource1 == POS_SOURCE_ENCODER)
				{	
					SPD.m_UserOrgPoint1 = (UINT16)AVD.m_EncoderPosition1;
					/* Save parameter */
					EEPROMSaveParam(eDataUserOrgPoint1_1);
					EEPROMSaveParam(eDataUserOrgPoint1_2);
				}
				else if(SPD.m_PosSource1 == POS_SOURCE_INTER_PUL_CNT)
				{
					MotorPwmSetOrigin(MOTOR_PORT_1);
				}
				else if(SPD.m_PosSource1 == POS_SOURCE_EXT_PUL_CNT)
				{
					CanElmoSetOrigin(MOTOR_PORT_1);
				}
			}
			else if(MOTOR_PORT_2 == pQueue->m_buffer[2])
			{
				if(SPD.m_PosSource2 == POS_SOURCE_ENCODER)
				{	
					SPD.m_UserOrgPoint2 = (UINT16)AVD.m_EncoderPosition2;
					/* Save parameter */
					EEPROMSaveParam(eDataUserOrgPoint2_1);
					EEPROMSaveParam(eDataUserOrgPoint2_2);
				}
				else if(SPD.m_PosSource2 == POS_SOURCE_INTER_PUL_CNT)
				{
					MotorPwmSetOrigin(MOTOR_PORT_2);
				}
				else if(SPD.m_PosSource2 == POS_SOURCE_EXT_PUL_CNT)
				{
					CanElmoSetOrigin(MOTOR_PORT_2);
				}
			}
#if 1			
			else if(MOTOR_PORT_3 == pQueue->m_buffer[2])
			{
				if(SPD.m_PosSource3 == POS_SOURCE_ENCODER)
				{	
					SPD.m_UserOrgPoint3 = (UINT16)AVD.m_EncoderPosition3;
					/* Save parameter */
					EEPROMSaveParam(eDataUserOrgPoint3_1);
					EEPROMSaveParam(eDataUserOrgPoint3_2);
				}
				else if(SPD.m_PosSource3 == POS_SOURCE_INTER_PUL_CNT)
				{
					MotorPwmSetOrigin(MOTOR_PORT_3);
				}
				else if(SPD.m_PosSource3 == POS_SOURCE_EXT_PUL_CNT)
				{
					CanElmoSetOrigin(MOTOR_PORT_3);
				}
			}
#endif			
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

			}
			break;
		}
		case FUNCTION_CODE_RELATIVE_MOVE:
		{
			i32CalcTemp = 0;
			i32CalcTemp |= pQueue->m_buffer[3];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue->m_buffer[4];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue->m_buffer[5];
			i32CalcTemp <<= 8;		
			i32CalcTemp |= pQueue->m_buffer[6];
//#if RELATIVE_LIMIT_EN
			if(0 == pQueue->m_buffer[2])
			{
				/* Axis1 limit dispose */
				if((SPD.m_PosLimitVal1 != SPD.m_NegLimitVal1) && 
					((AVD.m_UserPosition1 + i32CalcTemp) > SPD.m_PosLimitVal1
					|| (AVD.m_UserPosition1 + i32CalcTemp) < SPD.m_NegLimitVal1))
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}
			else if(1 == pQueue->m_buffer[2]) 
			{
				/* Axis2 limit dispose */
				if((SPD.m_PosLimitVal2 != SPD.m_NegLimitVal2) && 
					((AVD.m_UserPosition2 + i32CalcTemp) > SPD.m_PosLimitVal2
					|| (AVD.m_UserPosition2 + i32CalcTemp) < SPD.m_NegLimitVal2))
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}
			else if(2 == pQueue->m_buffer[2]) 
			{
				/* Axis3 limit dispose */
				if((SPD.m_PosLimitVal3 != SPD.m_NegLimitVal3) && 
					((AVD.m_UserPosition3 + i32CalcTemp) > SPD.m_PosLimitVal3
					|| (AVD.m_UserPosition3 + i32CalcTemp) < SPD.m_NegLimitVal3))
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}			
//#endif

			if(FALSE == MovePosCtrlSetAction((MOTOR_PORT)pQueue->m_buffer[2],
											i32CalcTemp))
			{
				/* Turn distance over range */
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

				pQueue->m_count = 3;
			}
			break;
		}
		case FUNCTION_CODE_ABSOLUTE_MOVE:
		{
			i32CalcTemp = 0;
			i32CalcTemp |= pQueue->m_buffer[3];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue->m_buffer[4];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue->m_buffer[5];
			i32CalcTemp <<= 8;		
			i32CalcTemp |= pQueue->m_buffer[6];


			if(0 == pQueue->m_buffer[2])
			{
				/* Axis1 limit dispose */
				if((SPD.m_PosLimitVal1 != SPD.m_NegLimitVal1) && 
					(i32CalcTemp > SPD.m_PosLimitVal1
					|| i32CalcTemp < SPD.m_NegLimitVal1))					
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}
			else if(1 == pQueue->m_buffer[2])
			{
				/* Axis2 limit dispose */
				if((SPD.m_PosLimitVal2 != SPD.m_NegLimitVal2) && 
					(i32CalcTemp > SPD.m_PosLimitVal2
					|| i32CalcTemp < SPD.m_NegLimitVal2))	
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}
			else if(2 == pQueue->m_buffer[2])
			{
				/* Axis3 limit dispose */
				if((SPD.m_PosLimitVal3 != SPD.m_NegLimitVal3) && 
					(i32CalcTemp > SPD.m_PosLimitVal3
					|| i32CalcTemp < SPD.m_NegLimitVal3))	
				{
					/* Turn distance over range */
					pQueue->m_buffer[1] |= 0x80;
					pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

					pQueue->m_count = 3;
					return;
				}
			}
			

			if(FALSE == MovePosCtrlSetPoint((MOTOR_PORT)pQueue->m_buffer[2],
											i32CalcTemp))
			{
				/* Turn distance over range */
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

				pQueue->m_count = 3;
			}
			break;
		}
		case FUNCTION_CODE_STOP_MOVE:
		{
			MoveStop(MOTOR_PORT_1);
			MoveStop(MOTOR_PORT_2);
			MoveStop(MOTOR_PORT_3);
			break;
		}
		case FUNCTION_CODE_SET_SPEED:
		{
			u16CalcTemp = pQueue->m_buffer[3];
			u16CalcTemp <<= 8;
			u16CalcTemp += pQueue->m_buffer[4];

			if(u16CalcTemp > 1800)
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
			}

			if(MOTOR_PORT_1 == pQueue->m_buffer[2])
			{
				SPD.m_MaxSpd1 = u16CalcTemp;
				//g_MoveControl[0].m_MaxSpd = u16CalcTemp;
				/* Save parameter */
				EEPROMSaveParam(eDataAxis1MaxSpd1);
			}
			else if(MOTOR_PORT_2 == pQueue->m_buffer[2])
			{
				SPD.m_MaxSpd2 = u16CalcTemp;
				//g_MoveControl[1].m_MaxSpd = u16CalcTemp;
				/* Save parameter */
				EEPROMSaveParam(eDataAxis1MaxSpd2);
			}

			else if(MOTOR_PORT_3 == pQueue->m_buffer[2])
			{
				SPD.m_MaxSpd3 = u16CalcTemp;
				//g_MoveControl[2].m_MaxSpd = u16CalcTemp;
				/* Save parameter */
				//EEPROMSaveParam(EEPROM_PARAM_Y_MAX_SPD);
				EEPROMSaveParam(eDataAxis1MaxSpd3);
			}
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
			}

			pQueue->m_count = 2;
			break;
		}
		case FUNCTION_CODE_SET_ACCEL:
		{
			u16CalcTemp = pQueue->m_buffer[3];
			u16CalcTemp <<= 8;
			u16CalcTemp += pQueue->m_buffer[4];

			if(MOTOR_PORT_1 == pQueue->m_buffer[2])
			{
				SPD.m_Accel1 = u16CalcTemp;
				//g_MoveControl[0].m_Accl = u16CalcTemp;
				//g_MoveControl[0].m_Decl = u16CalcTemp;
				/* Save parameter */
				//EEPROMSaveParam(EEPROM_PARAM_1_ACC);
			}
			else if(MOTOR_PORT_2 == pQueue->m_buffer[2])
			{
				SPD.m_Accel2 = u16CalcTemp;
				//g_MoveControl[1].m_Accl = u16CalcTemp;
				//g_MoveControl[1].m_Decl = u16CalcTemp;
				/* Save parameter */
				//EEPROMSaveParam(EEPROM_PARAM_Y_ACC);
			}
		
			else if(MOTOR_PORT_3 == pQueue->m_buffer[2])
			{
				SPD.m_Accel3 = u16CalcTemp;
				//g_MoveControl[2].m_Accl = u16CalcTemp;
				//g_MoveControl[2].m_Decl = u16CalcTemp;
				/* Save parameter */
				//EEPROMSaveParam(EEPROM_PARAM_Y_ACC);
			}		
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;

			}
			break;
		}
#if 0
		case FUNCTION_CODE_SET_TEST_SERIAL:
		{
			/* Get serial point Number. */
			SerialNum = pQueue->m_buffer[2];
			/* Get the start point of serial address. */
			pSerial = &pQueue->m_buffer[3];

			if((pQueue->m_count == (SerialNum * 4 + 3)) && (SerialNum >= 2))
			{
				g_SerialMovePointNumber = SerialNum;		// Store serial number
				for(cnt = 0; cnt < SerialNum; cnt ++)
				{
					/* Store X serial Points */
					u16CalcTemp = *pSerial ++;
					u16CalcTemp <<= 8;
					u16CalcTemp += *pSerial ++;
					g_MoveSerialTable[cnt][0] = u16CalcTemp;
					/* Store Y serial Points */
					u16CalcTemp = *pSerial ++;
					u16CalcTemp <<= 8;
					u16CalcTemp += *pSerial ++;
					g_MoveSerialTable[cnt][1] = u16CalcTemp;
				}
				/* Send response */
				pQueue->m_count = 2;
				/* Check serial */
				InitSerialMoveCtrl();
				/* Save parameter */
				EEPROMSaveParam(EEPROM_PARAM_SERIAL_NUMBER);
				OSTimeDlyHMSM(0, 0, 0, 20);
				EEPROMSaveParam(EEPROM_PARAM_SERIAL_POINT);
			}
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
			}
			break;
		}
		case FUNCTION_CODE_START_TEST_SERIAL:
		{
			if(g_SerialMovePointNumber >= 2)
			{
				StartCircularMove();
				/* Send response */
				pQueue->m_count = 2;
			}
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
			}
			break;
		}
		case FUNCTION_CODE_STOP_TEST_SERIAL:
		{
			if(SYS_STATUS_CIRCULAR == g_SysStatus)
			{
				StopCircularMove();
				/* Send response */
				pQueue->m_count = 2;
			}
			else
			{
				pQueue->m_buffer[1] |= 0x80;
				pQueue->m_buffer[2] = MB_ERR_ILLEGAL_DATA_VALUE;
				pQueue->m_count = 3;
			}
			break;
		}
#endif
		default:
		{
			/* Illegal function code. */
			pQueue->m_buffer[1] |= 0x80;
			pQueue->m_buffer[2] = MB_ERR_ILLEGAL_FUNCTION;
			pQueue->m_count = 3;
		}
	}
}


/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


