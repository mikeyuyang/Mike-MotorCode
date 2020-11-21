/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	ECD_SSI.c
//
// TITLE:	SSI encoder process source file.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	Mar 27, 2015
//
// Description: This file include source code for SSI encoder operation.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	Mar 31, 2015
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
#include "ECD_App.h"
#define   ECD_SSI_GLOBAL_
#include "ECD_SSI.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define SSI_DELAY_CNT 50
#define INC_SSI_CLK_CNT 6
/*
*********************************************************************************************************
*    Local data type
*********************************************************************************************************
*/
typedef enum tagSSI_ECD_PORT_PIN		// Ports of Incremental encoder.
{
	SSI_ECD_PORT_PIN_SCK = 0,
	SSI_ECD_PORT_PIN_DAT,
	SSI_ECD_PORT_PIN_MAX
}SSI_ECD_PORT_PIN;

typedef enum tagECD_CODE_MODE		// Encode code type.
{
	ECD_CODE_MODE_BIN = 0,
	ECD_CODE_MODE_GRY,
	ECD_CODE_MODE_MAX
}ECD_CODE_MODE;


/* Encoder Control Struct */
typedef struct tagENCODER_CTRL_STRUCT
{
	UINT16* m_pECDType;
	UINT16* m_pECDSingleTurnBitNum;
	UINT16* m_pECDMultiTurnBitNum;
	UINT16* m_pECDCodeMode;	

	BOOL m_ValidFlag;
	UINT32 m_RawData;
	UINT32 m_BinData;
	MOV_DIR m_MoveDir;
	INT32 m_OldTemp;
	UINT8 m_DiffCnt;
}ENCODER_CTRL_STRUCT;


/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

const GPIO_PORT_PIN ECD_PORT_PIN_TABLE[ENCODER_PORT_MAX][SSI_ECD_PORT_PIN_MAX] = {
/*			PORT,						PIN					*/
	{	{(VOID*)GPIOA,		GPIO_Pin_3},
		{(VOID*)GPIOD,		GPIO_Pin_12}},

	{	{(VOID*)GPIOB,		GPIO_Pin_5},
		{(VOID*)GPIOB,		GPIO_Pin_4}},
};
/*
********************************************************************************************************
*	 Global variables
********************************************************************************************************
*/
ENCODER_CTRL_STRUCT ECDControlStructs[ENCODER_PORT_MAX] =
{
	{
		&SPD.m_ECDType1,
		&SPD.m_ECDSingleTurnBitNum1,
		&SPD.m_ECDMultiTurnBitNum1,
		&SPD.m_ECDCodeMode1,	

		FALSE,
		0,
		0,
		(MOV_DIR)0,
		0,
		0
	},

	{
		&SPD.m_ECDType2,
		&SPD.m_ECDSingleTurnBitNum2,
		&SPD.m_ECDMultiTurnBitNum2,
		&SPD.m_ECDCodeMode2,	

		FALSE,
		0,
		0,
		(MOV_DIR)0,
		0,
		0
	},
#if 0
	{
		&SPD.m_ECDType3,
		&SPD.m_ECDSingleTurnBitNum3,
		&SPD.m_ECDMultiTurnBitNum3,
		&SPD.m_ECDCodeMode3,	
	
		FALSE,
		0,
		0,
		(MOV_DIR)0,
		0,
		0
	}
#endif	
};

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
* Description	: uS delay function.
  	1) Input argument  : usNum - Delay time in uS
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar,30 2015
**************************************************************************
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
VOID SSIEncoderInit(ENCODER_PORT port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize the GPIO Port.
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	/* SCK Pin config */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Pin = ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin;
	GPIO_Init((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, &GPIO_InitStructure);
	GPIO_SetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
	/* Data Pin config */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_DAT].pin;
	GPIO_Init((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_DAT].port, &GPIO_InitStructure);
}


/*
**************************************************************************
* Description	: Decimal to Gray covert;
  	1) Input argument  : decVal - Decimal value need for covert
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: Gray after covert
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar 31, 2015
**************************************************************************
*/
static UINT32 DecimaltoGray(UINT32 decVal)
{
	return decVal ^ (decVal >> 1);
}


/*
**************************************************************************
* Description	: Gray to decimal covert;
  	1) Input argument  : grayVal - Gray value need for covert
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: decimal after covert
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Mar 31, 2015
**************************************************************************
*/
static UINT32 GraytoDecimal(UINT32 grayVal)
{
	UINT32 calcTemp = grayVal;
	while( grayVal >>= 1)
		calcTemp ^= grayVal;
	return calcTemp;
}

/*
**************************************************************************
* Description	: Get current Angle.
  	1) Input argument  : None.
 	2) Modify argument : None
 	3) Output argument : Ang : angle value, Unit: 0.01deg
* Return      	: TRUE - Success, FALSE - Failed
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Mar 27, 2015
**************************************************************************
*/
INT32 SSIEncoderReadData(ENCODER_PORT port)
{
	UINT16 cnt1, cnt2 = 0;
	UINT32 shiftTemp = 0;
//	INT64 calcTemp;
	UINT8 dataBitLen;
	ENCODER_CTRL_STRUCT* pEncodeHandler;
//	UINT16 multiTurnData;
//	UINT32 singleTurnData;

	if(port >= ENCODER_PORT_MAX)
	{
		return FALSE;
	}
	else
		pEncodeHandler = &ECDControlStructs[port];
	/* Check whether the data is valid */
//	if(0 == GET_DAT)
//		return FALSE;
	/* Send first clock. */
	GPIO_ResetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
	/* Delay for 1us */
	for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
	{
		__NOP();
	}
	/* Special dispose for different SSI encoder type */
	if(*pEncodeHandler->m_pECDType == ECD_TYPE_RP3806_SSI)
	{
		GPIO_SetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
		/* Delay for 1us */
		for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
		{
			__NOP();
		}
		GPIO_ResetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
		/* Delay for 1us */
		for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
		{
			__NOP();
		}
	}	
	/* Special dispose for different SSI encoder type */
	if(*pEncodeHandler->m_pECDType == ECD_TYPE_INC_SSI)
	{
		for(cnt2 = 0; cnt2 < INC_SSI_CLK_CNT; cnt2 ++)
		{
			GPIO_SetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
			/* Delay for 1us */
			for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
			{
				__NOP();
			}
			GPIO_ResetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
			/* Delay for 1us */
			for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
			{
				__NOP();
			}
		}
	}	
	
	/* Begin to read data */
	dataBitLen = *pEncodeHandler->m_pECDMultiTurnBitNum + *pEncodeHandler->m_pECDSingleTurnBitNum;
	for(cnt2 = 0; cnt2 < dataBitLen; cnt2 ++)
	{
		shiftTemp <<= 1;	/*Read data*/
		GPIO_SetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
		/* Delay for 1us */
		for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
		{
			__NOP();
		}
		GPIO_ResetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);
		if(GPIO_ReadInputDataBit((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_DAT].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_DAT].pin))
		{
			shiftTemp += 1;	/*Read data*/
		}

		/* Delay for 1us */
		for(cnt1 = 0; cnt1 < SSI_DELAY_CNT; cnt1 ++)
		{
			__NOP();
		}
	}
	/* Finish transmit */
	GPIO_SetBits((GPIO_TypeDef *)ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].port, ECD_PORT_PIN_TABLE[port][SSI_ECD_PORT_PIN_SCK].pin);

	pEncodeHandler->m_RawData = shiftTemp;
	shiftTemp &= ((1 << dataBitLen) - 1);
	/* Convert gray to decimal */
	if(ECD_CODE_MODE_GRY == *pEncodeHandler->m_pECDCodeMode)
		shiftTemp = GraytoDecimal(shiftTemp);
	/* Save bin data */
	pEncodeHandler->m_BinData = shiftTemp;
	/* Error control ( need enhance )*/
	if(labs((INT32)pEncodeHandler->m_OldTemp - shiftTemp) > 200)
	{
		pEncodeHandler->m_DiffCnt ++;
		if(pEncodeHandler->m_DiffCnt > 5)
		{
			pEncodeHandler->m_DiffCnt = 0;
			pEncodeHandler->m_OldTemp = shiftTemp;
			return shiftTemp;
			
		}
		else
			return pEncodeHandler->m_OldTemp;
	}
	else
	{
		ECDControlStructs[port].m_DiffCnt = 0;
		ECDControlStructs[port].m_OldTemp = shiftTemp;
		return shiftTemp;
	}
}

/*
**************************************************************************
* Description	: Read Angle .
  	1) Input argument  : port: Encoder port.
 	2) Modify argument : None
 	3) Output argument : *pAng: Address to save the angle.
* Return      	: TRUE - Success, FALSE - Failed
* Notes       	:
* Author	  	: Chris_Shang
* Date			: June 11, 2015
**************************************************************************
*/
/*
BOOL SSIEncoderGetAngle(ENCODER_PORT port, INT32 *pAng)
{
	if(TRUE == ECDControlStructs[port].m_ValidFlag)
	{
		*pAng = ECDControlStructs[port].m_AngDegData;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
*/
/*
**************************************************************************
* Description	: Read move direction .
  	1) Input argument  : port: Encoder port.
 	2) Modify argument : None
 	3) Output argument : *pDir: Address to save the direciton.
* Return      	: TRUE - Success, FALSE - Failed
* Notes       	:
* Author	  	: Chris_Shang
* Date			: June 11, 2015
**************************************************************************
*/
BOOL SSIEncoderGetDir(ENCODER_PORT port, MOV_DIR *pDir)
{
	if(TRUE == ECDControlStructs[port].m_ValidFlag)
	{
		*pDir = ECDControlStructs[port].m_MoveDir;
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
