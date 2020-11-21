/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	COM_ModbusRTU.h
//
// TITLE:	This function realized the Modbus communication protocol.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 9,2014
//
// Description: Modbus communication protocol realization.
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

#if !defined(COM_MODBUS_RTU_H_)
#define COM_MODBUS_RTU_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef		COM_MODBUS_RTU_GLOBAL_
#define		COM_MODBUS_RTU_EXT
#else    
#define		COM_MODBUS_RTU_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include "BSP_BasicDataType.h"			// Basic data type define.
#include "UTL_USART.h"

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
#define SERIAL_Q_SIZE	256


/*
********************************************************************************************************
*	 Global data type
********************************************************************************************************
*/
/* USART PORT Enum */
typedef enum tagMODBUS_PORT
{
	MODBUS_PORT_232 = 0,
	MODBUS_PORT_232_M,
//	MODBUS_PORT_USB,
	MODBUS_PORT_MAX
}MODBUS_PORT;

/* Serial queue define */
typedef struct tagSERIAL_QUEUE
{
  UINT8		m_buffer[SERIAL_Q_SIZE];
  UINT16	m_count;
}SERIAL_QUEUE;

/* CDP frame define */
typedef struct tagCDP_CONTROL_FRAME
{
  UINT8		m_Head[2];
  UINT8		m_SourAdd;
  UINT8		m_DestAdd;
  UINT8		m_FrameLen;
  UINT8		m_CmdId;
  UINT8		m_Data[35];
  UINT8		m_Resv[6];
  UINT8		m_PortSta;
  UINT8		m_FrameCnt;
  UINT8		m_ChkSum;
}CDP_CONTROL_FRAME;

/* ATA ANS format define */
typedef struct tagABA_ANS_FRAME
{
  UINT8		m_Head[2];
  UINT8		m_SourAdd;
  UINT8		m_DestAdd;
  UINT8		m_FrameLen;
  UINT8		m_AntAngle[5];
  UINT8		m_LnAmpSta;
  UINT8		m_AntSta;
  UINT8		m_Resv[5];
  UINT8		m_PortSta;
  UINT8		m_FrameCnt;
  UINT8		m_ChkSum;
}ABA_ANS_FRAME;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
COM_MODBUS_RTU_EXT CDP_CONTROL_FRAME CdpControlBuf;

COM_MODBUS_RTU_EXT ABA_ANS_FRAME AbaAnsBuf;

COM_MODBUS_RTU_EXT	SERIAL_QUEUE	s_SerialQueue[SERIAL_PORT_MAX];

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
COM_MODBUS_RTU_EXT VOID ModbusRtuLoop(SERIAL_PORT port, UINT8 slvAdd);


COM_MODBUS_RTU_EXT VOID ProcessModbusRtuFrame(SERIAL_QUEUE* pQueue, UINT8 slvAdd);


COM_MODBUS_RTU_EXT void Task_CircularAnsSend(void* p_arg);

#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


