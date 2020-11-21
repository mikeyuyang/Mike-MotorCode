/*
//######################################################################################################
//
// FILE:	COM_SerialApplicaiton.h
//
// TITLE:	This function realized the serial communication functions.
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: All communication function for serial ports.
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

#if !defined(COM_ETHERNET_H_)
#define COM_ETHERNET_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef		COM_ETHERNET_GLOBAL_
#define		COM_ETHERNET_EXT
#else    
#define		COM_ETHERNET_EXT  extern
#endif


/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
#include "BSP_BasicDataType.h"			// Basic data type define.


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
COM_ETHERNET_EXT VOID InitLwIP(VOID);

COM_ETHERNET_EXT void Task_LwIP_DHCP(void* p_arg);


#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/

