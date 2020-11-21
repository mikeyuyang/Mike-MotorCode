/*
//######################################################################################################
//					(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	UTL_EEPROM.c
//
// TITLE:	EEPROM operation process, used for system parameter storage.
//
// AUTHOR:	CHRIS_SHANG
//
// DATE:	2014-11-13
//
// Description:
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris_Shang
// Created date:   	2014-11-13
// Version:
// Descriptions: 	Frist Version
//
//------------------------------------------------------------------------------------------------------
// Modified by: 	Chris_Shang
// Modified date:	Nov 30, 2015
// Version:				
// Descriptions: 	Use simulate GPIO mode for I2C interface
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
#include <string.h>
#include <stm32f4xx.h>
#include <includes.h>

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.
#include "DBM_Data.h"
#include "UTL_I2C.h"
#define UTL_EEPROM_GLOBAL_
#include "UTL_EEPROM.h"

/*
*********************************************************************************************************
*    Local macro Defines 
*********************************************************************************************************
*/
#define EEPROM_PARAM_VERSION_BASEADDD	0
#define EEPROM_SETPOINT_BASEADDD 		2


/*
*********************************************************************************************************
*    Local Data type defines
*********************************************************************************************************
*/
/* EEPROM parameter information */
typedef struct tagEEPROM_PARAM_INFO
{
    UINT16 m_ParamVer;
	SPDataItem m_SP;
}EEPROM_PARAM_INFO;



/*
********************************************************************************************************
*	 Const Defines
********************************************************************************************************
*/
#define CURRENT_PARAM_VERSION	110u

#define DEVICE_TYPE_24C01_02	0
#define DEVICE_TYPE_24C04_08_16	1
#define DEVICE_TYPE_24C32_64	2
#define DEVICE_TYPE_24C128_256	3
#define DEVICE_TYPE_24C512		4

#define DEVICE_TYPE DEVICE_TYPE_24C512

#if(DEVICE_TYPE == DEVICE_TYPE_24C01_02)
	#define DEVICE_PAGE_SIZE		8u
#elif(DEVICE_TYPE == DEVICE_TYPE_24C04_08_16)
	#define DEVICE_PAGE_SIZE		16u
#elif(DEVICE_TYPE == DEVICE_TYPE_24C32_64)
	#define DEVICE_PAGE_SIZE		32u
#elif(DEVICE_TYPE == DEVICE_TYPE_24C128_256)
	#define DEVICE_PAGE_SIZE		64u
#elif(DEVICE_TYPE == DEVICE_TYPE_24C512)
	#define DEVICE_PAGE_SIZE		128u
#endif

#define EEPROM_BASE_DEVICE_ADDRESS	0xA0

static UINT16 ParamVersion;

/* EEPROM Parameter table */
EEPROM_PARAM_INFO EEPROMParam;

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
* Description	: Initialize the I2C port for EEPROM
  	1) Input argument  : None
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov,13 2014
**************************************************************************
*/
VOID InitEEPROM(VOID)
{
//	static UINT8 SendData[] = {0x12, 0x34, 0x56, 0x78, 0x9a};
//	static UINT8 RecData[5] = {0};

	I2CInit(I2C_PORT_1);
	memset(&EEPROMParam, 0, sizeof(EEPROM_PARAM_INFO));

	/* EEPROM Test */
//	EEPROMWriteBuffer(SendData, 0, 5);
//	EEPROMReadBuffer(RecData, 0, 5);
}

/*
**************************************************************************
* Description	: Read bytes form buffer
  	1) Input argument  : pBuffer - Point to the read buffer
  						ReadAddr - EEPROM internal address
  						pNumByteToRead - Point to the read size
	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov,8 2014
**************************************************************************
*/

BOOL EEPROMReadBuffer(UINT8* pBuffer, UINT16 readAddr, UINT16 NumByteToRead)
{
	UINT16 cnt;
	UINT8 deviceAdd = 0;

	/* Start up I2C */
	I2CStart(I2C_PORT_1);

	/* Send address and random address*/
#if ((DEVICE_TYPE == DEVICE_TYPE_24C01_02) || (DEVICE_TYPE == DEVICE_TYPE_24C04_08_16))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS + ((UINT8)(readAddr >> 8) << 1); 
	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) return FALSE;
	/* Send the EEPROM's internal address to read from: Only one byte address */
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(readAddr & 0xff)))
		return FALSE;

#elif ((DEVICE_TYPE == DEVICE_TYPE_24C32_64) \
		|| (DEVICE_TYPE == DEVICE_TYPE_24C128_256) \
		|| (DEVICE_TYPE == DEVICE_TYPE_24C512))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS;
	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) return FALSE;
	/* Send the EEPROM's internal address to read from: Only one byte address */
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(readAddr >> 8))) return FALSE;
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(readAddr & 0xff))) return FALSE;
#endif

	/* Send STRAT condition a second time */
	I2CStart(I2C_PORT_1);

	/* Send device address */
#if ((DEVICE_TYPE == DEVICE_TYPE_24C01_02) || (DEVICE_TYPE == DEVICE_TYPE_24C04_08_16))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS + ((UINT8)(readAddr >> 8) << 1); 
	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, (deviceAdd | 0x01))) 
		return FALSE;

#elif ((DEVICE_TYPE == DEVICE_TYPE_24C32_64) \
		|| (DEVICE_TYPE == DEVICE_TYPE_24C128_256) \
		|| (DEVICE_TYPE == DEVICE_TYPE_24C512))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS;	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, (deviceAdd | 0x01))) 
		return FALSE;
#endif
	/* Read data */
	for(cnt = 0; cnt < NumByteToRead; cnt ++)
	{
		*pBuffer = I2CReadByte(I2C_PORT_1);
		pBuffer ++;
		if(cnt != (NumByteToRead - 1))
			I2CSendAck(I2C_PORT_1);
		else
			I2CSendNoAck(I2C_PORT_1);
	}
	/* Send stop condition */
	I2CStop(I2C_PORT_1);

	return TRUE;
}


/*
**************************************************************************
* Description	: Writes Buffer to EEPROM.
	1) Input argument  : pBuffer - Point to the write buffer
						WriteAddr - EEPROM internal address
						NumByteToWrite - Point to the write size
	2) Modify argument : None
	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov,14 2014
**************************************************************************
*/
BOOL EEPROMWriteBuffer(UINT8* pBuffer, UINT16 WriteAddr, UINT16 NumByteToWrite)
{
	UINT8 firstPageNum = 0, numOfPage = 0, numOfSingle = 0;
	UINT16 cnt1, cnt2;
	UINT8 deviceAdd = 0;

	firstPageNum = DEVICE_PAGE_SIZE - (WriteAddr % DEVICE_PAGE_SIZE);
	if(NumByteToWrite < firstPageNum)
		firstPageNum = NumByteToWrite;
	numOfPage =  (NumByteToWrite - firstPageNum) / DEVICE_PAGE_SIZE;
	numOfSingle = (NumByteToWrite - firstPageNum) % DEVICE_PAGE_SIZE;

	/* Write first page */
	/* Start up I2C */
	I2CStart(I2C_PORT_1);
	/* Send address and random address*/
#if ((DEVICE_TYPE == DEVICE_TYPE_24C01_02) || (DEVICE_TYPE == DEVICE_TYPE_24C04_08_16))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS + ((UINT8)(WriteAddr >> 8) << 1); 
	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) 
		return FALSE;
	/* Send the EEPROM's internal address to read from: Only one byte address */
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(WriteAddr & 0xff)))
		return FALSE;

#elif ((DEVICE_TYPE == DEVICE_TYPE_24C32_64) \
	|| (DEVICE_TYPE == DEVICE_TYPE_24C128_256)\
	|| (DEVICE_TYPE == DEVICE_TYPE_24C512))
	deviceAdd = EEPROM_BASE_DEVICE_ADDRESS;
	/* Send EEPROM address for write */
	if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) return FALSE;
	/* Send the EEPROM's internal address to read from: Only one byte address */
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(WriteAddr >> 8))) return FALSE;
	if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)(WriteAddr & 0xff))) return FALSE;
#endif

	for(cnt2 = 0; cnt2 < firstPageNum; cnt2 ++)
	{
		if(FALSE == I2CSendByte(I2C_PORT_1, *pBuffer)) return FALSE;
		pBuffer ++;
	}
	/* Send stop condition */
	I2CStop(I2C_PORT_1);
	vTaskDelay(50);	


	/* Write data */
	for(cnt1 = 0; cnt1 < numOfPage; cnt1 ++)
	{
		/* Start up I2C */
		I2CStart(I2C_PORT_1);
		/* Send address and random address*/
#if ((DEVICE_TYPE == DEVICE_TYPE_24C01_02) || (DEVICE_TYPE == DEVICE_TYPE_24C04_08_16))
		deviceAdd = EEPROM_BASE_DEVICE_ADDRESS + ((UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) >> 8) << 1); 
		/* Send EEPROM address for write */
		if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) 
			return FALSE;
		/* Send the EEPROM's internal address to read from: Only one byte address */
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) & 0xff)))
			return FALSE;

#elif ((DEVICE_TYPE == DEVICE_TYPE_24C32_64) \
		|| (DEVICE_TYPE == DEVICE_TYPE_24C128_256)\
		|| (DEVICE_TYPE == DEVICE_TYPE_24C512))
		deviceAdd = EEPROM_BASE_DEVICE_ADDRESS;
		/* Send EEPROM address for write */
		if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) 
			return FALSE;
		/* Send the EEPROM's internal address to read from: Only one byte address */
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) >> 8))) 
			return FALSE;
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) & 0xff))) 
			return FALSE;
#endif

		for(cnt2 = 0; cnt2 < DEVICE_PAGE_SIZE; cnt2 ++)
		{
			if(FALSE == I2CSendByte(I2C_PORT_1, *pBuffer)) 
				return FALSE;
			pBuffer ++;
		}
		/* Send stop condition */
		I2CStop(I2C_PORT_1);
		vTaskDelay(50);

	}
	/* Send last page */
	/* Start up I2C */
	I2CStart(I2C_PORT_1);
	/* Send address and random address*/
#if ((DEVICE_TYPE == DEVICE_TYPE_24C01_02) || (DEVICE_TYPE == DEVICE_TYPE_24C04_08_16))
		deviceAdd = EEPROM_BASE_DEVICE_ADDRESS + ((UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) >> 8) << 1); 
		/* Send EEPROM address for write */
		if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) 
			return FALSE;
		/* Send the EEPROM's internal address to read from: Only one byte address */
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) & 0xff)))
			return FALSE;
	
#elif ((DEVICE_TYPE == DEVICE_TYPE_24C32_64) \
			|| (DEVICE_TYPE == DEVICE_TYPE_24C128_256) \
			|| (DEVICE_TYPE == DEVICE_TYPE_24C512))
			deviceAdd = EEPROM_BASE_DEVICE_ADDRESS;
		/* Send EEPROM address for write */
		if(FALSE == I2CSendByte(I2C_PORT_1, deviceAdd)) return FALSE;
		/* Send the EEPROM's internal address to read from: Only one byte address */
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) >> 8))) return FALSE;
		if(FALSE == I2CSendByte(I2C_PORT_1, (UINT8)((WriteAddr + firstPageNum + DEVICE_PAGE_SIZE * cnt1) & 0xff))) return FALSE;
#endif

	for(cnt2 = 0; cnt2 < numOfSingle; cnt2 ++)
	{
		if(FALSE == I2CSendByte(I2C_PORT_1, *pBuffer)) return FALSE;
		pBuffer ++;
	}
	/* Send stop condition */
	I2CStop(I2C_PORT_1);
	vTaskDelay(50);
	return TRUE;
}
/*
**************************************************************************
* Description	: EEPROM save parameter.
  	1) Input argument  : paramHandler - Handler to parameter needed to be saved
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov, 15 2014
**************************************************************************
*/
BOOL EEPROMSaveParam(UINT16 paramId)
{
	BOOL ret = FALSE;

	if(paramId >= eDataSPMax)
		return FALSE;
	ret = EEPROMWriteBuffer(GetSPAdd(paramId),
			EEPROM_SETPOINT_BASEADDD + (UINT16)((UINT32)GetSPAdd(paramId) - (UINT32)GetSPAdd(0)),
			GetSPSize(paramId));
	return ret;
}

/*
**************************************************************************
* Description	: EEPROM read all parameter.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov, 16 2014
**************************************************************************
*/
BOOL EEPROMSaveAllParam(VOID)
{
	BOOL ret = FALSE;
	ret = EEPROMWriteBuffer((UINT8*)&SPD,
			EEPROM_SETPOINT_BASEADDD,
			sizeof(SPD));
	return ret;

}


/*
**************************************************************************
* Description	: EEPROM read parameter.
  	1) Input argument  : paramHandler - Handler to parameter needed to be readed
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov, 15 2014
**************************************************************************
*/
BOOL EEPROMReadParam(UINT16 paramId)
{
	BOOL ret = FALSE;
	ret = EEPROMReadBuffer(GetSPAdd(paramId),
			EEPROM_SETPOINT_BASEADDD + (UINT16)((UINT32)GetSPAdd(paramId) - (UINT32)GetSPAdd(0)),
			GetSPSize(paramId));
	return ret;
}

/*
**************************************************************************
* Description	: EEPROM read all parameter.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov, 16 2014
**************************************************************************
*/
BOOL EEPROMReadAllParam(VOID)
{
	BOOL ret = FALSE;
	ret = EEPROMReadBuffer((UINT8*)&SPD,
							EEPROM_SETPOINT_BASEADDD,
							sizeof(SPD));
	return ret;
}


/*
**************************************************************************
* Description	: EEPROM read all parameter.
  	1) Input argument  : None
 	2) Modify argument : None
 	3) Output argument : None
* Return      	: None
* Notes       	: None
* Author	  	: Chris_Shang
* Date			: Nov, 16 2014
**************************************************************************
*/
BOOL EEPROMInitParam(VOID)
{
//	UINT16 cnt = 0;

	/* Read Parameter version */
	EEPROMReadBuffer((UINT8*)&ParamVersion, 0, 2);

	if(CURRENT_PARAM_VERSION != ParamVersion)
	{
		/* Save version */
		ParamVersion = CURRENT_PARAM_VERSION;
		EEPROMWriteBuffer((UINT8*)&ParamVersion, 0, 2);
		/* Parameter version has been changed, all parameters need to be saved again. */
		EEPROMSaveAllParam();
	}
	else
	{
		/* If parameter version unchanged, read out all parameter */
		EEPROMReadAllParam();
		/* Recover data value to default for those data don't need save */
		RecoverVolatileData();
	}
	return TRUE;
}


VOID Task_EEPROM(void* p_arg)
{
	(void)p_arg;
	while(TRUE)
	{
		if(SPD.m_SaveSingleParamCmd)
		{
			EEPROMSaveParam(SPD.m_SaveParamId);
			SPD.m_SaveSingleParamCmd = 0;
		}
		else if(SPD.m_SaveAllParamCmd)
		{
			EEPROMSaveAllParam();
			SPD.m_SaveAllParamCmd = 0;
		}
		vTaskDelay(100);
	}
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


