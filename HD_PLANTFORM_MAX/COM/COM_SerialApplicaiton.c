/*
//######################################################################################################
//
// FILE:	COM_SerialApplicaiton.c
//
// TITLE:	This function realized the serial communication functions.
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: All communication function for serial ports.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris.Shang
// Created date:   	Oct 28, 2017
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
#include "COM_HMI.h"
#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "UTL_USART.h"
//#include "UTL_OCTimer.h"
#include "UTL_EEPROM.h"
//#include "APP_Control.h"
#include "COM_ModbusRTU.h"

#define	COM_SERIAL_APPLICAITON_GLOBAL_
#include "COM_SerialApplicaiton.h"
#include "COM_HDProtocol2.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#if 0
#define BIG_ENDIAN_MODE		0
#define LITTLE_ENDIAN_MODE	1

#define ENABLE_CRC 1

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
#endif 
#define MAX_SERIAL_COM_APP 8
/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
CONST UINT32 SerialBandRateTable[5] = 
{
	9600, 19200, 38400, 57600, 115200
};

/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/
/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/
/* Serial protocol type */
typedef	enum tagSERIAL_COM_PROTOCOL_TYPE
{
	SERIAL_PROTOCOL_TYPE_MODBUS = 0,
	SERIAL_PROTOCOL_TYPE_HD1,
	SERIAL_PROTOCOL_TYPE_HMI,
	SERIAL_PROTOCOL_TYPE_JZ_SENS,
	SERIAL_PROTOCOL_TYPE_LGLK_1,
	SERIAL_PROTOCOL_TYPE_MAX,
}SERIAL_COM_PROTOCOL_TYPE;
/* Serial Communication applicaiton handle */
typedef struct tagSERIAL_COM_APP_INFO
{
	BOOL m_Enable;
	SERIAL_COM_PROTOCOL_TYPE m_PrptocolType;
	USART_PARAM m_UartParam;
	UINT8 m_SlaveAdd;
	UINT8 m_ActSendTimeCnt;
}SERIAL_COM_APP_INFO;

/* COM task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} ComTaskInfo;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
static SERIAL_COM_APP_INFO s_SerialComAppInfo[MAX_SERIAL_COM_APP];
TaskHandle_t SerComAppTask_Handler[MAX_SERIAL_COM_APP];

/*
*********************************************************************************************************
*    Local function prototypes
*********************************************************************************************************
*/
void Task_SerComApp1(void* p_arg);
void Task_SerComApp2(void* p_arg);
void Task_SerComApp3(void* p_arg);
void Task_SerComApp4(void* p_arg);
void Task_SerComApp5(void* p_arg);
void Task_SerComApp6(void* p_arg);
void Task_SerComApp7(void* p_arg);
void Task_SerComApp8(void* p_arg);


const ComTaskInfo COM_TASK_INFO_TABLE[MAX_SERIAL_COM_APP] =
{
/*	task function,			task name,				Stack Size		  				arguments			priority		task handler		*/
	{ Task_SerComApp1,		"SerComTask1",			256,							(void *)0,			11,			&SerComAppTask_Handler[0]},
	{ Task_SerComApp2,		"SerComTask2",			256,							(void *)0,			11, 		&SerComAppTask_Handler[1]},
	{ Task_SerComApp3,		"SerComTask3",			256,							(void *)0,			11, 		&SerComAppTask_Handler[2]},
	{ Task_SerComApp4,		"SerComTask4",			256,							(void *)0,			11, 		&SerComAppTask_Handler[3]},
	{ Task_SerComApp5,		"SerComTask5",			256,							(void *)0,			12, 		&SerComAppTask_Handler[4]},
	{ Task_SerComApp6,		"SerComTask6",			256,							(void *)0,			12, 		&SerComAppTask_Handler[5]},
	{ Task_SerComApp7,		"SerComTask7",			256,							(void *)0,			12, 		&SerComAppTask_Handler[6]},
	{ Task_SerComApp8,		"SerComTask8",			256,							(void *)0,			12, 		&SerComAppTask_Handler[7]},
};
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
* Description	: Initilize serial communicaiton applicaiton, initilize UART ports, protocol and creat task
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: 
* Date		: Oct 28, 2017
**************************************************************************
*/
void InitSerialComApp(void)
{
	UINT8 cnt1 = 0;

	/* Copy eeprom parameter to local */
	s_SerialComAppInfo[0].m_Enable = (BOOL)SPD.m_SerEnable1;
	s_SerialComAppInfo[0].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType1;
	s_SerialComAppInfo[0].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate1];
	s_SerialComAppInfo[0].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen1;
	s_SerialComAppInfo[0].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit1;
	s_SerialComAppInfo[0].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity1;
	s_SerialComAppInfo[0].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[0].m_SlaveAdd = SPD.m_SerSlaveAddress1;
	s_SerialComAppInfo[0].m_ActSendTimeCnt = 0;
	
	s_SerialComAppInfo[1].m_Enable = (BOOL)SPD.m_SerEnable2;
	s_SerialComAppInfo[1].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType2;
	s_SerialComAppInfo[1].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate2];
	s_SerialComAppInfo[1].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen2;
	s_SerialComAppInfo[1].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit2;
	s_SerialComAppInfo[1].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity2;	
	s_SerialComAppInfo[1].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[1].m_SlaveAdd = SPD.m_SerSlaveAddress2;
	s_SerialComAppInfo[1].m_ActSendTimeCnt = 0;
	
	s_SerialComAppInfo[2].m_Enable = (BOOL)SPD.m_SerEnable3;
	s_SerialComAppInfo[2].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType3;
	s_SerialComAppInfo[2].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate3];
	s_SerialComAppInfo[2].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen3;
	s_SerialComAppInfo[2].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit3;
	s_SerialComAppInfo[2].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity3;
	s_SerialComAppInfo[2].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[2].m_SlaveAdd = SPD.m_SerSlaveAddress3;
	s_SerialComAppInfo[2].m_ActSendTimeCnt = 0;

	s_SerialComAppInfo[3].m_Enable = (BOOL)SPD.m_SerEnable4;
	s_SerialComAppInfo[3].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType4;
	s_SerialComAppInfo[3].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate4];
	s_SerialComAppInfo[3].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen4;
	s_SerialComAppInfo[3].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit4;
	s_SerialComAppInfo[3].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity4;
	s_SerialComAppInfo[3].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[3].m_SlaveAdd = SPD.m_SerSlaveAddress4;
	s_SerialComAppInfo[3].m_ActSendTimeCnt = 0;

	s_SerialComAppInfo[4].m_Enable = (BOOL)SPD.m_SerEnable5;
	s_SerialComAppInfo[4].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType5;
	s_SerialComAppInfo[4].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate5];
	s_SerialComAppInfo[4].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen5;
	s_SerialComAppInfo[4].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit5;
	s_SerialComAppInfo[4].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity5;
	s_SerialComAppInfo[4].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[4].m_SlaveAdd = SPD.m_SerSlaveAddress5;
	s_SerialComAppInfo[4].m_ActSendTimeCnt = 0;

	s_SerialComAppInfo[5].m_Enable = (BOOL)SPD.m_SerEnable6;
	s_SerialComAppInfo[5].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType6;
	s_SerialComAppInfo[5].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate6];
	s_SerialComAppInfo[5].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen6;
	s_SerialComAppInfo[5].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit6;
	s_SerialComAppInfo[5].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity6;
	s_SerialComAppInfo[5].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[5].m_SlaveAdd = SPD.m_SerSlaveAddress6;
	s_SerialComAppInfo[5].m_ActSendTimeCnt = 0;

	s_SerialComAppInfo[6].m_Enable = (BOOL)SPD.m_SerEnable7;
	s_SerialComAppInfo[6].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType7;
	s_SerialComAppInfo[6].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate7];
	s_SerialComAppInfo[6].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen7;
	s_SerialComAppInfo[6].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit7;
	s_SerialComAppInfo[6].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity7;
	s_SerialComAppInfo[6].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[6].m_SlaveAdd = SPD.m_SerSlaveAddress7;
	s_SerialComAppInfo[6].m_ActSendTimeCnt = 0;

	s_SerialComAppInfo[7].m_Enable = (BOOL)SPD.m_SerEnable8;
	s_SerialComAppInfo[7].m_PrptocolType = (SERIAL_COM_PROTOCOL_TYPE)SPD.m_SerProtocolType8;
	s_SerialComAppInfo[7].m_UartParam.m_Baudrate = SerialBandRateTable[SPD.m_SerBrandRate8];
	s_SerialComAppInfo[7].m_UartParam.m_BitLen = (USART_BIT_LEN)SPD.m_SerBitLen8;
	s_SerialComAppInfo[7].m_UartParam.m_Stopbits = (USART_STOP_BIT)SPD.m_SerStopBit8;
	s_SerialComAppInfo[7].m_UartParam.m_Parity = (USART_PARITY)SPD.m_SerParity8;
	s_SerialComAppInfo[7].m_UartParam.m_TimeOut= 5;
	s_SerialComAppInfo[7].m_SlaveAdd = SPD.m_SerSlaveAddress8;
	s_SerialComAppInfo[7].m_ActSendTimeCnt = 0;

	for(cnt1 = 0; cnt1 < SERIAL_PORT_MAX; cnt1 ++)
	{
		if(s_SerialComAppInfo[cnt1].m_Enable)
		{
			/* Initlize serial port */
			InitUSARTPort((SERIAL_PORT)cnt1, &s_SerialComAppInfo[cnt1].m_UartParam);
			/* Creat serial communication tasks */
			xTaskCreate(COM_TASK_INFO_TABLE[cnt1].m_TaskFunc,			 
				COM_TASK_INFO_TABLE[cnt1].m_TaskName,
				COM_TASK_INFO_TABLE[cnt1].m_StackSize,
				COM_TASK_INFO_TABLE[cnt1].m_TaskArg,
				COM_TASK_INFO_TABLE[cnt1].m_TaskPrio,
				COM_TASK_INFO_TABLE[cnt1].m_TaskHandler);
		}
	}

	return;
}
/*
**************************************************************************
* Description	: Initilize serial communicaiton applicaiton, initilize UART ports, protocol and creat task
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
VOID SerialComProtocolInit(SERIAL_PORT serialPort, SERIAL_COM_APP_INFO* pComAppInfo)
{
	if(SERIAL_PROTOCOL_TYPE_MODBUS == pComAppInfo->m_PrptocolType)
	{
	}
	else if(SERIAL_PROTOCOL_TYPE_HD1 == pComAppInfo->m_PrptocolType)
	{

	}
	else if(SERIAL_PROTOCOL_TYPE_HMI == pComAppInfo->m_PrptocolType)
	{

	}
	else if(SERIAL_PROTOCOL_TYPE_JZ_SENS == pComAppInfo->m_PrptocolType)
	{
//		InitJZSensor();
	}
}

VOID SerialComProtocolDispose(SERIAL_PORT serialPort, SERIAL_COM_APP_INFO* pComAppInfo)
{
	if(SERIAL_PROTOCOL_TYPE_MODBUS == pComAppInfo->m_PrptocolType)
	{
		ModbusRtuLoop(serialPort, pComAppInfo->m_SlaveAdd);
	}
	else if(SERIAL_PROTOCOL_TYPE_HD1 == pComAppInfo->m_PrptocolType)
	{
//		HDProtocol1Loop(serialPort, pComAppInfo->m_SlaveAdd);
		pComAppInfo->m_ActSendTimeCnt ++;
		if(pComAppInfo->m_ActSendTimeCnt >= 4)
		{
			pComAppInfo->m_ActSendTimeCnt = 0;
//			HD1ActSend(serialPort, pComAppInfo->m_SlaveAdd);
		}
	}
	else if(SERIAL_PROTOCOL_TYPE_HMI == pComAppInfo->m_PrptocolType)
	{
	  	HMIProtocol1Loop(serialPort, pComAppInfo->m_SlaveAdd);	
		HMIActSend( serialPort, 0x11,  0,0x0006);
		HMIActSend( serialPort, 0x13,  0,0x000b);
		HMIActSend2( serialPort, 0x11, 0,0x000e);
		HMIActSend2( serialPort, 0x13, 0,0x0005);
		HMIActSend3(serialPort, 0x11,  0,0x0010);
		HMIActSend3(serialPort, 0x13,  0,0x000C);
	}
	else if(SERIAL_PROTOCOL_TYPE_JZ_SENS == pComAppInfo->m_PrptocolType)
	{
//		JZSensorProtocolLoop(serialPort, pComAppInfo->m_SlaveAdd);
	}
}



/*
**************************************************************************
* Description	: Serial communication application tasks
  	1) Input argument  : None.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
void Task_SerComApp1(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[0];
	SERIAL_PORT serialPort = SERIAL_PORT_1;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		
		HDProtocolYLoop(SERIAL_PORT_5);

		vTaskDelay(10);
	}
}

void Task_SerComApp2(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[1];
	SERIAL_PORT serialPort = SERIAL_PORT_2;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		HDProtocolYLoop(SERIAL_PORT_5);

		vTaskDelay(10);
	}
}

void Task_SerComApp3(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[2];
	SERIAL_PORT serialPort = SERIAL_PORT_3;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}
}

void Task_SerComApp4(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[3];
	SERIAL_PORT serialPort = SERIAL_PORT_4;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}
}

void Task_SerComApp5(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[4];
	SERIAL_PORT serialPort = SERIAL_PORT_5;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}

}

void Task_SerComApp6(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[5];
	SERIAL_PORT serialPort = SERIAL_PORT_6;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}

}

void Task_SerComApp7(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[6];
	SERIAL_PORT serialPort = SERIAL_PORT_7;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}

}

void Task_SerComApp8(void* p_arg)
{
	(void)p_arg;

	SERIAL_COM_APP_INFO* pComAppInfo;
	pComAppInfo = &s_SerialComAppInfo[7];
	SERIAL_PORT serialPort = SERIAL_PORT_8;
	SerialComProtocolInit(serialPort, pComAppInfo);

	while(TRUE)
	{
		SerialComProtocolDispose(serialPort, pComAppInfo);

		vTaskDelay(10);
	}

}



/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


