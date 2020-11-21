/*
//######################################################################################################
//										(c) Copyright 
//                        All Rights Reserved
//
// FILE:	UTL_Ethernet.c
//
// TITLE:	Ethernet common operate functions
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2018-02-03
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2018-02-03
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

/******************************************************************************************
 *Includes
 ******************************************************************************************/
#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"
#include "stm32f4x7_eth.h"
#include <includes.h>
	 
#include "BSP_BasicDataType.h"	// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#define   UTL_ETHERNET_GLOBAL_
#include "UTL_Ethernet.h"

/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
#define PHY_ADDRESS 0x00

/* Ethernet port define */
typedef enum tagETH_PORT
{
	ETH_PORT_MDIO = 0,
	ETH_PORT_MDC,
	ETH_PORT_RMII_REF_CLK,
	ETH_PORT_RMII_CRS_DV,
	ETH_PORT_RMII_RXD0,
	ETH_PORT_RMII_RXD1,
	ETH_PORT_ETH_RMII_TX_EN,
	ETH_PORT_ETH_RMII_TXD0,
	ETH_PORT_ETH_RMII_TXD1,
	ETH_PORT_ETH_RMII_RXER,
	ETH_PORT_ETH_RESET,
	ETH_PORT_MAX
}ETH_PORT;

const GPIO_PORT_PIN ETH_PORT_PIN_TABLE[ETH_PORT_MAX] = {
/*		PORT,			PIN,			PIN Source	*/
	{(VOID*)GPIOA,	GPIO_Pin_2,		GPIO_PinSource2},
	{(VOID*)GPIOC,	GPIO_Pin_1,		GPIO_PinSource1},
	{(VOID*)GPIOA,	GPIO_Pin_1,		GPIO_PinSource1},
	{(VOID*)GPIOA,	GPIO_Pin_7,		GPIO_PinSource7},
	{(VOID*)GPIOC,	GPIO_Pin_4,		GPIO_PinSource4},
	{(VOID*)GPIOC,	GPIO_Pin_5,		GPIO_PinSource5},
	{(VOID*)GPIOB,	GPIO_Pin_11,	GPIO_PinSource11},
	{(VOID*)GPIOB,	GPIO_Pin_12,	GPIO_PinSource12},
	{(VOID*)GPIOB,	GPIO_Pin_13,	GPIO_PinSource13},
	{(VOID*)0,	0,	0},
	{(VOID*)GPIOH,	GPIO_Pin_2,		GPIO_PinSource2},
};

/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/

//struct netif lwip_netif;				//定义一个全局的网络接?


/*
********************************************************************************************************
*	 External reference
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
**************************************************************************
* Description	: Ethernet GPIO configuration.
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
VOID EthGpioConfig(VOID)
{
	UINT8 cnt = 0;
	UINT32 cnt2;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Set ethernet interface as RMII */
	SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII); 
	/* Initilize ethernet related GPIO */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; 	

	for(cnt = 0; cnt < ETH_PORT_ETH_RESET; cnt ++)
	{
		GPIO_InitStructure.GPIO_Pin = ETH_PORT_PIN_TABLE[cnt].pin;
		GPIO_Init(ETH_PORT_PIN_TABLE[cnt].port, &GPIO_InitStructure);
		GPIO_PinAFConfig(ETH_PORT_PIN_TABLE[cnt].port, ETH_PORT_PIN_TABLE[cnt].pinSource, GPIO_AF_ETH);
	}

	
	/* Initilize PHY RXER pin */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	/* PHY Address configuration */
	if(PHY_ADDRESS == 0)
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	else
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RMII_RXER].pin;
	GPIO_Init(ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RMII_RXER].port, &GPIO_InitStructure);

	/* Initilize PHY reset pin */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].pin;
	GPIO_Init(ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].port, &GPIO_InitStructure);

	/* Reset PHY Pin */	
	GPIO_ResetBits(ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].port 
				,ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].pin);
	for(cnt2 = 0; cnt2 < 0x3ffff; cnt2++);	
	GPIO_SetBits(ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].port 
				,ETH_PORT_PIN_TABLE[ETH_PORT_ETH_RESET].pin);

}

/*
**************************************************************************
* Description	: Ethernet NVIC configuration.
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
VOID EthNvicConfig(VOID)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = ETH_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 8;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/*
**************************************************************************
* Description	: Ethernet MAC and DMA configuration.
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
VOID EthMacDmaConfig(VOID)
{ 
	UINT8 retVal;
	ETH_InitTypeDef ETH_InitStructure; 

	/* Reset Eth configuration */                    
	ETH_DeInit();  	
	/* Soft reset Eth */
	ETH_SoftwareReset();  
	/* Wait for reset finish */
	while (ETH_GetSoftwareResetStatus() == SET);
	/* Initilize Ethernet initilization struct */
	ETH_StructInit(&ETH_InitStructure); 

	/* Set Ethernet initilization struct value */ 
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	/* Disable retry transmission */
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	/* Disable automatic strip CRC/PAD */
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	/* Disable receive all frame */
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	/* Accpet to receive all broadcast frame */
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	/* Disable  Promiscuous Mode for address filter*/
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	/* Use perfect filter for Multicast frames*/
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	/* Use perfect filter for Unicast frames*/
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
	/* Use hareware check sum */
#if LWIP_CHECKSUM_CTRL_PER_NETIF
	ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
#endif
	/* Discard TCP/IP frame when check sum error */
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable;
	/* We must enable store and forware mode when use hardware check sum, so DMA can dispose those error frame */
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;  
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;
	/* Disable to forware error frame */
	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;
	/* Don't forward undersize good frame. */
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;
	/* Enable second frame dispose function */
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	/* Enable address aligned function */
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable; 
	/* Enable fixed Burst function */
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;
	/* Max DMA Burst recieve/transmit length as 32 beats */
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;    
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	/* DMA Tx/Rx arbitration configur */
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;
		
	retVal = ETH_Init(&ETH_InitStructure,PHY_ADDRESS);
	if(retVal == ETH_SUCCESS)
	{
		/* Enable reciever interrupt */
		ETH_DMAITConfig(ETH_DMA_IT_NIS|ETH_DMA_IT_R,ENABLE);  
	}
		 
}

/*
**************************************************************************
* Description	: Ethernet MAC and DMA configuration.
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
void EthMacInit(void)
{
	/* Configure the GPIO ports for ethernet pins */
	EthGpioConfig();
	/* Nvic configuration */
	EthNvicConfig();
	/* Configure the Ethernet MAC/DMA */
	EthMacDmaConfig();
}
/*
**************************************************************************
* Description	: Get link state from PHY chip
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
ETH_LINK_STATE EthGetPHYLinkState(void)
{
	UINT16 temp;
	ETH_LINK_STATE sta ;
	
	/* Read basic status register of PHY */
	temp = ETH_ReadPHYRegister(PHY_ADDRESS, PHY_BSR);

	if(temp & PHY_Linked_Status)
		sta = ETH_LINK_STATE_UP;
	else
		sta = ETH_LINK_STATE_DOWN;

	return sta;
}

/*
**************************************************************************
* Description	: Get Rx pack size 
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
UINT32  EthGetRxPktSize(ETH_DMADESCTypeDef *DMARxDesc)
{
    UINT32 frameLength = 0;
    if(((DMARxDesc->Status&ETH_DMARxDesc_OWN)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARxDesc_ES)==(uint32_t)RESET) &&
     ((DMARxDesc->Status&ETH_DMARxDesc_LS)!=(uint32_t)RESET)) 
    {
        frameLength=((DMARxDesc->Status&ETH_DMARxDesc_FL)>>ETH_DMARXDESC_FRAME_LENGTHSHIFT);
    }
	
    return frameLength;
}

/*
**************************************************************************
* Description	: Interrupt handler of ethernet mac
  	1) Input argument  : 	None
 	2) Modify argument :
 	3) Output argument :
* Return      	: 
* Notes       	:
* Author	  	: Chris_Shang
* Date		: Feb 3, 2018
**************************************************************************
*/
void ETH_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

	/* Frame received */
	if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET) 
	{
		/* Give the semaphore to wakeup LwIP task */
		if(pEthRxSem != NULL)
		{
			xSemaphoreGiveFromISR(*pEthRxSem, &xHigherPriorityTaskWoken );  
		}
	}

	/* Clear the interrupt flags. */
	/* Clear the Eth DMA Rx IT pending bits */
	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);

	/* Switch tasks if necessary. */	
	if( xHigherPriorityTaskWoken != pdFALSE )
	{
		portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
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

