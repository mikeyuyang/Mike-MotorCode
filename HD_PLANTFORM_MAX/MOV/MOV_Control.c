/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	MOV_Control.c
//
// TITLE:	Move control function.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2015-02-02
//
// Description: Control the move of motors
//
//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2015-02-02
// Version:
// Descriptions: 	First Version
//
//------------------------------------------------------------\------------------------------------------
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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <includes.h>

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "MOT_Pwm.h"
#include "COM_CanElmoDrv.h"
#include "UTL_Led.h"
#include "DIO_Control.h"
#include "DBM_Data.h"
#include "MTH_PID.h"
#include "UTL_EEPROM.h"
#include "SYS_Application.h"

#define MOV_CONTROL_GLOBAL_
#include "MOV_Control.h"

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
#define MAX_MOVE_CTRL_APP 3

/* Move control parameters */
MOVE_CTRL g_MoveControl[MOTOR_PORT_MAX] =
{
	/* Port initialize parameter */
	{
		/* Set Point */
		&SPD.m_AxisEnable1,			// Axis Enable
		&SPD.m_PosSource1,			// Whole Range Pulse Number;
		&SPD.m_MechType1,			// Move mechanical type
		&SPD.m_MotDirResvert1,		// Motor direction resvert
		&SPD.m_ECDSignEn1,			// Sign enable		
		&SPD.m_DrvCtrlType1,		// Driver control type
		&SPD.m_MotEncodeLineNum1,	// Line number of the encoder
		&SPD.m_MotGearRatioN1,		// Gear Ratio N
		&SPD.m_MotGearRatioD1,		// Gear Ratio D
		&SPD.m_MotUnitRatio1,		// Unit Ratio
		&SPD.m_MoveCtrlMode1,		// Move control mode 
		&SPD.m_SpdCtrlAcclEn1,		// Whether use acceleration when speed control
		&SPD.m_WholeRangeDis1,		// Whole Range Distance;
		&SPD.m_TrimDisLimt1,		// Trimming start distance, Unit: 0.01°
		&SPD.m_ErrTol1,				// Error tolerance, Unit: 0.01°
		&SPD.m_WaitStopTime1,		// Wait stop time, Unit: 0.01s
		&SPD.m_MaxSpd1,				// Max Speed, Unit: 0.01°/s
		&SPD.m_MinSpd1,				// Min speed, Unit: 0.01°/s
		&SPD.m_Accel1,				// Acceleration,  Unit: 0.1°/s/s
		&SPD.m_Decel1,				// Deceleration,  Unit: 0.1°/s/s
		&SPD.m_PidKp1,				// PID Kp
		&SPD.m_PidKi1,				// PID Ki
		&SPD.m_PidKd1,				// PID Kd
		&SPD.m_IntegralEnRng1,		// PID Integral enable range
		&SPD.m_NegLimitPort1,
		&SPD.m_PosLimitPort1,
		&SPD.m_OriginalPointPort1,
		&SPD.m_DrvFaultPort1,
		&SPD.m_NegLimitVal1,
		&SPD.m_PosLimitVal1,
		&SPD.m_EnableStartUpFindOrig1,
		&SPD.m_FindOrigSpd1,
		&SPD.m_FindOrigMode1,
		&SPD.m_OrigOffSet1,
		/* Control variables */
		&SPD.m_MoveStartCmd1,		// Start command
		&SPD.m_MoveStopCmd1,		// Stop command
		&SPD.m_MoveEmgStopCmd1,		// Emergency Stop command
		&SPD.m_ClearDrvFaultCmd1,
		&SPD.m_MoveSpdSetPoint1,	// Speed set point
		&SPD.m_MovePosSetPoint1,	// Position set point
		&AVD.m_UserPosition1,		// Current point, Unit: 0.01°
		CONTROL_STATE_STOP,			// Control status
		0,							// Current speed, Unit: 0.01°/s
		0,							// Last speed, Unit: 0.01°/s
		MOV_DIR_POS,				// Current direction
		0,							// Start point, Unit: 0.01°
		0,							// End point, Unit: 0.01°
		0,							// Accelerate distance
		0,							// Decelerate distance
		0,							// Moved distance
		0,							// Total distance
		0,							// Current distance
		0,
		0,							// Wait stop time count
		LIMIT_STATUS_NONE,
		0,
		
		/* Control functions */
		&MotorPwmInit,
		&MotorPwmEnable,
		&MotorPwmDisable,
		&MotorPwmStart,
		&MotorPwmStop,
		&MotorPwmSetSpeed,
		&MotorPwmClearFault
	},

	{
		/* Set Point */
		&SPD.m_AxisEnable2,			// Axis Enable
		&SPD.m_PosSource2,			// Whole Range Pulse Number;
		&SPD.m_MechType2,			// Move mechanical type
		&SPD.m_MotDirResvert2,		// Motor direction resvert
		&SPD.m_ECDSignEn2,			// Sign enable		
		&SPD.m_DrvCtrlType2,		// Driver control type
		&SPD.m_MotEncodeLineNum2,	// Line number of the encoder
		&SPD.m_MotGearRatioN2,		// Gear Ratio N
		&SPD.m_MotGearRatioD2,		// Gear Ratio D
		&SPD.m_MotUnitRatio2,		// Unit Ratio
		&SPD.m_MoveCtrlMode2,		// Move control mode 
		&SPD.m_SpdCtrlAcclEn2,		// Whether use acceleration when speed control
		&SPD.m_WholeRangeDis2,		// Whole Range Distance;
		&SPD.m_TrimDisLimt2,		// Trimming start distance, Unit: 0.01°
		&SPD.m_ErrTol2,				// Error tolerance, Unit: 0.01°
		&SPD.m_WaitStopTime2,		// Wait stop time, Unit: 0.01s
		&SPD.m_MaxSpd2,				// Max Speed, Unit: 0.01°/s
		&SPD.m_MinSpd2,				// Min speed, Unit: 0.01°/s
		&SPD.m_Accel2,				// Acceleration,  Unit: 0.1°/s/s
		&SPD.m_Decel2,				// Deceleration,  Unit: 0.1°/s/s
		&SPD.m_PidKp2,				// PID Kp
		&SPD.m_PidKi2,				// PID Ki
		&SPD.m_PidKd2,				// PID Kd
		&SPD.m_IntegralEnRng2,		// PID Integral enable range
		&SPD.m_NegLimitPort2,
		&SPD.m_PosLimitPort2,
		&SPD.m_OriginalPointPort2,
		&SPD.m_DrvFaultPort2,
		&SPD.m_NegLimitVal2,
		&SPD.m_PosLimitVal2,
		&SPD.m_EnableStartUpFindOrig2,
		&SPD.m_FindOrigSpd2,
		&SPD.m_FindOrigMode2,
		&SPD.m_OrigOffSet2,

		/* Control variables */
		&SPD.m_MoveStartCmd2,		// Start command
		&SPD.m_MoveStopCmd2,		// Stop command
		&SPD.m_MoveEmgStopCmd2,		// Emergency Stop command
		&SPD.m_ClearDrvFaultCmd2,
		&SPD.m_MoveSpdSetPoint2,	// Speed set point
		&SPD.m_MovePosSetPoint2,	// Position set point
		&AVD.m_UserPosition2,		// Current point, Unit: 0.01°
		CONTROL_STATE_STOP,			// Control status
		0,							// Current speed, Unit: 0.01°/s
		0,							// Last speed, Unit: 0.01°/s
		MOV_DIR_POS,				// Current direction
		0,							// Start point, Unit: 0.01°
		0,							// End point, Unit: 0.01°
		0,							// Accelerate distance
		0,							// Decelerate distance
		0,							// Moved distance
		0,							// Total distance
		0,							// Current distance
		0,
		0,							// Wait stop time count
		LIMIT_STATUS_NONE,
		0,
		
		/* Control functions */
		&MotorPwmInit,
		&MotorPwmEnable,
		&MotorPwmDisable,
		&MotorPwmStart,
		&MotorPwmStop,
		&MotorPwmSetSpeed,
		&MotorPwmClearFault
	},
	{
		/* Set Point */
		&SPD.m_AxisEnable3,			// Axis Enable
		&SPD.m_PosSource3,			// Whole Range Pulse Number;
		&SPD.m_MechType3,			// Move mechanical type
		&SPD.m_MotDirResvert3,		// Motor direction resvert
		&SPD.m_ECDSignEn3,			// Sign enable		
		&SPD.m_DrvCtrlType3,		// Driver control type
		&SPD.m_MotEncodeLineNum3,	// Line number of the encoder
		&SPD.m_MotGearRatioN3,		// Gear Ratio N
		&SPD.m_MotGearRatioD3,		// Gear Ratio D
		&SPD.m_MotUnitRatio3,		// Unit Ratio
		&SPD.m_MoveCtrlMode3,		// Move control mode 
		&SPD.m_SpdCtrlAcclEn3,		// Whether use acceleration when speed control
		&SPD.m_WholeRangeDis3,		// Whole Range Distance;
		&SPD.m_TrimDisLimt3,		// Trimming start distance, Unit: 0.01°
		&SPD.m_ErrTol3,				// Error tolerance, Unit: 0.01°
		&SPD.m_WaitStopTime3,		// Wait stop time, Unit: 0.01s
		&SPD.m_MaxSpd3,				// Max Speed, Unit: 0.01°/s
		&SPD.m_MinSpd3,				// Min speed, Unit: 0.01°/s
		&SPD.m_Accel3,				// Acceleration,  Unit: 0.1°/s/s
		&SPD.m_Decel3,				// Deceleration,  Unit: 0.1°/s/s
		&SPD.m_PidKp3,				// PID Kp
		&SPD.m_PidKi3,				// PID Ki
		&SPD.m_PidKd3,				// PID Kd
		&SPD.m_IntegralEnRng3,		// PID Integral enable range
		&SPD.m_NegLimitPort3,
		&SPD.m_PosLimitPort3,
		&SPD.m_OriginalPointPort3,
		&SPD.m_DrvFaultPort3,
		&SPD.m_NegLimitVal3,
		&SPD.m_PosLimitVal3,
		&SPD.m_EnableStartUpFindOrig3,
		&SPD.m_FindOrigSpd3,
		&SPD.m_FindOrigMode3,
		&SPD.m_OrigOffSet3,

		/* Control variables */
		&SPD.m_MoveStartCmd3,		// Start command
		&SPD.m_MoveStopCmd3,		// Stop command
		&SPD.m_MoveEmgStopCmd3,		// Emergency Stop command
		&SPD.m_ClearDrvFaultCmd3,
		&SPD.m_MoveSpdSetPoint3,	// Speed set point
		&SPD.m_MovePosSetPoint3,	// Position set point
		&AVD.m_UserPosition3,		// Current point, Unit: 0.01¡ã
		CONTROL_STATE_STOP,			// Control status
		0,							// Current speed, Unit: 0.01¡ã/s
		0,							// Last speed, Unit: 0.01¡ã/s
		MOV_DIR_POS,				// Current direction
		0,							// Start point, Unit: 0.01¡ã
		0,							// End point, Unit: 0.01¡ã
		0,							// Accelerate distance
		0,							// Decelerate distance
		0,							// Moved distance
		0,							// Total distance
		0,							// Current distance
		0,
		0,							// Wait stop time count
		LIMIT_STATUS_NONE,
		0,

		/* Control functions */
		&MotorPwmInit,
		&MotorPwmEnable,
		&MotorPwmDisable,
		&MotorPwmStart,
		&MotorPwmStop,
		&MotorPwmSetSpeed,
		&MotorPwmClearFault
	},
};

PID_HANDLER g_PidHandlers[MOTOR_PORT_MAX] =
{
	{	
		0,			// Set Value
		0,			// Actual Value
		0,			// Current Error
		0,			// Last Error
		0,			// Before Last Error
		0,			// Sum Error
		500,		// Kp
		100,		// Ki
		0,			// Kd
		200,		// Integral Enable Range
		0,			// Out Value
		1000,		// Max Out Value
		10			// Max Out Value
	},
	
	{	
		0,			// Set Value
		0,			// Actual Value
		0,			// Current Error
		0,			// Last Error
		0,			// Before Last Error
		0,			// Sum Error
		500,		// Kp
		100,		// Ki
		0,			// Kd
		200,		// Integral Enable Range
		0,			// Out Value
		1000,		// Max Out Value
		10			// Max Out Value
	},
	{	
		0,			// Set Value
		0,			// Actual Value
		0,			// Current Error
		0,			// Last Error
		0,			// Before Last Error
		0,			// Sum Error
		500,		// Kp
		100,		// Ki
		0,			// Kd
		200,		// Integral Enable Range
		0,			// Out Value
		1000,		// Max Out Value
		10			// Max Out Value
	}	
};

/*
********************************************************************************************************
*	 Local type.
********************************************************************************************************
*/
/* Move Ctrl task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} MoveCtrlTaskInfo;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
TaskHandle_t MoveCtrlTask_Handler[MAX_MOVE_CTRL_APP];
TaskHandle_t FindZeroPosTask_Handler;

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
void Task_MoveCtrl1(void* p_arg);
void Task_MoveCtrl2(void* p_arg);
void Task_MoveCtrl3(void* p_arg);
void Task_FindZeroPos(void* p_arg);


const MoveCtrlTaskInfo MOVE_CTRL_TASK_INFO_TABLE[MAX_MOVE_CTRL_APP] =
{
/*	task function,			task name,				Stack Size						arguments			priority		task handler		*/
	{ Task_MoveCtrl1,		"MoveCtrlTask1",		256,							(void *)0,			15, 		&MoveCtrlTask_Handler[0]},
	{ Task_MoveCtrl2,		"MoveCtrlTask2",		256,							(void *)0,			15, 		&MoveCtrlTask_Handler[1]},
	{ Task_MoveCtrl3,		"MoveCtrlTask3",		256,							(void *)0,			15, 		&MoveCtrlTask_Handler[3]},
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
* Description	: Scanning frame move initilize.
  	1) Input argument  :
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jan,23 2010
**************************************************************************
*/
VOID MoveInit(VOID)
{
	UINT8 cnt;

	//MoveUpdateConfig();
	/* Assign control function for axis */
	for(cnt = 0; cnt < MOTOR_PORT_MAX; cnt ++)
	{
		if(*g_MoveControl[cnt].m_pAxisEnable)
		{
			switch(*g_MoveControl[cnt].m_pDrvCtrlType)
			{
				case DRV_CTRL_TYPE_PWM:
				{
					g_MoveControl[cnt].m_pDrvInit = &MotorPwmInit;
					g_MoveControl[cnt].m_pDrvEn = &MotorPwmEnable;
					g_MoveControl[cnt].m_pDrvDis = &MotorPwmDisable;
					g_MoveControl[cnt].m_pDrvStart = &MotorPwmStart;
					g_MoveControl[cnt].m_pDrvStop = &MotorPwmStop;
					g_MoveControl[cnt].m_pDrvSetSpd = &MotorPwmSetSpeed;
					break;
				}
				case DRV_CTRL_TYPE_ELMO_CAN:
				{
 					g_MoveControl[cnt].m_pDrvInit = &CanElmoDrvInit;
					g_MoveControl[cnt].m_pDrvEn = &CanElmoDriverEn;
					g_MoveControl[cnt].m_pDrvDis= &CanElmoDriverDis;
					g_MoveControl[cnt].m_pDrvStart = &CanElmoStart;
					g_MoveControl[cnt].m_pDrvStop = &CanElmoStop;
					g_MoveControl[cnt].m_pDrvSetSpd = &CanElmoSetSpeed;
					break;
				}
				default:
					break;
			}
			/* Initialize driver port */
			(*g_MoveControl[cnt].m_pDrvInit)((MOTOR_PORT)cnt);
			/* Enable driver */
			(*g_MoveControl[cnt].m_pDrvEn)((MOTOR_PORT)cnt);
			/* Start up fine orignal dispose */
			if(*(g_MoveControl[cnt].m_pEnableStartUpFindOrig))
			{
				/* Save setted control mode */
				g_MoveControl[cnt].m_LastMoveCtrlMode = *(g_MoveControl[cnt].m_pMoveCtrlMode);
				*(g_MoveControl[cnt].m_pMoveCtrlMode) = MOVE_CTRL_MODE_FIND_ORIG;
				*(g_MoveControl[cnt].m_pStartCmd) = 1;
			}

			
			/* Creat Move control tasks */
			xTaskCreate(MOVE_CTRL_TASK_INFO_TABLE[cnt].m_TaskFunc,			 
				MOVE_CTRL_TASK_INFO_TABLE[cnt].m_TaskName,
				MOVE_CTRL_TASK_INFO_TABLE[cnt].m_StackSize,
				MOVE_CTRL_TASK_INFO_TABLE[cnt].m_TaskArg,
				MOVE_CTRL_TASK_INFO_TABLE[cnt].m_TaskPrio,
				MOVE_CTRL_TASK_INFO_TABLE[cnt].m_TaskHandler);
		}
	}
}

/*
**************************************************************************
* Description	: Set move control mode
  	1) Input argument  : port - Motor port for control
						mode - Move Mode
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date		: Jul 17, 2018
**************************************************************************
*/
BOOL MoveSetCtrlMode(MOTOR_PORT port ,MOVE_CTRL_MODE mode)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlHandler = _NULL;

	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlHandler = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	/* Set control mode */
	*((MOVE_CTRL_MODE*)(pCtrlHandler->m_pMoveCtrlMode)) = mode;

	return ret;
}


/*
**************************************************************************
* Description	: Start to move with fix speed
  	1) Input argument  : port - Motor port for control
						spd - Move speed
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date		: Jul 17, 2018
**************************************************************************
*/
BOOL MoveSpdCtrlSetAction(MOTOR_PORT port, INT32 spd)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;

	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	/* Whether port is ready for new action */
	if(CONTROL_STATE_STOP != pCtrlStruct->m_CtrlState)
	{
		ret = FALSE;
		return ret;
	}

	/* Swith  control mode */
	if(*((MOVE_CTRL_MODE*)(pCtrlStruct->m_pMoveCtrlMode)) != MOVE_CTRL_MODE_SPD)
		*((MOVE_CTRL_MODE*)(pCtrlStruct->m_pMoveCtrlMode)) = MOVE_CTRL_MODE_SPD;
	/* Set speed */
	if((abs(spd) > *pCtrlStruct->m_pMaxSpd) || (abs(spd) < *pCtrlStruct->m_pMinSpd))
	{
		/* Spd set over range */
		ret = FALSE;
		return ret;
	}
	*pCtrlStruct->m_pSpdSetPoint = spd;

	/* Start to move */
	*pCtrlStruct->m_pStartCmd = 1;

	/* Set run led blink status */
//	LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_BLINK);

	return ret;
}



/*
**************************************************************************
* Description	: Set move for a special distance
  	1) Input argument  : port - Motor port for control
						dis - Move distance to be set(Unit may various)
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 03, 2015
**************************************************************************
*/
BOOL MovePosCtrlSetAction(MOTOR_PORT port, INT32 dis)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;
//	INT64 calcTemp;
	INT32 setTemp;

	/* Update motor configuration */
//	MotorUpdateConfig();

	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	/* Whether port is ready for new action */
	if((CONTROL_STATE_STOP != pCtrlStruct->m_CtrlState) || (0 == dis))
	{
		ret = FALSE;
		return ret;
	}

	/* Swith  control mode */
	if(*((MOVE_CTRL_MODE*)(pCtrlStruct->m_pMoveCtrlMode)) != MOVE_CTRL_MODE_FIX_POINT)
		*((MOVE_CTRL_MODE*)(pCtrlStruct->m_pMoveCtrlMode)) = MOVE_CTRL_MODE_FIX_POINT;
	/* Set position */
	setTemp = dis + *pCtrlStruct->m_pCurrentPoint;
	if((*pCtrlStruct->m_pNegLimitVal != *pCtrlStruct->m_pPosLimitVal)
		&& ((setTemp < *pCtrlStruct->m_pNegLimitVal) || (setTemp > *pCtrlStruct->m_pPosLimitVal)))
	{
		/* Out of range */
		ret = FALSE;
		return ret;		
	}
	
	*pCtrlStruct->m_pPosSetPoint = dis + *pCtrlStruct->m_pCurrentPoint;

	pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;

	/* Start to move */
	*pCtrlStruct->m_pStartCmd = 1;
//	LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_BLINK);


	return TRUE;
}

/*
**************************************************************************
* Description	: Set move to an absolute position
  	1) Input argument  : port - Motor port for control
						position - Destination(Unit may various)
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MovePosCtrlSetPoint(MOTOR_PORT port, INT32 position)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}


	/* Set destination point */
	if((*pCtrlStruct->m_pNegLimitVal != *pCtrlStruct->m_pPosLimitVal)
		&& ((position < *pCtrlStruct->m_pNegLimitVal) || (position > *pCtrlStruct->m_pPosLimitVal)))
	{
		/* Out of range */
		ret = FALSE;
		return ret;		
	}
	
	*pCtrlStruct->m_pPosSetPoint = position;

	/* Set limit status */
	pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;
	/* Send move command */
	*pCtrlStruct->m_pStartCmd = 1;
//	LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_BLINK);
	
	return ret;
}

/*
**************************************************************************
* Description	: Start to find orignal
  	1) Input argument  : port - Motor port for control
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
VOID MoveFindOrigStart(MOTOR_PORT port)
{
	MOVE_CTRL *pCtrlStruct = _NULL;
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		return;
	}


	/* Save setted control mode */
	pCtrlStruct->m_LastMoveCtrlMode = *(pCtrlStruct->m_pMoveCtrlMode);
	*(pCtrlStruct->m_pMoveCtrlMode) = MOVE_CTRL_MODE_FIND_ORIG;
	pCtrlStruct->m_CtrlState = CONTROL_STATE_STOP;
	*(pCtrlStruct->m_pStartCmd) = 1;
}


/*
**************************************************************************
* Description	: Update the current move speed
  	1) Input argument  : port - Motor port for control
  						pCtrlHandler - move control handler
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: Update the PWM timer according the current move speed 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveUpdateSpeed(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	INT64 i64Temp;
	
	/* Calculate the Pulse pre second */
	i64Temp = pCtrlHandler->m_CurrentSpd;
	i64Temp = i64Temp
				*  (*pCtrlHandler->m_pMotGearRatioD) / (*pCtrlHandler->m_pMotGearRatioN);

	/* Send speed to motor */
	//if(*pCtrlHandler->m_pMotDirResvert)
	//	(*pCtrlHandler->m_pDrvSetSpd)(port, -(INT32)i64Temp);
	//else
		(*pCtrlHandler->m_pDrvSetSpd)(port, (INT32)i64Temp);

	if(i64Temp >= 0)
		pCtrlHandler->m_CurrentDir = MOV_DIR_POS;
	else
		pCtrlHandler->m_CurrentDir = MOV_DIR_NEG;

	return TRUE;
}

/*
**************************************************************************
* Description	: Update the current move speed
  	1) Input argument  : port - Motor port for control
  						pCtrlHandler - move control handler
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: Update the PWM timer according the current move speed 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
VOID SetCurrentAsOrig(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	/* Position source is pulse count */
	if(*pCtrlHandler->m_pPosSource == POS_SOURCE_INTER_PUL_CNT)
	{
		MotorPwmSetOrigin(port);
	}
	else if(*pCtrlHandler->m_pPosSource == POS_SOURCE_EXT_PUL_CNT)
	{
		CanElmoSetOrigin(port);
	}
	else
	{
		if(0 == port)
		{
			SPD.m_UserOrgPoint1 = (UINT16)AVD.m_EncoderPosition1;
			/* Save parameter */
			EEPROMSaveParam(eDataUserOrgPoint1_1);
			EEPROMSaveParam(eDataUserOrgPoint1_2);			
		}
		else if(1 == port)
		{
			SPD.m_UserOrgPoint2 = (UINT16)AVD.m_EncoderPosition2;
			/* Save parameter */
			EEPROMSaveParam(eDataUserOrgPoint2_1);
			EEPROMSaveParam(eDataUserOrgPoint2_2);	
		}
		else if(2 == port)
		{
			SPD.m_UserOrgPoint3 = (UINT16)AVD.m_EncoderPosition3;
			/* Save parameter */
			EEPROMSaveParam(eDataUserOrgPoint3_1);
			EEPROMSaveParam(eDataUserOrgPoint3_2);	
		}
	}
}
/*
**************************************************************************
* Description	: Stop to move
  	1) Input argument  : port - Motor port for control
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveStop(MOTOR_PORT port)
{
	MOVE_CTRL *pCtrlStruct = _NULL;
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		return FALSE;
	}

	(*pCtrlStruct->m_pDrvStop)(port);
	pCtrlStruct->m_CtrlState = CONTROL_STATE_STOP;
	*pCtrlStruct->m_pStartCmd = 0;
	
//	LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);

	return TRUE;
}

/*
**************************************************************************
* Description	: Check whether moving has stopped
  	1) Input argument  : port - Motor port for control
 	2) Modify argument :None
 	3) Output argument :None
* Return      	: TURE - Stop, FALSE - Moving
* Notes       	:
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL IsMoveStop(MOTOR_PORT port)
{
	return g_MoveControl[port].m_CtrlState == CONTROL_STATE_STOP;

}

/*
**************************************************************************
* Description	: Get distance
  	1) Input argument  : 	pCtrlHandler - Move control handler						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: Distance value
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
INT32 MoveGetDistance(MOVE_CTRL *pCtrlHandler)
{
	INT32 calcTemp = 0;
	if((*pCtrlHandler->m_pMechType == MOVE_MECH_TYPE_CIRCULE) 
		&& (abs(*pCtrlHandler->m_pPosSetPoint - *pCtrlHandler->m_pCurrentPoint) > *pCtrlHandler->m_pMotUnitRatio / 2))
	{
		if((*pCtrlHandler->m_pPosSetPoint - *pCtrlHandler->m_pCurrentPoint) > 0)
			calcTemp = *pCtrlHandler->m_pPosSetPoint - *pCtrlHandler->m_pCurrentPoint - *pCtrlHandler->m_pMotUnitRatio;
		else
			calcTemp = *pCtrlHandler->m_pPosSetPoint - *pCtrlHandler->m_pCurrentPoint + *pCtrlHandler->m_pMotUnitRatio;
	}
	else
		calcTemp = *pCtrlHandler->m_pPosSetPoint - *pCtrlHandler->m_pCurrentPoint;

	return calcTemp;
}

/*
**************************************************************************
* Description	: Speed mode control function 
  	1) Input argument  : port - Motor port
  						pCtrlHandler - Move control handler						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveSpeedModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	BOOL ret = TRUE;

	/* Check for stop command */
	if(0 != *(pCtrlHandler->m_pStopCmd))
	{
		*(pCtrlHandler->m_pStopCmd) = 0;
		*(pCtrlHandler->m_pStartCmd) = 0;
		
		if(pCtrlHandler->m_CtrlState == CONTROL_STATE_SPD_TRACK)
		{
			pCtrlHandler->m_AccStep = *pCtrlHandler->m_pDecl * 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s
			pCtrlHandler->m_CtrlState = CONTROL_STATE_DECEL;
		}
	}

	/* Check for emergency stop command */
	if(0 != *(pCtrlHandler->m_pEmgStopCmd))
	{
		if(pCtrlHandler->m_CtrlState != CONTROL_STATE_STOP)
		{
			*(pCtrlHandler->m_pEmgStopCmd) = 0;
			pCtrlHandler->m_CurrentSpd = 0;
			(*pCtrlHandler->m_pDrvStop)(port);
			pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
		}
	}

	/* Move speed control. */
	switch(pCtrlHandler->m_CtrlState)
	{
		case CONTROL_STATE_STOP:
		{
			/* Check whether need start to move */
			if(0 != *(pCtrlHandler->m_pStartCmd))
			{
				*(pCtrlHandler->m_pStartCmd) = 0;
				//if(0 != *(pCtrlHandler->m_pSpdSetPoint))
				//{
					pCtrlHandler->m_CurrentSpd = 0;
					pCtrlHandler->m_LastSpd = 0;
					pCtrlHandler->m_CtrlState = CONTROL_STATE_SPD_TRACK;
					pCtrlHandler->m_AccStep = *pCtrlHandler->m_pAccl* 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s
					/* Update direction */
					if(*(pCtrlHandler->m_pSpdSetPoint) > 0)
						pCtrlHandler->m_CurrentDir = MOV_DIR_POS;
					else if(*(pCtrlHandler->m_pSpdSetPoint) < 0)
						pCtrlHandler->m_CurrentDir = MOV_DIR_NEG;
						
				//}
			}
			break;
		}
		case CONTROL_STATE_SPD_TRACK:
		{
			if(pCtrlHandler->m_CurrentSpd !=  *(pCtrlHandler->m_pSpdSetPoint))
			{
				if(pCtrlHandler->m_CurrentSpd < *(pCtrlHandler->m_pSpdSetPoint))
				{
					/* Target speed is bigger than current speed */
					pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
					if(pCtrlHandler->m_CurrentSpd > *(pCtrlHandler->m_pSpdSetPoint))
					{
						pCtrlHandler->m_CurrentSpd = *(pCtrlHandler->m_pSpdSetPoint);
					}
				}
				else
				{
					/* Target speed is smaller than current speed */
					pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
					if(pCtrlHandler->m_CurrentSpd < *(pCtrlHandler->m_pSpdSetPoint))
					{
						pCtrlHandler->m_CurrentSpd = *(pCtrlHandler->m_pSpdSetPoint);
					}
				}
				if(pCtrlHandler->m_CurrentSpd != 0)
				{
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					if(pCtrlHandler->m_LastSpd == 0)
						(*pCtrlHandler->m_pDrvStart)(port);
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_BLINK);
				}
				else
				{
					/* Stop move */
					(*pCtrlHandler->m_pDrvStop)(port);
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
				}
			}
			pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;
			break;
		}
		case CONTROL_STATE_DECEL:
		{
			if(pCtrlHandler->m_CurrentSpd < 0)
			{
				/* Target speed is bigger than current speed */
				pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd >= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
				}
			}
			else if(pCtrlHandler->m_CurrentSpd > 0)
			{
				/* Target speed is smaller than current speed */
				pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd <= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
				}
			}
			else
			{
				(*pCtrlHandler->m_pDrvStop)(port);
				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;	
//				LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
			}
			/* Update speed */
			MoveUpdateSpeed(port, pCtrlHandler);
			pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;
			break;
		}
		default :
		{
			/* Error dispose. */
		}
	}
	return ret;
}


/*
**************************************************************************
* Description	: Fix point mode control
  	1) Input argument  : port - Motor port for control						
						pCtrlHandler - Move control handler 					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveFixPointModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	BOOL ret = TRUE;
	PID_HANDLER *pPidHandler = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pPidHandler = &g_PidHandlers[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	/* Check for stop command */
	if(0 != *(pCtrlHandler->m_pStopCmd))
	{
		*(pCtrlHandler->m_pStopCmd) = 0;
		if(pCtrlHandler->m_CtrlState == CONTROL_STATE_RUN_TO_TARGET)
		{
			pCtrlHandler->m_AccStep = *pCtrlHandler->m_pDecl * 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s
			pCtrlHandler->m_CtrlState = CONTROL_STATE_DECEL;
		}
	}

	/* Check for emergency stop command */
	if(0 != *(pCtrlHandler->m_pEmgStopCmd))
	{
		if(pCtrlHandler->m_CtrlState != CONTROL_STATE_STOP)
		{
			*(pCtrlHandler->m_pEmgStopCmd) = 0;
			pCtrlHandler->m_CurrentSpd = 0;
			(*pCtrlHandler->m_pDrvStop)(port);
			pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
		}
	}


	/* Move control. */
	switch(pCtrlHandler->m_CtrlState)
	{
		case CONTROL_STATE_STOP:
		{
			/* Check whether need start to move */
			if(0 != *(pCtrlHandler->m_pStartCmd))
			{
				*(pCtrlHandler->m_pStartCmd) = 0;
				pCtrlHandler->m_EndPoint = *(pCtrlHandler->m_pPosSetPoint);
				pCtrlHandler->m_CurrentSpd = 0;
				pCtrlHandler->m_LastSpd = 0;
				if(abs(*(pCtrlHandler->m_pCurrentPoint) - *(pCtrlHandler->m_pPosSetPoint)) > *(pCtrlHandler->m_pErrTol))
				{
					pCtrlHandler->m_CtrlState = CONTROL_STATE_RUN_TO_TARGET;
					pCtrlHandler->m_AccStep = *pCtrlHandler->m_pAccl* 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s

					/* Update PID parameter */
					memset(pPidHandler, 0, sizeof(PID_HANDLER));
					pPidHandler->m_Kp = *pCtrlHandler->m_pPidKp;
					pPidHandler->m_Ki = *pCtrlHandler->m_pPidKi;
					pPidHandler->m_Kd = *pCtrlHandler->m_pPidKd;
					pPidHandler->m_IntegralEnRange = *pCtrlHandler->m_pIntegralEnRng;
					pPidHandler->m_MaxOutVal = *pCtrlHandler->m_pMaxSpd;
					pPidHandler->m_MinOutVal = *pCtrlHandler->m_pMinSpd;

					/* Feed input value */
					pPidHandler->m_SetVal = *pCtrlHandler->m_pPosSetPoint;
					pPidHandler->m_ActVal = *pCtrlHandler->m_pCurrentPoint;
					if(MoveGetDistance(pCtrlHandler) > 0)
						pCtrlHandler->m_CurrentDir = MOV_DIR_POS;
					else if(MoveGetDistance(pCtrlHandler) < 0)
						pCtrlHandler->m_CurrentDir = MOV_DIR_NEG;

//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_BLINK);
				}
			}
			break;
		}
		case CONTROL_STATE_RUN_TO_TARGET:
		{
			if(abs(MoveGetDistance(pCtrlHandler)) > *(pCtrlHandler->m_pErrTol))
			{
				pPidHandler->m_ActVal = *pCtrlHandler->m_pCurrentPoint;
				/* Set error directly */
				pPidHandler->m_Err = MoveGetDistance(pCtrlHandler);
				/* Pid calculate */
				PidProcess(pPidHandler);
				/* Speed adjust */
				if(pPidHandler->m_ActOutVal > pCtrlHandler->m_CurrentSpd)
				{
					if((pPidHandler->m_ActOutVal - pCtrlHandler->m_CurrentSpd) > pCtrlHandler->m_AccStep)
						pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
					else
						pCtrlHandler->m_CurrentSpd = pPidHandler->m_ActOutVal;
				}
				else if(pPidHandler->m_ActOutVal < pCtrlHandler->m_CurrentSpd)
				{
					if((pCtrlHandler->m_CurrentSpd - pPidHandler->m_OutVal) > pCtrlHandler->m_AccStep)
						pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
					else
						pCtrlHandler->m_CurrentSpd = pPidHandler->m_ActOutVal;					
				}

				/* Update speed */
				if(pCtrlHandler->m_CurrentSpd != 0)
				{
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					if(pCtrlHandler->m_LastSpd == 0)
						(*pCtrlHandler->m_pDrvStart)(port);
				}
				else
				{
					/* Stop move */
					(*pCtrlHandler->m_pDrvStop)(port);
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
				}
				pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;
			}
			else
			{
				(*pCtrlHandler->m_pDrvStop)(port);
				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;	
				pCtrlHandler->m_CurrentSpd = 0;
				pCtrlHandler->m_LastSpd = 0;
//				LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
			}
			break;
		}
		case CONTROL_STATE_DECEL:
		{
			if(pCtrlHandler->m_CurrentSpd < 0)
			{
				/* Target speed is bigger than current speed */
				pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd >= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
				}
			}
			else if(pCtrlHandler->m_CurrentSpd > 0)
			{
				/* Target speed is smaller than current speed */
				pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd <= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
//					LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
				}
			}
			else
			{
				(*pCtrlHandler->m_pDrvStop)(port);
				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;	
//				LedSetStatus(LED_PORT_RUN_IND, LED_STATUS_ON);
			}
			/* Update speed */
			MoveUpdateSpeed(port, pCtrlHandler);
			pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;
			break;
		}		
		default :
		{
			/* Error dispose. */
		}
	}
	return TRUE;
}

/*
**************************************************************************
* Description	: Tracking mode control
  	1) Input argument  : port - Motor port for control						
						pCtrlHandler - Move control handler 					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveTrackModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	BOOL ret = TRUE;
	PID_HANDLER *pPidHandler = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pPidHandler = &g_PidHandlers[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	/* Check for stop command */
	if(0 != *(pCtrlHandler->m_pStopCmd))
	{
		*(pCtrlHandler->m_pStopCmd) = 0;
		if((pCtrlHandler->m_CtrlState == CONTROL_STATE_TRACKING) 
			|| (pCtrlHandler->m_CtrlState == CONTROL_STATE_WAIT_TRACK))
		{
			pCtrlHandler->m_AccStep = *pCtrlHandler->m_pDecl * 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s
			pCtrlHandler->m_CtrlState = CONTROL_STATE_DECEL;
		}
	}

	/* Check for emergency stop command */
	if(0 != *(pCtrlHandler->m_pEmgStopCmd))
	{
		if(pCtrlHandler->m_CtrlState != CONTROL_STATE_STOP)
		{
			*(pCtrlHandler->m_pEmgStopCmd) = 0;
			pCtrlHandler->m_CurrentSpd = 0;
			(*pCtrlHandler->m_pDrvStop)(port);
			pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
		}
	}


	/* Move control. */
	switch(pCtrlHandler->m_CtrlState)
	{
		case CONTROL_STATE_STOP:
		{
			/* Check whether need start to move */
			if(0 != *(pCtrlHandler->m_pStartCmd))
			{
				*(pCtrlHandler->m_pStartCmd) = 0;
				pCtrlHandler->m_CurrentSpd = 0;
				pCtrlHandler->m_LastSpd = 0;

				pCtrlHandler->m_CtrlState = CONTROL_STATE_TRACKING;
				pCtrlHandler->m_AccStep = *pCtrlHandler->m_pAccl* 10 / 100;	// Speed change step every 10ms, unit: 0.01°/s

				/* Update PID parameter */
				memset(pPidHandler, 0, sizeof(PID_HANDLER));
				pPidHandler->m_Kp = *pCtrlHandler->m_pPidKp;
				pPidHandler->m_Ki = *pCtrlHandler->m_pPidKi;
				pPidHandler->m_Kd = *pCtrlHandler->m_pPidKd;
				pPidHandler->m_IntegralEnRange = *pCtrlHandler->m_pIntegralEnRng;
				pPidHandler->m_MaxOutVal = *pCtrlHandler->m_pMaxSpd;
				pPidHandler->m_MinOutVal = *pCtrlHandler->m_pMinSpd;

				/* Feed input value */
				pPidHandler->m_SetVal = *pCtrlHandler->m_pPosSetPoint;
				pPidHandler->m_ActVal = *pCtrlHandler->m_pCurrentPoint;

				if(MoveGetDistance(pCtrlHandler) > 0)
					pCtrlHandler->m_CurrentDir = MOV_DIR_POS;
				else if(MoveGetDistance(pCtrlHandler) < 0)
					pCtrlHandler->m_CurrentDir = MOV_DIR_NEG;
			}
			break;
		}
		case CONTROL_STATE_TRACKING:
		{
			pPidHandler->m_ActVal = *pCtrlHandler->m_pCurrentPoint;
			/* Set error directly */
			pPidHandler->m_Err = MoveGetDistance(pCtrlHandler);
			if(abs(pPidHandler->m_Err) <= *(pCtrlHandler->m_pErrTol))
			{
				(*pCtrlHandler->m_pDrvStop)(port);
				pCtrlHandler->m_CtrlState = CONTROL_STATE_WAIT_TRACK;	
				pCtrlHandler->m_CurrentSpd = 0;
				pCtrlHandler->m_LastSpd = 0;
				break;
			}
			
			/* Pid calculate */
			PidProcess(pPidHandler);
			/* Speed adjust */
			if(pPidHandler->m_ActOutVal > pCtrlHandler->m_CurrentSpd)
			{
				if((pPidHandler->m_ActOutVal - pCtrlHandler->m_CurrentSpd) > pCtrlHandler->m_AccStep)
					pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
				else
					pCtrlHandler->m_CurrentSpd = pPidHandler->m_ActOutVal;
			}
			else if(pPidHandler->m_ActOutVal < pCtrlHandler->m_CurrentSpd)
			{
				if((pCtrlHandler->m_CurrentSpd - pPidHandler->m_ActOutVal) > pCtrlHandler->m_AccStep)
					pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
				else
					pCtrlHandler->m_CurrentSpd = pPidHandler->m_ActOutVal;					
			}

			/* Update speed */
			if(pCtrlHandler->m_CurrentSpd != 0)
			{
				/* Update speed */
				MoveUpdateSpeed(port, pCtrlHandler);
				/* Start move */
				if(pCtrlHandler->m_LastSpd == 0)
					(*pCtrlHandler->m_pDrvStart)(port);
			}
			else
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);
			}
			pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;

			break;
		}
		case CONTROL_STATE_WAIT_TRACK:
		{
			if(abs(MoveGetDistance(pCtrlHandler)) > *(pCtrlHandler->m_pErrTol))
			{
				pCtrlHandler->m_CtrlState = CONTROL_STATE_TRACKING;	
			}
			break;
		}
		case CONTROL_STATE_DECEL:
		{
			if(pCtrlHandler->m_CurrentSpd < 0)
			{
				/* Target speed is bigger than current speed */
				pCtrlHandler->m_CurrentSpd += pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd >= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
				}
			}
			else if(pCtrlHandler->m_CurrentSpd > 0)
			{
				/* Target speed is smaller than current speed */
				pCtrlHandler->m_CurrentSpd -= pCtrlHandler->m_AccStep;
				if(pCtrlHandler->m_CurrentSpd <= 0)
				{
					pCtrlHandler->m_CurrentSpd = 0;
					(*pCtrlHandler->m_pDrvStop)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
				}
			}
			else
			{
				(*pCtrlHandler->m_pDrvStop)(port);
				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;				
			}
			/* Update speed */
			MoveUpdateSpeed(port, pCtrlHandler);
			pCtrlHandler->m_LastSpd = pCtrlHandler->m_CurrentSpd;
			break;
		}		
		default :
		{
			/* Error dispose. */
		}
	}
	return TRUE;
}

/*
**************************************************************************
* Description	: Find orig control
  	1) Input argument  : port - Motor port for control						
						pCtrlHandler - Move control handler 					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveFindOrigPoint(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler)
{
	BOOL ret = TRUE;

	/* Check for stop command */
	if(0 != *(pCtrlHandler->m_pStopCmd))
	{
		*(pCtrlHandler->m_pStopCmd) = 0;
		/* Stop move */
		(*pCtrlHandler->m_pDrvStop)(port);
		
		pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP; 
		*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
	}

	/* Check for emergency stop command */
	if(0 != *(pCtrlHandler->m_pEmgStopCmd))
	{
		if(pCtrlHandler->m_CtrlState != CONTROL_STATE_STOP)
		{
			*(pCtrlHandler->m_pEmgStopCmd) = 0;
			pCtrlHandler->m_CurrentSpd = 0;
			(*pCtrlHandler->m_pDrvStop)(port);
			pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;
		}
	}


	/* Move control. */
	switch(pCtrlHandler->m_CtrlState)
	{
		case CONTROL_STATE_STOP:
		{
			/* Check whether need start to move */
			if(0 != *(pCtrlHandler->m_pStartCmd))
			{
				*(pCtrlHandler->m_pStartCmd) = 0;
				pCtrlHandler->m_CurrentSpd = 0;
				pCtrlHandler->m_LastSpd = 0;

				/* Clean position count */
				SetCurrentAsOrig(port, pCtrlHandler);
				
				if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_O_AS_O)
				{
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = -(*pCtrlHandler->m_pFindOrigSpd);
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_ORIG_TRIG;
				}
				else if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_N_AS_O)
				{
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = -*pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_NEG_TRIG;					
				}
				else if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_P_AS_O)
				{
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = *pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_POS_TRIG;						
				}
				else if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_CENTRE_OF_N_AND_P)
				{
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = -*pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_NEG_TRIG;						
				}
				else
					return FALSE;
			}
			break;
		}
		case CONTROL_STATE_FIND_ORIG_TRIG:
		{
			if(DIO_STATUS_ON == GetOrigTrigStatus(port))
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);

				if(*pCtrlHandler->m_pOrigOffSet == 0)
				{
					/* Set orignal */
					SetCurrentAsOrig(port, pCtrlHandler);

					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;	
					*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				}
				else
				{
					pCtrlHandler->m_StartPoint = *pCtrlHandler->m_pCurrentPoint;
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = *pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_RUN_TO_OFFSET;						
				}
			}

			if(abs(*pCtrlHandler->m_pCurrentPoint) > (*pCtrlHandler->m_pWholeRangeDis))
			{
				/* Can't find after on circle, stop to find orignal. */
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);

				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
				*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
			}
			break;
		}
		case CONTROL_STATE_FIND_NEG_TRIG:
		{
			if(DIO_STATUS_ON == GetNegTrigStatus(port))
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);
				if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_N_AS_O)
				{
					/* Set orignal */
					SetCurrentAsOrig(port, pCtrlHandler);

					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
					*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				}
				else if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_CENTRE_OF_N_AND_P)
				{
					/* Set orignal */
					SetCurrentAsOrig(port, pCtrlHandler);
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = *pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_POS_TRIG;					
				}
			}

			if(abs(*pCtrlHandler->m_pCurrentPoint) > (*pCtrlHandler->m_pWholeRangeDis))
			{
				/* Can't find after on circle, stop to find orignal. */
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);

				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
				*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				break;
			}
			MoveUpdateSpeed(port, pCtrlHandler);
			break;
		}			
		case CONTROL_STATE_FIND_POS_TRIG:
		{
			if(DIO_STATUS_ON == GetPosTrigStatus(port))
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);
				if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_P_AS_O)
				{
					/* Set orignal */
					SetCurrentAsOrig(port, pCtrlHandler);

					pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
					*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				}
				else if(*pCtrlHandler->m_pFindOrigMode == FIND_ORIG_MODE_CENTRE_OF_N_AND_P)
				{
					pCtrlHandler->m_StartPoint = (*pCtrlHandler->m_pCurrentPoint);
					/* Run to orignal */
					pCtrlHandler->m_CurrentSpd = -*pCtrlHandler->m_pFindOrigSpd;
					/* Update speed */
					MoveUpdateSpeed(port, pCtrlHandler);
					/* Start move */
					(*pCtrlHandler->m_pDrvStart)(port);
					pCtrlHandler->m_CtrlState = CONTROL_STATE_FIND_NEG_POS_CENTRE;					
				}
			}

			if(abs(*pCtrlHandler->m_pCurrentPoint) > *pCtrlHandler->m_pWholeRangeDis)
			{
				/* Can't find after on circle, stop to find orignal. */
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);

				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
				*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				break;
			}
			MoveUpdateSpeed(port, pCtrlHandler);
			break;
		}
		case CONTROL_STATE_FIND_NEG_POS_CENTRE:
		{
			if(abs((pCtrlHandler->m_StartPoint / 2) - *pCtrlHandler->m_pCurrentPoint) < (*(pCtrlHandler->m_pErrTol) + 10))
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);
				/* Set orignal */
				SetCurrentAsOrig(port, pCtrlHandler);

				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
				*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
				break;
			}
			MoveUpdateSpeed(port, pCtrlHandler);
			break;
		}
		case CONTROL_STATE_FIND_RUN_TO_OFFSET:
		{
			INT32 desTemp = 0;
			if(*pCtrlHandler->m_pMechType == MOVE_MECH_TYPE_CIRCULE)
				desTemp = (pCtrlHandler->m_StartPoint + *pCtrlHandler->m_pOrigOffSet) % 36000;
			else
				desTemp = pCtrlHandler->m_StartPoint + *pCtrlHandler->m_pOrigOffSet;
			if(abs(desTemp - *pCtrlHandler->m_pCurrentPoint) < (*(pCtrlHandler->m_pErrTol) + 10))
			{
				/* Stop move */
				(*pCtrlHandler->m_pDrvStop)(port);
				/* Set orignal */
				SetCurrentAsOrig(port, pCtrlHandler);

				pCtrlHandler->m_CtrlState = CONTROL_STATE_STOP;		
				*(pCtrlHandler->m_pMoveCtrlMode) = pCtrlHandler->m_LastMoveCtrlMode;
			}

			break;
		}		
		default :
		{
			/* Error dispose. */
		}
	}
	return TRUE;
}


/*
**************************************************************************
* Description	: Move Set point control process
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL MoveCtrl(MOTOR_PORT port)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlHandler = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlHandler = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	INT32 posTemp = 0;
	if(*(pCtrlHandler->m_pPosSource) == POS_SOURCE_INTER_PUL_CNT)
	{
		posTemp = MotorPwmGetPostion(port);
		if(*(pCtrlHandler->m_pMechType) == MOVE_MECH_TYPE_CIRCULE) 
		{
			posTemp %= *(pCtrlHandler->m_pMotUnitRatio);
		

			if(*(pCtrlHandler->m_pECDSignEn) == 0)
			{
				if(posTemp < 0)
					posTemp = *(pCtrlHandler->m_pMotUnitRatio) + posTemp;
			}
			else if(*(pCtrlHandler->m_pECDSignEn) == 1)
			{
				if(posTemp > 180)
				{
					posTemp = posTemp - *(pCtrlHandler->m_pMotUnitRatio);
				}
				else if(posTemp <= -180)
				{
					posTemp = posTemp + *(pCtrlHandler->m_pMotUnitRatio);
				}
			}
		}
		*(pCtrlHandler->m_pCurrentPoint) = posTemp;
	}
	else if(*(pCtrlHandler->m_pPosSource) == POS_SOURCE_EXT_PUL_CNT)
	{
		posTemp = CanElmoGetPostion(port);
		if(*(pCtrlHandler->m_pMechType) == MOVE_MECH_TYPE_CIRCULE) 
		{
			posTemp %= *(pCtrlHandler->m_pMotUnitRatio);
			
			if(*(pCtrlHandler->m_pECDSignEn) == 0)
			{
				if(posTemp < 0)
					posTemp = *(pCtrlHandler->m_pMotUnitRatio) + posTemp;
			}
			else if(*(pCtrlHandler->m_pECDSignEn) == 1)
			{
				if(posTemp > 180)
				{
					posTemp = posTemp - *(pCtrlHandler->m_pMotUnitRatio);
				}
				else if(posTemp <= -180)
				{
					posTemp = posTemp + *(pCtrlHandler->m_pMotUnitRatio);
				}
			}
		}
		*(pCtrlHandler->m_pCurrentPoint) = posTemp;
	}

	/* Clean fault dispose */
	if(*pCtrlHandler->m_pClearFaultCmd)
	{
		(*pCtrlHandler->m_pDrvClearFault) (port);
		*pCtrlHandler->m_pClearFaultCmd = 0;
	}

	/* Move control. */
	switch((MOVE_CTRL_MODE)*pCtrlHandler->m_pMoveCtrlMode)
	{
		case MOVE_CTRL_MODE_SPD:
		{
			MoveSpeedModeCtrl(port, pCtrlHandler);
			break;
		}
		case MOVE_CTRL_MODE_FIX_POINT:
		{
			MoveFixPointModeCtrl(port, pCtrlHandler);
			break;
		}
		case MOVE_CTRL_MODE_TRACKING:
		{
			MoveTrackModeCtrl(port, pCtrlHandler);
			break;
		}
		case MOVE_CTRL_MODE_FIND_ORIG:
		{
			MoveFindOrigPoint(port, pCtrlHandler);
			break;
		}
		default :
		{
			/* Error dispose. */
		}
	}
	return TRUE;
}

/*
**************************************************************************
* Description	: Get original trig status
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
DIO_STATUS GetOrigTrigStatus(MOTOR_PORT port)
{
	MOVE_CTRL *pCtrlStruct = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		return DIO_STATUS_OFF;
	}

	if(0 == *(pCtrlStruct->m_pOriginalPointPort))
		return DIO_STATUS_OFF;
	
	return GetDIStatus((DI_PORT)(*pCtrlStruct->m_pOriginalPointPort - 1));

}

/*
**************************************************************************
* Description	: Get negative trig status
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
DIO_STATUS GetNegTrigStatus(MOTOR_PORT port)
{
	MOVE_CTRL *pCtrlStruct = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		return DIO_STATUS_OFF;
	}

	if(0 == *(pCtrlStruct->m_pNegLimitPort))
		return DIO_STATUS_OFF;
	
	return GetDIStatus((DI_PORT)(*pCtrlStruct->m_pNegLimitPort - 1));

}

/*
**************************************************************************
* Description	: Get positive trig status
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
DIO_STATUS GetPosTrigStatus(MOTOR_PORT port)
{
	MOVE_CTRL *pCtrlStruct = _NULL;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		return DIO_STATUS_OFF;
	}

	if(0 == *(pCtrlStruct->m_pPosLimitPort))
		return DIO_STATUS_OFF;
	
	return GetDIStatus((DI_PORT)(*pCtrlStruct->m_pPosLimitPort - 1));

}

/*
**************************************************************************
* Description	: Check and dispose limit event
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL NegLimitTrigDispose(MOTOR_PORT port)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;
	DIO_STATUS diStatus;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	if(0 == *(pCtrlStruct->m_pNegLimitPort))
		return FALSE;
	
	if((MOVE_CTRL_MODE)(*pCtrlStruct->m_pMoveCtrlMode) == MOVE_CTRL_MODE_FIND_ORIG)
	{
		/* Forbid limit dispose while find orig */
		return FALSE;
	}
		

	/* Get Limit status. */
	diStatus = GetDIStatus((DI_PORT)(*pCtrlStruct->m_pNegLimitPort - 1));

	/* Limit dispose */
	if((MOV_DIR_NEG == pCtrlStruct->m_CurrentDir) 
		&& (DIO_STATUS_ON == diStatus)
		/*&& ((*pCtrlStruct->m_pCurrentPoint) < (*pCtrlStruct->m_pNegLimitVal + 500))*/)
	{
		MoveStop(port);
		//pCtrlStruct->m_CtrlState = CONTROL_STATE_STOP;
		pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NEG;
		/* Turn on fault LED */ 		
//			LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_ON);
	}
	else if(LIMIT_STATUS_POS != pCtrlStruct->m_LimitStatus)
	{
		pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;
//			LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_OFF);
	}

	
	if(LIMIT_STATUS_NEG == pCtrlStruct->m_LimitStatus)
	{
		if((MOV_DIR_POS == pCtrlStruct->m_CurrentDir) && (DIO_STATUS_OFF == diStatus))
		{
			if(LIMIT_STATUS_POS != pCtrlStruct->m_LimitStatus)
			{
				pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;
	//			LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_OFF);
			}
		}
	}

	return ret;
}

/*
**************************************************************************
* Description	: Check and dispose limit event
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL PosLimitTrigDispose(MOTOR_PORT port)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;
	DIO_STATUS diStatus;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	if(0 == *(pCtrlStruct->m_pPosLimitPort))
		return FALSE;
	
	if((MOVE_CTRL_MODE)(*pCtrlStruct->m_pMoveCtrlMode) == MOVE_CTRL_MODE_FIND_ORIG)
	{
		/* Forbid limit dispose while find orig */
		return FALSE;
	}

	/* Get Limit status. */
	diStatus = GetDIStatus((DI_PORT)(*pCtrlStruct->m_pPosLimitPort - 1));

	/* Limit dispose */
	if((MOV_DIR_POS == pCtrlStruct->m_CurrentDir) 
		&& (DIO_STATUS_ON == diStatus)
		/*&& ((*pCtrlStruct->m_pCurrentPoint) > (*pCtrlStruct->m_pPosLimitVal - 500))*/)
	{
		MoveStop(port);
		//pCtrlStruct->m_CtrlState = CONTROL_STATE_STOP;
		pCtrlStruct->m_LimitStatus = LIMIT_STATUS_POS;
//		LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_ON);
	}
	else if(LIMIT_STATUS_NEG != pCtrlStruct->m_LimitStatus)
	{
		pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;
//			LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_OFF);
	} 
	
	if(LIMIT_STATUS_POS == pCtrlStruct->m_LimitStatus)
	{
		if((MOV_DIR_NEG == pCtrlStruct->m_CurrentDir) && (DIO_STATUS_OFF == diStatus))
		{
			if(LIMIT_STATUS_NEG != pCtrlStruct->m_LimitStatus)
			{
				pCtrlStruct->m_LimitStatus = LIMIT_STATUS_NONE;
	//			LedSetStatus(LED_PORT_FAULT_IND, LED_STATUS_OFF);
			}
		}
	}
	return ret;
}


/*
**************************************************************************
* Description	: Driver fault dispose
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL DrvFaultDispose(MOTOR_PORT port)
{
	BOOL ret = TRUE;
	MOVE_CTRL *pCtrlStruct = _NULL;
	DIO_STATUS diStatus;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	if(0 == *(pCtrlStruct->m_pDrvFaultPort))
		return FALSE;
	
	/* Get Limit status. */
	diStatus = GetDIStatus((DI_PORT)(*pCtrlStruct->m_pDrvFaultPort - 1));

	/* Driver fault dispose */
	if(DIO_STATUS_ON == diStatus)
	{
		MoveStop(port);
		switch(port)
		{
			case MOTOR_PORT_1:
			{
				AVD.m_SysFault |= SYS_FAULT_DRIVER_FAILED1;
				break;
			}
			case MOTOR_PORT_2:
			{
				AVD.m_SysFault |= SYS_FAULT_DRIVER_FAILED2;
				break;
			}
			case MOTOR_PORT_3:
			{
				AVD.m_SysFault |= SYS_FAULT_DRIVER_FAILED3;
				break;
			}
		}
	}
	else
	{
		switch(port)
		{
			case MOTOR_PORT_1:
			{
				AVD.m_SysFault &= ~SYS_FAULT_DRIVER_FAILED1;
				break;
			}
			case MOTOR_PORT_2:
			{
				AVD.m_SysFault &= ~SYS_FAULT_DRIVER_FAILED2;
				break;
			}
			case MOTOR_PORT_3:
			{
				AVD.m_SysFault &= ~SYS_FAULT_DRIVER_FAILED3;
				break;
			}
		}
	}
	return ret;
}

/*
**************************************************************************
* Description	: Check and dispose limit event
  	1) Input argument  : port - Motor port for control						
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 04, 2015
**************************************************************************
*/
BOOL OrigPointTrigDispose(MOTOR_PORT port)
{
	BOOL ret = FALSE;
	MOVE_CTRL *pCtrlStruct = _NULL;
	DIO_STATUS diStatus;
	
	/* Get move control struct for port */
	if(MOTOR_PORT_MAX > port)
	{
		pCtrlStruct = &g_MoveControl[port];
	}
	else
	{
		ret = FALSE;
		return ret;
	}

	if(0 == *(pCtrlStruct->m_pOriginalPointPort))
		return FALSE;
	

	/* Get Limit status. */
	diStatus = GetDIStatus((DI_PORT)(*pCtrlStruct->m_pOriginalPointPort - 1));

	if(DIO_STATUS_ON == diStatus)
	{
		MoveStop(port);
		MotorPwmSetOrigin(port);
		ret = TRUE;
	}

	return ret;
}



/*
**************************************************************************
* Description	: Move control task1.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
VOID Task_MoveCtrl1(void* p_arg)
{
	(void)p_arg;


	while(TRUE)
	{
		MoveCtrl(MOTOR_PORT_1);
		NegLimitTrigDispose(MOTOR_PORT_1);
		PosLimitTrigDispose(MOTOR_PORT_1);
//		DrvFaultDispose(MOTOR_PORT_1);

		vTaskDelay(MOVE_CTRL_TASK_PERIOD);
	}
}

/*
**************************************************************************
* Description	: Move control task2.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
VOID Task_MoveCtrl2(void* p_arg)
{
	(void)p_arg;


	while(TRUE)
	{
		MoveCtrl(MOTOR_PORT_2);
		NegLimitTrigDispose(MOTOR_PORT_2);
		PosLimitTrigDispose(MOTOR_PORT_2);
//		DrvFaultDispose(MOTOR_PORT_2);
		vTaskDelay(MOVE_CTRL_TASK_PERIOD);
	}
}

/*
**************************************************************************
* Description	: Move control task3.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
VOID Task_MoveCtrl3(void* p_arg)
{
	(void)p_arg;
	
	while(TRUE)
	{
		MoveCtrl(MOTOR_PORT_3);
		NegLimitTrigDispose(MOTOR_PORT_3);
		PosLimitTrigDispose(MOTOR_PORT_3);
		DrvFaultDispose(MOTOR_PORT_1);
		vTaskDelay(MOVE_CTRL_TASK_PERIOD);
	}
}



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

