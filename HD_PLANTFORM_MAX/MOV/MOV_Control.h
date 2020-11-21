/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	MOV_Control.h
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


#if !defined(MOV_CONTROL_H_)
#define MOV_CONTROL_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   MOV_CONTROL_GLOBAL_
#define  MOV_CONTROL_EXT
#else    
#define  MOV_CONTROL_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include "BSP_BasicDataType.h"			// Basic data type define.
#include "MOT_Pwm.h"
#include "DIO_Control.h"
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
#define MOVE_CTRL_TASK_PERIOD	10		// Unit: ms
/*
********************************************************************************************************
*	 Global data type
********************************************************************************************************
*/
/* Mechanical type */
typedef enum tagMOVE_MECH_TYPE
{
	MOVE_MECH_TYPE_CIRCULE = 0,
	MOVE_MECH_TYPE_LINE,
	MOVE_MECH_TYPE_MAX
}MOVE_MECH_TYPE;

/* Driver control type */
typedef enum tagDRV_CTRL_TYPE
{
	DRV_CTRL_TYPE_PWM = 0,
	DRV_CTRL_TYPE_ELMO_CAN,
	DRV_CTRL_TYPE_MAX
}DRV_CTRL_TYPE;

/* Driver control type */
typedef enum tagMOVE_CTRL_MODE
{
	MOVE_CTRL_MODE_SPD = 0,
	MOVE_CTRL_MODE_FIX_POINT,
	MOVE_CTRL_MODE_TRACKING,
	MOVE_CTRL_MODE_FIND_ORIG,
	MOVE_CTRL_MODE_MAX
}MOVE_CTRL_MODE;

/* Find orignal MODE */
typedef enum tagFIND_ORIG_MODE
{
	FIND_ORIG_MODE_O_AS_O = 0,
	FIND_ORIG_MODE_N_AS_O,
	FIND_ORIG_MODE_P_AS_O,
	FIND_ORIG_MODE_CENTRE_OF_N_AND_P,
	FIND_ORIG_MODE_MAX
}FIND_ORIG_MODE;


/* Control status */
typedef enum tagCONTROL_STATE  	//  of the switch.
{
	CONTROL_STATE_STOP = 1,
	CONTROL_STATE_SPD_TRACK,
	CONTROL_STATE_RUN_TO_TARGET,
	CONTROL_STATE_TRACKING,
	CONTROL_STATE_WAIT_TRACK,
	CONTROL_STATE_ACCEL,
	CONTROL_STATE_UNIFORM,
	CONTROL_STATE_DECEL,
	CONTROL_STATE_TRIMMING,
	CONTROL_STATE_WAIT_STOP,
	CONTROL_STATE_FIND_ORIG_TRIG,
	CONTROL_STATE_FIND_NEG_TRIG,
	CONTROL_STATE_FIND_POS_TRIG,
	CONTROL_STATE_FIND_NEG_POS_CENTRE,
	CONTROL_STATE_FIND_RUN_TO_OFFSET,
	CONTROL_STATE_MAX
}CONTROL_STATE;

/* Limit state */
typedef enum tagLIMIT_STATUS  	//  Limit status.
{
	LIMIT_STATUS_NONE = 0,
	LIMIT_STATUS_POS,
	LIMIT_STATUS_NEG,
	LIMIT_STATUS_MAX
}LIMIT_STATUS;



/* Move control struct */
typedef struct tagMOVE_CTRL  	//  of the switch.
{
	/* Set Point */
	UINT16* m_pAxisEnable;
	UINT16* m_pPosSource;
	UINT16* m_pMechType;
	UINT16* m_pMotDirResvert;
	UINT16* m_pECDSignEn;
	UINT16* m_pDrvCtrlType;
	UINT32* m_pMotEncodeLineNum;
	UINT16* m_pMotGearRatioN;
	UINT16* m_pMotGearRatioD;
	UINT32* m_pMotUnitRatio;
	UINT16* m_pMoveCtrlMode;
	UINT16* m_pSpdCtrlAcclEn;		// Only use by speed control
	UINT32* m_pWholeRangeDis;
	UINT16* m_pTrimDisLimt;			// If the distance is less than this value, Trim will start.
	UINT16* m_pErrTol;
	UINT16* m_pWaitStopTime;			// Unit: 0.01s
	UINT16* m_pMaxSpd;				// Used by speed control and position control
	UINT16* m_pMinSpd;
	UINT16* m_pAccl;
	UINT16* m_pDecl;
	UINT16* m_pPidKp;				// PID Kp
	UINT16* m_pPidKi;				// PID Ki
	UINT16* m_pPidKd;				// PID Kd
	UINT16* m_pIntegralEnRng;		// PID Integral enable range
	UINT16* m_pNegLimitPort;		// 
	UINT16* m_pPosLimitPort;		// 
	UINT16* m_pOriginalPointPort;	// 
	UINT16* m_pDrvFaultPort;		// 
	INT32 *m_pNegLimitVal;			// Negative limit for axis
	INT32 *m_pPosLimitVal;			// Positive limit for axis
	UINT16* m_pEnableStartUpFindOrig;	
	UINT16* m_pFindOrigSpd;	
	UINT16* m_pFindOrigMode;
	INT32* m_pOrigOffSet;
	
	/* Control variables */
	UINT16 *m_pStartCmd;
	UINT16 *m_pStopCmd;
	UINT16 *m_pEmgStopCmd;
	UINT16 *m_pClearFaultCmd;
	INT32 *m_pSpdSetPoint;
	INT32 *m_pPosSetPoint;
	INT32 *m_pCurrentPoint;
	CONTROL_STATE m_CtrlState;
	INT32 m_CurrentSpd;
	INT32 m_LastSpd;
	MOV_DIR m_CurrentDir;
	INT32 m_StartPoint;
	INT32 m_EndPoint;
	INT32 m_AccDis;	
	INT32 m_DeclDis;	
	INT32 m_MovedDis;	
	INT32 m_TotalDis;	
	INT32 m_CurrentDis;	
	UINT16 m_AccStep;
	UINT16 m_WaitStopTimeCnt;
	LIMIT_STATUS m_LimitStatus;
	UINT16 m_LastMoveCtrlMode;

	/* Control functions */
	VOID (*m_pDrvInit) (MOTOR_PORT motPort);
	VOID (*m_pDrvEn) (MOTOR_PORT motPort);
	VOID (*m_pDrvDis) (MOTOR_PORT motPort);
	VOID (*m_pDrvStart) (MOTOR_PORT motPort);
	VOID (*m_pDrvStop) (MOTOR_PORT motPort);
	BOOL (*m_pDrvSetSpd) (MOTOR_PORT motPort, INT32 spd);
	VOID (*m_pDrvClearFault) (MOTOR_PORT motPort);
}MOVE_CTRL;


/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
/* Move control struct */
//MOV_CONTROL_EXT ;


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

MOV_CONTROL_EXT VOID MoveInit(VOID);

MOV_CONTROL_EXT BOOL MoveSetCtrlMode(MOTOR_PORT port ,MOVE_CTRL_MODE mode);

MOV_CONTROL_EXT BOOL MoveSpdCtrlSetAction(MOTOR_PORT port, INT32 spd);

MOV_CONTROL_EXT BOOL MovePosCtrlSetAction(MOTOR_PORT port, INT32 dis);

MOV_CONTROL_EXT BOOL MovePosCtrlSetPoint(MOTOR_PORT port, INT32 position);

MOV_CONTROL_EXT VOID MoveFindOrigStart(MOTOR_PORT port);

MOV_CONTROL_EXT BOOL MoveSpdCtrlSetAction(MOTOR_PORT port, INT32 spd);

MOV_CONTROL_EXT BOOL MoveUpdateSpeed(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler);

MOV_CONTROL_EXT BOOL MoveStop(MOTOR_PORT port);

MOV_CONTROL_EXT BOOL IsMoveStop(MOTOR_PORT port);

MOV_CONTROL_EXT BOOL MoveSpeedModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler);

MOV_CONTROL_EXT BOOL MoveFixPointModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler);

MOV_CONTROL_EXT BOOL MoveTrackModeCtrl(MOTOR_PORT port, MOVE_CTRL *pCtrlHandler);

MOV_CONTROL_EXT BOOL MoveCtrl(MOTOR_PORT port);

MOV_CONTROL_EXT DIO_STATUS GetOrigTrigStatus(MOTOR_PORT port);

MOV_CONTROL_EXT DIO_STATUS GetNegTrigStatus(MOTOR_PORT port);
	
MOV_CONTROL_EXT DIO_STATUS GetPosTrigStatus(MOTOR_PORT port);



#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/

