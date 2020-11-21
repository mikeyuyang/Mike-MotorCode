/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	UTL_USART.h
//
// TITLE:	USART Driver functions.(ATMage16)
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Jul 6,2014
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Jul 6,2014
// Version:			V1.0
// Descriptions: 	First Version
//
//------------------------------------------------------------------------------------------------------
// Modified by:		Chris Shang
// Modified date:	Nov, 5 2014
// Version:			V1.1
// Descriptions:
//
//------------------------------------------------------------------------------------------------------
//######################################################################################################
*/

/*
********************************************************************************************************
********************************************************************************************************
*/

#if !defined(UTL_USART_H_)
#define UTL_USART_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef		UTL_USART_GLOBAL_				
#define		UTL_USART_EXT
#else    
#define		UTL_USART_EXT  extern
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
#define USART_BUF_SIZE		512

#define		FRAME_HEAD1_OFFSET		0
#define		FRAME_HEAD2_OFFSET		1
#define		FRAME_ADD_OFFSET		2
#define		FRAME_FUNCODE_OFFSET	3
#define		FRAME_DATA_OFFSET		4

#define		REC_BUF_LEN		20						// USART receive Buffer length
#define		RD_BUF_LEN		20						// USART read Buffer length
#define		SEND_BUF_LEN	20						// USART send Buffer length


/*
********************************************************************************************************
*	 Global data type
********************************************************************************************************
*/
/* Serial port define. */
typedef enum tagSERIAL_PORT
{
	SERIAL_PORT_1 = 0,
	SERIAL_PORT_2,
	SERIAL_PORT_3,
	SERIAL_PORT_4,
	SERIAL_PORT_5,
	SERIAL_PORT_6,
	SERIAL_PORT_7,
	SERIAL_PORT_8,
	SERIAL_PORT_MAX
}SERIAL_PORT;

/* USART BitLen enum */
typedef enum tagUSART_BIT_LEN
{
	USART_BIT_LEN_8 = 0,
	USART_BIT_LEN_9,
}USART_BIT_LEN;


/* USART STOP bit enum */
typedef enum tagUSART_STOP_BIT
{
	USART_STOP_BIT_ONE = 0,
	USART_STOP_BIT_ONEHALF,
	USART_STOP_BIT_TWO
} USART_STOP_BIT;

/* USART parity bit enum */
typedef enum tagUSART_PARITY
{
	USART_PARITY_NO = 0,
	USART_PARITY_ODD,
	USART_PARITY_EVEN
} USART_PARITY;

/* USART configures parameter types enum */
typedef enum tagUSART_CFG
{
	USART_CFG_BAUDRATE = 0,
	USART_CFG_DATABITS,
	USART_CFG_STOPBITS,
	USART_CFG_PARITY
} USART_CFG;

/* USART status */
typedef enum tagUSART_STATUS
{
	USART_STATUS_RECEIVING = 0,
	USART_STATUS_TRANSMITTING,
} USART_STATUS;

/* USART parameter */
typedef struct tagUSART_PARAM
{
	UINT32	m_Baudrate;
    USART_BIT_LEN	m_BitLen;
    USART_STOP_BIT	m_Stopbits;
    USART_PARITY	m_Parity;
    UINT16	m_TimeOut;
}USART_PARAM;

/* USART Buffer */
typedef struct tagUART_BUF
{
  UINT8		m_buffer[USART_BUF_SIZE];
  UINT16	m_count;
  UINT16	m_WrIdx;
  UINT16	m_RdIdx;
}USART_BUF;

/* USART Port buffers */
typedef struct
{
	USART_BUF RXBuffer;
	USART_BUF TXBuffer;
	UINT16 TXCount;
} USART_PORT_BUFS;

/* Uart communication handler */
typedef struct tagUSART_COM_HANDLE
{
	USART_PORT_BUFS m_Buf;
	USART_STATUS	m_Status;
	SemaphoreHandle_t	m_RxFrameSem;
	SemaphoreHandle_t	m_TxSem;
	TIMER m_Timer;
}USART_COM_HANDLE;

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
UTL_USART_EXT BOOL SetUSARTPortParams(SERIAL_PORT port, USART_PARAM* param);

UTL_USART_EXT BOOL InitUSARTPort(SERIAL_PORT port, USART_PARAM* param);

UTL_USART_EXT VOID RS485Input(VOID);

UTL_USART_EXT VOID RS485Output(VOID);

UTL_USART_EXT VOID USARTSendnByte(SERIAL_PORT port, UCHAR *Ptr, UCHAR Len);

UTL_USART_EXT USART_STATUS GetUSARTPortStatus(SERIAL_PORT port);

UTL_USART_EXT VOID SetUSARTPortStatus(SERIAL_PORT port, USART_STATUS status);

UTL_USART_EXT USART_COM_HANDLE* GetUSARTPortHandler(SERIAL_PORT port);

UTL_USART_EXT EventGroupHandle_t* GetUSARTPortSem(SERIAL_PORT port);

UTL_USART_EXT VOID StartComCtrlTimer(SERIAL_PORT port);

UTL_USART_EXT VOID StopComCtrlTimer(SERIAL_PORT port);

UTL_USART_EXT BOOL IsComCtrlTimerElapsed(SERIAL_PORT port);


#endif	//#if !defined(UTL_US_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


