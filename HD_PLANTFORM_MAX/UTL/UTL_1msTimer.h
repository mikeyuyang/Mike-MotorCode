/*
//######################################################################################################
//
// FILE:	UTL_1msTimer.h
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

#if !defined(UTL_1MS_TIMER_H_)
#define UTL_1MS_TIMER_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   SYS_1MS_TIMER_GLOBAL_				
#define  UTL_1MS_TIMER_EXT 
#else    
#define  UTL_1MS_TIMER_EXT  extern
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
// 32-bit rollover protection for timer diff
#define CALC_TIMER_DIFF(x,y) ( (x >= y) ? (x - y) : (0xffffffff - y + x) )

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
/* Timer state define */
typedef enum tagTIMER_STATE		// Timer states
{
	TIMER_STATE_STOPPED,		// Timer is not running - isElapsed always returns false
	TIMER_STATE_SUSPENDED,		// Timer is suspended - isElapsed always returns false
	TIMER_STATE_RUNNING,		// Timer is running - isElapsed checks the time
	TIMER_STATE_ELAPSED,	  	// Timer has elapsed - isElapsed always returns false
	TIMER_STATE_MAX
}TIMER_STATE;

/* Timer control struct */
typedef struct {
	UINT8 timerState;		// State of the timer.
	UINT32 timeDelay; 		// Required time delay in milliseconds
	UINT32 startTime; 		// Value of free running milliseconds counter when timer was started
	UINT32 suspendTime;		// Accumulated elapsed time due to suspensions.
}TIMER;

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
extern UINT32 GetSystemMsTick(VOID);

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


UTL_1MS_TIMER_EXT UINT32 GetTimerState( TIMER * aTimer );

UTL_1MS_TIMER_EXT VOID SetTimeDelay(TIMER * aTimer, UINT32 aTimeDelay);

UTL_1MS_TIMER_EXT VOID InitmsTimer(TIMER * aTimer, UINT32 aTimeDelay);

UTL_1MS_TIMER_EXT UINT32 GetTimeDelay(TIMER * aTimer );

UTL_1MS_TIMER_EXT VOID StartTimer( TIMER * aTimer );

UTL_1MS_TIMER_EXT VOID ResumeTimer( TIMER * aTimer );

UTL_1MS_TIMER_EXT UINT32 GetElapsedTime( TIMER * aTimer );

UTL_1MS_TIMER_EXT void SuspendTimer( TIMER * aTimer );

UTL_1MS_TIMER_EXT void StopTimer( TIMER * aTimer );

// Returns true if the timer is stopped, otherwise false.
UTL_1MS_TIMER_EXT BOOL isStopped( TIMER * aTimer );


// Returns true if the timer is stopped, otherwise false.
UTL_1MS_TIMER_EXT BOOL isRunning( TIMER * aTimer );


//! Returns true if the desired time delay has elapsed, otherwise false.
UTL_1MS_TIMER_EXT BOOL isElapsed( TIMER * aTimer );


#endif	//#if !defined(XXX_H_)
/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/

