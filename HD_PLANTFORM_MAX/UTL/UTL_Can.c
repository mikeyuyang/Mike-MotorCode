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
#define UTL_CAN_GLOBAL_
#include "UTL_CAN.h"

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
typedef enum tagCAN_PORT_PIN		// Ports of DO.
{
	CAN_PORT_PIN_TX = 0,
	CAN_PORT_PIN_RX,
	CAN_PORT_PIN_MAX
}CAN_PORT_PIN;

/* CAN Hardward INFO */
typedef struct tagCAN_HW_INFO
{
	CAN_TypeDef*	m_CanHandle;
	UINT8			m_AF;
    UINT16	m_IntChanTx;
    UINT16	m_IntChanRx0;
    UINT16	m_IntChanRx1;
	GPIO_PORT_PIN 	m_CanPin[CAN_PORT_PIN_MAX];
}CAN_HW_INFO;


/*
********************************************************************************************************
*	 Local variables
********************************************************************************************************
*/

const CAN_HW_INFO CAN_HW_INFO_TABLE[CAN_PORT_MAX] =
{
/*		PORT,		AF				IRQ_TX			IRQ_RX0		IRQ_RX1		GPIO_PORT					GPIO_PIN		PINSOURCE*/
	{	CAN1,		GPIO_AF_CAN1,	CAN1_TX_IRQn,	CAN1_RX0_IRQn,	CAN1_RX1_IRQn,	{{(GPIO_TypeDef*)GPIOA,		GPIO_Pin_12,	12},	
																					{(GPIO_TypeDef*)GPIOA,		GPIO_Pin_11,	11}}				
	},
	{	CAN2,		GPIO_AF_CAN2,	CAN2_TX_IRQn,	CAN2_RX0_IRQn,	CAN2_RX1_IRQn,	{{(GPIO_TypeDef*)GPIOB,		GPIO_Pin_6,		6},
																					{(GPIO_TypeDef*)GPIOB,		GPIO_Pin_5,		5}}
	}
};

CAN_InitTypeDef Can_InitStructure;
CAN_FilterInitTypeDef  Can_FilterInitStructure;


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
BOOL SetCanPortParams(CAN_PORT port, CAN_PARAM* param)
{
	CAN_TypeDef* CANReg;
	CANReg = (CAN_TypeDef* )CAN_HW_INFO_TABLE[port].m_CanHandle;

	/* CAN register init */
	CAN_DeInit(CANReg);
	
	/* CAN cell init */
	Can_InitStructure.CAN_TTCM = DISABLE;
	Can_InitStructure.CAN_ABOM = DISABLE;
	Can_InitStructure.CAN_AWUM = DISABLE;
	Can_InitStructure.CAN_NART = DISABLE;
	Can_InitStructure.CAN_RFLM = DISABLE;
	Can_InitStructure.CAN_TXFP = DISABLE;
	Can_InitStructure.CAN_Mode = CAN_Mode_Normal;//CAN_Mode_LoopBack;//
	Can_InitStructure.CAN_SJW = CAN_SJW_1tq;
	  
	/* CAN Baudrate init (CAN clocked at 90 MHz) */
	switch(param->m_Baudrate)
	{
		case CAN_BANDRATE_100K:
		{
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 30;
			break;
		}
		case CAN_BANDRATE_125K:
		{
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 24;
			break;
		}
		case CAN_BANDRATE_250K:
		{
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 12;
			break;
		}
		case CAN_BANDRATE_500K:
		{
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 6;
			break;
		}
		case CAN_BANDRATE_1M:
		{
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 3;
			break;
		}
		default:
		{
			/* Default as 1M */
			Can_InitStructure.CAN_BS1 = CAN_BS1_9tq;
			Can_InitStructure.CAN_BS2 = CAN_BS2_5tq;
			Can_InitStructure.CAN_Prescaler = 3;			
		}
	}
	CAN_Init(CANReg, &Can_InitStructure);
	
	/* CAN filter init */
	if(CANReg == CAN1)
		Can_FilterInitStructure.CAN_FilterNumber = 0;
	else
	{
		CAN_SlaveStartBank(14);
		Can_FilterInitStructure.CAN_FilterNumber = 14;
	}
	Can_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	Can_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	Can_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	Can_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	Can_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	Can_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	Can_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	Can_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&Can_FilterInitStructure);
	

	return TRUE;
}

/*
**************************************************************************
* Description	: Initialization CAN Ports.
  	1) Input argument  : 	port - CAN port
  							setParams - CAN port parameters
 	2) Modify argument :
 	3) Output argument :
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov6, 2014
**************************************************************************
*/
BOOL InitCanPort(CAN_PORT port, CAN_PARAM* param)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	CAN_HW_INFO*	pHwInfo;

	pHwInfo = (CAN_HW_INFO*)&CAN_HW_INFO_TABLE[port];
	
	/* GPIO initialization */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	/* Configure Can TX as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CanPin[CAN_PORT_PIN_TX].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CanPin[CAN_PORT_PIN_TX].port, &GPIO_InitStructure);
	GPIO_PinAFConfig((GPIO_TypeDef* )pHwInfo->m_CanPin[CAN_PORT_PIN_TX].port, pHwInfo->m_CanPin[CAN_PORT_PIN_TX].pinSource, pHwInfo->m_AF);

	/* Configure CAN RX as alternate function */
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CanPin[CAN_PORT_PIN_RX].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CanPin[CAN_PORT_PIN_RX].port, &GPIO_InitStructure);
	GPIO_PinAFConfig((GPIO_TypeDef* )pHwInfo->m_CanPin[CAN_PORT_PIN_RX].port, pHwInfo->m_CanPin[CAN_PORT_PIN_RX].pinSource, pHwInfo->m_AF);

	/* Set CAN Port parameter */
	SetCanPortParams(port, param);

	/* Buffer initialization */
	memset((void*)&CanHandler[port].m_Buf, 0, sizeof(CAN_PORT_BUFS));
	CanHandler[port].m_Status= CAN_STATUS_RECEIVING;
	InitmsTimer(&CanHandler[port].m_Timer, (UINT32)param->m_TimeOut);

	/* Configures interrupts */
	/* NVIC configure */
	NVIC_InitStructure.NVIC_IRQChannel = pHwInfo->m_IntChanTx;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = pHwInfo->m_IntChanRx0;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = pHwInfo->m_IntChanRx1;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable Can Receive and Transmit interrupts */
	CAN_ITConfig((CAN_TypeDef* )pHwInfo->m_CanHandle, CAN_IT_FMP0 | CAN_IT_FMP1 | CAN_IT_TME, ENABLE);

	return TRUE;
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
VOID StartCanComCtrlTimer(CAN_PORT port)
{
	StartTimer(&CanHandler[port].m_Timer);
}

VOID StopCanComCtrlTimer(CAN_PORT port)
{
	StopTimer(&CanHandler[port].m_Timer);
}

BOOL isCanComCtrlTimerElapsed(CAN_PORT port)
{
	return (isElapsed(&CanHandler[port].m_Timer));
}

/*
**************************************************************************
* Description	: CAN1 RX0 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN1_RX0_IRQHandler(void)
{
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	CanRxMsg canRxMsg;	
	/* Check receive not empty status */
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET )
	{
		/* Clear IT flag */
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);		
		/* Receive can massage from can devive */
		CAN_Receive(CAN1, CAN_FIFO0, &canRxMsg);
		/* Copy massage to Can Buffer */
		pCANBuf = &CanHandler[CAN_PORT_1].m_Buf.m_RXBuffer;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_ExtId = canRxMsg.ExtId;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_StdId = canRxMsg.StdId;
		if(canRxMsg.IDE == CAN_ID_STD)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 1;
		if(canRxMsg.RTR == CAN_RTR_DATA)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 1;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_DLC = canRxMsg.DLC;
		/* Copy data */
		for(cnt = 0; cnt < canRxMsg.DLC; cnt++)
		{
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_Data[cnt] = canRxMsg.Data[cnt];
		}
		pCANBuf->m_WrIdx ++;
		pCANBuf->m_WrIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count ++;
	}
}

/*
**************************************************************************
* Description	: CAN1 RX1 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN1_RX1_IRQHandler(void)
{
	CanRxMsg canRxMsg;	
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	/* Check receive not empty status */
	if(CAN_GetITStatus(CAN1, CAN_IT_FMP1) != RESET )
	{
		/* Clear IT flag */
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP1);		
		/* Receive can massage from can devive */
		CAN_Receive(CAN1, CAN_FIFO1, &canRxMsg);
		/* Copy massage to Can Buffer */
		pCANBuf = &CanHandler[CAN_PORT_1].m_Buf.m_RXBuffer;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_ExtId = canRxMsg.ExtId;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_StdId = canRxMsg.StdId;
		if(canRxMsg.IDE == CAN_ID_STD)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 1;
		if(canRxMsg.RTR == CAN_RTR_DATA)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 1;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_DLC = canRxMsg.DLC;
		/* Copy data */
		for(cnt = 0; cnt < canRxMsg.DLC; cnt++)
		{
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_Data[cnt] = canRxMsg.Data[cnt];
		}
		pCANBuf->m_WrIdx ++;
		pCANBuf->m_WrIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count ++;
	}
}

/*
**************************************************************************
* Description	: CAN1 TX interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/

void CAN1_TX_IRQHandler(void)
{
	CanTxMsg canTxMsg;	
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	/* Clear IT flag */
	CAN_ClearITPendingBit(CAN1, CAN_IT_TME);
	pCANBuf = &CanHandler[CAN_PORT_1].m_Buf.m_TXBuffer;
	/* Check Whether there a data for send */
	if(pCANBuf->m_RdIdx != pCANBuf->m_WrIdx && pCANBuf->m_Count != 0)
	{
		/* Copy massage to Can Buffer */
		canTxMsg.ExtId = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_ExtId;
		canTxMsg.StdId = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_StdId;
		if(0 == pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_IDE)
			canTxMsg.IDE = CAN_ID_STD;
		else
			canTxMsg.IDE = CAN_ID_EXT;
		if(0 == pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_RTR)
			canTxMsg.RTR = CAN_RTR_DATA;
		else
			canTxMsg.RTR = CAN_RTR_REMOTE;

		canTxMsg.DLC = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_DLC;
		/* Copy data */
		for(cnt = 0; cnt < canTxMsg.DLC; cnt++)
		{
			canTxMsg.Data[cnt] = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_Data[cnt];
		}
		/* Send can massage to can devive */
		CAN_Transmit(CAN1, &canTxMsg);
		
		pCANBuf->m_RdIdx ++;
		pCANBuf->m_RdIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count --;
	}
	else
	{
		/* Buffer error dispose */
		pCANBuf->m_Count = 0;
		pCANBuf->m_RdIdx = pCANBuf->m_WrIdx;
	}
}

/*
**************************************************************************
* Description	: CAN1 ERR interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN1_SCE_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN1, CAN_IT_EWG) != RESET )
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_EWG);
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_EPV) != RESET )
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_EPV);
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_BOF) != RESET )
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_BOF);
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_LEC) != RESET )
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_LEC);
	}
	if(CAN_GetITStatus(CAN1, CAN_IT_ERR) != RESET )
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_ERR);
	}
}

/*
**************************************************************************
* Description	: CAN2 RX0 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN2_RX0_IRQHandler(void)
{
	CanRxMsg canRxMsg;	
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	/* Check receive not empty status */
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP0) != RESET )
	{
		/* Clear IT flag */
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP0);		
		/* Receive can massage from can devive */
		CAN_Receive(CAN2, CAN_FIFO0, &canRxMsg);
		/* Copy massage to Can Buffer */
		pCANBuf = &CanHandler[CAN_PORT_2].m_Buf.m_RXBuffer;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_ExtId = canRxMsg.ExtId;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_StdId = canRxMsg.StdId;
		if(canRxMsg.IDE == CAN_ID_STD)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 1;
		if(canRxMsg.RTR == CAN_RTR_DATA)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 1;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_DLC = canRxMsg.DLC;
		/* Copy data */
		for(cnt = 0; cnt < canRxMsg.DLC; cnt++)
		{
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_Data[cnt] = canRxMsg.Data[cnt];
		}
		pCANBuf->m_WrIdx ++;
		pCANBuf->m_WrIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count ++;
	}
}

/*
**************************************************************************
* Description	: CAN2 RX1 interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN2_RX1_IRQHandler(void)
{
	CanRxMsg canRxMsg;	
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	/* Check receive not empty status */
	if(CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET )
	{
		/* Clear IT flag */
		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);		
		/* Receive can massage from can devive */
		CAN_Receive(CAN2, CAN_FIFO1, &canRxMsg);
		/* Copy massage to Can Buffer */
		pCANBuf = &CanHandler[CAN_PORT_2].m_Buf.m_RXBuffer;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_ExtId = canRxMsg.ExtId;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_StdId = canRxMsg.StdId;
		if(canRxMsg.IDE == CAN_ID_STD)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_IDE = 1;
		if(canRxMsg.RTR == CAN_RTR_DATA)
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 0;
		else
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_RTR = 1;
		pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_DLC = canRxMsg.DLC;
		/* Copy data */
		for(cnt = 0; cnt < canRxMsg.DLC; cnt++)
		{
			pCANBuf->m_buffer[pCANBuf->m_WrIdx].m_Data[cnt] = canRxMsg.Data[cnt];
		}
		pCANBuf->m_WrIdx ++;
		pCANBuf->m_WrIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count ++;
	}
}

/*
**************************************************************************
* Description	: CAN2 TX interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN2_TX_IRQHandler(void)
{
	CanTxMsg canTxMsg;	
	CAN_BUF* pCANBuf;
	UINT8 cnt;
	/* Clear IT flag */
	CAN_ClearITPendingBit(CAN2, CAN_IT_TME);
	pCANBuf = &CanHandler[CAN_PORT_2].m_Buf.m_TXBuffer;
	/* Check Whether there a data for send */
	if(pCANBuf->m_RdIdx != pCANBuf->m_WrIdx && pCANBuf->m_Count != 0)
	{
		/* Copy massage to Can Buffer */
		canTxMsg.ExtId = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_ExtId;
		canTxMsg.StdId = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_StdId;
		if(0 == pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_IDE)
			canTxMsg.IDE = CAN_ID_STD;
		else
			canTxMsg.IDE = CAN_ID_EXT;
		if(0 == pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_RTR)
			canTxMsg.RTR = CAN_RTR_DATA;
		else
			canTxMsg.RTR = CAN_RTR_REMOTE;

		canTxMsg.DLC = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_DLC;
		/* Copy data */
		for(cnt = 0; cnt < canTxMsg.DLC; cnt++)
		{
			canTxMsg.Data[cnt] = pCANBuf->m_buffer[pCANBuf->m_RdIdx].m_Data[cnt];
		}
		/* Send can massage to can devive */
		CAN_Transmit(CAN2, &canTxMsg);
		
		pCANBuf->m_RdIdx ++;
		pCANBuf->m_RdIdx %= CAN_BUF_SIZE;
		pCANBuf->m_Count --;
	}
	else
	{
		/* Buffer error dispose */
		pCANBuf->m_Count = 0;
		pCANBuf->m_RdIdx = pCANBuf->m_WrIdx;
	}
}

/*
**************************************************************************
* Description	: CAN2 ERR interrupt handler
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 7 2014
**************************************************************************
*/
void CAN2_SCE_IRQHandler(void)
{
	if(CAN_GetITStatus(CAN2, CAN_IT_EWG) != RESET )
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_EWG);
	}
	if(CAN_GetITStatus(CAN2, CAN_IT_EPV) != RESET )
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_EPV);
	}
	if(CAN_GetITStatus(CAN2, CAN_IT_BOF) != RESET )
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_BOF);
	}
	if(CAN_GetITStatus(CAN2, CAN_IT_LEC) != RESET )
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_LEC);
	}
	if(CAN_GetITStatus(CAN2, CAN_IT_ERR) != RESET )
	{
		CAN_ClearITPendingBit(CAN2, CAN_IT_ERR);
	}
}


/*
**************************************************************************
* Description	: Send a massage.
  	1) Input argument  : port - CAN port
  							pMsg - Point of can massage
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jul,6 2014
**************************************************************************
*/
VOID CanSendOneMsg(CAN_PORT port, CAN_MSG *pMsg)
{
	CanTxMsg canTxMsg;	
	UINT8 cnt;
	CAN_TypeDef* CANReg;
	
	CANReg = (CAN_TypeDef* )CAN_HW_INFO_TABLE[port].m_CanHandle;	
	/* Copy massage to Can Buffer */
	canTxMsg.ExtId = pMsg->m_ExtId;
	canTxMsg.StdId = pMsg->m_StdId;
	if(0 == pMsg->m_IDE)
		canTxMsg.IDE = CAN_ID_STD;
	else
		canTxMsg.IDE = CAN_ID_EXT;
	if(0 == pMsg->m_RTR)
		canTxMsg.RTR = CAN_RTR_DATA;
	else
		canTxMsg.RTR = CAN_RTR_REMOTE;

	canTxMsg.DLC = pMsg->m_DLC;
	/* Copy data */
	for(cnt = 0; cnt < canTxMsg.DLC; cnt++)
	{
		canTxMsg.Data[cnt] = pMsg->m_Data[cnt];
	}
	/* Send can massage to can devive */
	CAN_Transmit(CANReg, &canTxMsg);
}


/*
**************************************************************************
* Description	: Push a massage to queue.
  	1) Input argument  : port - CAN port
  							pMsg - Point of can massage
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jul,6 2014
**************************************************************************
*/
VOID CanPushMsg(CAN_PORT port, CAN_MSG *pMsg)
{
	CAN_BUF* pCANBuf;

	pCANBuf = &CanHandler[port].m_Buf.m_TXBuffer;
	/* Copy datas */
	memcpy(&pCANBuf->m_buffer[pCANBuf->m_WrIdx], pMsg, sizeof(CAN_MSG));
	pCANBuf->m_WrIdx ++;
	pCANBuf->m_WrIdx %= CAN_BUF_SIZE;
	pCANBuf->m_Count ++;
}

/*
**************************************************************************
* Description	: Get current CAN status.
  	1) Input argument  : port - CAN port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
CAN_STATUS GetCanPortStatus(CAN_PORT port)
{
	return CanHandler[port].m_Status;
}

/*
**************************************************************************
* Description	: Set current CAN status.
  	1) Input argument  : port - CAN port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
VOID SetCanPortStatus(CAN_PORT port, CAN_STATUS status)
{
	CanHandler[port].m_Status = status;
}

/*
**************************************************************************
* Description	: Get buffer of CAN Port.
  	1) Input argument  : port - CAN port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
CAN_PORT_BUFS* GetCanPortBuffer(CAN_PORT port)
{
	return (&CanHandler[port].m_Buf);
}

/*
**************************************************************************
* Description	: Get buffer of CAN Port.
  	1) Input argument  : port - CAN port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
EventGroupHandle_t* GetCanPortSem(CAN_PORT port)
{
	return (&CanHandler[port].m_Sem);
}

/*
**************************************************************************
* Description	: Check whether the can TX is idle.
  	1) Input argument  : port - CAN port
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current status
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 9 2014
**************************************************************************
*/
BOOL IsCanTxIdle(CAN_PORT port)
{
	CAN_TypeDef* CANReg;
	CANReg = (CAN_TypeDef* )CAN_HW_INFO_TABLE[port].m_CanHandle;
	
	if((CAN_GetFlagStatus(CANReg, CAN_FLAG_RQCP0) ==RESET) && \
			 (CAN_GetFlagStatus(CANReg, CAN_FLAG_RQCP1) ==RESET) && \
			 (CAN_GetFlagStatus(CANReg, CAN_FLAG_RQCP2) ==RESET))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
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
	
	
	
	
	
	
	
	
	
	
		
