/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	UTL_I2C.h
//
// TITLE:	I2C interface process functions.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Nov 30, 2015
//
// Description: This file realize I2C interface use a simulate GPIO way.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Nov 30, 2015
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <stm32f4xx.h>

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "DBM_Data.h"
#define   UTL_I2C_GLOBAL_
#include "UTL_I2C.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/

#define I2C_DATA_BIT_NUM		8

#define I2C_CLOCK_DELAY_400KHz	56
/*
*********************************************************************************************************
*    Local data type
*********************************************************************************************************
*/
typedef enum tagI2C_PORT_PIN		// Ports of Incremental encoder.
{
	I2C_PORT_PIN_SCL = 0,
	I2C_PORT_PIN_SDA,
	I2C_PORT_PIN_MAX
}I2C_PORT_PIN;

/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

const GPIO_PORT_PIN I2C_PORT_PIN_TABLE[I2C_PORT_MAX][I2C_PORT_PIN_MAX] = {
/*			PORT,						PIN					*/
	{	{(VOID*)GPIOH,		GPIO_Pin_4},
		{(VOID*)GPIOH,		GPIO_Pin_5}}
};
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
* Description	: Initialize the control port of SSI encoder;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar 31, 2015
**************************************************************************
*/
VOID I2CInit(I2C_PORT port)
{
	UINT8 cnt1;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
	/* SCL Pin configure */
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, &GPIO_InitStructure);
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* SDA Pin configure */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);

	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

}

/*
**************************************************************************
* Description	: Start I2C;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
VOID I2CStart(I2C_PORT port)
{
	UINT8 cnt1;
	/* Set up start condition */
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
}

/*
**************************************************************************
* Description	: Stop I2C;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
VOID I2CStop(I2C_PORT port)
{
	UINT8 cnt1;
	/* Set up start condition */
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);
	//GPIO_ResetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, ECD_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
}

/*
**************************************************************************
* Description	: Send a byte with I2C;
  	1) Input argument  : port - I2C port for operate, data - Byte for send
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: TRUE - Send success, FALSE - Send failed
* Notes       	: If acknowledge received success then the send is OK, otherwise failed
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
BOOL I2CSendByte(I2C_PORT port, UINT8 data)
{
	UINT8 cnt1, cnt2;
	BOOL ret = TRUE;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin;

	/* Send data */
	for(cnt2 = 0; cnt2 < I2C_DATA_BIT_NUM; cnt2 ++)
	{
		/* Set data bit */
		if(data & 0x80)
			GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);
		else
			GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);

		/* Shift data */
		data <<= 1;
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

		/* Send clock */
		GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

		GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	}
	/* Switch SDA to input for receive acknowledge */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
	/* Set clock */
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

	if(GPIO_ReadInputDataBit((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin))
		ret = FALSE;
	/* Reset clock */
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

	/* Switch SDA to output */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);

	return ret;
}

/*
**************************************************************************
* Description	: Send Acknowledge;
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
VOID I2CSendAck(I2C_PORT port)
{
	UINT8 cnt1;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin;

	/* Switch SDA to output for send ACK/NOACK */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
	/* Sent ACk */
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);

	/* Set clock */
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	/* Reset clock */
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

	/* Switch SDA to input */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init((GPIO_TypDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
}

/*
**************************************************************************
* Description	: Send No Acknowledge;
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
VOID I2CSendNoAck(I2C_PORT port)
{
	UINT8 cnt1;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin;

	/* Switch SDA to output for send ACK/NOACK */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
	/* Sent no ack */
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin);

	/* Set clock */
	GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	/* Reset clock */
	GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
	/* Delay for 400KHz frequency */
	for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

	/* Switch SDA to input */
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_Init((GPIO_TypDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
}

/*
**************************************************************************
* Description	: Read a byte with I2C;
  	1) Input argument  : port - I2C port for operate, data - Byte for send
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: TRUE - Send success, FALSE - Send failed
* Notes       	: If acknowledge received success then the send is OK, otherwise failed
* Author	  	: Chris_Shang
* Date			: Nov 30, 2015
**************************************************************************
*/
UINT8 I2CReadByte(I2C_PORT port)
{
	UINT8 cnt1, cnt2;
	UINT8 readTemp;

	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin;
	GPIO_Init((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, &GPIO_InitStructure);
	
	/* Send data */
	for(cnt2 = 0; cnt2 < I2C_DATA_BIT_NUM; cnt2 ++)
	{
		/* Shift data */
		readTemp <<= 1;
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

		/* Send clock */
		GPIO_SetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();

		if(GPIO_ReadInputDataBit((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SDA].pin))
			readTemp += 1;

		GPIO_ResetBits((GPIO_TypeDef *)I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].port, I2C_PORT_PIN_TABLE[port][I2C_PORT_PIN_SCL].pin);
		/* Delay for 400KHz frequency */
		for(cnt1 = 0; cnt1 < I2C_CLOCK_DELAY_400KHz; cnt1 ++)	__NOP();
	}
	return readTemp;
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
