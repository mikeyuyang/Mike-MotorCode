/*
//######################################################################################################
//
// FILE:	UTL_1msTimer.c
//
// TITLE:	1ms Timer, Use for system tick.
//
// AUTHOR:
//
// DATE:
//
// Description:This file provides a timer that can be used for timing anything requiring 1ms resolution
// 	 	 	 	with minimum processing overhead. Elapsed times of up to 50 days can be measured with
//            	the timer.
//			Instructions:
//			1. Create a timer with a time delay T;
               start the timer.
            2. Toggle port pin that can be monitored with a scope.
            3. Monitor the status of the timer in a loop until 'isElapsed'
               is true.
            4. Toggle port pin from step 2.
            5. Port pin pulse time should be equal to expected time delay
               delay T.
            6. Repeat test with different values of T.
//------------------------------------------------------------------------------------------------------
// Created by: 		
// Created date:  
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

#define SYS_1MS_TIMER_GLOBAL_
#include "UTL_1msTimer.h"

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
// Evaluates the timer to see if it has elapsed and returns the state.
__inline UINT32 GetTimerState( TIMER * aTimer )
{
	UINT32 now = GetSystemMsTick();
	if((aTimer->timerState == TIMER_STATE_RUNNING) &&  (CALC_TIMER_DIFF(now, aTimer->startTime) >= aTimer->timeDelay))
		aTimer->timerState = TIMER_STATE_ELAPSED;
	return aTimer->timerState;
}



/*
**************************************************************************
* Description	: Sets the time delay used to determine if the timer has elapsed
  	1) Input argument  : aTimer - Timer handle, aTimeDelay - Delay Timer (ms).
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: 
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
VOID SetTimeDelay(TIMER * aTimer, UINT32 aTimeDelay)
{
	aTimer->timeDelay = aTimeDelay;
}

/*
**************************************************************************
* Description	: The initialize function allows the desired time delay to be entered and
					initializes internal variables.
  	1) Input argument  : aTimer - Timer handle, aTimeDelay - Delay Timer (ms).
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
VOID InitmsTimer(TIMER * aTimer, UINT32 aTimeDelay)
{
	aTimer->timerState = TIMER_STATE_STOPPED;
	SetTimeDelay(aTimer, aTimeDelay );
	aTimer->suspendTime = 0;
}


/*
**************************************************************************
* Description	: Returns the time delay used to determine if the timer has elapsed.
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: Timer delay
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
UINT32 GetTimeDelay(TIMER * aTimer )
{
	return aTimer->timeDelay;
}

/*
**************************************************************************
* Description	: Starts the timer object by capturing the present value of the free
				running milliseconds.
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
VOID StartTimer( TIMER * aTimer )
{
	UINT32 now = GetSystemMsTick();
	aTimer->startTime = now;
	aTimer->timerState = TIMER_STATE_RUNNING;
	aTimer->suspendTime = 0;
}

/*
**************************************************************************
* Description	: Resume the timer object by capturing the present value of the free
				running milliseconds.
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
VOID ResumeTimer( TIMER * aTimer )
{
	UINT32 now = GetSystemMsTick();
	if( aTimer->timerState == TIMER_STATE_SUSPENDED )
	{
		aTimer->startTime = now;
		aTimer->timerState = TIMER_STATE_RUNNING;
	}
}

/*
**************************************************************************
* Description	: Calculates the time elapsed since the timer was started
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: Elapsed time.
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
UINT32 GetElapsedTime( TIMER * aTimer )
{
	UINT32 t;
	UINT32 now = GetSystemMsTick();
	switch( GetTimerState(aTimer) )
	{
	case TIMER_STATE_ELAPSED:
		t = aTimer->timeDelay;
		break;
	case TIMER_STATE_RUNNING:
		t = aTimer->startTime;
		t = CALC_TIMER_DIFF(now, t);
		break;
	case TIMER_STATE_STOPPED:
	case TIMER_STATE_SUSPENDED:
	default:
		t = 0;
	}
	return (t + aTimer->suspendTime);
}

/*
**************************************************************************
* Description	: Suspend the timer object by capturing the present value of the free
				running milliseconds.
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
void SuspendTimer( TIMER * aTimer )
{
	if( aTimer->timerState == TIMER_STATE_RUNNING )
	{
		aTimer->timerState = TIMER_STATE_SUSPENDED;
		aTimer->suspendTime = GetElapsedTime(aTimer);
	}
}

/*
**************************************************************************
* Description	: Suspend the timer object by capturing the present value of the free
				running milliseconds.
  	1) Input argument  : aTimer - Timer handle
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Aug 11, 2015
**************************************************************************
*/
void StopTimer( TIMER * aTimer )
{
	aTimer->timerState = TIMER_STATE_STOPPED;
}


// Returns true if the timer is stopped, otherwise false.
__inline BOOL isStopped( TIMER * aTimer )
{
	return aTimer->timerState == TIMER_STATE_STOPPED;
}

// Returns true if the timer is stopped, otherwise false.
__inline BOOL isRunning( TIMER * aTimer )
{
	return GetTimerState(aTimer) == TIMER_STATE_RUNNING;
}

//! Returns true if the desired time delay has elapsed, otherwise false.
__inline BOOL isElapsed( TIMER * aTimer )
{
	return GetTimerState(aTimer) == TIMER_STATE_ELAPSED;
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


