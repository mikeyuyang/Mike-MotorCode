/*
//######################################################################################################
//					(c) Copyright HD
//                        All Rights Reserved
//
// FILE:	includes.h
//
// TITLE:	Put all include file together
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

#ifndef __INCLUDES_H_
#define __INCLUDES_H_


/*========= Standard libary =============*/
#include <stdarg.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <math.h>

/*========= OS Head file ===============*/

#include "FreeRTOS.h" 
#include "task.h" 
#include "queue.h" 
#include "semphr.h"

#include "croutine.h"
#include "event_groups.h"

/*========= STDPERIPH_DRIVER head file ===============*/
#include "stm32f4xx.h"


#endif //#ifndef __INCLUDES_H_
