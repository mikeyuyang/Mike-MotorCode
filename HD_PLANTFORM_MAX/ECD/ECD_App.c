/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	ECD_App.c
//
// TITLE:	Encoder application.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Mar 27, 2015
//
// Description: This file include source code for encoder Application operation.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Mar 31, 2015
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "includes.h"

#include <stm32f4xx.h>

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "DBM_Data.h"
#include "ECD_SSI.h"
#include "MOV_Control.h"
#define   ECD_APP_GLOBAL_
#include "ECD_APP.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define ENCODER_TASK_PERIOD 5	//ms
/*
*********************************************************************************************************
*    Local data type
*********************************************************************************************************
*/


/* Encoder Control Struct */
typedef struct tagENCODER_APP_STRUCT
{
	/* Setpoint */
	UINT16* m_pEnable;
	UINT16* m_pECDType;
	UINT16* m_pECDSingleTurnBitNum;
	UINT16* m_pECDMultiTurnBitNum;
	UINT16* m_pECDGearRatioN;
	UINT16* m_pECDGearRatioD;
	UINT16* m_pECDSignEn;
	UINT32*	m_pMotUnitRatio;	
	UINT16* m_pMechType;
	INT32* m_pUserOrgPoint;
	/* Actual vaule */
	INT32* m_pEncoderPosition;
	INT32* m_pUserPosition;
	/* Encoder operation function */
	VOID (*m_pEncoderInit) (ENCODER_PORT port);
	INT32 (*m_pEncoderRead) (ENCODER_PORT port);
}ENCODER_APP_STRUCT;

/* Encoder application task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} EncoderAppTaskInfo;


/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
ENCODER_APP_STRUCT ECDAppStructs[ENCODER_PORT_MAX] =
{
	{
		/* Setpoint */
		&SPD.m_ECDEnable1,
		&SPD.m_ECDType1,
		&SPD.m_ECDSingleTurnBitNum1,
		&SPD.m_ECDMultiTurnBitNum1,
		&SPD.m_ECDGearRatioN1,
		&SPD.m_ECDGearRatioD1,
		&SPD.m_ECDSignEn1,
		&SPD.m_MotUnitRatio1,
		&SPD.m_MechType1,
		&SPD.m_UserOrgPoint1,
		/* Actual vaule */
		&AVD.m_EncoderPosition1,
		&AVD.m_UserPosition1,
		/* Function */
		NULL,
		NULL,
	},

	{
		/* Setpoint */
		&SPD.m_ECDEnable2,
		&SPD.m_ECDType2,
		&SPD.m_ECDSingleTurnBitNum2,
		&SPD.m_ECDMultiTurnBitNum2,
		&SPD.m_ECDGearRatioN2,
		&SPD.m_ECDGearRatioD2,
		&SPD.m_ECDSignEn2,
		&SPD.m_MotUnitRatio2,
		&SPD.m_MechType2,
		&SPD.m_UserOrgPoint2,
		/* Actual vaule */
		&AVD.m_EncoderPosition2,
		&AVD.m_UserPosition2,
		/* Function */
		NULL,
		NULL,
	},
#if 0
	{
		/* Setpoint */
		&SPD.m_ECDEnable3,
		&SPD.m_ECDType3,
		&SPD.m_ECDSingleTurnBitNum3,
		&SPD.m_ECDMultiTurnBitNum3,
		&SPD.m_ECDGearRatioN3,
		&SPD.m_ECDGearRatioD3,
		&SPD.m_ECDSignEn3,
		&SPD.m_MotUnitRatio3,	
		&SPD.m_MechType3,
		&SPD.m_UserOrgPoint3,
		/* Setpoint */
		&AVD.m_EncoderPosition3,
		&AVD.m_UserPosition3,
		/* Function */
		NULL,
		NULL,
	}
#endif	
};

TaskHandle_t EncodeAppTask_Handler[ENCODER_PORT_MAX];
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
void Task_EncoderApp1(void* p_arg);
void Task_EncoderApp2(void* p_arg);
void Task_EncoderApp3(void* p_arg);


const EncoderAppTaskInfo ENCODER_APP_TASK_INFO_TABLE[ENCODER_PORT_MAX] =
{
/*	task function,			task name,				Stack Size						arguments			priority		task handler		*/
	{ Task_EncoderApp1,		"EncoderAppTask1",		256,							(void *)0,			14, 		&EncodeAppTask_Handler[0]},
	{ Task_EncoderApp2,		"EncoderAppTask2",		256,							(void *)0,			14, 		&EncodeAppTask_Handler[1]},
//	{ Task_EncoderApp3,		"EncoderAppTask3",		256,							(void *)0,			14, 		&EncodeAppTask_Handler[2]},
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
* Description	: uS delay function.
  	1) Input argument  : usNum - Delay time in uS
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar,30 2015
**************************************************************************
*/

/*
**************************************************************************
* Description	: Initialize the control port of SSI encoder;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar 31, 2015
**************************************************************************
*/
VOID InitEncoderApp(VOID)
{
	UINT8 cnt;

	for(cnt = 0; cnt < ENCODER_PORT_MAX; cnt ++)
	{
		if(*ECDAppStructs[cnt].m_pEnable)
		{
			switch(*ECDAppStructs[cnt].m_pECDType)
			{
				case ECD_TYPE_HENGSTLER_SSI:
				case ECD_TYPE_RP3806_SSI:
				{
					ECDAppStructs[cnt].m_pEncoderInit = SSIEncoderInit;
					ECDAppStructs[cnt].m_pEncoderRead = SSIEncoderReadData;
					break;
				}
				case ECD_TYPE_INC_SSI:
				{
					ECDAppStructs[cnt].m_pEncoderInit = SSIEncoderInit;
					ECDAppStructs[cnt].m_pEncoderRead = SSIEncoderReadData;
					break;
				}
				
				default:
				{
					ECDAppStructs[cnt].m_pEncoderInit = NULL;
					ECDAppStructs[cnt].m_pEncoderRead = NULL;	
					break;
				}
			}
			/* Initilize encoder */
			if(ECDAppStructs[cnt].m_pEncoderInit != NULL)
				(*ECDAppStructs[cnt].m_pEncoderInit)((ENCODER_PORT)cnt);
			/* Creat tasks */
			if(ECDAppStructs[cnt].m_pEncoderRead != NULL)
			{
				xTaskCreate(ENCODER_APP_TASK_INFO_TABLE[cnt].m_TaskFunc,			 
					ENCODER_APP_TASK_INFO_TABLE[cnt].m_TaskName,
					ENCODER_APP_TASK_INFO_TABLE[cnt].m_StackSize,
					ENCODER_APP_TASK_INFO_TABLE[cnt].m_TaskArg,
					ENCODER_APP_TASK_INFO_TABLE[cnt].m_TaskPrio,
					ENCODER_APP_TASK_INFO_TABLE[cnt].m_TaskHandler);
			}
		}
	}

}

/*
**************************************************************************
* Description	: Encoder application task1.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
VOID Task_EncoderApp1(void* p_arg)
{
	(void)p_arg;
	INT64 calcTemp;
	INT64 maxUserRatioRange = 0;
	ENCODER_PORT port = ENCODER_PORT_1;
	ENCODER_APP_STRUCT *pEncoderAppHandler = &ECDAppStructs[port];

	while(TRUE)
	{
		/* Get encoder range at user uint */
		maxUserRatioRange = ((UINT32)1 << *pEncoderAppHandler->m_pECDMultiTurnBitNum);
		maxUserRatioRange *= *pEncoderAppHandler->m_pMotUnitRatio;
		maxUserRatioRange *= (*pEncoderAppHandler->m_pECDGearRatioN);
		maxUserRatioRange /= (*pEncoderAppHandler->m_pECDGearRatioD);
		/* Read encoder data */ 
		calcTemp = (*pEncoderAppHandler->m_pEncoderRead)(port);
		/* Convert to user unit */	
		calcTemp = calcTemp * (*pEncoderAppHandler->m_pECDGearRatioN) / (*pEncoderAppHandler->m_pECDGearRatioD) \
					* (*pEncoderAppHandler->m_pMotUnitRatio);
		calcTemp >>= (*pEncoderAppHandler->m_pECDSingleTurnBitNum);
		/* Save encode position */
		*pEncoderAppHandler->m_pEncoderPosition = (INT32)calcTemp;
		/* User orignal dispose */
		calcTemp = calcTemp - *pEncoderAppHandler->m_pUserOrgPoint;
		/* Menchnical type dispose */
		if(*pEncoderAppHandler->m_pMechType == MOVE_MECH_TYPE_CIRCULE)
		{
			calcTemp %= (*pEncoderAppHandler->m_pMotUnitRatio);

			/* Sign convert */
			if(*pEncoderAppHandler->m_pECDSignEn)
			{
				if((calcTemp > 0) && (calcTemp > (*pEncoderAppHandler->m_pMotUnitRatio) / 2))
					calcTemp = calcTemp - (*pEncoderAppHandler->m_pMotUnitRatio);
				else if((calcTemp < 0) && (calcTemp <= -((*pEncoderAppHandler->m_pMotUnitRatio) / 2)))
					calcTemp = calcTemp + (*pEncoderAppHandler->m_pMotUnitRatio);
			}
			else
			{
				if(calcTemp < 0)
					calcTemp = calcTemp + (*pEncoderAppHandler->m_pMotUnitRatio); 
			}
		}
		else if(*pEncoderAppHandler->m_pMechType == MOVE_MECH_TYPE_LINE)
		{
			/* Sign convert */
			if(*pEncoderAppHandler->m_pECDSignEn)
			{
				if((calcTemp > 0) && (calcTemp > maxUserRatioRange / 2))
					calcTemp = calcTemp - maxUserRatioRange;
				else if((calcTemp < 0) && (calcTemp <= -(maxUserRatioRange / 2)))
					calcTemp = calcTemp + maxUserRatioRange;
			}
			else
			{
				if(calcTemp < 0)
					calcTemp = calcTemp + maxUserRatioRange; 
			}
		}		
			
		/* Save user position */
		*pEncoderAppHandler->m_pUserPosition = (INT32)calcTemp;

		vTaskDelay(ENCODER_TASK_PERIOD);
	}
}

/*
**************************************************************************
* Description	: Encoder application task2.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
VOID Task_EncoderApp2(void* p_arg)
{
	(void)p_arg;
	INT64 calcTemp;
	INT64 maxUserRatioRange = 0;
	ENCODER_PORT port = ENCODER_PORT_2;
	ENCODER_APP_STRUCT *pEncoderAppHandler = &ECDAppStructs[port];

	while(TRUE)
	{
		/* Get encoder range at user uint */
		maxUserRatioRange = ((UINT32)1 << *pEncoderAppHandler->m_pECDMultiTurnBitNum);
		maxUserRatioRange *= *pEncoderAppHandler->m_pMotUnitRatio;
		maxUserRatioRange *= (*pEncoderAppHandler->m_pECDGearRatioN);
		maxUserRatioRange /= (*pEncoderAppHandler->m_pECDGearRatioD);
		/* Read encoder data */ 
		calcTemp = (*pEncoderAppHandler->m_pEncoderRead)(port);
		/* Convert to user unit */	
		calcTemp = calcTemp * (*pEncoderAppHandler->m_pECDGearRatioN) / (*pEncoderAppHandler->m_pECDGearRatioD) \
					* (*pEncoderAppHandler->m_pMotUnitRatio);
		calcTemp >>= (*pEncoderAppHandler->m_pECDSingleTurnBitNum);
		/* Save encode position */
		*pEncoderAppHandler->m_pEncoderPosition = (INT32)calcTemp;
		/* User orignal dispose */
		calcTemp = calcTemp - *pEncoderAppHandler->m_pUserOrgPoint;
		/* Sign convert */
		if(*pEncoderAppHandler->m_pECDSignEn)
		{
			if((calcTemp > 0) && (calcTemp > maxUserRatioRange / 2))
				calcTemp = calcTemp - maxUserRatioRange;
			else if((calcTemp < 0) && (calcTemp <= -(maxUserRatioRange / 2)))
				calcTemp = calcTemp + maxUserRatioRange;
		}
		else
		{
			if(calcTemp < 0)
				calcTemp = calcTemp + maxUserRatioRange; 
		}
			
		/* Save user position */
		*pEncoderAppHandler->m_pUserPosition = (INT32)calcTemp;

		vTaskDelay(ENCODER_TASK_PERIOD);
	}
}

/*
**************************************************************************
* Description	: Encoder application task3.
  	1) Input argument  : None					
 	2) Modify argument :None 
 	3) Output argument :None
* Return      	: None
* Notes       	: 
* Author	  	: Chris Shang
* Date			: Feb 05, 2015
**************************************************************************
*/
#if 0
VOID Task_EncoderApp3(void* p_arg)
{
	(void)p_arg;
	INT64 calcTemp;
	INT64 maxUserRatioRange = 0;
	ENCODER_PORT port = ENCODER_PORT_3;
	ENCODER_APP_STRUCT *pEncoderAppHandler = &ECDAppStructs[port];

	while(TRUE)
	{
		/* Get encoder range at user uint */
		maxUserRatioRange = ((UINT32)1 << *pEncoderAppHandler->m_pECDMultiTurnBitNum);
		maxUserRatioRange *= *pEncoderAppHandler->m_pMotUnitRatio;
		maxUserRatioRange *= (*pEncoderAppHandler->m_pECDGearRatioN);
		maxUserRatioRange /= (*pEncoderAppHandler->m_pECDGearRatioD);
		/* Read encoder data */ 
		calcTemp = (*pEncoderAppHandler->m_pEncoderRead)(port);
		/* Convert to user unit */	
		calcTemp = calcTemp * (*pEncoderAppHandler->m_pECDGearRatioN) / (*pEncoderAppHandler->m_pECDGearRatioD) \
					* (*pEncoderAppHandler->m_pMotUnitRatio);
		calcTemp >>= (*pEncoderAppHandler->m_pECDSingleTurnBitNum);
		/* Save encode position */
		*pEncoderAppHandler->m_pEncoderPosition = (INT32)calcTemp;
		/* User orignal dispose */
		calcTemp = calcTemp - *pEncoderAppHandler->m_pUserOrgPoint;
		/* Sign convert */
		if(*pEncoderAppHandler->m_pECDSignEn)
		{
			if((calcTemp > 0) && (calcTemp > maxUserRatioRange / 2))
				calcTemp = calcTemp - maxUserRatioRange;
			else if((calcTemp < 0) && (calcTemp <= -(maxUserRatioRange / 2)))
				calcTemp = calcTemp + maxUserRatioRange;
		}
		else
		{
			if(calcTemp < 0)
				calcTemp = calcTemp + maxUserRatioRange; 
		}
			
		/* Save user position */
		*pEncoderAppHandler->m_pUserPosition = (INT32)calcTemp;

		vTaskDelay(ENCODER_TASK_PERIOD);
	}
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
