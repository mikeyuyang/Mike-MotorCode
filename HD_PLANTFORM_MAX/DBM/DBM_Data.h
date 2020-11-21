/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	DBM_Data.h
//
// TITLE:	System data set define
//
// AUTHOR:	
//
// DATE:	
//
// Description:
//

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

#if !defined(DBM_DATA_H_)
#define DBM_DATA_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   DBM_DATA_GLOBAL_
#define  DBM_DATA_EXT
#else    
#define  DBM_DATA_EXT  extern
#endif

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "DBM_Enum.h"
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



/* Core reference date type */
typedef struct {
	void* m_DataPtr;		// Data Pointer
	INT32 m_Min;				// Min value to check range before writing
	INT32 m_Max;				// Max value
	UINT16 m_StepValue;		// step value for the register
	INT32 m_Default;			// Default value
	UINT16 m_Attrib;			// Attribute bit set/cleared - see attrib enum for bits description
	eDataFormat m_FormatCode;	//Format code
}CoreRefDataDef;


/* Actual value data type definition */
typedef struct tagAVDataItem
{
//===========================================
// Common data for platform
	UINT16 	m_ProductCode;
	UINT16 	m_HardwareVersion;
	UINT16 	m_FirmwareVersion;
	UINT16	m_BuildDate[6];
	UINT16	m_BuildTime[4];
	UINT16	m_SystemTime[10];
//===========================================
	UINT16	m_DIDirectStatus;
	UINT16	m_DIUserStatus;
	UINT16	m_DODirectStatus;
	UINT16	m_DOUserStatus;

	INT32	m_EncoderPosition1;
	INT32	m_UserPosition1;
	INT32	m_EncoderPosition2;
	INT32	m_UserPosition2;
	INT32	m_EncoderPosition3;
	INT32	m_UserPosition3;

	UINT16	m_InclinometerOnLine;
	UINT16	m_CompassOnLine;
	INT32	m_XInclination;
	INT32	m_YInclination;
	
	UINT16	m_GpsOnLine;
	INT32	m_Longitude;
	INT32	m_Latitude;
	INT32	m_Course;

	UINT16	m_GpsTime[10];

	UINT16	m_SysWarning;
	UINT16	m_SysFault;
}AVDataItem;

/* Set point data type definition */
typedef struct tagSPDataItem
{
	UINT16	m_SerEnable1;
	UINT16	m_SerSlaveAddress1;
	UINT16	m_SerProtocolType1;
	UINT16 	m_SerBrandRate1;
	UINT16 	m_SerBitLen1;
	UINT16 	m_SerStopBit1;
	UINT16 	m_SerParity1;
	UINT16	m_SerEnable2;
	UINT16	m_SerSlaveAddress2;
	UINT16	m_SerProtocolType2;
	UINT16 	m_SerBrandRate2;
	UINT16 	m_SerBitLen2;
	UINT16 	m_SerStopBit2;
	UINT16 	m_SerParity2;
	UINT16	m_SerEnable3;
	UINT16	m_SerSlaveAddress3;
	UINT16	m_SerProtocolType3;
	UINT16 	m_SerBrandRate3;
	UINT16 	m_SerBitLen3;
	UINT16 	m_SerStopBit3;
	UINT16 	m_SerParity3;
	UINT16	m_SerEnable4;
	UINT16	m_SerSlaveAddress4;
	UINT16	m_SerProtocolType4;
	UINT16 	m_SerBrandRate4;
	UINT16 	m_SerBitLen4;
	UINT16 	m_SerStopBit4;
	UINT16 	m_SerParity4;
	UINT16	m_SerEnable5;
	UINT16	m_SerSlaveAddress5;
	UINT16	m_SerProtocolType5;
	UINT16 	m_SerBrandRate5;
	UINT16 	m_SerBitLen5;
	UINT16 	m_SerStopBit5;
	UINT16 	m_SerParity5;
	UINT16	m_SerEnable6;
	UINT16	m_SerSlaveAddress6;
	UINT16	m_SerProtocolType6;
	UINT16 	m_SerBrandRate6;
	UINT16 	m_SerBitLen6;
	UINT16 	m_SerStopBit6;
	UINT16 	m_SerParity6;	
	UINT16	m_SerEnable7;
	UINT16	m_SerSlaveAddress7;
	UINT16	m_SerProtocolType7;
	UINT16 	m_SerBrandRate7;
	UINT16 	m_SerBitLen7;
	UINT16 	m_SerStopBit7;
	UINT16 	m_SerParity7;
	UINT16	m_SerEnable8;
	UINT16	m_SerSlaveAddress8;
	UINT16	m_SerProtocolType8;
	UINT16 	m_SerBrandRate8;
	UINT16 	m_SerBitLen8;
	UINT16 	m_SerStopBit8;
	UINT16 	m_SerParity8;
	
	UINT16	m_CanEnable1;		
	UINT16	m_CanSlaveAddress1;	
	UINT16	m_CanProtocolType1;	
	UINT16	m_CanBrandRate1;	
	UINT16	m_CanEnable2;		
	UINT16	m_CanSlaveAddress2;
	UINT16	m_CanProtocolType2;
	UINT16	m_CanBrandRate2;

	UINT16	m_AxisEnable1;
	UINT16	m_MechType1;
	UINT16	m_MotDirResvert1;
	UINT16	m_DrvCtrlType1;
	UINT16	m_MasteDrvAddr1;
	UINT16	m_SlaveDrvAddr1;
	UINT16	m_DualMotEn1;
	UINT32	m_MotEncodeLineNum1;
	UINT16	m_MotGearRatioN1;
	UINT16	m_MotGearRatioD1;
	UINT32	m_MotUnitRatio1;
	UINT16	m_MoveCtrlMode1;
	UINT16	m_SpdCtrlAcclEn1;
	UINT16	m_MaxSpd1;
	UINT16	m_MinSpd1;
	UINT16	m_Accel1;
	UINT16	m_Decel1;
	UINT16	m_TrimDisLimt1;			// If the distance is less than this value, Trim will start.
	UINT16 	m_ErrTol1;
	UINT16	m_WaitStopTime1;			// Unit: 0.01s
	UINT16	m_PosSource1;
	INT32	m_UserOrgPoint1;
	UINT32	m_WholeRangeDis1;
	UINT32	m_WholeRangePulNum1;
	UINT16	m_PidKp1;		
	UINT16	m_PidKi1;		
	UINT16	m_PidKd1;		
	UINT16	m_IntegralEnRng1;
	UINT16	m_NegLimitPort1;
	UINT16	m_PosLimitPort1;
	UINT16	m_OriginalPointPort1;
	UINT16	m_DrvFaultPort1;
	INT32	m_NegLimitVal1;
	INT32	m_PosLimitVal1;
	UINT16	m_EnableStartUpFindOrig1;
	UINT16	m_FindOrigSpd1;
	UINT16	m_FindOrigMode1;
	INT32	m_OrigOffSet1;
	UINT16	m_MoveStartCmd1;			
	UINT16	m_MoveStopCmd1;			
	UINT16	m_MoveEmgStopCmd1; 
	UINT16	m_ClearDrvFaultCmd1; 
	INT32	m_MoveSpdSetPoint1;		
	INT32	m_MovePosSetPoint1;	

	UINT16	m_AxisEnable2;
	UINT16	m_MechType2;
	UINT16	m_MotDirResvert2;
	UINT16	m_DrvCtrlType2;
	UINT16	m_MasteDrvAddr2;
	UINT16	m_SlaveDrvAddr2;
	UINT16	m_DualMotEn2;
	UINT32	m_MotEncodeLineNum2;
	UINT16	m_MotGearRatioN2;
	UINT16	m_MotGearRatioD2;
	UINT32	m_MotUnitRatio2;
	UINT16	m_MoveCtrlMode2;
	UINT16	m_SpdCtrlAcclEn2;
	UINT16	m_MaxSpd2;
	UINT16	m_MinSpd2;
	UINT16	m_Accel2;
	UINT16	m_Decel2;
	UINT16	m_TrimDisLimt2;			// If the distance is less than this value, Trim will start.
	UINT16 	m_ErrTol2;
	UINT16	m_WaitStopTime2;			// Unit: 0.01s
	UINT16	m_PosSource2;
	INT32	m_UserOrgPoint2;
	UINT32	m_WholeRangeDis2;
	UINT32	m_WholeRangePulNum2;
	UINT16	m_PidKp2;		
	UINT16	m_PidKi2;		
	UINT16	m_PidKd2;		
	UINT16	m_IntegralEnRng2;
	UINT16	m_NegLimitPort2;
	UINT16	m_PosLimitPort2;
	UINT16	m_OriginalPointPort2;	
	UINT16	m_DrvFaultPort2;
	INT32	m_NegLimitVal2;
	INT32	m_PosLimitVal2;
	UINT16	m_EnableStartUpFindOrig2;
	UINT16	m_FindOrigSpd2;
	UINT16	m_FindOrigMode2;
	INT32	m_OrigOffSet2;
	UINT16	m_MoveStartCmd2;		
	UINT16	m_MoveStopCmd2; 		
	UINT16	m_MoveEmgStopCmd2;
	UINT16	m_ClearDrvFaultCmd2; 
	INT32	m_MoveSpdSetPoint2;
	INT32	m_MovePosSetPoint2;		

	UINT16	m_AxisEnable3;
	UINT16	m_MechType3;
	UINT16	m_MotDirResvert3;
	UINT16	m_DrvCtrlType3;
	UINT16	m_MasteDrvAddr3;
	UINT16	m_SlaveDrvAddr3;
	UINT16	m_DualMotEn3;
	UINT32	m_MotEncodeLineNum3;
	UINT16	m_MotGearRatioN3;
	UINT16	m_MotGearRatioD3;
	UINT32	m_MotUnitRatio3;
	UINT16	m_MoveCtrlMode3;
	UINT16	m_SpdCtrlAcclEn3;
	UINT16	m_MaxSpd3;
	UINT16	m_MinSpd3;
	UINT16	m_Accel3;
	UINT16	m_Decel3;
	UINT16	m_TrimDisLimt3;			// If the distance is less than this value, Trim will start.
	UINT16 	m_ErrTol3;
	UINT16	m_WaitStopTime3;			// Unit: 0.01s
	UINT16	m_PosSource3;
	INT32	m_UserOrgPoint3;
	UINT32	m_WholeRangeDis3;
	UINT32	m_WholeRangePulNum3;
	UINT16	m_PidKp3;		
	UINT16	m_PidKi3;		
	UINT16	m_PidKd3;		
	UINT16	m_IntegralEnRng3;
	UINT16	m_NegLimitPort3;
	UINT16	m_PosLimitPort3;
	UINT16	m_OriginalPointPort3;	
	UINT16	m_DrvFaultPort3;
	INT32	m_NegLimitVal3;
	INT32	m_PosLimitVal3;
	UINT16	m_EnableStartUpFindOrig3;
	UINT16	m_FindOrigSpd3;
	UINT16	m_FindOrigMode3;
	INT32	m_OrigOffSet3;
	UINT16	m_MoveStartCmd3;		
	UINT16	m_MoveStopCmd3;		
	UINT16	m_MoveEmgStopCmd3;
	UINT16	m_ClearDrvFaultCmd3; 
	INT32	m_MoveSpdSetPoint3;
	INT32	m_MovePosSetPoint3;

	UINT16	m_ECDEnable1;	
	UINT16	m_ECDType1;
	UINT16	m_ECDSingleTurnBitNum1;
	UINT16	m_ECDMultiTurnBitNum1;
	UINT16	m_ECDCodeMode1;			
	UINT16	m_ECDGearRatioN1;
	UINT16	m_ECDGearRatioD1;	
	UINT16	m_ECDSignEn1;	
	
	UINT16	m_ECDEnable2;			
	UINT16	m_ECDType2;
	UINT16	m_ECDSingleTurnBitNum2;
	UINT16	m_ECDMultiTurnBitNum2;
	UINT16	m_ECDCodeMode2;		
	UINT16	m_ECDGearRatioN2;
	UINT16	m_ECDGearRatioD2;
	UINT16	m_ECDSignEn2;	

	UINT16	m_ECDEnable3;			
	UINT16	m_ECDType3;
	UINT16	m_ECDSingleTurnBitNum3;
	UINT16	m_ECDMultiTurnBitNum3;
	UINT16	m_ECDCodeMode3;			
	UINT16	m_ECDGearRatioN3;
	UINT16	m_ECDGearRatioD3;
	UINT16	m_ECDSignEn3;	

	UINT16	m_InclinometerMode;			
	UINT16	m_3DCompassMode;	
	UINT16	m_LevelCompEn;	

	UINT16	m_GpsMode;	
	UINT16	m_SyncTimeWithGpsCmd;	

	UINT16	m_DIInverseCtrl;
	UINT16	m_DI1FilterTime;
	UINT16	m_DI2FilterTime;
	UINT16	m_DI3FilterTime;
	UINT16	m_DI4FilterTime;
	UINT16	m_DI5FilterTime;
	UINT16	m_DI6FilterTime;
	UINT16	m_DI7FilterTime;
	UINT16	m_DI8FilterTime;
	UINT16	m_DI9FilterTime;
	UINT16	m_DOInverseCtrl;
	UINT16	m_DO1Mode;
	UINT16	m_DO1PulseTime;
	UINT16	m_DO2Mode;
	UINT16	m_DO2PulseTime;
	UINT16	m_DO3Mode;
	UINT16	m_DO3PulseTime;
	UINT16	m_DO4Mode;
	UINT16	m_DO4PulseTime;
	UINT16	m_DO5Mode;
	UINT16	m_DO5PulseTime;
	UINT16	m_DO6Mode;
	UINT16	m_DO6PulseTime;
	UINT16	m_DO7Mode;
	UINT16	m_DO7PulseTime;
	UINT16	m_DO8Mode;
	UINT16	m_DO8PulseTime;
	UINT16	m_DO9Mode;
	UINT16	m_DO9PulseTime;
//===========================================
	UINT16	m_SaveSingleParamCmd;
	UINT16	m_SaveParamId;
	UINT16	m_SaveAllParamCmd;
	UINT16	m_SaveParamPercent;
	UINT16	m_ClearSysFaultCmd;

}SPDataItem;

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
/* Actual value declaration */
DBM_DATA_EXT AVDataItem AVD;
/* Set point declaration */
DBM_DATA_EXT SPDataItem SPD;
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
DBM_DATA_EXT VOID *GetAVDataPoint(UINT16 index);

DBM_DATA_EXT UINT16 GetAVDefaultValue(UINT16 index);

DBM_DATA_EXT UINT16 GetAVMaxValue(UINT16 index);

DBM_DATA_EXT UINT16 GetAVMinValue(UINT16 index);

DBM_DATA_EXT UINT16 GetAVStepValue(UINT16 index);

DBM_DATA_EXT VOID *GetSPDataPoint(UINT16 index);

DBM_DATA_EXT UINT16 GetSPDefaultValue(UINT16 index);

DBM_DATA_EXT UINT16 GetSPMaxValue(UINT16 index);

DBM_DATA_EXT UINT16 GetSPMinValue(UINT16 index);

DBM_DATA_EXT UINT16 GetSPStepValue(UINT16 index);

DBM_DATA_EXT VOID* GetSPAdd(UINT16 index);

DBM_DATA_EXT UINT8 GetSPSize(UINT16 index);

DBM_DATA_EXT BOOL GetNonvolatileAttrib(UINT16 index);

DBM_DATA_EXT VOID InitGlobalData(VOID);

DBM_DATA_EXT VOID RecoverVolatileData(VOID);

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
