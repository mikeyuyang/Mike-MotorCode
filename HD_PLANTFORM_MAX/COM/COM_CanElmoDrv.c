/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
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
#include "includes.h"

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#include "UTL_CRC.h"
#include "MOV_Control.h"
#include "DIO_Control.h"
#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "UTL_Can.h"
//#include "UTL_OCTimer.h"
#include "UTL_EEPROM.h"
//#include "APP_Control.h"

#define COM_CAN_ELMO_DRV_GLOBAL_
#include "COM_CanElmoDrv.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/


/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/
extern MOVE_CTRL  g_MoveControl[MOTOR_PORT_MAX];
/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/
/* Emlo driver command set */
typedef enum tagEMLO_CMD
{
	EMLO_CMD_AC = 0,	// Set Accel
	EMLO_CMD_BG,		// Begain to move
	EMLO_CMD_DC,		// Set Decel
	EMLO_CMD_HM,		// Event dispose option
	EMLO_CMD_ID,		// Read reactive current
	EMLO_CMD_IQ,		// Read active current
	EMLO_CMD_JV,		// Set move speed
	EMLO_CMD_MF,		// Report motor fault
	EMLO_CMD_MO,		// Motor Enable/Disable
	EMLO_CMD_PA,		// Motor Next absolut point set
	EMLO_CMD_PX,		// Main feedback value
	EMLO_CMD_PR,		// Motor Next relative point set	
	EMLO_CMD_SD,		// Stop decel
	EMLO_CMD_ST,		// Stop move
	EMLO_CMD_SV,		// Save parameter
	EMLO_CMD_TC,		// Torque current command
	EMLO_CMD_UM,		// User mode
	EMLO_CMD_MAX
}EMLO_CMD;

/* Motor control struct */
typedef struct tagELMO_MOTOR_CTRL
{
	/* Set Point */
	UINT16* m_pMasteAddr;	
	UINT16* m_pSlaveAddr;	
	UINT16* m_pDualMotorEn;
	UINT32* m_pMotEncoderLineNum;
	UINT16* m_pPosSource;
	UINT16* m_pMotDirResvert;
	UINT16* m_pMotGearRatioN;
	UINT16* m_pMotGearRatioD;
	UINT32* m_pMotUnitRatio;
	UINT16* m_Accel;
	UINT16* m_Decel;
	

	/* Control variables */
	CAN_PORT m_CanPort;
	UINT16 m_UM;
	INT32 m_AccumPluseCnt;
	MOV_DIR m_CurrentDir;
}ELMO_MOTOR_CTRL;


/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
/* Command */
CHAR EMLO_CMD_TABLE[EMLO_CMD_MAX][3] = 
{
	{"AC"},		// Set Accel
	{"BG"},		// Begain to move
	{"DC"},		// Set Decel
	{"HM"}, 	// Event dispose option
	{"ID"},		// Read reactive current
	{"IQ"},		// Read active current
	{"JV"},		// Set move speed
	{"MF"},		// Report motor fault
	{"MO"},		// Motor Enable/Disable
	{"PA"},		// Motor Next absolut point set
	{"PX"},		// Main feedback value
	{"PR"},		// Motor Next relative point set	
	{"SD"}, 	// Set stop decel
	{"ST"},		// Stop move
	{"SV"},		// Save parameter
	{"TC"},		// Torque current command
	{"UM"}		// User mode
};

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
ELMO_MOTOR_CTRL EmloDrvCtrlTable[MOTOR_PORT_MAX] = 
{
	{
		/* Set Point */
		&SPD.m_MasteDrvAddr1,		// Master driver address;
		&SPD.m_SlaveDrvAddr1,		// Slave driver address;
		&SPD.m_DualMotEn1,			// Dual motor enable;
		&SPD.m_MotEncodeLineNum1,	// Encoder line number of motor;
		&SPD.m_PosSource1,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert1,		// Motor direction resvert
		&SPD.m_MotGearRatioN1,		// Gear Ratio N
		&SPD.m_MotGearRatioD1,		// Gear Ratio D
		&SPD.m_MotUnitRatio1,		// Unit Ratio
		&SPD.m_Accel1,		// Accel
		&SPD.m_Decel1,		// Decel

		/* Control variables */
		CAN_PORT_1,
		2,											//mode
		0,			// Total Pluse count;
		MOV_DIR_POS // Current Turn Direction;
	},

	{
		/* Set Point */
		&SPD.m_MasteDrvAddr2,		// Master driver address;
		&SPD.m_SlaveDrvAddr2,		// Slave driver address;
		&SPD.m_DualMotEn2,			// Dual motor enable;
		&SPD.m_MotEncodeLineNum2,	// Encoder line number of motor;
		&SPD.m_PosSource2,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert2,		// Motor direction resvert
		&SPD.m_MotGearRatioN2,		// Gear Ratio N
		&SPD.m_MotGearRatioD2,		// Gear Ratio D
		&SPD.m_MotUnitRatio2,		// Unit Ratio
		&SPD.m_Accel2,		// Accel
		&SPD.m_Decel2,		// Decel
		/* Control variables */
		CAN_PORT_1,
		2,			// Speed mode;
		0,			// Total Pluse count;
		MOV_DIR_POS // Current Turn Direction;
	},
#if 0
	{
		/* Set Point */
		&SPD.m_MasteDrvAddr3,		// Master driver address;
		&SPD.m_SlaveDrvAddr3,		// Slave driver address;
		&SPD.m_DualMotEn3,			// Dual motor enable;
		&SPD.m_MotEncodeLineNum3,	// Encoder line number of motor;
		&SPD.m_PosSource3,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert3,		// Motor direction resvert
		&SPD.m_MotGearRatioN3,		// Gear Ratio N
		&SPD.m_MotGearRatioD3,		// Gear Ratio D
		&SPD.m_MotUnitRatio3,		// Unit Ratio
		&SPD.m_Accel3,		// Accel
		&SPD.m_Decel3,		// Decel

		/* Control variables */
		CAN_PORT_1,
		2,			// Speed mode;
		0,			// Total Pluse count;
		MOV_DIR_POS // Current Turn Direction;
	}
#endif	

};



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
* Description	: Process CAN data frame For Elmo driver
  	1) Input argument  : port - Canport, slvAdd - Slave address of driver.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov,9 2014
**************************************************************************
*/
VOID CanElmoDrvLoop(CAN_PORT port)
{
	CAN_BUF* pCANBuf;
	UINT8 canAddr;
	static ELMO_MOTOR_CTRL* pMotCtrlHandler = NULL;
	UINT8 i;


	pCANBuf = &GetCanPortBuffer(port)->m_TXBuffer;
	/* Tx dispose */
	if(IsCanTxIdle(port) && (pCANBuf->m_RdIdx != pCANBuf->m_WrIdx))
	{
		CanSendOneMsg(port, &pCANBuf->m_buffer[pCANBuf->m_RdIdx]);
		pCANBuf->m_RdIdx ++;
		pCANBuf->m_RdIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count --;
	}

	/* Rx dispose */
	pCANBuf = &GetCanPortBuffer(port)->m_RXBuffer;
	if(pCANBuf->m_RdIdx != pCANBuf->m_WrIdx)
	{
		CAN_MSG* pCanMsg;
		pCanMsg = &(pCANBuf->m_buffer[pCANBuf->m_RdIdx]);
		pCANBuf->m_RdIdx ++;
		pCANBuf->m_RdIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count --;

		/* Analizy can frame */
		/* Is a respone frame */
		if(0 == (pCanMsg->m_StdId & 0x80))
			return;
		/* Get address */
		canAddr = pCanMsg->m_StdId & 0x7F;
		for(i = 0; i < MOTOR_PORT_MAX; i ++)
		{
			if((UINT8)(*EmloDrvCtrlTable[i].m_pMasteAddr) == canAddr)
			{
				break;
			}
		}
		
		if(i >= MOTOR_PORT_MAX)
			return;

		/* Get move control handler */
		pMotCtrlHandler = &(EmloDrvCtrlTable[i]);
		if(0 == memcmp(EMLO_CMD_TABLE[EMLO_CMD_PX], &pCanMsg->m_Data[0], 2))
		{
			INT32 i32Temp;
			/* PX command */
			i32Temp = pCanMsg->m_Data[7];
			i32Temp <<= 8;
			i32Temp += pCanMsg->m_Data[6];
			i32Temp <<= 8;
			i32Temp += pCanMsg->m_Data[5];
			i32Temp <<= 8;
			i32Temp += pCanMsg->m_Data[4];
			pMotCtrlHandler->m_AccumPluseCnt = i32Temp;
			//memcpy((VOID *)(&pMotCtrlHandler->m_AccumPluseCnt), &pCanMsg->m_Data[4], 4);
		}
	}
}

/*
**************************************************************************
* Description	: Active canport
  	1) Input argument  : port - CAN port.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov,9 2014
**************************************************************************
*/
VOID CanElmoActiveCan(CAN_PORT port)
{
	CAN_MSG canMsg;

	/* Send a broadcast can frame to active CAN bus */
	canMsg.m_ExtId = 0;
	canMsg.m_StdId = 0;
	canMsg.m_IDE = 0;
	canMsg.m_RTR = 0;
	canMsg.m_DLC = 2;
	canMsg.m_Data[0] = 0x01;
	canMsg.m_Data[1] = 0x00;
	CanPushMsg(port, &canMsg);
}

/*
**************************************************************************
* Description	: Send command and data to Motor control
  	1) Input argument  : port - CAN port
  						Addr - Dirver address
  						cmd - command type
  						idx - command sub index
  						val - val to set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov,9 2014
**************************************************************************
*/
VOID CanElmoSendCmdWithData(CAN_PORT port, UINT8 addr, EMLO_CMD cmd, UINT8 idx, INT32 val)
{
	CAN_MSG canMsg;

	/* Send a broadcast can frame to active CAN bus */
	canMsg.m_ExtId = 0;
	canMsg.m_StdId = 0x300 | addr;
	canMsg.m_IDE = 0;
	canMsg.m_RTR = 0;
	canMsg.m_DLC = 8;
	canMsg.m_Data[0] = EMLO_CMD_TABLE[cmd][0];
	canMsg.m_Data[1] = EMLO_CMD_TABLE[cmd][1];
	if(0 != idx)
	{
		if(idx >= 10)
		{
			canMsg.m_Data[2] = idx / 10;
			canMsg.m_Data[3] = idx % 10;
		}
		else
		{
			canMsg.m_Data[2] = idx;
			canMsg.m_Data[3] = 0;
		}
//		canMsg.m_DLC = 4;
	}
	else
	{
		canMsg.m_Data[2] = 0;
		canMsg.m_Data[3] = 0;
	}
	canMsg.m_Data[4] = ((UINT8*)&val)[0];
	canMsg.m_Data[5] = ((UINT8*)&val)[1];
	canMsg.m_Data[6] = ((UINT8*)&val)[2];
	canMsg.m_Data[7] = ((UINT8*)&val)[3];
	
	CanPushMsg(port, &canMsg);
}

/*
**************************************************************************
* Description	: Send command without data 
  	1) Input argument  : port - CAN port
  						Addr - Dirver address
  						cmd - command type
						idx - command sub index
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov,9 2014
**************************************************************************
*/
VOID CanElmoSendOnlyCmd(CAN_PORT port, UINT8 addr, EMLO_CMD cmd, UINT8 idx)
{
	CAN_MSG canMsg;

	/* Send a broadcast can frame to active CAN bus */
	canMsg.m_StdId = 0x300 | addr;
	canMsg.m_IDE = 0;
	canMsg.m_RTR = 0;
	canMsg.m_DLC = 4;
	canMsg.m_Data[0] = EMLO_CMD_TABLE[cmd][0];
	canMsg.m_Data[1] = EMLO_CMD_TABLE[cmd][1];
	if(0 != idx)
	{
		if(idx >= 10)
		{
			canMsg.m_Data[2] = idx / 10;
			canMsg.m_Data[3] = idx % 10;
		}
		else
		{
			canMsg.m_Data[2] = idx;
			canMsg.m_Data[3] = 0;
		}
	}
	else
	{
		canMsg.m_Data[2] = 0;
		canMsg.m_Data[3] = 0;
	}
	CanPushMsg(port, &canMsg);
}

//======================================================================
// Motor applicaiton interface
//======================================================================


/*
**************************************************************************
* Description	: Enable driver
  	1) Input argument  : 	motPort - Motor port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov,9 2014
**************************************************************************
*/
VOID CanElmoDriverEn(MOTOR_PORT motPort)
{
	/* Send enable command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_MO, 0, 1);
}

/*
**************************************************************************
* Description	: Disable driver
  	1) Input argument  : 	motPort - Motor port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov,9 2014
**************************************************************************
*/
VOID CanElmoDriverDis(MOTOR_PORT motPort)
{
	/* Send Disable command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_MO, 0, 0);
}


/*
**************************************************************************
* Description	: Start motor
  	1) Input argument  : 	motPort - Motor port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoStart(MOTOR_PORT motPort)
{
	/* Send start motor command */
	CanElmoSendOnlyCmd(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_BG, 0);
	vTaskDelay(5);
}

/*
**************************************************************************
* Description	: Stop motor
  	1) Input argument  : 	motPort - Motor port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoStop(MOTOR_PORT motPort)
{
	/* Send stop motor command */
	CanElmoSendOnlyCmd(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_ST, 0);
}

/*
**************************************************************************
* Description	: Set speed
  	1) Input argument  : 	motPort - Motor port
						spd - speed value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
BOOL CanElmoSetSpeed(MOTOR_PORT motPort, INT32 spd)
{
	int i;
	spd = (INT32)((INT64)spd * (*EmloDrvCtrlTable[motPort].m_pMotEncoderLineNum) / (*EmloDrvCtrlTable[motPort].m_pMotUnitRatio));

	if(*EmloDrvCtrlTable[motPort].m_pMotDirResvert)
		spd = -spd; 
	/* Send set speed command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_JV, 0, spd);
//	vTaskDelay(5);
	for(i=0;i<5000;i++);
	
	/* Send start motor command */
	CanElmoSendOnlyCmd(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_BG, 0);	
	return TRUE;
}

/*
**************************************************************************
* Description	: Set torque
  	1) Input argument  : 	motPort - Motor port
						torque - torque value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoSetTorque(MOTOR_PORT motPort, INT32 torque)
{
	/* Send set torque command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_TC, 0, torque);
}


/*
**************************************************************************
* Description	: Set accel
  	1) Input argument  : 	motPort - Motor port
  						acc - accel value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoSetAccel(MOTOR_PORT motPort, UINT32 acc)
{
	/* Send set accel command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_AC, 0, acc);
}

/*
**************************************************************************
* Description	: Set Decel
  	1) Input argument  : 	motPort - Motor port
  						dec - accel value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoSetDecel(MOTOR_PORT motPort, UINT32 dec)
{
	/* Send set decel command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_DC, 0, dec);
}

/*
**************************************************************************
* Description	: Set stop Decel
  	1) Input argument  : 	motPort - Motor port
  						dec - accel value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoSetStopDecel(MOTOR_PORT motPort, UINT32 dec)
{
	/* Send set decel command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_SD, 0, dec);
}


/*
**************************************************************************
* Description	: Set user mode
  	1) Input argument  : 	motPort - Motor port
						mode - mode value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoSetUserMode(MOTOR_PORT motPort, EMLO_UM mode)
{
	/* Send set mode command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_UM, 0, (INT32)mode);
}

/*
**************************************************************************
* Description	: Absoult move
  	1) Input argument  : 	motPort - Motor port
						absPos - absoult position value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoAbsMove(MOTOR_PORT motPort, INT32 absPos)
{
	/* Send set absoulte position command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_PA, 0, absPos);
}

/*
**************************************************************************
* Description	: Relative move
  	1) Input argument  : 	motPort - Motor port
						relPos - relative position value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoRelMove(MOTOR_PORT motPort, INT32 relPos)
{
	/* Send set relative position command */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[motPort].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[motPort].m_pMasteAddr), 
					EMLO_CMD_PR, 0, relPos);
}

/*
**************************************************************************
* Description	: Send get position command
  	1) Input argument  : 	motPort - Motor port
						relPos - relative position value for set
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/

VOID CanElmoSendReadPosCmd(MOTOR_PORT port)
{
	/* Send Command */
 	 CanElmoSendOnlyCmd(EmloDrvCtrlTable[port].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[port].m_pMasteAddr), 
					EMLO_CMD_PX, 0);
}


/*
**************************************************************************
* Description	: Get current position from pulse count number of driver
  	1) Input argument  : port - Motor port for set
	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current position
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 12, 2015
**************************************************************************
*/
INT32 CanElmoGetPostion(MOTOR_PORT port)
{
	INT64 totalPulseCnt = 0;
	ELMO_MOTOR_CTRL* pMotCtrlHandler;
	pMotCtrlHandler = &EmloDrvCtrlTable[port];

	if(*EmloDrvCtrlTable[port].m_pMotDirResvert)
		totalPulseCnt = -pMotCtrlHandler->m_AccumPluseCnt;
	else
		totalPulseCnt = pMotCtrlHandler->m_AccumPluseCnt;
		
	totalPulseCnt *= (*(pMotCtrlHandler->m_pMotUnitRatio));;
	totalPulseCnt *= (*(pMotCtrlHandler->m_pMotGearRatioN));
	totalPulseCnt /= (*(pMotCtrlHandler->m_pMotGearRatioD ));
	totalPulseCnt /= (*(pMotCtrlHandler->m_pMotEncoderLineNum));
	//totalPulseCnt %= (*(pMotCtrlHandler->m_pMotUnitRatio));
	/* Send command to get next position */
	CanElmoSendReadPosCmd(port);
	return (INT32)totalPulseCnt;
}

/*
**************************************************************************
* Description	: Clean the count of the driver, so set current as origianl point;
  	1) Input argument  : port - Motor port for set
	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current position
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 12, 2015
**************************************************************************
*/
VOID CanElmoSetOrigin(MOTOR_PORT port)
{
	/* 1. Disable the motor */
	CanElmoDriverDis(port);
	/* 2. Clean count of driver */
	CanElmoSendCmdWithData(EmloDrvCtrlTable[port].m_CanPort,
					(UINT8)(*EmloDrvCtrlTable[port].m_pMasteAddr), 
					EMLO_CMD_PX, 0, 0);
	/* Reenable the motor */
	CanElmoDriverEn(port);
}


/*
**************************************************************************
* Description	: Initilize elmo driver
  	1) Input argument  : 	motPort - Motor port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Nov 18, 2017
**************************************************************************
*/
VOID CanElmoDrvInit(MOTOR_PORT motPort)
{
	/* Initialize can port */
	if(SPD.m_CanProtocolType1 == CAN_PROTOCOL_TYPE_ELMO_DRV)
	{
		EmloDrvCtrlTable[0].m_CanPort = CAN_PORT_1;
		EmloDrvCtrlTable[1].m_CanPort = CAN_PORT_1;
		//EmloDrvCtrlTable[2].m_CanPort = CAN_PORT_1;
	}
	else
	{
		EmloDrvCtrlTable[0].m_CanPort = CAN_PORT_2;
		EmloDrvCtrlTable[1].m_CanPort = CAN_PORT_2;
		//EmloDrvCtrlTable[2].m_CanPort = CAN_PORT_2;
	}	
	/* Active CAN */
	CanElmoActiveCan(EmloDrvCtrlTable[motPort].m_CanPort);
	vTaskDelay(5);
	/* Disable Driver */
	CanElmoDriverDis(motPort);
	vTaskDelay(5);
	/* Set User mode */
	CanElmoSetUserMode(motPort, EmloDrvCtrlTable[motPort].m_UM);
	vTaskDelay(5);
	/* Set Accel */
	//CanElmoSetAccel(motPort, EmloDrvCtrlTable[motPort].m_Accel);
	vTaskDelay(5);
	/* Set Decel */
	//CanElmoSetDecel(motPort, EmloDrvCtrlTable[motPort].m_Decel);

	vTaskDelay(5);
	/* Set stop Decel */
	//CanElmoSetStopDecel(motPort, EmloDrvCtrlTable[motPort].m_StopDecel);
	vTaskDelay(5);
	/* Enable Driver */
	//CanElmoDriverEn(motPort);
	vTaskDelay(5);
}




/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


