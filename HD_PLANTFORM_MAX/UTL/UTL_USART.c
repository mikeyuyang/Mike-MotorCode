/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	UTL_USART.c
//
// TITLE:	USART Driver functions.(ATMage16)
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 6, 2014
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Nov 6, 2014
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
#include <includes.h>						// OS include files
#include <string.h>
#include <stm32f4xx.h>

#include "BSP_BasicDataType.h"	// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
//#include "SYS_FIFOOpt.h"
//#include "UTL_OCTimer.h"
#define UTL_USART_GLOBAL_
#include "UTL_USART.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/



/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
//#define		F_CPU			72000000

const UINT32 UART_BANDRATE_TABLE[5] = {9600,	19200, 38400, 57600, 115200};

#define 	TIMEOUT_9600_BAUD	8400
#define 	TIMEOUT_19200_BAUD	4200
#define 	TIMEOUT_38400_BAUD	2100
#define 	TIMEOUT_57600_BAUD	1400
#define 	TIMEOUT_115200_BAUD	1400

/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/

/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/
typedef enum tagUSART_PORT_PIN		// Ports of DO.
{
	USART_PORT_PIN_TX = 0,
	USART_PORT_PIN_RX,
	USART_PORT_PIN_DIR,
	USART_PORT_PIN_MAX
}USART_PORT_PIN;

/* USART Hardward INFO */
typedef struct tagUSART_HW_INFO
{
	USART_TypeDef*	m_UartHandle;
	UINT8			m_AF;
    UINT16	m_IntChan;
	GPIO_PORT_PIN 	m_UartPin[USART_PORT_PIN_MAX];
}USART_HW_INFO;


/*
********************************************************************************************************
*	 Local variables
********************************************************************************************************
*/

const USART_HW_INFO USART_HW_INFO_TABLE[SERIAL_PORT_MAX] =
{
/*		PORT,		AF				IRQ				GPIO_PORT					GPIO_PIN		PINSOURCE*/
	{	USART1,		GPIO_AF_USART1,	USART1_IRQn,	{{(GPIO_TypeDef*)GPIOA,		GPIO_Pin_9,		9},	
													{(GPIO_TypeDef*)GPIOA,		GPIO_Pin_10,	10},
													{(GPIO_TypeDef*)0,			0,				0}}				
	},
	{	USART2,		GPIO_AF_USART2,	USART2_IRQn,	{{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_5,		5},
													{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_6,		6},
													{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_2,		2}}
	},
	{	USART3,		GPIO_AF_USART3,	USART3_IRQn,	{{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_8,	8},
													{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_9,	9},
													{(GPIO_TypeDef*)0,			0,				0}}
	},
	{	UART4,		GPIO_AF_UART4,	UART4_IRQn,	{{(GPIO_TypeDef*)GPIOC,		GPIO_Pin_10,	10},
													{(GPIO_TypeDef*)GPIOC,		GPIO_Pin_11,	11},
													{(GPIO_TypeDef*)0,			0,				0}}
	},
	{	UART5, 		GPIO_AF_UART5,	UART5_IRQn,		{{(GPIO_TypeDef*)GPIOC,		GPIO_Pin_12,	12},
													{(GPIO_TypeDef*)GPIOD,		GPIO_Pin_2,		2},
													{(GPIO_TypeDef*)0,			0,				0}}
	},
	{	USART6, 	GPIO_AF_USART6,	USART6_IRQn,	{{(GPIO_TypeDef*)GPIOC,		GPIO_Pin_6,		6},
													{(GPIO_TypeDef*)GPIOC,		GPIO_Pin_7,		7},
													{(GPIO_TypeDef*)GPIOG,		GPIO_Pin_3,		3}}
	},
	{	UART7, 	GPIO_AF_UART7,	UART7_IRQn,	{{(GPIO_TypeDef*)GPIOE,		GPIO_Pin_8,		8},
													{(GPIO_TypeDef*)GPIOE,		GPIO_Pin_7,		7},
													{(GPIO_TypeDef*)0,		0,		0}}
	},
	{	UART8, 	GPIO_AF_UART8,	UART8_IRQn,	{{(GPIO_TypeDef*)GPIOE,		GPIO_Pin_1,		1},
													{(GPIO_TypeDef*)GPIOE,		GPIO_Pin_0,		0},
													{(GPIO_TypeDef*)0,		0,		0}}
	}
};

USART_InitTypeDef USART_InitStructure;

USART_COM_HANDLE UARTHandler[SERIAL_PORT_MAX];

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
* Description	: Set parameter of USART.
  	1) Input argument  :
 	2) Modify argument :
 	3) Output argument :
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Oct 21, 2017
**************************************************************************
*/
BOOL SetUSARTPortParams(SERIAL_PORT port, USART_PARAM* param)
{
	USART_TypeDef* USARTReg;
	USARTReg = (USART_TypeDef* )USART_HW_INFO_TABLE[port].m_UartHandle;
	/* Set USART initialization struct. */
	USART_InitStructure.USART_BaudRate = param->m_Baudrate;
	/* Set bit length */
	if(param->m_BitLen == USART_BIT_LEN_8)
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	else if(param->m_BitLen == USART_BIT_LEN_9)
		USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	else
		return FALSE;
	
	switch(param->m_Stopbits)
	{
		case USART_STOP_BIT_ONE:
		{
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			break;
		}
		case USART_STOP_BIT_ONEHALF:
		{
			USART_InitStructure.USART_StopBits = USART_StopBits_1_5;
			break;
		}
		case USART_STOP_BIT_TWO:
		{
			USART_InitStructure.USART_StopBits = USART_StopBits_2;
			break;
		}
		default:
			return FALSE;
	}
	switch(param->m_Parity)
	{
		case USART_PARITY_NO:
		{
			USART_InitStructure.USART_Parity = USART_Parity_No;
			break;
		}
		case USART_PARITY_ODD:
		{
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			break;
		}
		case USART_PARITY_EVEN:
		{
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			break;
		}
		default:
			return FALSE;
	}
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USARTReg, &USART_InitStructure);

	return TRUE;
}

/*
**************************************************************************
* Description	: Initialization USART Ports.
  	1) Input argument  : 	port - USART port
  							setParams - USART port parameters
 	2) Modify argument :
 	3) Output argument :
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov6, 2014
**************************************************************************
*/
BOOL InitUSARTPort(SERIAL_PORT port, USART_PARAM* param)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	USART_HW_INFO*	pHwInfo;

	pHwInfo = (USART_HW_INFO*)&USART_HW_INFO_TABLE[port];
	
	/* GPIO initialization */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* Configure USARTx Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_UartPin[USART_PORT_PIN_TX].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_TX].port, &GPIO_InitStructure);
	GPIO_PinAFConfig((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_TX].port, pHwInfo->m_UartPin[USART_PORT_PIN_TX].pinSource, pHwInfo->m_AF);

	/* Configure USARTx Rx as alternate function */
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_UartPin[USART_PORT_PIN_RX].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_RX].port, &GPIO_InitStructure);
	GPIO_PinAFConfig((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_RX].port, pHwInfo->m_UartPin[USART_PORT_PIN_RX].pinSource, pHwInfo->m_AF);


	/* Configure DIR PIN as pull up input */
	if(0 != pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port)
	{
		GPIO_InitStructure.GPIO_Pin = pHwInfo->m_UartPin[USART_PORT_PIN_DIR].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port, &GPIO_InitStructure);
	}


	/* Set USART Port parameter */
	SetUSARTPortParams(port, param);


	/* Buffer initialization */
	UARTHandler[port].m_Buf.RXBuffer.m_count = 0;
	UARTHandler[port].m_Buf.RXBuffer.m_WrIdx= 0;
	UARTHandler[port].m_Buf.RXBuffer.m_RdIdx= 0;
	
	UARTHandler[port].m_Buf.TXBuffer.m_count = 0;
	UARTHandler[port].m_Buf.TXCount = 0;
	UARTHandler[port].m_Status= USART_STATUS_RECEIVING;
	/* Creat TX semaphore */
	UARTHandler[port].m_TxSem = xSemaphoreCreateBinary();
	xSemaphoreGive(UARTHandler[port].m_TxSem);
	InitmsTimer(&UARTHandler[port].m_Timer, (UINT32)param->m_TimeOut);

	/* Enable USART Re-map */
//	if(USART2 == USARTPortCfgs[port].pUart)
//		GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);

	/* Configures interrupts */
	/* NVIC configure */
	NVIC_InitStructure.NVIC_IRQChannel = pHwInfo->m_IntChan;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable USART1 Receive and Transmit interrupts */
	USART_ITConfig((USART_TypeDef* )pHwInfo->m_UartHandle, USART_IT_RXNE, ENABLE);
	/* Enable the USART */
	USART_Cmd((USART_TypeDef* )pHwInfo->m_UartHandle, ENABLE);

	return TRUE;
}


/*
**************************************************************************
* Description	: Set Direction as input.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov 7, 2014
**************************************************************************
*/
VOID SetSerialAsInput(SERIAL_PORT port)
{
	USART_HW_INFO*	pHwInfo;
	
	pHwInfo = (USART_HW_INFO*)&USART_HW_INFO_TABLE[port];
	if(pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port)
		GPIO_ResetBits((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port, pHwInfo->m_UartPin[USART_PORT_PIN_DIR].pin);
}

/*
**************************************************************************
* Description	: Set Direction as output.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov,7 2014
**************************************************************************
*/
VOID SetSerialAsOutput(SERIAL_PORT port)
{
	USART_HW_INFO*	pHwInfo;
	
	pHwInfo = (USART_HW_INFO*)&USART_HW_INFO_TABLE[port];
	if(pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port)
		GPIO_SetBits((GPIO_TypeDef* )pHwInfo->m_UartPin[USART_PORT_PIN_DIR].port, pHwInfo->m_UartPin[USART_PORT_PIN_DIR].pin);
}

/*
**************************************************************************
* Description	: Comunication control timer functions.
  	1) Input argument  : 
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
VOID StartComCtrlTimer(SERIAL_PORT port)
{
	StartTimer(&UARTHandler[port].m_Timer);
}

VOID StopComCtrlTimer(SERIAL_PORT port)
{
	StopTimer(&UARTHandler[port].m_Timer);
}

BOOL IsComCtrlTimerElapsed(SERIAL_PORT port)
{
	return (isElapsed(&UARTHandler[port].m_Timer));
}

/*
**************************************************************************
* Description	: USART1 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void USART1_IRQHandler(void)
{
	USART_BUF* pBuf;
	BaseType_t higherPriorityTaskWoken;
	/* Is there data in receive data register */
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_1].m_Buf.RXBuffer;
		/* Read one byte from the receive data register */
		pBuf->m_buffer[pBuf->m_WrIdx ++] = USART_ReceiveData(USART1);
		if(pBuf->m_WrIdx >= USART_BUF_SIZE)
		{
			pBuf->m_WrIdx = 0;
		}
		pBuf->m_count ++;
		/* Clear 3.5 char time count, then restart */
		StartComCtrlTimer(SERIAL_PORT_1);
	}
	/* Tx buffer empty process */
	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_1].m_Buf.TXBuffer;
		/* Write one byte to the transmit data register */
		USART_SendData(USART1, pBuf->m_buffer[UARTHandler[SERIAL_PORT_1].m_Buf.TXCount ++]);
		/* Transmit finish */
		if(UARTHandler[SERIAL_PORT_1].m_Buf.TXCount == pBuf->m_count)
		{
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
			/* Enable transmit completed interrupt */
			USART_ITConfig(USART1, USART_IT_TC, ENABLE);
		}
	}
	/* Transmit completed interrupt process */
	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
	{
		/* Disable the USART1 Transmit  interrupt */
		USART_ITConfig(USART1, USART_IT_TC, DISABLE);
		pBuf = &UARTHandler[SERIAL_PORT_1].m_Buf.TXBuffer;
		/* Reset buffer count can transmit count */
		pBuf->m_count = 0;
		UARTHandler[SERIAL_PORT_1].m_Buf.TXCount = 0;

		SetSerialAsInput(SERIAL_PORT_1);
		UARTHandler[SERIAL_PORT_1].m_Status = USART_STATUS_RECEIVING;
		xSemaphoreGiveFromISR(UARTHandler[SERIAL_PORT_1].m_TxSem, &higherPriorityTaskWoken);
		/* Wait 3.5 char time after Tx finish */
		//StartComCtrlTimer(SERIAL_PORT_1);
	}
}

/*
**************************************************************************
* Description	: USART2 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void USART2_IRQHandler(void)
{
	USART_BUF* pBuf;
	BaseType_t higherPriorityTaskWoken;
	/* Is there data in receive data register */
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_2].m_Buf.RXBuffer;
		/* Read one byte from the receive data register */
		pBuf->m_buffer[pBuf->m_WrIdx ++] = USART_ReceiveData(USART2);
		if(pBuf->m_WrIdx >= USART_BUF_SIZE)
		{
			pBuf->m_WrIdx = 0;
		}
		pBuf->m_count ++;
		/* Clear 3.5 char time count, then restart */
		StartComCtrlTimer(SERIAL_PORT_2);
	}
	/* Tx buffer empty process */
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_2].m_Buf.TXBuffer;
		/* Write one byte to the transmit data register */
		USART_SendData(USART2, pBuf->m_buffer[UARTHandler[SERIAL_PORT_2].m_Buf.TXCount ++]);
		/* Transmit finish */
		if(UARTHandler[SERIAL_PORT_2].m_Buf.TXCount == pBuf->m_count)
		{
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
			/* Enable transmit completed interrupt */
			USART_ITConfig(USART2, USART_IT_TC, ENABLE);
		}
	}
	/* Transmit completed interrupt process */
	if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)
	{
		/* Disable the USART1 Transmit  interrupt */
		USART_ITConfig(USART2, USART_IT_TC, DISABLE);
		pBuf = &UARTHandler[SERIAL_PORT_2].m_Buf.TXBuffer;
		/* Reset buffer count can transmit count */
		pBuf->m_count = 0;
		UARTHandler[SERIAL_PORT_2].m_Buf.TXCount = 0;

		SetSerialAsInput(SERIAL_PORT_2);
		UARTHandler[SERIAL_PORT_2].m_Status = USART_STATUS_RECEIVING;
		xSemaphoreGiveFromISR(UARTHandler[SERIAL_PORT_2].m_TxSem, &higherPriorityTaskWoken);
		/* Wait 3.5 char time after Tx finish */
		//StartComCtrlTimer(SERIAL_PORT_1);
	}
}


/*
**************************************************************************
* Description	: USART3 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Oct 28, 2017
**************************************************************************
*/
void USART3_IRQHandler(void)
{
	USART_BUF* pBuf;
	BaseType_t higherPriorityTaskWoken;
	/* Is there data in receive data register */
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_3].m_Buf.RXBuffer;
		/* Read one byte from the receive data register */
		pBuf->m_buffer[pBuf->m_WrIdx ++] = USART_ReceiveData(USART3);
		pBuf->m_WrIdx %= USART_BUF_SIZE;

		pBuf->m_count ++;
		/* Clear 3.5 char time count, then restart */
		StartComCtrlTimer(SERIAL_PORT_3);
	}
	/* Tx buffer empty process */
	if(USART_GetITStatus(USART3, USART_IT_TXE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_3].m_Buf.TXBuffer;
		/* Write one byte to the transmit data register */
		USART_SendData(USART3, pBuf->m_buffer[UARTHandler[SERIAL_PORT_3].m_Buf.TXCount ++]);
		/* Transmit finish */
		if(UARTHandler[SERIAL_PORT_3].m_Buf.TXCount == pBuf->m_count)
		{
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
			/* Enable transmit completed interrupt */
			USART_ITConfig(USART3, USART_IT_TC, ENABLE);
		}
	}
	/* Transmit completed interrupt process */
	if(USART_GetITStatus(USART3, USART_IT_TC) != RESET)
	{
		/* Disable the USART2 Transmit	interrupt */
		USART_ITConfig(USART3, USART_IT_TC, DISABLE);
		pBuf = &UARTHandler[SERIAL_PORT_3].m_Buf.TXBuffer;
		/* Reset buffer count can transmit count */
		pBuf->m_count = 0;
		UARTHandler[SERIAL_PORT_3].m_Buf.TXCount = 0;
		SetSerialAsInput(SERIAL_PORT_3);
		UARTHandler[SERIAL_PORT_3].m_Status = USART_STATUS_RECEIVING;
		xSemaphoreGiveFromISR(UARTHandler[SERIAL_PORT_3].m_TxSem, &higherPriorityTaskWoken);
		/* Wait 3.5 char time after Tx finish */
		//StartComCtrlTimer(SERIAL_PORT_3);
	}
}

/*
**************************************************************************
* Description	: USART5 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Oct 28, 2017
**************************************************************************
*/
#if 1
void UART5_IRQHandler(void)
{
	USART_BUF* pBuf;
	/* Is there data in receive data register */
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_5].m_Buf.RXBuffer;
		/* Read one byte from the receive data register */
		pBuf->m_buffer[pBuf->m_count ++] = USART_ReceiveData(UART5);
		if(pBuf->m_count >= USART_BUF_SIZE)
		{
			pBuf->m_count = 0;
		}
		/* Clear 3.5 char time count, then restart */
		StartComCtrlTimer(SERIAL_PORT_5);
	}
	/* Tx buffer empty process */
	if(USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_5].m_Buf.TXBuffer;
		/* Write one byte to the transmit data register */
		USART_SendData(UART5, pBuf->m_buffer[UARTHandler[SERIAL_PORT_5].m_Buf.TXCount ++]);
		/* Transmit finish */
		if(UARTHandler[SERIAL_PORT_5].m_Buf.TXCount == pBuf->m_count)
		{
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(UART5, USART_IT_TXE, DISABLE);
			/* Enable transmit completed interrupt */
			USART_ITConfig(UART5, USART_IT_TC, ENABLE);
		}
	}
	/* Transmit completed interrupt process */
	if(USART_GetITStatus(UART5, USART_IT_TC) != RESET)
	{
		/* Disable the USART2 Transmit	interrupt */
		USART_ITConfig(UART5, USART_IT_TC, DISABLE);
		pBuf = &UARTHandler[SERIAL_PORT_5].m_Buf.TXBuffer;
		/* Reset buffer count can transmit count */
		pBuf->m_count = 0;
		UARTHandler[SERIAL_PORT_5].m_Buf.TXCount = 0;
		SetSerialAsInput(SERIAL_PORT_5);
		UARTHandler[SERIAL_PORT_5].m_Status = USART_STATUS_RECEIVING;
		/* Wait 3.5 char time after Tx finish */
		//StartComCtrlTimer(SERIAL_PORT_5);
	}
}
#endif
/*
**************************************************************************
* Description	: USART6 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Oct 28, 2017
**************************************************************************
*/
#if 1
void USART6_IRQHandler(void)
{
	USART_BUF* pBuf;
	/* Is there data in receive data register */
	if(USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_6].m_Buf.RXBuffer;
		/* Read one byte from the receive data register */
		pBuf->m_buffer[pBuf->m_count ++] = USART_ReceiveData(UART5);
		if(pBuf->m_count >= USART_BUF_SIZE)
		{
			pBuf->m_count = 0;
		}
		/* Clear 3.5 char time count, then restart */
		StartComCtrlTimer(SERIAL_PORT_6);
	}
	/* Tx buffer empty process */
	if(USART_GetITStatus(USART6, USART_IT_TXE) != RESET)
	{
		pBuf = &UARTHandler[SERIAL_PORT_6].m_Buf.TXBuffer;
		/* Write one byte to the transmit data register */
		USART_SendData(USART6, pBuf->m_buffer[UARTHandler[SERIAL_PORT_6].m_Buf.TXCount ++]);
		/* Transmit finish */
		if(UARTHandler[SERIAL_PORT_6].m_Buf.TXCount == pBuf->m_count)
		{
			/* Disable the USART1 Transmit interrupt */
			USART_ITConfig(USART6, USART_IT_TXE, DISABLE);
			/* Enable transmit completed interrupt */
			USART_ITConfig(USART6, USART_IT_TC, ENABLE);
		}
	}
	/* Transmit completed interrupt process */
	if(USART_GetITStatus(USART6, USART_IT_TC) != RESET)
	{
		/* Disable the USART2 Transmit	interrupt */
		USART_ITConfig(USART6, USART_IT_TC, DISABLE);
		pBuf = &UARTHandler[SERIAL_PORT_6].m_Buf.TXBuffer;
		/* Reset buffer count can transmit count */
		pBuf->m_count = 0;
		UARTHandler[SERIAL_PORT_6].m_Buf.TXCount = 0;
		SetSerialAsInput(SERIAL_PORT_6);
		UARTHandler[SERIAL_PORT_6].m_Status = USART_STATUS_RECEIVING;
		
		/* Wait 3.5 char time after Tx finish */
		//StartComCtrlTimer(SERIAL_PORT_6);
	}
}
#endif
/*
**************************************************************************
* Description	: Send n Byte at one time.
  	1) Input argument  : port - USART port
  							Ptr - Start point of sending data
  							Len - Length of data
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jul,6 2014
**************************************************************************
*/
VOID USARTSendnByte(SERIAL_PORT port, UCHAR *Ptr, UCHAR Len)
{
	USART_BUF* pBuf;
	USART_TypeDef* USARTReg;

	
	if(pdTRUE == xSemaphoreTake(UARTHandler[port].m_TxSem, portMAX_DELAY))
	{
		USARTReg = (USART_TypeDef* )USART_HW_INFO_TABLE[port].m_UartHandle;
		/* Get port TX Buffer point */
		pBuf = &UARTHandler[port].m_Buf.TXBuffer;
		/* Copy datas */
		memcpy(pBuf->m_buffer, Ptr, Len);
		pBuf->m_count = Len;
		UARTHandler[port].m_Buf.TXCount = 0;

		USART_ITConfig(USARTReg, USART_IT_TXE, ENABLE);

		UARTHandler[port].m_Status = USART_STATUS_TRANSMITTING;
		
		SetSerialAsOutput(port);
	}
}

/*
**************************************************************************
* Description	: Get current USART status.
  	1) Input argument  : port - USART port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
USART_STATUS GetUSARTPortStatus(SERIAL_PORT port)
{
	return UARTHandler[port].m_Status;
}

/*
**************************************************************************
* Description	: Set current USART status.
  	1) Input argument  : port - USART port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
VOID SetUSARTPortStatus(SERIAL_PORT port, USART_STATUS status)
{
	UARTHandler[port].m_Status = status;
}

/*
**************************************************************************
* Description	: Get Handle of USART Port.
  	1) Input argument  : port - USART port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Handle of uart
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
USART_COM_HANDLE* GetUSARTPortHandler(SERIAL_PORT port)
{
	return (&UARTHandler[port]);
}

/*
**************************************************************************
* Description	: Get buffer of USART Port.
  	1) Input argument  : port - USART port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
EventGroupHandle_t* GetUSARTPortSem(SERIAL_PORT port)
{
	return (&UARTHandler[port].m_TxSem);
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
	
	
	
	
	
	
	
	
	
	
		
