/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	UTL_Led.h
//
// TITLE:	LED Control functions
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2014-11-03
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2014-11-03
// Version:
// Descriptions: 	Frist Version
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

#if !defined(UTL_LED_H_)
#define UTL_LED_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   UTL_LED_GLOBAL_
#define  UTL_LED_EXT
#else    
#define  UTL_LED_EXT  extern
#endif

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define LED_TASK_PERIOD 50		// Led task call period, Unit: ms
/*
*********************************************************************************************************
*                                           Global data types
*********************************************************************************************************
*/
/* Led port define */
typedef enum tagLED_PORT
{
	LED_PORT_ON_BOARD_RED = 0,
	LED_PORT_ON_BOARD_GER,
	LED_PORT_RUN_IND,
	LED_PORT_FAULT_IND,
	LED_PORT_MAX
}LED_PORT;

/* Led status define */
typedef enum tagLED_STATUS
{
	LED_STATUS_OFF = 0,
	LED_STATUS_ON,
	LED_STATUS_BLINK,
	LED_STATUS_MAX
}LED_STATUS;


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
UTL_LED_EXT BOOL g_TestLedStatus;
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
UTL_LED_EXT VOID LedInit(VOID);

UTL_LED_EXT	BOOL LedSetStatus(LED_PORT ledPort, LED_STATUS ledStatus);

UTL_LED_EXT	BOOL LedSetBlinkFreq(LED_PORT ledPort, UINT8 ledBlinkFreq);

UTL_LED_EXT VOID Task_LedCtrl(void* p_arg);

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