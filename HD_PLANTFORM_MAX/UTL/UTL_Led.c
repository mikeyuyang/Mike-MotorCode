/*
//######################################################################################################

// FILE:	UTL_Led.c
//
// TITLE: Led control functions.
//
// AUTHOR:	
//
// DATE:	
//
// Description: LED operation functions.
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
#define   UTL_LED_GLOBAL_
#include "UTL_Led.h"

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
const GPIO_PORT_PIN LED_PORT_PIN_TABLE[LED_PORT_MAX] = {
/*			PORT,						PIN					*/
		{(VOID*)GPIOG,		GPIO_Pin_1},
		{(VOID*)GPIOG,		GPIO_Pin_0},
//		{(VOID*)GPIOE,		GPIO_Pin_9},
//		{(VOID*)GPIOE,		GPIO_Pin_10}
};

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
LED_CTRL_STRUCT LedControlStructs[LED_PORT_MAX] =
{
	{LED_STATUS_BLINK,	1,	10,	0},
	{LED_STATUS_OFF,	2,	5,	0},
//	{LED_STATUS_ON,		5,	2,	0},
//	{LED_STATUS_OFF,	2,	5,	0},
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
* Description	: LED initialization.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 3 2014
**************************************************************************
*/ 
VOID LedInit(VOID)
{
	UINT8 cnt;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	for(cnt = LED_PORT_ON_BOARD_RED; cnt < LED_PORT_MAX; cnt ++)
	{
		GPIO_InitStructure.GPIO_Pin = LED_PORT_PIN_TABLE[cnt].pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init((GPIO_TypeDef *)LED_PORT_PIN_TABLE[cnt].port, &GPIO_InitStructure);
		// Out put high at begining
		GPIO_SetBits((GPIO_TypeDef *)LED_PORT_PIN_TABLE[cnt].port, LED_PORT_PIN_TABLE[cnt].pin);
	}
}

/*
**************************************************************************
* Description	: Turn on LED.
  	1) Input argument  : ledPort - Led need to turn on
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 3 2014
**************************************************************************
*/
VOID LedON(LED_PORT ledPort)
{
	GPIO_SetBits((GPIO_TypeDef *)LED_PORT_PIN_TABLE[ledPort].port,
								LED_PORT_PIN_TABLE[ledPort].pin);
}

/*
**************************************************************************
* Description	: Turn off led.
  	1) Input argument  : ledPort - Led need to turn off
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 3 2014
**************************************************************************
*/
VOID LedOFF(LED_PORT ledPort)
{
	GPIO_ResetBits((GPIO_TypeDef *)LED_PORT_PIN_TABLE[ledPort].port,
								LED_PORT_PIN_TABLE[ledPort].pin);
}

/*
**************************************************************************
* Description	: Toggle led status.
  	1) Input argument  : ledPort - Led need to be toggled
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 3 2014
**************************************************************************
*/
VOID LedToggle(LED_PORT ledPort)
{
	if(GPIO_ReadOutputDataBit((GPIO_TypeDef *)LED_PORT_PIN_TABLE[ledPort].port,
											LED_PORT_PIN_TABLE[ledPort].pin))
	{
		GPIO_ResetBits((GPIO_TypeDef *)LED_PORT_PIN_TABLE[ledPort].port,
										LED_PORT_PIN_TABLE[ledPort].pin);
	}
	else
	{
		GPIO_SetBits((GPIO_TypeDef *)LED_PORT_PIN_TABLE[ledPort].port,
										LED_PORT_PIN_TABLE[ledPort].pin);
	}

}

/*
**************************************************************************
* Description	: Set LED status.
  	1) Input argument  : ledPort - Led need to turn on
  						ledStatus - Status of led
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: TRUE - Success, FALSE - Failed
* Author	  	: Chris_Shang
* Date			: April 30, 2015
**************************************************************************
*/
BOOL LedSetStatus(LED_PORT ledPort, LED_STATUS ledStatus)
{
	if((ledPort >= LED_PORT_MAX) || (ledStatus >= LED_STATUS_MAX))
	{
		return FALSE;
	}

	LedControlStructs[ledPort].m_Status = ledStatus;
	if(ledStatus == LED_STATUS_BLINK)
	{
		LedOFF(ledPort);
		LedControlStructs[ledPort].m_DelayCnt = 0;
	}

	return TRUE;
}

/*
**************************************************************************
* Description	: Set LED blink frequency.
  	1) Input argument  : ledPort - Led need to turn on
  						ledFreq - Led blink frequency
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: TRUE - Success, FALSE - Failed
* Author	  	: Chris_Shang
* Date			: April 30, 2015
**************************************************************************
*/
BOOL LedSetBlinkFreq(LED_PORT ledPort, UINT8 ledBlinkFreq)
{
	if((ledPort >= LED_PORT_MAX) || (ledBlinkFreq >= (1000 / LED_TASK_PERIOD / 2)))
	{
		return FALSE;
	}

	LedControlStructs[ledPort].m_Freq = ledBlinkFreq;
	LedControlStructs[ledPort].m_Reload = 1000 / ledBlinkFreq / 2 / LED_TASK_PERIOD;
	LedControlStructs[ledPort].m_DelayCnt = 0;

	return TRUE;
}


/*
**************************************************************************
* Description	: Led control task.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: April 30, 2015
**************************************************************************
*/
VOID Task_LedCtrl(void* p_arg)
{
	UINT8 cnt;

	(void)p_arg;
	while(TRUE)
	{
		for(cnt = 0; cnt < LED_PORT_MAX; cnt ++)
		{
			switch(LedControlStructs[cnt].m_Status)
			{
				case LED_STATUS_OFF:
				{
					LedOFF((LED_PORT)cnt);
					break;
				}
				case LED_STATUS_ON:
				{
					LedON((LED_PORT)cnt);
					break;
				}
				case LED_STATUS_BLINK:
				{
					LedControlStructs[cnt].m_DelayCnt ++;
					if(LedControlStructs[cnt].m_DelayCnt >= LedControlStructs[cnt].m_Reload)
					{
						LedToggle((LED_PORT)cnt);
						LedControlStructs[cnt].m_DelayCnt = 0;
					}
					break;
				}
			}
		}
		vTaskDelay(LED_TASK_PERIOD);
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
		LedSetStatus(LED_PORT_MOVE_IND, LED_STATUS_BLINK);
		moveLedBLinkFlag = TRUE;
	}
}
else
{
	LedSetStatus(LED_PORT_MOVE_IND, LED_STATUS_OFF);
	moveLedBLinkFlag = FALSE;
}

/* Limit indicate LED dispose */
if((LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_1].m_LimitStatus)
		|| (LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_2].m_LimitStatus)
		|| (LIMIT_STATUS_NONE != g_MoveControl[MOTOR_PORT_3].m_LimitStatus))
{
	LedSetStatus(LED_PORT_LIMT_IND, LED_STATUS_ON);
//			limLedBLinkFlag = TRUE;
}
else
{
	LedSetStatus(LED_PORT_LIMT_IND, LED_STATUS_OFF);
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

