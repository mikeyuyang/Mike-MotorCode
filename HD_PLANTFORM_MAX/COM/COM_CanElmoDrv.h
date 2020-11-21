/*
//######################################################################################################
//
// FILE:	COM_CanElmoDrv.c
//
// TITLE:	This function realized the Elmo driver communication protocol.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 9,2014
//
// Description: Elmo can communication protocol realization.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Nov 9,2014
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


#if !defined(COM_CAN_ELMO_DRV_H_)
#define COM_CAN_ELMO_DRV_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef		COM_CAN_ELMO_DRV_GLOBAL_
#define		COM_CAN_ELMO_DRV_EXT
#else    
#define		COM_CAN_ELMO_DRV_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include "BSP_BasicDataType.h"			// Basic data type define.
#include "UTL_CAN.h"
#include "MOT_Pwm.h"


/*
********************************************************************************************************
*	 Macro Defines
********************************************************************************************************
*/


/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/


/*
********************************************************************************************************
*	 Global data type
********************************************************************************************************
*/
/* Emlo driver user mode define */
typedef enum tagEMLO_UM
{
	EMLO_UM_TORQUE = 0, // Torque mode
	EMLO_UM_SPEED,		// Begain to move
	EMLO_UM_MICROSTEP,	// Set Decel
	EMLO_UM_DFPOINT,	// Double feed postion
	EMLO_UM_SFPOINT,	// Signel feed postion
	EMLO_UM_MAX
}EMLO_UM;

/* CAN protocol type */
typedef	enum tagCAN_COM_PROTOCOL_TYPE
{
	CAN_PROTOCOL_TYPE_ELMO_DRV = 0,
	CAN_PROTOCOL_TYPE_MAX,
}CAN_COM_PROTOCOL_TYPE;

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
*    Program Start 
*********************************************************************************************************
*/
COM_CAN_ELMO_DRV_EXT VOID CanElmoDrvLoop(CAN_PORT port);

COM_CAN_ELMO_DRV_EXT VOID CanElmoDriverEn(MOTOR_PORT motPort);

COM_CAN_ELMO_DRV_EXT VOID CanElmoDriverDis(MOTOR_PORT motPort);

COM_CAN_ELMO_DRV_EXT VOID CanElmoStart(MOTOR_PORT motPort);

COM_CAN_ELMO_DRV_EXT VOID CanElmoStop(MOTOR_PORT motPort);

COM_CAN_ELMO_DRV_EXT BOOL CanElmoSetSpeed(MOTOR_PORT motPort, INT32 spd);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetTorque(MOTOR_PORT motPort, INT32 torque);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetAccel(MOTOR_PORT motPort, UINT32 acc);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetDecel(MOTOR_PORT motPort, UINT32 dec);

COM_CAN_ELMO_DRV_EXT VOID CanElmoAbsMove(MOTOR_PORT motPort, INT32 absPos);

COM_CAN_ELMO_DRV_EXT VOID CanElmoRelMove(MOTOR_PORT motPort, INT32 relPos);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetStopDecel(MOTOR_PORT motPort, UINT32 dec);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetUserMode(MOTOR_PORT motPort, EMLO_UM mode);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSendReadPosCmd(MOTOR_PORT port);

COM_CAN_ELMO_DRV_EXT INT32 CanElmoGetPostion(MOTOR_PORT port);

COM_CAN_ELMO_DRV_EXT VOID CanElmoSetOrigin(MOTOR_PORT port);

COM_CAN_ELMO_DRV_EXT VOID CanElmoDrvInit(MOTOR_PORT motPort);


#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


