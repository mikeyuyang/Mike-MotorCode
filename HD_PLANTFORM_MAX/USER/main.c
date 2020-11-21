/******************************************************************************************
*                             HD Microwave Co,.Ltd 2017-2037
*                                  All rights Reserved	
===========================================================================================	
*File:		.c
*Author:	hd-company
*Date:		2017-06-17
*Description: This file implement several methods for x funcitons.
*******************************************************************************************/


/******************************************************************************************
*Modify record
===========================================================================================
*Author:	
*Date:		2017-x-x
*Version	KFD01
*Description: This file implement several methods for x funcitons.
-------------------------------------------------------------------------------------------
*Author:	
*Date:		2017-x-x
*Version	KFD02
*Description: This file implement several methods for x funcitons.
*******************************************************************************************/

/******************************************************************************************
 *Includes
 ******************************************************************************************/
#include "BSP_BasicDataType.h"	// Basic data type define.
#include "BSP_TgtCfg.h"

#include "includes.h"
#include "UTL_Led.h"
#include "COM_SerialApplicaiton.h"
#include "COM_CanApplicaiton.h"
#include "MOV_Control.h"
#include "DIO_Control.h"
#include "UTL_EEPROM.h"
#include "UTL_Ethernet.h"
#include "COM_Ethernet.h"
#include "COM_ModbusTcp.h"
#include "ECD_APP.h"
#include "DBM_Data.h"
#include "SYS_Application.h"
#include "stdio.h"



/******************************************************************************************
 *Local macro
 ******************************************************************************************/
 /* Only for debug */
#define ITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define ITM_Port16(n)   (*((volatile unsigned short*)(0xE0000000+4*n)))
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
	
#define DEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define TRCENA          0x01000000



/* OS task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	CHAR const *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} OSTaskInfo;



/******************************************************************************************
 *Local const
 ******************************************************************************************/
/* Task enum define */
typedef enum tagOS_TASK_ITEM
{
	OS_TASK_ITEM_SYS_APP = 0,
	OS_TASK_ITEM_LED,
	OS_TASK_ITEM_DIO,
	OS_TASK_ITEM_EEPROM,
	OS_TASK_ITEM_MAX
}OS_TASK_ITEM;

/* Stack size definition */
#define APP_TASK_START_STK_SIZE		128



#define APP_TASK_SYS_APP_STK_SIZE		128
#define APP_TASK_LED_STK_SIZE			64
#define APP_TASK_DIO_STK_SIZE			64
#define APP_TASK_EPM_STK_SIZE			64

/* Start task protity definition */
#define  APP_TASK_START_PRIO            1

/******************************************************************************************
 *Local variables
 ******************************************************************************************/
/* Only for debug */
struct __FILE { int handle; /* Add whatever needed */ };
FILE __stdout;
FILE __stdin;



/* System 1ms timer counter */
static UINT32 SystemMsCnt = 0;


/* Start task protity definition */
TaskHandle_t StartTask_Handler;
TaskHandle_t LedTask_Handler;
TaskHandle_t DIOTask_Handler;
TaskHandle_t EPMTask_Handler;
TaskHandle_t SysAppTask_Handler;


/******************************************************************************************
 *Local fucntion
 ******************************************************************************************/
static  void  StartUp_Task (void *p_arg);


const OSTaskInfo APP_TASK_INFO_TABLE[] =
{
/*	task function,			task name,				Stack Size		  				arguments			priority		task handler		*/
	{ Task_SysApp, 			"SysAppTask",			APP_TASK_SYS_APP_STK_SIZE,		(void *)0,			14, 		&SysAppTask_Handler},
	{ Task_LedCtrl,			"LEDTask",				APP_TASK_LED_STK_SIZE,			(void *)0,			5,			&LedTask_Handler},
	{ Task_DIOCtrl, 		"DIOTask",				APP_TASK_DIO_STK_SIZE,			(void *)0,			10,			&DIOTask_Handler},
	{ Task_EEPROM, 			"EPMTask",				APP_TASK_EPM_STK_SIZE,			(void *)0,			4, 			&EPMTask_Handler},

#if 0 
VOID Task_EEPROM(void* p_arg);



//	{ "ModbusTask",			Task_RS232Modbus,		(void *)0,			&AppTaskMBRS232Stk[APP_TASK_MBRS232_STK_SIZE - 1],		12},
//	{ "UpdatePositionnTask",Task_UpdatePosition,	(void *)0,			&AppTaskUpdatePosStk[APP_TASK_UPDATE_POS_STK_SIZE - 1],	2},
//	{ "MoveControlTask",	Task_MoveCtrl,			(void *)0,			&AppTaskMoveCtrlStk[APP_TASK_MOVE_CTRL_STK_SIZE - 1],	3},
//	{ "CirAnsSendTask",		Task_CircularAnsSend,	(void *)0,			&AppTaskCirAnsSendStk[APP_TASK_CIR_ANS_STK_SIZE - 1],	4},

//	{ "ToneTask",			Task_ToneCtrl,			(void *)0,			&AppTaskToneStk[APP_TASK_TONE_STK_SIZE - 1],			15},
//	{ "KeyTask",			Task_KeyCtrl,			(void *)0,			&AppTaskKeyStk[APP_TASK_KEY_STK_SIZE - 1],				14},
//	{ "DisplayTask",		Task_DisplayCtrl,		(void *)0,			&AppTaskDisplayStk[APP_TASK_DISPLAY_STK_SIZE - 1],		20},
//	{ "MoveControlTask",	Task_MoveCtrl,			(void *)0,			&AppTaskMoveCtrlStk[APP_TASK_MOVE_CTRL_STK_SIZE - 1],	3}
#endif
};



/******************************************************************************************
 *Description: os lock for ?
 *IN	:
 *OUT	:
 *Modify:
 *Author:
 *Date	:
 *Notes	:
 ******************************************************************************************/
int main(void)
{
//	__set_PRIMASK(1);
	
	xTaskCreate((TaskFunction_t )StartUp_Task,           
	    (const char*    )"start_task",          
	    (uint16_t       )APP_TASK_START_STK_SIZE,        
	    (void*          )NULL,                  
	    (UBaseType_t    )APP_TASK_START_PRIO,       
	    (TaskHandle_t*  )&StartTask_Handler);  
	    
	vTaskStartScheduler(); 

	while(1); 
	
	return 0;
}


/*
**************************************************************************
* Description	: System tick initialization.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: June 10, 2015
**************************************************************************
*/
static void SystickInit(void)
{
	RCC_ClocksTypeDef rcc_clocks;

	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

	/* Get system clock frequency */
	RCC_GetClocksFreq(&rcc_clocks);
	SysTick_Config(rcc_clocks.HCLK_Frequency / configTICK_RATE_HZ);
}


/*
**************************************************************************
* Description	: System initial function, the configuration of RCC, NVIC,
* 				and system tick will be initialized.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Jan,23 2010
**************************************************************************
*/
VOID SysInit(VOID)
{
	enum { CLK_SET_RETRY_TIME = 100};
	ErrorStatus SystemInitStatus = SUCCESS;
	UINT8 overtimeCnt = 0;
	// Initialize Value of global variable.
	//--------------------------------------------------------------------------
	// Initialize system clock.
#if 1
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);			// Turn on external high speed clock.
	SystemInitStatus = ERROR;
	while((SystemInitStatus == ERROR) && (overtimeCnt < CLK_SET_RETRY_TIME))
	{
		SystemInitStatus = RCC_WaitForHSEStartUp();
		overtimeCnt ++;
	}
#endif 	
	// Is External start up?
	if(SystemInitStatus == SUCCESS)
	{
		// Start up ok.
		// Enable prefetch buffer.
//		FLASH_PrefetchBufferCmd(ENABLE);
		// 2 Clock delay for code.
//		FLASH_SetLatency(FLASH_Latency_2);
		// Config PLL.
		RCC_PLLConfig(RCC_PLLSource_HSE, 25, 360, 2, 8);
		// Turn PLL
		RCC_PLLCmd(ENABLE);
		// Wait PLL start up;
		while(RESET == RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
		// Use PLL Clock as system clock source.
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		// Wait system clock ready.
		while(0x08 != RCC_GetSYSCLKSource());

		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div4);
		RCC_PCLK2Config(RCC_HCLK_Div2);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA
								| RCC_AHB1Periph_GPIOB
								| RCC_AHB1Periph_GPIOC
								| RCC_AHB1Periph_GPIOD
								| RCC_AHB1Periph_GPIOE
								| RCC_AHB1Periph_GPIOF
								| RCC_AHB1Periph_GPIOG
								| RCC_AHB1Periph_GPIOH
								| RCC_AHB1Periph_GPIOI
								| RCC_AHB1Periph_GPIOJ
								| RCC_AHB1Periph_GPIOK
								| RCC_AHB1Periph_CRC
								| RCC_AHB1Periph_BKPSRAM
								//| RCC_AHB1Periph_CCMDATARAMEN
								//| RCC_AHB1Periph_DMA1
								//| RCC_AHB1Periph_DMA2
								//| RCC_AHB1Periph_DMA2D
								| RCC_AHB1Periph_ETH_MAC
								| RCC_AHB1Periph_ETH_MAC_Tx
								| RCC_AHB1Periph_ETH_MAC_Rx
								| RCC_AHB1Periph_ETH_MAC_PTP
								//| RCC_AHB1Periph_OTG_HS
								//| RCC_AHB1Periph_OTG_HS_ULPI
								, ENABLE);
#if 0		
		RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI
								| RCC_AHB2Periph_CRYP
								| RCC_AHB2Periph_HASH
								| RCC_AHB2Periph_RNG
								| RCC_AHB2Periph_OTG_FS
								, ENABLE);
#endif

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1
								| RCC_APB2Periph_TIM8
								| RCC_APB2Periph_USART1
								| RCC_APB2Periph_USART6
								| RCC_APB2Periph_ADC1
								| RCC_APB2Periph_ADC2
								| RCC_APB2Periph_ADC3
								//| RCC_APB2Periph_SDIO
								| RCC_APB2Periph_SPI1
								| RCC_APB2Periph_SPI4
								| RCC_APB2Periph_SYSCFG
								| RCC_APB2Periph_EXTIT
								| RCC_APB2Periph_TIM9
								| RCC_APB2Periph_TIM10
								| RCC_APB2Periph_TIM11
								| RCC_APB2Periph_SPI5
								| RCC_APB2Periph_SPI6
								//| RCC_APB2Periph_SAI1
								//| RCC_APB2Periph_SAI2
								//| RCC_APB2Periph_LTDC
								//| RCC_APB2Periph_DSI
								//| RCC_APB2Periph_DFSDM1
								//| RCC_APB2Periph_DFSDM2
								//| RCC_APB2Periph_UART9
								//| RCC_APB2Periph_UART10
								, ENABLE);
		// Enable PWR and BKP clock
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2
								| RCC_APB1Periph_TIM3
								| RCC_APB1Periph_TIM4
								| RCC_APB1Periph_TIM5
								| RCC_APB1Periph_TIM6
								| RCC_APB1Periph_TIM7
								| RCC_APB1Periph_TIM12
								| RCC_APB1Periph_TIM13
								| RCC_APB1Periph_TIM14
								//| RCC_APB1Periph_LPTIM1
								| RCC_APB1Periph_WWDG
								| RCC_APB1Periph_SPI2
								| RCC_APB1Periph_SPI3
								//| RCC_APB1Periph_SPDIF
								| RCC_APB1Periph_USART2
								| RCC_APB1Periph_USART3
								| RCC_APB1Periph_UART4
								| RCC_APB1Periph_UART5
								| RCC_APB1Periph_I2C1
								| RCC_APB1Periph_I2C2
								| RCC_APB1Periph_I2C3
								//| RCC_APB1Periph_FMPI2C1
								| RCC_APB1Periph_CAN1
								| RCC_APB1Periph_CAN2
								//| RCC_APB1Periph_CEC
								| RCC_APB1Periph_PWR
								//| RCC_APB1Periph_DAC
								| RCC_APB1Periph_UART7
								| RCC_APB1Periph_UART8
								, ENABLE);
		// Enable write access to Backup domain
		PWR_BackupAccessCmd(ENABLE);
	}
	else
	{
		// External clock error dispose.
//		printf("External high speed clock can't start up.");
		while(1);
	}
	//--------------------------------------------------------------------------
	// Initialize NVIC
//	NVIC_DeInit();
	// Configure two bits for preemption priority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	// Vector table set.
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
	//--------------------------------------------------------------------------
	/* // Initialize system tick. */
	SystickInit();
}


/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/

static  void  AppTaskCreate (void)
{
//	UINT8 err;
	UINT8 cnt;
	for(cnt = 0; cnt < OS_TASK_ITEM_MAX; cnt ++)
	{
		xTaskCreate(APP_TASK_INFO_TABLE[cnt].m_TaskFunc,			 
			(const char * const)APP_TASK_INFO_TABLE[cnt].m_TaskName,
			APP_TASK_INFO_TABLE[cnt].m_StackSize,
			APP_TASK_INFO_TABLE[cnt].m_TaskArg,
			APP_TASK_INFO_TABLE[cnt].m_TaskPrio,
			APP_TASK_INFO_TABLE[cnt].m_TaskHandler);
	}
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/
static  void  StartUp_Task (void *p_arg)
{
//	CPU_INT32U  cpu_clk_freq;
//	CPU_INT32U  cnts;

	(void)p_arg;
	/* System initialization */
	SysInit();
	/* Global value initilization */
	InitGlobalData();
	/* Initlize eeprom */
	InitEEPROM();
	/* Initialize parameter */
	EEPROMInitParam();	
	/* Ethernet initilization */
	EthMacInit();
	InitLwIP();
	/* Serial communication initilization */
	InitSerialComApp();
	/* Can communication initilization */
	InitCanComApp();
	/* Led initilization */
	LedInit();
	/* Encoder initilization */
	InitEncoderApp();
	/* Initialize move control function. */
	MoveInit();
	/* Initialize system applicaiton */
	SysAppInit();
	

	InitModbusTcpServer();
	
	vTaskDelay(500);

//    APP_TRACE_INFO(("Creating Application Tasks...\n\r"));
	AppTaskCreate();                                 /* Create Application Tasks                             */

						                                       /* Create Application Objects                           */
	vTaskDelete(StartTask_Handler);
}


/*
**************************************************************************
* Description	: Get system ms tick value.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: System ms tick
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
UINT32 GetSystemMsTick(VOID)
{
	return SystemMsCnt;
}

/*
*********************************************************************************************************
*                                          OS HOOKS 
*
* Description : Here define all hook functions used by OS
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 
*********************************************************************************************************
*/


void vApplicationTickHook( void )
{
	/* System tick increase */
    SystemMsCnt ++;
};

void vApplicationMallocFailedHook( void )
{
	
};

/*
*********************************************************************************************************
*                                          PRINTF REDEFFINATION 
*
* Description : For trace
*
* Arguments   : none
*
* Returns     : none
*
* Notes       : 
*********************************************************************************************************
*/
int fputc(int ch, FILE *f) {
  if (DEMCR & TRCENA) {
	while (ITM_Port32(0) == 0);
	ITM_Port8(0) = ch;
  }
  return(ch);
}


