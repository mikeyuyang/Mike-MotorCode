/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	DIO_Control.c
//
// TITLE: 	General DI/DO control functions.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2015-08-15
//
// Description: This function include the functions for DIO operation. For DO, here include the functions
 	 	 	 	 for delay, output mode, inverse control etc. For DI, here include the functions for filter
 	 	 	 	 and inverse control.
//
//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2015-08-15
// Version:
// Descriptions: 	First Version
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
#include <stm32f4xx.h>
#include <includes.h>

#include "BSP_BasicDataType.h"	// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "DBM_Data.h"
#include "UTL_1msTimer.h"
#define   DIO_CONTROL_GLOBAL_
#include "DIO_Control.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
/* DI Control Struct */
typedef struct tagDI_CTRL_STRUCT
{
	TIMER	m_Timer;
	UINT16* m_pFilterTime;
}DI_CTRL_STRUCT;

/* DO Control Struct */
typedef struct tagDO_CTRL_STRUCT
{
	TIMER	m_Timer;
	UINT16* m_pPulseTime;
	DO_MODE* m_pDOMode;
}DO_CTRL_STRUCT;

/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
const GPIO_PORT_PIN DI_PORT_PIN_TABLE[DI_PORT_MAX] = {
/*			PORT,		PIN				*/
	{(VOID*)GPIOG,		GPIO_Pin_7},
	{(VOID*)GPIOG,		GPIO_Pin_6},
	{(VOID*)GPIOG,		GPIO_Pin_5},
	{(VOID*)GPIOG,		GPIO_Pin_4},
	{(VOID*)GPIOG,		GPIO_Pin_3},
	{(VOID*)GPIOG,		GPIO_Pin_2}
};

const GPIO_PORT_PIN DO_PORT_PIN_TABLE[DO_PORT_MAX] = {
/*			PORT,		PIN				*/
	{(VOID*)GPIOB,		GPIO_Pin_14},
	{(VOID*)GPIOB,		GPIO_Pin_15}
};

#define DIDirectValue	AVD.m_DIDirectStatus
#define DIUserValue		AVD.m_DIUserStatus
#define DIInverse 		SPD.m_DIInverseCtrl

#define DODirectValue	AVD.m_DODirectStatus
#define DOUserValue		AVD.m_DOUserStatus
#define DOInverse			SPD.m_DOInverseCtrl

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/

static UINT16 DIValBeforeInv;

static UINT16 DOValBeforeInv;

/* DI operation control struct */
static DI_CTRL_STRUCT DIControlStructs[DI_PORT_MAX] =
{
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI1FilterTime},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI2FilterTime},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI3FilterTime},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI4FilterTime},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI5FilterTime},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DI6FilterTime},
};

/* DO operation control struct */
static DO_CTRL_STRUCT DOControlStructs[DO_PORT_MAX] =
{
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DO1PulseTime,	(DO_MODE*)&SPD.m_DO1Mode},
	{TIMER_STATE_STOPPED,	0,	0,	0,	&SPD.m_DO2PulseTime,	(DO_MODE*)&SPD.m_DO2Mode},
};
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
* Description	: DIO initialization.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 3 2014
**************************************************************************
*/ 
VOID DIOInit(VOID)
{
	UINT8 cnt;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port for DI.
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	for(cnt = 0; cnt < DI_PORT_MAX; cnt ++)
	{
		GPIO_InitStructure.GPIO_Pin = DI_PORT_PIN_TABLE[cnt].pin;
		GPIO_Init((GPIO_TypeDef *)DI_PORT_PIN_TABLE[cnt].port, &GPIO_InitStructure);
		/* Timer Initialize  */
		SetTimeDelay(&DIControlStructs[cnt].m_Timer, (UINT32)*DIControlStructs[cnt].m_pFilterTime);
	}

	// Initialize the GPIO Port for DO.
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	
	for(cnt = 0; cnt < DO_PORT_MAX; cnt ++)
	{
		GPIO_InitStructure.GPIO_Pin = DO_PORT_PIN_TABLE[cnt].pin;
		GPIO_Init((GPIO_TypeDef *)DO_PORT_PIN_TABLE[cnt].port, &GPIO_InitStructure);
		// Out put Low at beginning
		GPIO_ResetBits((GPIO_TypeDef *)DO_PORT_PIN_TABLE[cnt].port, DO_PORT_PIN_TABLE[cnt].pin);
		/* Timer Initialize  */
		SetTimeDelay(&DOControlStructs[cnt].m_Timer, (UINT32)*DOControlStructs[cnt].m_pPulseTime);
	}
}

/*
**************************************************************************
* Description	: Read DI port.
  	1) Input argument  : DIPort - DI port for read
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: DI status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug, 15 2015
**************************************************************************
*/
DIO_STATUS GetDIStatus(DI_PORT DIPort)
{
	if(GET_BIT(DIUserValue, DIPort))
	{
		return DIO_STATUS_ON;
	}
	else
	{
		return DIO_STATUS_OFF;
	}
}

/*
**************************************************************************
* Description	: SET DO port.
  	1) Input argument  : DOPort - DO port for read
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris.Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID SetDO(DO_PORT DOPort)
{
	SET_BIT2(DOUserValue, DOPort);
}

/*
**************************************************************************
* Description	: CLEAR DO port.
  	1) Input argument  : DOPort - DO port for read
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris.Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID ClearDO(DO_PORT DOPort)
{
	CLEAR_BIT2(DOUserValue, DOPort);
}

/*
**************************************************************************
* Description	: TOGGLE DO port.
  	1) Input argument  : DOPort - DO port for read
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris.Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID ToggleDO(DO_PORT DOPort)
{
	INV_BIT(DOUserValue, DOPort);
}
/*
**************************************************************************
* Description	: DIO control task.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID Task_DIOCtrl(void* p_arg)
{
	UINT8 cnt;

	(void)p_arg;
	while(TRUE)
	{
		/* DI dispose */
		for(cnt = 0; cnt < DI_PORT_MAX; cnt ++)
		{
			/* Read direct value */
			if(GPIO_ReadInputDataBit((GPIO_TypeDef *)DI_PORT_PIN_TABLE[cnt].port, DI_PORT_PIN_TABLE[cnt].pin))
				SET_BIT2(DIDirectValue, cnt);
			else
				CLEAR_BIT2(DIDirectValue, cnt);

			if(GET_BIT(DIDirectValue, cnt) != GET_BIT(DIValBeforeInv, cnt))
			{
				if(0 == *DIControlStructs[cnt].m_pFilterTime)
				{
					/* Don't need filter */
					if(GET_BIT(DIDirectValue, cnt))
						SET_BIT2(DIValBeforeInv, cnt);
					else
						CLEAR_BIT2(DIValBeforeInv, cnt);
				}
				else
				{
					if(isStopped(&DIControlStructs[cnt].m_Timer))
					{
						StartTimer(&DIControlStructs[cnt].m_Timer);
					}
					else if(isElapsed(&DIControlStructs[cnt].m_Timer))
					{
						if(GET_BIT(DIDirectValue, cnt))
							SET_BIT2(DIValBeforeInv, cnt);
						else
							CLEAR_BIT2(DIValBeforeInv, cnt);
						/* Stop Time */
						StopTimer(&DIControlStructs[cnt].m_Timer);
					}
				}
			}
			else
			{
				if(isRunning(&DIControlStructs[cnt].m_Timer))
				{
					StopTimer(&DIControlStructs[cnt].m_Timer);
				}
			}
		}
		/* DI Inverse dispose */
		DIUserValue = DIValBeforeInv ^( DIInverse);

		/* DO dispose */
		for(cnt = 0; cnt < DO_PORT_MAX; cnt ++)
		{
			if(DO_STATUS_NORMAL == (*DOControlStructs[cnt].m_pDOMode))
			{
				if(GET_BIT(DOUserValue, cnt) != GET_BIT(DOValBeforeInv, cnt))
				{
					/* Copy bit status directly. */
					if(GET_BIT(DOUserValue, cnt))
						SET_BIT2(DOValBeforeInv, cnt);
					else
						CLEAR_BIT2(DOValBeforeInv, cnt);
				}
			}
			else
			{
				if(GET_BIT(DOUserValue, cnt))
				{
					if(isStopped(&DOControlStructs[cnt].m_Timer))
					{
						StartTimer(&DOControlStructs[cnt].m_Timer);
						SET_BIT2(DOValBeforeInv, cnt);
					}
					else if(isElapsed(&DOControlStructs[cnt].m_Timer))
					{
						CLEAR_BIT2(DOUserValue, cnt);
						CLEAR_BIT2(DOValBeforeInv, cnt);
						StopTimer(&DIControlStructs[cnt].m_Timer);
					}
				}
				else
				{
					if(isRunning(&DOControlStructs[cnt].m_Timer))
					{
						StopTimer(&DOControlStructs[cnt].m_Timer);
						CLEAR_BIT2(DOValBeforeInv, cnt);
					}
				}
			}

			/* DO Inverse dispose */
			DODirectValue = DOValBeforeInv ^( DOInverse);

			/* Send to direct value */
			if(GET_BIT(DODirectValue, cnt))
				GPIO_SetBits((GPIO_TypeDef *)DO_PORT_PIN_TABLE[cnt].port, DO_PORT_PIN_TABLE[cnt].pin);
			else
				GPIO_ResetBits((GPIO_TypeDef *)DO_PORT_PIN_TABLE[cnt].port, DO_PORT_PIN_TABLE[cnt].pin);
		}
		vTaskDelay(2);
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

