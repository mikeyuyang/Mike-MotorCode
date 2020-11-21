/*
//######################################################################################################
//
// FILE:	MOT_Pwm.c
//
// TITLE:	PWM motor Control process source file.
//
// AUTHOR:	
//
// DATE:	
//
// Description: This file include source code for pwm motor driver, and control functions.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:    May 10, 2015
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

#if !defined(MOT_PWM_H_)
#define MOT_PWM_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   MOT_PWM_GLOBAL_
#define  MOT_PWM_EXT
#else    
#define  MOT_PWM_EXT  extern
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

/*
*********************************************************************************************************
*                                           Global data types
*********************************************************************************************************
*/

typedef enum tagMOTOR_PORT		// Ports of Motor.
{
	MOTOR_PORT_1 = 0,
	MOTOR_PORT_2,
	MOTOR_PORT_3,
	MOTOR_PORT_MAX
}MOTOR_PORT;

typedef enum tagPOS_SOURCE		// Ports of Motor.
{
	POS_SOURCE_INTER_PUL_CNT = 0,
	POS_SOURCE_EXT_PUL_CNT,
	POS_SOURCE_ENCODER,
	POS_SOURCE_MAX
}POS_SOURCE;
/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
#define ONE_TURN_RANGE 36000

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/

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
*********************************************************************************************************
* 	Global function implementations
*********************************************************************************************************
*/
MOT_PWM_EXT VOID MotorPwmInit(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmEnable(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmDisable(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmClearFault(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmStart(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmStop(MOTOR_PORT port);

MOT_PWM_EXT BOOL MotorPwmSetDir(MOTOR_PORT port, MOV_DIR dir);

MOT_PWM_EXT BOOL MotorPwmSetSpeed(MOTOR_PORT port, INT32 spd);

MOT_PWM_EXT INT32 MotorPwmGetPostion(MOTOR_PORT port);

MOT_PWM_EXT VOID MotorPwmSetOrigin(MOTOR_PORT port);


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

