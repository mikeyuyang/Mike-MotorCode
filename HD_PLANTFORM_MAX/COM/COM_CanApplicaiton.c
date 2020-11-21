/*
//######################################################################################################
//
// FILE:	CAN_SerialApplicaiton.c
//
// TITLE:	This function realized the CAN communication functions.
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: All communication function for CAN ports.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris.Shang
// Created date:   	Nov 6, 2017
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
#include "UTL_CAN.h"
//#include "UTL_OCTimer.h"
#include "UTL_EEPROM.h"
//#include "APP_Control.h"
#include "COM_CanElmoDrv.h"

#define	COM_CAN_APPLICAITON_GLOBAL_
#include "COM_CanApplicaiton.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define MAX_CAN_COM_APP 2
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
/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/
/* Serial Communication applicaiton handle */
typedef struct tagCAN_COM_APP_INFO
{
	BOOL m_Enable;
	CAN_COM_PROTOCOL_TYPE m_PrptocolType;
	CAN_PARAM m_CanParam;
	UINT8 m_SlaveAdd;
}CAN_COM_APP_INFO;

/* COM task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} CanComTaskInfo;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
static CAN_COM_APP_INFO s_CanComAppInfo[MAX_CAN_COM_APP];
TaskHandle_t CanComAppTask_Handler[MAX_CAN_COM_APP];

/*
*********************************************************************************************************
*    Local function prototypes
*********************************************************************************************************
*/
void Task_CanComApp1(void* p_arg);
void Task_CanComApp2(void* p_arg);


const CanComTaskInfo CAN_COM_TASK_INFO_TABLE[MAX_CAN_COM_APP] =
{
/*	task function,			task name,				Stack Size		  				arguments			priority		task handler		*/
	{ Task_CanComApp1,		"CanComTask1",			256,							(void *)0,			13,			&CanComAppTask_Handler[0]},
	{ Task_CanComApp2,		"CanComTask2",			256,							(void *)0,			13, 		&CanComAppTask_Handler[1]},
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
* Description	: Initilize Can communicaiton applicaiton, initilize UART ports, protocol and creat task
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
void InitCanComApp(void)
{
	UINT8 cnt1 = 0;

	/* Copy eeprom parameter to local */
	s_CanComAppInfo[0].m_Enable = (BOOL)SPD.m_CanEnable1;
	s_CanComAppInfo[0].m_PrptocolType = (CAN_COM_PROTOCOL_TYPE)SPD.m_CanProtocolType1;
	s_CanComAppInfo[0].m_CanParam.m_Baudrate = (CAN_BANDRATE)SPD.m_CanBrandRate1;
	s_CanComAppInfo[0].m_CanParam.m_TimeOut= 5;
	s_CanComAppInfo[0].m_SlaveAdd = SPD.m_CanSlaveAddress1;
	
	s_CanComAppInfo[1].m_Enable = (BOOL)SPD.m_CanEnable2;
	s_CanComAppInfo[1].m_PrptocolType = (CAN_COM_PROTOCOL_TYPE)SPD.m_CanProtocolType2;
	s_CanComAppInfo[1].m_CanParam.m_Baudrate = (CAN_BANDRATE)SPD.m_CanBrandRate2;
	s_CanComAppInfo[1].m_CanParam.m_TimeOut= 5;
	s_CanComAppInfo[1].m_SlaveAdd = SPD.m_CanSlaveAddress2;


	for(cnt1 = 0; cnt1 < CAN_PORT_MAX; cnt1 ++)
	{
		if(s_CanComAppInfo[cnt1].m_Enable)
		{
			/* Initlize serial port */
			InitCanPort((CAN_PORT)cnt1, &s_CanComAppInfo[cnt1].m_CanParam);
			/* Creat serial communication tasks */
			xTaskCreate(CAN_COM_TASK_INFO_TABLE[cnt1].m_TaskFunc,			 
				CAN_COM_TASK_INFO_TABLE[cnt1].m_TaskName,
				CAN_COM_TASK_INFO_TABLE[cnt1].m_StackSize,
				CAN_COM_TASK_INFO_TABLE[cnt1].m_TaskArg,
				CAN_COM_TASK_INFO_TABLE[cnt1].m_TaskPrio,
				CAN_COM_TASK_INFO_TABLE[cnt1].m_TaskHandler);
		}
	}

	return;
}

/*
**************************************************************************
* Description	: Can communication application tasks
  	1) Input argument  : None.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
void Task_CanComApp1(void* p_arg)
{
	(void)p_arg;
	CAN_MSG canMsg;
	UINT8 cnt = 0;

	while(TRUE)
	{
		canMsg.m_ExtId = 0x321;
		canMsg.m_StdId = 0x321;
		canMsg.m_DLC = 8;
		canMsg.m_IDE = 0;
		canMsg.m_RTR= 0;
		canMsg.m_Data[0] = cnt;
		cnt ++;
		//CanPushMsg(CAN_PORT_1, &canMsg);

		CanElmoDrvLoop(CAN_PORT_1);	

		vTaskDelay(3);
	}
}

void Task_CanComApp2(void* p_arg)
{
	(void)p_arg;
	CAN_MSG canMsg;
	UINT8 cnt = 0;

	while(TRUE)
	{
		canMsg.m_ExtId = 0x321;
		canMsg.m_StdId = 0x321;
		canMsg.m_DLC = 8;
		canMsg.m_IDE = 0;
		canMsg.m_RTR= 0;
		canMsg.m_Data[0] = cnt;
		cnt ++;
		//CanPushMsg(CAN_PORT_2, &canMsg);
		//CanElmoSendReadPosCmd(MOTOR_PORT_2);
		
		//CanElmoDrvLoop(CAN_PORT_2, s_CanComAppInfo[1].m_SlaveAdd);		

		vTaskDelay(10);
	}
}


/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


