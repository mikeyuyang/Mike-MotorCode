/*
//######################################################################################################
//
// FILE:	MOT_Pwm.c
//
// TITLE:	PWM motor Control process source file.
//
// AUTHOR:	
//
// DATE:	
//
// Description: This file include source code for pwm motor driver, and control functions.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:    May 10, 2015
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

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "DBM_Data.h"
#define   MOT_PWM_GLOBAL_
#include "MOT_Pwm.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define	SET_AXIS1_PUL()		GPIOC->BSRRL = GPIO_Pin_7
#define	RESET_AXIS1_PUL()	GPIOC->BSRRH = GPIO_Pin_7
#define	SET_AXIS2_PUL()		GPIOD->BSRRL = GPIO_Pin_13
#define	RESET_AXIS2_PUL()	GPIOD->BSRRH = GPIO_Pin_13

#define	SET_AXIS3_PUL()		GPIOB->BSRRL = GPIO_Pin_0
#define	RESET_AXIS3_PUL()	GPIOB->BSRRH = GPIO_Pin_0

#define PWM_PORT_MAX 3;



/*
*********************************************************************************************************
*    Local data type
*********************************************************************************************************
*/
typedef enum tagMOTOR_PORT_PIN		// Ports of Motor.
{
	MOTOR_PORT_PIN_EN = 0,
	MOTOR_PORT_PIN_PUL,
	MOTOR_PORT_PIN_DIR,
	MOTOR_PORT_PIN_CLR,
	MOTOR_PORT_PIN_MAX,
}MOTOR_PORT_PIN;


/* Motor control struct */
typedef struct tagMOTOR_CTRL
{
	/* Set Point */
	UINT32* m_pMotEncoderLineNum;
	UINT16* m_pPosSource;
	UINT16* m_pMotDirResvert;
	UINT16* m_pMotGearRatioN;
	UINT16* m_pMotGearRatioD;
	UINT32* m_pMotUnitRatio;

	/* Control variables */
	VOLATILE INT64 m_AccumPluseCnt;
	VOLATILE INT32 m_RemainPluseCnt;
	MOV_DIR m_CurrentDir;
}MOTOR_CTRL;

/* PWM Hardward INFO */
typedef struct tagPWM_HW_INFO
{
	TIM_TypeDef*	m_MasterTimHandle;
	UINT8			m_MasterAF;
    UINT16			m_MasterTimChan;
	TIM_TypeDef*	m_SlaveTimHandle;
	UINT16			m_SlaveTrigSrc;
    UINT16			m_SlaveIntChan;
    UINT16			m_SlaveIntSource;
	GPIO_PORT_PIN 	m_CtrlPin[MOTOR_PORT_PIN_MAX];
}PWM_HW_INFO;



/*
********************************************************************************************************
*	 Local variables
********************************************************************************************************
*/
MOTOR_CTRL MotorControlStruct[MOTOR_PORT_MAX] =
{
	{
		/* Set Point */
		&SPD.m_MotEncodeLineNum1,	// Encoder line number of motor;
		&SPD.m_PosSource1,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert1,		// Motor direction resvert
		&SPD.m_MotGearRatioN1,		// Gear Ratio N
		&SPD.m_MotGearRatioD1,		// Gear Ratio D
		&SPD.m_MotUnitRatio1,		// Unit Ratio

		/* Control variables */
		0,			// Total Pluse count;
		0,			// Remain Pluse Count;
		MOV_DIR_POS // Current Turn Direction;
	},

	{
		/* Set Point */
		&SPD.m_MotEncodeLineNum2,	// Encoder line number of motor;
		&SPD.m_PosSource2,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert2,		// Motor direction resvert
		&SPD.m_MotGearRatioN2,		// Gear Ratio N
		&SPD.m_MotGearRatioD2,		// Gear Ratio D
		&SPD.m_MotUnitRatio2,		// Unit Ratio

		/* Control variables */
		0,			// Total Pluse count;
		0,			// Remain Pluse Count;
		MOV_DIR_POS // Current Turn Direction;
	},
	{
		/* Set Point */
		&SPD.m_MotEncodeLineNum3,	// Encoder line number of motor;
		&SPD.m_PosSource3,			// Whole Range Pulse Number;
		&SPD.m_MotDirResvert3,		// Motor direction resvert
		&SPD.m_MotGearRatioN3,		// Gear Ratio N
		&SPD.m_MotGearRatioD3,		// Gear Ratio D
		&SPD.m_MotUnitRatio3,		// Unit Ratio

		/* Control variables */
		0,			// Total Pluse count;
		0,			// Remain Pluse Count;
		MOV_DIR_POS // Current Turn Direction;
	}	
};



/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
const PWM_HW_INFO PWM_HW_INFO_TABLE[MOTOR_PORT_MAX] =
{
			{	
/*		MasterTimer,			AF				TIM_CH	*/	
		TIM1,				GPIO_AF_TIM1,	TIM_Channel_2,
/*		SlaveTimer, 			TrigSrc,			TimerIRQ_CH 	IRQ_SRC 	*/
		TIM2,				TIM_TS_ITR0,	TIM2_IRQn,		TIM_IT_Update,		
/*		GPIO_PORT			GPIO_PIN		PINSOURCE*/
		{{(VOID*)GPIOE, 	GPIO_Pin_12,		12},
		{(VOID*)GPIOE,		GPIO_Pin_11, 	11},
		{(VOID*)GPIOE,		GPIO_Pin_10, 	10},
		{(VOID*)GPIOE,		GPIO_Pin_13, 	13}}				
	},
	{
/*		MasterTimer,			AF				TIM_CH	*/	
		TIM8,				GPIO_AF_TIM8,	TIM_Channel_1,
/*		SlaveTimer, 			TrigSrc,			TimerIRQ_CH 	IRQ_SRC 	*/
		TIM5,				TIM_TS_ITR3,	TIM5_IRQn,		TIM_IT_Update,		
/*		GPIO_PORT			GPIO_PIN		PINSOURCE*/
		{{(VOID*)GPIOI, 	GPIO_Pin_7,		7},
		{(VOID*)GPIOI,		GPIO_Pin_5,		5},
		{(VOID*)GPIOI,		GPIO_Pin_6,	6},
		{(VOID*)GPIOI,		GPIO_Pin_4,	4}}			
	},
	{
/*		MasterTimer,			AF				TIM_CH	*/	
		TIM4,				GPIO_AF_TIM4,	TIM_Channel_2,
/*		SlaveTimer, 			TrigSrc,			TimerIRQ_CH 	IRQ_SRC 	*/
		TIM3,				TIM_TS_ITR3,	TIM3_IRQn,		TIM_IT_Update,		
/*		GPIO_PORT			GPIO_PIN		PINSOURCE*/
		{{(VOID*)GPIOG, 	GPIO_Pin_15,		15},
		{(VOID*)GPIOB,		GPIO_Pin_7,		7},
		{(VOID*)GPIOG,		GPIO_Pin_11,	11},
		{(VOID*)GPIOG,		GPIO_Pin_10,	10}}			
	}
};



#define MOTOR_MAX_SPD 9000
#define DRV_CLEAR_FAULT_PLUSE_WIDTH 10

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
* Description	: Delay function for Motor control.
  	1) Input argument  : Delay time ,Uint :us
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Fec,02 2012
**************************************************************************
*/
VOID MotorDelayus(UINT8 timeus)
{
	UINT8 cnt = 8;
	while(cnt --);
}

/*
**************************************************************************
* Description	: Initialize the PWM for Motor;
  	1) Input argument  : port: Motor port for set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 11, 2015
**************************************************************************
*/
VOID MotorPwmInit(MOTOR_PORT port)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	PWM_HW_INFO* pHwInfo;

	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];
		
	/* GPIO initialization */
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;
	/* Configure EN,DIR,CLR pin */
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].pin;
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].port, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].pin;
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].port, &GPIO_InitStructure);
	/* Configure PLUSE pin */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_PUL].pin;
	GPIO_Init((GPIO_TypeDef* )pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_PUL].port, &GPIO_InitStructure);
	GPIO_PinAFConfig((GPIO_TypeDef* )pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_PUL].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_PUL].pinSource, pHwInfo->m_MasterAF);

	// Time Base configuration
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	/* Frequency 8Hz ~ 250kHz */
	//TIM_TimeBaseStructure.TIM_Prescaler = 90;		// Max: 1K
	if(pHwInfo->m_MasterTimHandle == TIM1 || pHwInfo->m_MasterTimHandle == TIM8)
		TIM_TimeBaseStructure.TIM_Period = 179;		// Max Freq: 500 K
	else
		TIM_TimeBaseStructure.TIM_Period = 89;		// Max Freq: 500 K

	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
	TIM_TimeBaseInit(pHwInfo->m_MasterTimHandle, &TIM_TimeBaseStructure);
	/* Enable preload for ARR */
	TIM_ARRPreloadConfig(pHwInfo->m_MasterTimHandle, ENABLE);
	/* Set master mode: update event as TRGO output  */
	TIM_SelectMasterSlaveMode(pHwInfo->m_MasterTimHandle, TIM_MasterSlaveMode_Enable);
	TIM_SelectOutputTrigger(pHwInfo->m_MasterTimHandle, TIM_TRGOSource_Update);
	// TIMx channel1 configuration in PWM mode
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	if(pHwInfo->m_MasterTimHandle == TIM1 || pHwInfo->m_MasterTimHandle == TIM8)
		TIM_OCInitStructure.TIM_Pulse = 89;		// Duty = 50 %
	else
		TIM_OCInitStructure.TIM_Pulse = 44;		// Duty = 50 %	
	//TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	switch(pHwInfo->m_MasterTimChan)
	{
		case TIM_Channel_1:
		{
			TIM_OC1Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
			/* Enable preload for OC1 */
			TIM_OC1PreloadConfig(pHwInfo->m_MasterTimHandle, TIM_OCPreload_Enable);
			if(pHwInfo->m_MasterTimHandle == TIM1 || pHwInfo->m_MasterTimHandle == TIM8)
				TIM_CtrlPWMOutputs(pHwInfo->m_MasterTimHandle, ENABLE);
			break;
		}
		case TIM_Channel_2:
		{
			TIM_OC2Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
			/* Enable preload for OC2 */
			TIM_OC2PreloadConfig(pHwInfo->m_MasterTimHandle, TIM_OCPreload_Enable);
			break;
		}
		case TIM_Channel_3:
		{
			TIM_OC3Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
			/* Enable preload for OC3 */
			TIM_OC3PreloadConfig(pHwInfo->m_MasterTimHandle, TIM_OCPreload_Enable);
			break;
		}
		case TIM_Channel_4:
		{
			TIM_OC4Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
			/* Enable preload for OC4 */
			TIM_OC4PreloadConfig(pHwInfo->m_MasterTimHandle, TIM_OCPreload_Enable);
			break;
		}
		default:
			break;
	}

	/* If user pluse cnt as position source, Slave Time should be enable as pluse counter*/

	/* NVIC Initialize. */
	/* Time Interrupt initialize */
	if(POS_SOURCE_INTER_PUL_CNT == *MotorControlStruct[port].m_pPosSource)
	{
		// Time Base configuration
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		/* Frequency 8Hz ~ 250kHz */
		//TIM_TimeBaseStructure.TIM_Period = 65535;
		
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(pHwInfo->m_SlaveTimHandle, &TIM_TimeBaseStructure);
		/* Set slave mode: update event as TRGO output  */
		TIM_SelectSlaveMode(pHwInfo->m_SlaveTimHandle, TIM_SlaveMode_External1);
		TIM_SelectInputTrigger(pHwInfo->m_SlaveTimHandle, pHwInfo->m_SlaveTrigSrc);
		
		NVIC_InitStructure.NVIC_IRQChannel = pHwInfo->m_SlaveIntChan;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		/* Interrupt config */
		TIM_ClearFlag(pHwInfo->m_SlaveTimHandle, TIM_FLAG_Update);
		TIM_ITConfig(pHwInfo->m_SlaveTimHandle, pHwInfo->m_SlaveIntSource , ENABLE);
		TIM_Cmd(pHwInfo->m_SlaveTimHandle, ENABLE);
	}
	
}

/*
**************************************************************************
* Description	: Enable Motor.
  	1) Input argument  : port: Motor port for set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
VOID MotorPwmEnable(MOTOR_PORT port)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];

	GPIO_SetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].pin);
}

/*
**************************************************************************
* Description	: Disable Motor.
  	1) Input argument  : port: Motor port for set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
VOID MotorPwmDisable(MOTOR_PORT port)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];

	GPIO_ResetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_EN].pin);
}

/*
**************************************************************************
* Description	: Clean Driver fault.
  	1) Input argument  : port: Motor port for clear.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
VOID MotorPwmClearFault(MOTOR_PORT port)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];

	GPIO_SetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].pin);
	vTaskDelay(DRV_CLEAR_FAULT_PLUSE_WIDTH);
	GPIO_ResetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_CLR].pin);

}


/*
**************************************************************************
* Description	: Start Motor.
  	1) Input argument  : port: Motor port for set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
VOID MotorPwmStart(MOTOR_PORT port)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];

	TIM_SetCounter(pHwInfo->m_MasterTimHandle, 0);		// Clean count value
#if 0
	/* Time Interrupt initialize */
	if(POS_SOURCE_INTER_PUL_CNT == *MotorControlStruct[port].m_pPosSource)
	{
		/* Interrupt configure */
		TIM_ITConfig(pHwInfo->m_MasterTimHandle, pHwInfo->m_IntSource , ENABLE);
	}
#endif
	// TIM1 counter enable
	TIM_Cmd(pHwInfo->m_MasterTimHandle, ENABLE);
}

/*
**************************************************************************
* Description	: Stop Motor.
  	1) Input argument  : port: Motor port for set.
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
VOID MotorPwmStop(MOTOR_PORT port)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];
#if 0
	/* Time Interrupt initialize */
	if(POS_SOURCE_PUL_CNT == *MotorControlStruct[port].m_pPosSource)
	{
		/* Interrupt configure */
		TIM_ITConfig(pHwInfo->m_TimHandle, pHwInfo->m_IntSource , DISABLE);
	}
#endif
	// TIM1 counter enable
	TIM_Cmd(pHwInfo->m_MasterTimHandle, DISABLE);
}

/*
**************************************************************************
* Description	: Set turn direction of motor.
  	1) Input argument  :port - Motor port for set.
  						Dir - Turn direction
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: TRUE - Success, FALSE - Failed 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Feb 02, 2015
**************************************************************************
*/
BOOL MotorPwmSetDir(MOTOR_PORT port, MOV_DIR dir)
{
	PWM_HW_INFO* pHwInfo;
	MOTOR_CTRL* pMotCtrlHandler;

	if(port >= MOTOR_PORT_MAX)
		return FALSE;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];
	pMotCtrlHandler = &MotorControlStruct[port];

	pMotCtrlHandler->m_CurrentDir = dir;

	if (MOV_DIR_POS == dir)
	{
		TIM_CounterModeConfig(pHwInfo->m_SlaveTimHandle, TIM_CounterMode_Up);
		if(*(pMotCtrlHandler->m_pMotDirResvert) == 0)
			GPIO_SetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].pin);
		else
			GPIO_ResetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].pin);
	}
	else
	{
		TIM_CounterModeConfig(pHwInfo->m_SlaveTimHandle, TIM_CounterMode_Down);
		if(*(pMotCtrlHandler->m_pMotDirResvert) == 0)
			GPIO_ResetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].pin);
		else
			GPIO_SetBits((GPIO_TypeDef *)pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].port, pHwInfo->m_CtrlPin[MOTOR_PORT_PIN_DIR].pin);
		
	}
	return TRUE;
}

/*
**************************************************************************
* Description	: Set Motor speed
  	1) Input argument  :port - Motor port for set
  						Speed - Motor turn speed
						IOclk = 1MHz, pwm frequency range is 500kHz ~ 16Hz
	2) Modify argument : None
 	3) Output argument : None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 12, 2015
**************************************************************************
*/
BOOL MotorPwmSetSpeed(MOTOR_PORT port, INT32 spd)
{
	//UINT32 freq;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef TIM_OCInitStructure;
	PWM_HW_INFO* pHwInfo;
	MOTOR_CTRL* pMotCtrlHandler;
	INT64 freq;

	
	if(port >= MOTOR_PORT_MAX)
			return FALSE;
		
	pMotCtrlHandler = &MotorControlStruct[port];

	/* Move direction control */
	if(spd > 0)
		MotorPwmSetDir(port, MOV_DIR_POS);
	else if(spd < 0)
	{
		MotorPwmSetDir(port, MOV_DIR_NEG);	
		spd = -spd;
	}
	else
		return FALSE;

	freq = (INT64)spd * (*pMotCtrlHandler->m_pMotEncoderLineNum) / (*pMotCtrlHandler->m_pMotUnitRatio); 
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[port];



	if((8 <= freq) && (freq <= 250000))
	{
		TIM_PrescalerConfig(pHwInfo->m_MasterTimHandle, (UINT16)(1000000 / freq - 1), TIM_PSCReloadMode_Update);
#if 0

		// Time Base configuration
		TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
		/* Frequency 15Hz ~ 500kHz */
		TIM_TimeBaseStructure.TIM_Prescaler = 90;		// Max: 1M
		TIM_TimeBaseStructure.TIM_Period = (UINT16)(1000000 / freq);

		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(pHwInfo->m_MasterTimHandle, &TIM_TimeBaseStructure);
		// TIM1 channel1 configuration in PWM mode
		TIM_OCStructInit(&TIM_OCInitStructure);
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = TIM_TimeBaseStructure.TIM_Period / 2;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;


		switch(pHwInfo->m_MasterTimChan)
		{
			case TIM_Channel_1:
			{
				TIM_OC1Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
				break;
			}
			case TIM_Channel_2:
			{
				TIM_OC2Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
				break;
			}
			case TIM_Channel_3:
			{
				TIM_OC3Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
				break;
			}
			case TIM_Channel_4:
			{
				TIM_OC4Init(pHwInfo->m_MasterTimHandle, &TIM_OCInitStructure);
				break;
			}
			default:
				break;
		}
		
#endif		
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*
**************************************************************************
* Description	: Get current position, when use pulse count number mode
  	1) Input argument  : port - Motor port for set
	2) Modify argument : None
 	3) Output argument : None
* Return      	: Current position
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 12, 2015
**************************************************************************
*/
INT32 MotorPwmGetPostion(MOTOR_PORT port)
{
	INT64 totalPulseCnt = 0;
	MOTOR_CTRL* pMotCtrlHandler;
	pMotCtrlHandler = &MotorControlStruct[port];

	totalPulseCnt = pMotCtrlHandler->m_AccumPluseCnt + TIM_GetCounter(PWM_HW_INFO_TABLE[port].m_SlaveTimHandle);

	totalPulseCnt *= (*(pMotCtrlHandler->m_pMotUnitRatio));;
	totalPulseCnt *= (*(pMotCtrlHandler->m_pMotGearRatioN));
	totalPulseCnt /= (*(pMotCtrlHandler->m_pMotGearRatioD ));
	totalPulseCnt /= (*(pMotCtrlHandler->m_pMotEncoderLineNum));
	//totalPulseCnt %= (*(pMotCtrlHandler->m_pMotUnitRatio));
	return (INT32)totalPulseCnt;
}

/*
**************************************************************************
* Description	: Set current position as origin
  	1) Input argument  : port - Motor port for set
	2) Modify argument : None
 	3) Output argument : None
* Return      	: TURE - Success, FALSE - Failed
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: May 12, 2015
**************************************************************************
*/
VOID MotorPwmSetOrigin(MOTOR_PORT port)
{
	MotorControlStruct[port].m_AccumPluseCnt = 0;
	TIM_SetCounter(PWM_HW_INFO_TABLE[port].m_SlaveTimHandle, 0);

	
}

/*
**************************************************************************
* Description	: Timer3  interrupt dispose;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: October 9, 2015
**************************************************************************
*/

VOID TIM2_IRQHandler(VOID)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[MOTOR_PORT_1];
	
	/* Timer over flow interrupt. */
	if(TIM_GetITStatus(pHwInfo->m_SlaveTimHandle, TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(pHwInfo->m_SlaveTimHandle, TIM_FLAG_Update);
		
		if(MotorControlStruct[MOTOR_PORT_1].m_CurrentDir == MOV_DIR_POS)
			MotorControlStruct[MOTOR_PORT_1].m_AccumPluseCnt += 0x100000000;
		else
			MotorControlStruct[MOTOR_PORT_1].m_AccumPluseCnt -= 0x100000000;
	}
}

/*
**************************************************************************
* Description	: Timer3  interrupt dispose;
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: June 12, 2015
**************************************************************************
*/
VOID TIM5_IRQHandler(VOID)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[MOTOR_PORT_2];
	
	/* Timer over flow interrupt. */
	if(TIM_GetITStatus(pHwInfo->m_SlaveTimHandle, TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(pHwInfo->m_SlaveTimHandle, TIM_FLAG_Update);
		if(MotorControlStruct[MOTOR_PORT_2].m_CurrentDir == MOV_DIR_POS)
			MotorControlStruct[MOTOR_PORT_2].m_AccumPluseCnt += 0x100000000;
		else
			MotorControlStruct[MOTOR_PORT_2].m_AccumPluseCnt -= 0x100000000;
	}

}
VOID TIM3_IRQHandler(VOID)
{
	PWM_HW_INFO* pHwInfo;
	
	pHwInfo = (PWM_HW_INFO*)&PWM_HW_INFO_TABLE[MOTOR_PORT_3];
	
	/* Timer over flow interrupt. */
	if(TIM_GetITStatus(pHwInfo->m_SlaveTimHandle, TIM_IT_Update) != RESET)
	{
		TIM_ClearFlag(pHwInfo->m_SlaveTimHandle, TIM_FLAG_Update);
		if(MotorControlStruct[MOTOR_PORT_3].m_CurrentDir == MOV_DIR_POS)
			MotorControlStruct[MOTOR_PORT_3].m_AccumPluseCnt += 0x10000;
		else
			MotorControlStruct[MOTOR_PORT_3].m_AccumPluseCnt -= 0x10000;
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
