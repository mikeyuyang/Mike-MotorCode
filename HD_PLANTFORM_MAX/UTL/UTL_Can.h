/*
//######################################################################################################
//
// FILE:	UTL_Can.c
//
// TITLE:	Can Driver functions.
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: Can Driver
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris.Shang
// Created date:   	Oct 28, 2017
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

#if !defined(UTL_CAN_H_)
#define UTL_CAN_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef		UTL_CAN_GLOBAL_				
#define		UTL_CAN_EXT
#else    
#define		UTL_CAN_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include <stm32f4xx.h>

#include <includes.h>
#include "BSP_BasicDataType.h"			// Basic data type define.
#include "UTL_1msTimer.h"

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
#define CAN_BUF_SIZE	32

/*
********************************************************************************************************
*	 Global data type
********************************************************************************************************
*/
/* Serial port define. */
typedef enum tagCAN_PORT
{
	CAN_PORT_1 = 0,
	CAN_PORT_2,
	CAN_PORT_MAX
}CAN_PORT;


/* Can port define. */
typedef enum tagCAN_BANDRATE
{
	CAN_BANDRATE_100K = 0,
	CAN_BANDRATE_125K,
	CAN_BANDRATE_250K,
	CAN_BANDRATE_500K,
	CAN_BANDRATE_1M,
	CAN_BANDRATE_MAX,
}CAN_BANDRATE;

/* CAN parameter */
typedef struct tagCAN_PARAM
{
	CAN_BANDRATE	m_Baudrate;
    UINT16	m_TimeOut;
}CAN_PARAM;

typedef struct tagCAN_MSG
{
	UINT32 m_StdId;  
	UINT32 m_ExtId;  
	UINT8 m_IDE; 
	UINT8 m_RTR; 
	UINT8 m_DLC; 
	UINT8 m_Data[8]; 
} CAN_MSG;


/* CAN TX Buffer */
typedef struct tagCAN_BUF
{
  CAN_MSG m_buffer[CAN_BUF_SIZE];
  UINT8	m_WrIdx;
  UINT8	m_RdIdx;
  UINT8	m_Count;
}CAN_BUF;

/* USART Port buffers */
typedef struct tagCAN_PORT_BUFS
{
	CAN_BUF m_RXBuffer;
	CAN_BUF m_TXBuffer;
}CAN_PORT_BUFS;

/* CAN status */
typedef enum tagCAN_STATUS
{
	CAN_STATUS_RECEIVING = 0,
	CAN_STATUS_TRANSMITTING,
} CAN_STATUS;


/* CAN communication handler */
typedef struct tagCAN_COM_HANDLER
{
	CAN_PORT_BUFS m_Buf;
	CAN_STATUS	m_Status;
	EventGroupHandle_t	m_Sem;
	TIMER m_Timer;
}CAN_COM_HANDLER;

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
UTL_CAN_EXT CAN_COM_HANDLER CanHandler[CAN_PORT_MAX];

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
UTL_CAN_EXT BOOL InitCanPort(CAN_PORT port, CAN_PARAM* param);

UTL_CAN_EXT VOID StartCanComCtrlTimer(CAN_PORT port);

UTL_CAN_EXT VOID StopCanComCtrlTimer(CAN_PORT port);

UTL_CAN_EXT BOOL isCanComCtrlTimerElapsed(CAN_PORT port);

UTL_CAN_EXT VOID CanSendOneMsg(CAN_PORT port, CAN_MSG *pMsg);

UTL_CAN_EXT VOID CanPushMsg(CAN_PORT port, CAN_MSG *pMsg);

UTL_CAN_EXT CAN_STATUS GetCanPortStatus(CAN_PORT port);

UTL_CAN_EXT VOID SetCanPortStatus(CAN_PORT port, CAN_STATUS status);

UTL_CAN_EXT CAN_PORT_BUFS* GetCanPortBuffer(CAN_PORT port);

UTL_CAN_EXT EventGroupHandle_t* GetCanPortSem(CAN_PORT port);

UTL_CAN_EXT BOOL IsCanTxIdle(CAN_PORT port);

#endif	//#if !defined(UTL_US_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


