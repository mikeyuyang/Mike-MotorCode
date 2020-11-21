/*
//######################################################################################################
//
// FILE:	MTH_PID.c
//
// TITLE:	Sourse file for PID algorithm.
//
// AUTHOR:	
//
// DATE:	
//
// Description: This file include the PID algorithm dispose.
//
//------------------------------------------------------------------------------------------------------
// Created by: 		
// Created date:  
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
#include "math.h"
#include "stdlib.h"
#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#define MTH_PID_GLOBAL_
#include "MTH_PID.h"



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
* Description	: PID Process(Increment)
  	1) Input argument  :
 	2) Modify argument : 
 	3) Output argument : 
* Return      	: 
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Jan,23 2010
**************************************************************************
*/
VOID PidProcess(PID_HANDLER* pPidHandler)
{
	INT32 incVal;
	INT32 integralEn = 0;

	//pPidHandler->m_Err = pPidHandler->m_SetVal - pPidHandler->m_ActVal;	// Get error

	/* Inegral limit dispose */
	if(abs(pPidHandler->m_Err) <= pPidHandler->m_IntegralEnRange)
	{
		pPidHandler->m_SumErr += pPidHandler->m_Err;		// Calculate summer error
		if(pPidHandler->m_OutVal >= pPidHandler->m_MaxOutVal)
		{
			if(pPidHandler->m_Err < 0)
				integralEn = 1;
		}
		else if(pPidHandler->m_OutVal <= pPidHandler->m_MinOutVal)
		{
			if(pPidHandler->m_Err > 0)
				integralEn = 1;
		}
		else
			integralEn = 1;
	}
	else
		pPidHandler->m_SumErr = 0;
	/* Calculate increment value */
	incVal = (INT32)pPidHandler->m_Kp * (pPidHandler->m_Err - pPidHandler->m_LastErr)
							+ (INT32)pPidHandler->m_Ki * pPidHandler->m_Err * integralEn
							+ (INT32)pPidHandler->m_Kd * (pPidHandler->m_Err - 2 * pPidHandler->m_LastErr + pPidHandler->m_BeforeLastErr);
	pPidHandler->m_OutVal += incVal;
	pPidHandler->m_BeforeLastErr = pPidHandler->m_LastErr;
	pPidHandler->m_LastErr = pPidHandler->m_Err;
	pPidHandler->m_ActOutVal = (INT32)(pPidHandler->m_OutVal / 1000);

	/* Limit dispose */
	if(abs(pPidHandler->m_ActOutVal) > pPidHandler->m_MaxOutVal)
	{
		if(pPidHandler->m_ActOutVal > 0)
			pPidHandler->m_ActOutVal = pPidHandler->m_MaxOutVal;
		else
			pPidHandler->m_ActOutVal = -pPidHandler->m_MaxOutVal;
	}
#if 0	
	if(abs(pPidHandler->m_ActOutVal) < pPidHandler->m_MinOutVal)
	{
		if(pPidHandler->m_ActOutVal > 0)
			pPidHandler->m_ActOutVal = pPidHandler->m_MinOutVal;
		else
			pPidHandler->m_ActOutVal = -pPidHandler->m_MinOutVal;
	}
#endif
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

