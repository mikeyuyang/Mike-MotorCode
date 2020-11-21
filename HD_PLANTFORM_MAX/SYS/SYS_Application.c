/*
//######################################################################################################

// FILE:	SYS_Application.c
//
// TITLE: System application functions.
//
// AUTHOR:	
//
// DATE:	
//
// Description: User application functions will be put here.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		
// Created date:   	
// Version:
// Descriptions: 	Frist Version
//
//------------------------------------------------------------------------------------------------------

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
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include <includes.h>

#include "BSP_BasicDataType.h"	// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "MOV_Control.h"
#include "DIO_Control.h"

#define   SYS_APPLICATION_GLOBAL_
#include "SYS_Application.h"
#include "UTL_Led.h"

extern MOVE_CTRL g_MoveControl[MOTOR_PORT_MAX];
/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
/* LED Control Struct */
typedef struct tagLED_CTRL_STRUCT
{
	UINT8 m_Status;
	UINT8 m_Freq;
	UINT8 m_Reload;
	UINT8 m_DelayCnt;
}LED_CTRL_STRUCT;

/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
#define FIND_ORIGINAL_POINT_SPD -1000

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
#if 0
LED_CTRL_STRUCT LedControlStructs[LED_PORT_MAX] =
{
	{LED_STATUS_BLINK,	1,	10,	0},
	{LED_STATUS_OFF,	2,	5,	0},
	{LED_STATUS_ON,		2,	5,	0},
	{LED_STATUS_OFF,	2,	5,	0},
};
#endif
/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/

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
* Description	: System application initialization.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: 
* Date		: Jul 17, 2018
**************************************************************************
*/ 
VOID SysAppInit(VOID)
{
	g_SystemStatus = SYS_STATUS_IDLE;
}

/*
**************************************************************************
* Description	: Switch system status.
  	1) Input argument  : status - System Status
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: 
* Date		: Jul 17, 2018
**************************************************************************
*/ 
VOID SwitchSysStatus(SYS_STATUS status)
{
	g_SystemStatus = status;
}

/*
**************************************************************************
* Description	: Start to fine original point.
  	1) Input argument  : port - Port for operation
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: 
* Date		: Jul 17, 2018
**************************************************************************
*/ 
VOID StartToFindOriginalPoint(MOTOR_PORT port)
{
	/* Set Move control mode */
	MoveSpdCtrlSetAction(port, FIND_ORIGINAL_POINT_SPD);
	/* Switch system status to fine original point */
	//SwitchSysStatus(SYS_STATUS_FIND_ORIG_X);
}


/*
**************************************************************************
* Description	: System application task.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: 
* Date		: Jul 17, 2018
**************************************************************************
*/
VOID Task_SysApp(void* p_arg)
{
	(void)p_arg;
	/* Wait for 3s when power up */
	vTaskDelay(2000);
	MOVE_CTRL *pCtrlStruct = _NULL;

	/* Start to find original point when system power up */
	//StartToFindOriginalPoint(MOTOR_PORT_1);
	//g_SystemStatus = SYS_STATUS_FIND_ORIG_X;
	//StartToFindOriginalPoint(MOTOR_PORT_2);

	
	while(TRUE)
	{
		switch(g_SystemStatus)
		{
			case SYS_STATUS_IDLE:
			{
				break;
			}
			case SYS_STATUS_FIND_ORIG_X:
			{
				if(DIO_STATUS_ON == GetOrigTrigStatus(MOTOR_PORT_1))
				{
					MoveStop(MOTOR_PORT_1);
					MovePosCtrlSetAction(MOTOR_PORT_1 , 17580);
					g_SystemStatus = SYS_STATUS_TRACK_POINT_X;
				}
				break;
			}
			case SYS_STATUS_TRACK_POINT_X:
			{
					if(MOTOR_PORT_MAX > MOTOR_PORT_1)
					{
						pCtrlStruct = &g_MoveControl[MOTOR_PORT_1];
					}
					vTaskDelay(300);
					if(CONTROL_STATE_STOP == pCtrlStruct->m_CtrlState) 
					{
						
						g_SystemStatus = SYS_STATUS_FIND_ORIG_Y;
						MotorPwmSetOrigin(MOTOR_PORT_1);
						StartToFindOriginalPoint(MOTOR_PORT_2);
					}
				
				break;
			}
			case SYS_STATUS_FIND_ORIG_Y:
			{
				if(DIO_STATUS_ON == GetOrigTrigStatus(MOTOR_PORT_2))
				{
					MoveStop(MOTOR_PORT_2);
					MovePosCtrlSetAction(MOTOR_PORT_2 , 12000);
					g_SystemStatus = SYS_STATUS_TRACK_POINT_Y;
				}
				break;
			}
			case SYS_STATUS_TRACK_POINT_Y:
			{
					if(MOTOR_PORT_MAX > MOTOR_PORT_2)
					{
						pCtrlStruct = &g_MoveControl[MOTOR_PORT_2];
					}
					vTaskDelay(300);
					if(CONTROL_STATE_STOP == pCtrlStruct->m_CtrlState) 
					{
						
						g_SystemStatus = SYS_STATUS_IDLE;
						MotorPwmSetOrigin(MOTOR_PORT_2);
					}
				
				break;
			}
			default:
			{
				
			}
		}
		vTaskDelay(5);
	}
}


// Some Led Application
#if 0

/* Led control */
/* Move indicate LED dispose */
if((CONTROL_STATE_STOP != g_MoveControl[MOTOR_PORT_1].m_CtrlState)
		|| (CONTROL_STATE_STOP != g_MoveControl[MOTOR_PORT_2].m_CtrlState)
		|| (CONTROL_STATE_STOP != g_MoveControl[MOTOR_PORT_3].m_CtrlState))
{
	if(moveLedBLinkFlag == FALSE)
	{
//		LedSetStatus(LED_PORT_MOVE_IND, LED_STATUS_BLINK);
		moveLedBLinkFlag = TRUE;
	}
}
else
{
//	LedSetStatus(LED_PORT_MOVE_IND, LED_STATUS_OFF);
	moveLedBLinkFlag = FALSE;
}

/* Limit indicate LED dispose */
if((LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_1].m_LimitStatus)
		|| (LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_2].m_LimitStatus)
		|| (LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_3].m_LimitStatus))
{
//	LedSetStatus(LED_PORT_LIMT_IND, LED_STATUS_ON);
//			limLedBLinkFlag = TRUE;
}
else
{
//	LedSetStatus(LED_PORT_LIMT_IND, LED_STATUS_OFF);
//			limLedBLinkFlag = FALSE;
}
#endif

/*
*********************************************************************************************************
*                                           #error section
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           End of file
*********************************************************************************************************
*/

