/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	ECD_SSI.h
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

#if !defined(ECD_APP_H_)
#define ECD_APP_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef  ECD_APP_GLOBAL_
#define  ECD_APP_EXT
#else    
#define  ECD_APP_EXT  extern
#endif

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
//#include <avr/io.h>

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                           Global data types
*********************************************************************************************************
*/
typedef enum tag_ECD_TYPE		// Encoder type.
{
	ECD_TYPE_INTERNAL_PLUSE_CNT = 0,
	ECD_TYPE_HENGSTLER_SSI,
	ECD_TYPE_RP3806_SSI,
	ECD_TYPE_INC_SSI,
	ECD_TYPE_INC_ABZ,
	ECD_TYPE_MAX
}ECD_TYPE;


typedef enum tagENCODER_PORT
{
	ENCODER_PORT_1 = 0,
	ENCODER_PORT_2,
//	ENCODER_PORT_3,
	ENCODER_PORT_MAX
}ENCODER_PORT;
/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/

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
*********************************************************************************************************
* 	Global function implementations
*********************************************************************************************************
*/
ECD_APP_EXT VOID InitEncoderApp(VOID);

#endif
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


