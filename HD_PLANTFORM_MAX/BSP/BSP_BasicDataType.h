/*
//######################################################################################################
//					(c) Copyright HD
//                        All Rights Reserved
//
// FILE:	Sys_Define.h
//
// TITLE:	Define the basic data type for the system and so on.
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

#ifndef __SYS_DEF_H_
#define __SYS_DEF_H_

#include "stm32f4xx.h"
//#include "includes.h"

/*===== 0bit base types  ====================*/
#define VOID                void

/*===== 8bit base types  ====================*/
typedef   signed char       CHAR;
typedef unsigned char       UCHAR;
typedef          UCHAR      BYTE;


/*===== 16bit base types  ===================*/
typedef   signed short 		SHORT;
typedef unsigned short 		USHORT;
typedef          USHORT     WORD;


/*===== 32bit base types  ===================*/
typedef unsigned long       DWORD;
typedef unsigned long       *HANDLE;


/*===== float base types  ===================*/
typedef          float      FLOAT32;
typedef         double      FLOAT64;



/*===== integer base types  =============*/
typedef          CHAR       INT8;
typedef          UCHAR      UINT8;
typedef          SHORT      INT16;
typedef          USHORT     UINT16;
typedef signed long         INT32;
typedef unsigned long       UINT32;
typedef signed long long	INT64;
typedef unsigned long  long     UINT64;


/*===== Boolean types  ======================*/
#ifndef BOOL
#define BOOL UINT8
#endif /* ifndef BOOL */

#ifndef	FALSE
#define FALSE       0U
#endif /* ifndef FALSE */

#ifndef TRUE
#define TRUE        1U
#endif /* ifndef TRUE */

/*===== Data storage flags ===================*/
#define EXTERN		extern		// import global data/function
#define LOCAL		static		// static
#define VOLATILE	volatile

#define CONST		const
#define IN						// input arg
#define OUT						// output arg
#define MODIFY					// modify arg


/*===== function unused arg====================*/
#define UNUSED(x)	((x)=(x))

/*===== misc macros       ====================*/
#define itemof(array_prt)	(sizeof((array_prt))/sizeof((array_prt)[0]))


/************************************************************************/
/* WORD/DWORD swap, split and merge                                     */
/************************************************************************/
#define SWAP_WORD_BYTE(w)			\
	( ((((WORD)(w))>>8) & (WORD)0x00ff) + \
	  ((((WORD)(w))<<8) & (WORD)0xff00) )

#define SWAP_DWORD_BYTE(dw)		\
	( ((((DWORD)(dw))>>24) & (DWORD)0x000000ff) + \
	  ((((DWORD)(dw))<<24) & (DWORD)0xff000000) + \
	  ((((DWORD)(dw))>> 8) & (DWORD)0x0000ff00) + \
	  ((((DWORD)(dw))<< 8) & (DWORD)0x00ff0000) )

#define MK_WORD(Lo, Hi)  ((WORD)(((BYTE)((DWORD)(Lo)  & 0xff))   | (((WORD)((BYTE)((DWORD)(Hi)  & 0xff)))   << 8)))
#define MK_DWORD(Lo, Hi) ((DWORD)(((WORD)((DWORD)(Lo) & 0xffff)) | (((DWORD)((WORD)((DWORD)(Hi) & 0xffff))) << 16)))
#define LO_WORD(l)       ((WORD)((DWORD)(l)  & 0xffff))
#define HI_WORD(l)       ((WORD)((DWORD)(l) >> 16))
#define LO_BYTE(w)       ((BYTE)((DWORD)(w)  & 0xff))
#define HI_BYTE(w)       ((BYTE)((DWORD)(w) >> 8))

#define	_NULL 	((void *) 0)
#define GET_BIT(value, bit)		(value & (((UINT32)1) << bit))
#define SET_BIT2(value, bit)		(value |= (((UINT32)1) << bit))
#define CLEAR_BIT2(value, bit)	(value &= (~(((UINT32)1) << bit)))
#define	INV_BIT(value, bit)		(value ^= (((UINT32)1) << bit))
#define LEFT_SHIFT(value, bits)  ((DWORD)(value) <<= bits)
#define RIGHT_SHIFT(value, bits)  ((DWORD)(value) >>= bits)



#define BIT0    0x0001
#define BIT1    0x0002
#define BIT2    0x0004
#define BIT3    0x0008
#define BIT4    0x0010
#define BIT5    0x0020
#define BIT6    0x0040
#define BIT7    0x0080
#define BIT8    0x0100
#define BIT9    0x0200
#define BIT10   0x0400
#define BIT11   0x0800
#define BIT12   0x1000
#define BIT13   0x2000
#define BIT14   0x4000
#define BIT15   0x8000

/*===== Data type convert Macro====================*/
typedef union tagU8_TO_F32
{
	float m_F32;
	UINT8 m_U8[4];
}U8_TO_F32;


/*===== Move Control Macro====================*/
/* Move direction */
typedef enum tagMOV_DIR
{
	MOV_DIR_POS = 0,
	MOV_DIR_NEG,
	MOV_DIR_MAX
}MOV_DIR;


/*===== STM32 Special Macro====================*/
/* GPIO PORT & PIN. */
typedef struct tagGPIO_PORT_PIN
{
	GPIO_TypeDef* port;
	UINT16 pin;
	UINT16 pinSource;
}GPIO_PORT_PIN;

/*===== FreeRTOS special type====================*/


/* Macro for Critical protect. */
#define ENTER_CRITICAL_SECTION()  cpsid i
#define EXIT_CRITICAL_SECTION()  cpsie i


#endif //#ifndef __SYS_DEF_H_
