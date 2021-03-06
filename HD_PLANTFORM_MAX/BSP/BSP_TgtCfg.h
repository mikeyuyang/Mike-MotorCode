/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	TgtCfg.h
//
// TITLE:	Target config.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2009-12-22
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2009-12-22
// Version:			V1.0
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

/*
********************************************************************************************************
********************************************************************************************************
*/

#if !defined(TGT_CFG_H_)
#define TGT_CFG_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   TGT_CFG_GLOBAL_				
#define  TGT_CFG_EXT 
#else    
#define  TGT_CFG_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include "BSP_BasicDataType.h"			// Basic data type define.
//#include "includes.h"
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
#define		FREQ_CPU 180000000				//晶振频率
#define 	SYS_TICK 1						// The system tick(ms)
#define		SYSTEM_TIME_BASE	1			//系统时基，单位为ms
#define		TCNT1_INITIAL		(256-(F_CPU*SYSTEM_TIME_BASE/1024))				
												//系统时基，单位为ms

#define BIG_ENDIAN_MODE		0
#define LITTLE_ENDIAN_MODE	1

#define ONE_TURN_RANGE 36000

/*
********************************************************************************************************
*	 Global data type
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
*    Program Start 
*********************************************************************************************************
*/



/*
**************************************************************************
* Description	: 
  	1) Input argument  :
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jan,23 2010
**************************************************************************
*/


/*
**************************************************************************
* Description	: 
  	1) Input argument  :
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jan,23 2010
**************************************************************************
*/


#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/

