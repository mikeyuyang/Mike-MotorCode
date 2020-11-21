/*
//######################################################################################################
//
// FILE:	COM_ModbusTCP.c
//
// TITLE:	This function realized the Modbus Tcp server funciton(LWIP).
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: Set up a Modbus TCP Server.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris.Shang
// Created date:   	Feb 3, 2018
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
#include <stdio.h>
#include "includes.h"

#include "sys.h"
#include "api.h"
#include "sockets.h"

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "COM_ModbusRTU.h"
#include "MOV_Control.h"
#define	COM_MODBUS_TCP_GLOBAL_
#include "COM_ModbusTCP.h"
#include "UTL_CRC.h"
#include "UTL_EEPROM.h"
/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/

/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

#define  MODBUS_TCP_TASK_PRIO    10
#define  MODBUS_TCP_PORT		9000

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

/* COM task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} TcpServerTaskInfo;

/*
********************************************************************************************************
*	 Local variables
********************************************************************************************************
*/

TaskHandle_t TcpServerTask_Handler;

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
typedef	enum tagTCPFUNCTION_CODE
{

	FUNCTION_CODE_GET_ANGLE = 0x41,
	FUNCTION_CODE_SET_CURRENT_AS_ORG = 0x42,
	FUNCTION_CODE_RELATIVE_MOVE = 0x43,
	FUNCTION_CODE_ABSOLUTE_MOVE = 0x44,
	FUNCTION_CODE_STOP_MOVE = 0x45,
	FUNCTION_CODE_SET_SPEED = 0x46,
  FUNCTION_CODE_SET_MODEL = 0x4C,
	FUNCTION_CODE_MAX
}TCP_FUNCTION_CODE;
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
TCPFUNCTION_CODE TCP_SEND_DATA;

void ModbusTcpServerProcess(int conn) 
{
	int buflen = 500;
	int readRet;
	unsigned char recv_buffer[500];

	/* Read data for modbus tcp */
	while(1)
	{
		readRet = read(conn, recv_buffer, buflen); 
		if ( readRet > 0 )
		{
		  ProcessModbusTcpFrame(recv_buffer,&TCP_SEND_DATA,conn, 0);	
			memset(&recv_buffer, 0, 100);
		}
		else if(readRet < 0)
			return;
	}
}


VOID ProcessModbusTcpFrame(unsigned char *pQueue,TCPFUNCTION_CODE *msg,int conn, UINT8 slvAdd)
{
	
	UINT8  functionCode;
	UINT16 u16CalcTemp = 0;
	INT32 i32CalcTemp;
	int  wrtRet;

#if(ENABLE_CRC)
	if(0 != GenMBCRC16( pQueue->m_buffer, pQueue->m_count))
	{
		pQueue->m_count = 0;
		return;
	}
	pQueue->m_count -= 2;
#endif

  functionCode=pQueue[1];
	switch(functionCode)
	{	
		case FUNCTION_CODE_GET_ANGLE:
		{
			msg->cmd_head=pQueue[0];  
	    msg->cmd_type=pQueue[1];
			msg->m_AxisEnable1[0] = (AVD.m_UserPosition1 >> 24) & 0xFF;
			msg->m_AxisEnable1[1] = (AVD.m_UserPosition1 >> 16) & 0xFF;
			msg->m_AxisEnable1[2] = (AVD.m_UserPosition1 >> 8) & 0xFF;
			msg->m_AxisEnable1[3] = AVD.m_UserPosition1 & 0xFF;
			 
			msg->m_AxisEnable2[0] = (AVD.m_UserPosition2 >> 24) & 0xFF;
			msg->m_AxisEnable2[1] = (AVD.m_UserPosition2 >> 16) & 0xFF;
			msg->m_AxisEnable2[2] = (AVD.m_UserPosition2 >> 8) & 0xFF;
			msg->m_AxisEnable2[3] = AVD.m_UserPosition2 & 0xFF;
			   
			if(AVD.m_UserPosition3>=0)
			{
				AVD.m_UserPosition3=AVD.m_UserPosition3%36000;
				if(AVD.m_UserPosition3>18000)
				AVD.m_UserPosition3=AVD.m_UserPosition3-36000;
				msg->m_AxisEnable3[0] = ((AVD.m_UserPosition3) >> 24) & 0xFF;
				msg->m_AxisEnable3[1] = ((AVD.m_UserPosition3) >> 16) & 0xFF;
				msg->m_AxisEnable3[2] = ((AVD.m_UserPosition3) >> 8) & 0xFF;
				msg->m_AxisEnable3[3] = (AVD.m_UserPosition3) & 0xFF;
			}
			else
			{
				AVD.m_UserPosition3=-(AVD.m_UserPosition3%36000);
				if(AVD.m_UserPosition3<-18000)
				AVD.m_UserPosition3=AVD.m_UserPosition3+36000;	
				msg->m_AxisEnable3[0] = (-(AVD.m_UserPosition3) >> 24) & 0xFF;
				msg->m_AxisEnable3[1] = (-(AVD.m_UserPosition3) >> 16) & 0xFF;
				msg->m_AxisEnable3[2] = (-(AVD.m_UserPosition3) >> 8) & 0xFF;
				msg->m_AxisEnable3[3] = (-(AVD.m_UserPosition3)) & 0xFF;
			}
			msg->m_CRC[0]=((GenMBCRC16((UCHAR*)msg,14))>>8) & 0xFF;
			msg->m_CRC[1]=GenMBCRC16((UCHAR*)msg,14) & 0xFF;
			
			wrtRet = write(conn, &TCP_SEND_DATA, 16);
			break;
		}
		case FUNCTION_CODE_SET_CURRENT_AS_ORG:
		{
			if(1 == pQueue[2])
			{
				SPD.m_UserOrgPoint1 = (UINT16)AVD.m_EncoderPosition1;
				/* Save parameter */
				EEPROMSaveParam(eDataUserOrgPoint1_1);
				EEPROMSaveParam(eDataUserOrgPoint1_2);
				MotorPwmSetOrigin(MOTOR_PORT_1);
		
			}
			else if(2== pQueue[2])
			{
				SPD.m_UserOrgPoint2 = (UINT16)AVD.m_EncoderPosition2;
				/* Save parameter */
				EEPROMSaveParam(eDataUserOrgPoint2_1);
				EEPROMSaveParam(eDataUserOrgPoint2_2);
				MotorPwmSetOrigin(MOTOR_PORT_2);
			}
			
			else if(3 == pQueue[2])
			{
				SPD.m_UserOrgPoint3 = (UINT16)AVD.m_EncoderPosition3;
				/* Save parameter */
				EEPROMSaveParam(eDataUserOrgPoint3_1);
				EEPROMSaveParam(eDataUserOrgPoint3_2);
				MotorPwmSetOrigin(MOTOR_PORT_3);
			}			
			else
			{
				pQueue[2] |= 0x80;
			}
			break;
		}
		case FUNCTION_CODE_RELATIVE_MOVE:
		{
			i32CalcTemp = 0;
			i32CalcTemp |= pQueue[3];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue[4];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue[5];
			i32CalcTemp <<= 8;		
			i32CalcTemp |= pQueue[6];
			
			if(pQueue[2]==1)
			{ 
			MovePosCtrlSetAction(MOTOR_PORT_1,i32CalcTemp);}
			if(pQueue[2]==2)
		  {
			MovePosCtrlSetAction(MOTOR_PORT_2,i32CalcTemp);}
			if(pQueue[2]==3)
		  {
			MovePosCtrlSetAction(MOTOR_PORT_3,i32CalcTemp);}
			break;
		}
		case FUNCTION_CODE_ABSOLUTE_MOVE:
		{
			i32CalcTemp = 0;
			i32CalcTemp |= pQueue[3];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue[4];
			i32CalcTemp <<= 8;
			i32CalcTemp |= pQueue[5];
			i32CalcTemp <<= 8;		
			i32CalcTemp |= pQueue[6];

			if(pQueue[2]==1)
			{ 
			MovePosCtrlSetPoint(MOTOR_PORT_1,i32CalcTemp);}
			if(pQueue[2]==2)
		  {
			MovePosCtrlSetPoint(MOTOR_PORT_2,i32CalcTemp);}
			if(pQueue[2]==3)
		  {
			MovePosCtrlSetPoint(MOTOR_PORT_3,i32CalcTemp);}
		  break;
		}
		case FUNCTION_CODE_STOP_MOVE:
		{
			if(1 == pQueue[2])
			{
				MoveStop(MOTOR_PORT_1);
			}
				if(2 == pQueue[2])
			{
				MoveStop(MOTOR_PORT_2);
			}
				if(3 == pQueue[2])
			{
				MoveStop(MOTOR_PORT_3);
			}
			break;
		}
		case FUNCTION_CODE_SET_SPEED:
		{
			u16CalcTemp = pQueue[3];
			u16CalcTemp <<= 8;
			u16CalcTemp += pQueue[4];

			if(u16CalcTemp > 1800)
			{
				pQueue[1] |= 0x80;
			}

			if(1 == pQueue[2])
			{
				SPD.m_MaxSpd1 = u16CalcTemp;
				EEPROMSaveParam(eDataAxis1MaxSpd1);
			}
			else if(2 == pQueue[2])
			{
				SPD.m_MaxSpd2 = u16CalcTemp;
				EEPROMSaveParam(eDataAxis1MaxSpd2);
			}

			else if(3 == pQueue[2])
			{
				SPD.m_MaxSpd3 = u16CalcTemp;
				EEPROMSaveParam(eDataAxis1MaxSpd3);
			}
			else
			{
				pQueue[1] |= 0x80;
			}
			break;
		}
			case FUNCTION_CODE_SET_MODEL:
		{
				if(1 == pQueue[2])
			{
				SPD.m_MoveCtrlMode1=pQueue[3];
				EEPROMSaveParam(eMoveCtrlMode1);
			}
				if(2 == pQueue[2])
			{
				SPD.m_MoveCtrlMode2=pQueue[3];
				EEPROMSaveParam(eMoveCtrlMode2);
			}
				if(3 == pQueue[2])
			{
				SPD.m_MoveCtrlMode3=pQueue[3];
				EEPROMSaveParam(eMoveCtrlMode3);
			}
			break;
		}
		default:
		{
			/* Illegal function code. */
			pQueue[1] |= 0x80;
		}
	}
}
/*
*********************************************************************************************************
* 	Global function implementations
*********************************************************************************************************
*/

/*
**************************************************************************
* Description	: Modbus Tcp server Process
	1) Input argument  : conn - Tcp connect
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
* Author		: Chris.Shang
* Date		: Feb 10, 2018
**************************************************************************
*/



/*
**************************************************************************
* Description	: Modbus Tcp server task
	1) Input argument  : None
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
* Author		: Chris.Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
static void ModbusTcpServer_Task(void *arg)
{
	int sock, newconn, size;
	struct sockaddr_in address, remotehost;


	/* create a TCP socket */
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		/* can not create socket */;
		vTaskDelay(10);
	}

	address.sin_family = AF_INET;
	address.sin_port = htons(MODBUS_TCP_PORT);	 // mosbus tcp port
	address.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
	{
		/* Can not bind socket*/
		close(sock);
		vTaskDelay(10);
	}

	/* listen for incoming connections (TCP listen backlog = 1) */
	listen(sock, 1);

	while(1)
	{
		size = sizeof(remotehost);
		newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);

		ModbusTcpServerProcess(newconn);
		close(newconn);
	}
}
/*
**************************************************************************
* Description	: Initilize tcp server
	1) Input argument  : None
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
* Author		: Chris.Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
void InitModbusTcpServer(void)
{
  sys_thread_new("ModbusTcpServerTask", ModbusTcpServer_Task, NULL, 2 * DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}

/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


