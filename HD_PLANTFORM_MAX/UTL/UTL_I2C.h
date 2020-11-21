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

#if !defined(UTL_I2C_H_)
#define UTL_I2C_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef  UTL_I2C_GLOBAL_
#define  UTL_I2C_EXT
#else    
#define  UTL_I2C_EXT  extern
#endif

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/
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
typedef enum tagI2C_PORT
{
	I2C_PORT_1 = 0,
	//I2C_PORT_2,
	I2C_PORT_MAX
}I2C_PORT;
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
UTL_I2C_EXT VOID I2CInit(I2C_PORT port);

UTL_I2C_EXT VOID I2CStart(I2C_PORT port);

UTL_I2C_EXT VOID I2CStop(I2C_PORT port);

UTL_I2C_EXT BOOL I2CSendByte(I2C_PORT port, UINT8 data);

UTL_I2C_EXT VOID I2CSendAck(I2C_PORT port);

UTL_I2C_EXT VOID I2CSendNoAck(I2C_PORT port);

UTL_I2C_EXT UINT8 I2CReadByte(I2C_PORT port);


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


