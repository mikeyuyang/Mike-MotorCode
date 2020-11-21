/*
//######################################################################################################
//										(c) Copyright Chris_Shang
//                        All Rights Reserved
//
// FILE:	DS_Enum.h
//
// TITLE:	System enum define
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

#if !defined(DBM_ENUM_H_)
#define DBM_ENUM_H_

/*
********************************************************************************************************
*	Predefine
********************************************************************************************************
*/
#ifdef   DBM_ENUM_GLOBAL_
#define  DBM_ENUM_EXT
#else
#define  DBM_ENUM_EXT  extern
#endif

/*
********************************************************************************************************
*	Add include files
********************************************************************************************************
*/

#include "BSP_BasicDataType.h"			// Basic data type define.

/*
*********************************************************************************************************
*    Local macro Defines
*********************************************************************************************************
*/
#define OFFSET_ACTUAL_VALUES    30000
#define OFFSET_SETPOINTS        40000

/*
*********************************************************************************************************
*                                           Global data types
*********************************************************************************************************
*/
/* Global unit define */
typedef enum
{
  eUnitNone,
  eUnitPercent,
  eUnitSeconds,
  eUnitMs,
  eUnitAmps,
  eUnitVolts,
  eUnitHz,
  eUnitKw,
  eUnitDegC,
  eUnitHp,
  eUnitMWh,
  eUnitMvarh,
  eUnitKvar,
  eUnitKVA,
  eUnitHours,
  eUnitDegrees,
  eUnitFLA,
  eUnitCT,
  eUnitPF,
  eUnitDays,
  eUnitMins,
  eUnitXRated,
  eUnitPU,
  eUnitOps,
  eUnitCycles,
  eUnitOhms,
  eUnitPercentMNV,
  eUnitPercentMNR,
  eUnitPercentFLA,
  eUnitPercentUnb,
  eUnitVolts_ab,
  eUnitVolts_bc,
  eUnitVolts_ca,
  eUnitVolts_an,
  eUnitVolts_bn,
  eUnitVolts_cn,
  eUnitVolts_LL,
  eUnitVolts_Aux,
  eUnitKOhms,
  eUnitNCV,
  eUnitPercentRating,
  eUnitTen_msec,
  eUnitVT,
  eUnitDegLead,
  eUnitDegLag,
  eUnit_ISquared,
  eUnitkAmps,
  eUnitkVolts,
  eUnitRatio,
  eUnitTrips,
  eUnitCounts,
  eUnitMVA,
  eUnitCTW1,
  eUnitCTW2,
  eUnitPercentfo,
  eUnitCTg,
  eUnitDay
}eDataUnit;

/* Global data format define */
typedef enum
{
	eFormat_F1, 		// Unsigned 16bit Value
	eFormat_F2, 		// Unsigned Value, 1 Decimal Place
	eFormat_F3, 		// Unsigned Value, 2 Decimal Places
	eFormat_F4, 		// Unsigned Value, 3 Decimal Places
	eFormat_F5, 		// Signed 16bit Value
	eFormat_F6, 		// Signed Value, 1 Decimal Place
	eFormat_F7, 		// Signed Value, 2 Decimal Places
	eFormat_F8, 		// Signed Value, 3 Decimal Places	
	eFormat_F9, 		// 2's Complement Signed Value
	eFormat_F11, 		// Unsigned 32bit Value
	eFormat_F12, 		// Unsigned 32bit Value, 1 Decimal Place
	eFormat_F13, 		// Unsigned 32bit Value, 2 Decimal Places
	eFormat_F14, 		// Unsigned 32bit Value, 3 Decimal Places
	eFormat_F15,		// Signed 32bit Value
	eFormat_F16,		// Signed 32bit Value, 1 Decimal Place
	eFormat_F17,		// Signed 32bit Value, 2 Decimal Places
	eFormat_F18,		// Signed 32bit Value, 3 Decimal Places
	eFormat_F22,		// Two 8-BIT Characters Packed Into 16-BIT Unsigned
	eFormat_F101,		// Serial protocol type
	eFormat_F102,		// UART Baud Rate
	eFormat_F103,		// CAN protocol type
	eFormat_F104,		// CAN Baud Rate	
	eFormat_F105,		// Modulate mode
	eFormat_F106		// Polarization Mode
}eDataFormat;



enum eForceCoilCommands {
  eCmdNone = 0,
  eCmdReset = 1,
  eCmdLockoutReset = 2,
  eCmdStop = 3,
  eCmdOpen = 4,
  eCmdClose = 5,
  eCmdDisplayMessage = 6,
};

enum eAccess {
  eNone = 0,
  eProt = 1,
  eComm = 2,
  eIO = 4,
};

enum eDataItemAttrib {
  eAttrib_None=0,
  eAttrib_ACardDependent = 1,
  eAttrib_ACardNeeded = 2,
  eAttrib_BCardDependent = 4,
  eAttrib_BCardNeeded = 8,
  eAttrib_AutomationS = 16,
  eAttrib_Automation1 = 32,
  eAttrib_Automation2 = 64,
  eAttrib_Automation3 = 128,
  eAttrib_CommS = 256,
  eAttrib_CommD = 512,
  eAttrib_CommP = 1024,
  eAttrib_Group = 2048,
  eAttrib_IsFirst = 4096,
  eAttrib_FRAM = 8192,
};

enum eFormatTextOffset {
  eFMTStr_1_A_Secondary,
  eFMTStr_1_Cycle,
  eFMTStr_1_Hour,
  eFMTStr_1_Minute,
  eFMTStr_1_Second,
  eFMTStr_10_Ohm_Copper,
  eFMTStr_100_Ohm_Nickel,
  eFMTStr_100_Ohm_Platinum,
  eFMTStr_115200_baud,
  eFMTStr_120_Ohm_Nickel,
  eFMTStr_125_kbps,
  eFMTStr_19200_baud,
  eFMTStr_1st,
  eFMTStr_1x192,
  eFMTStr_250_kbps,
  eFMTStr_2nd,
  eFMTStr_2W,
  eFMTStr_38400_baud,
  eFMTStr_3rd,
  eFMTStr_3W,
  eFMTStr_3x64,
  eFMTStr_4th,
  eFMTStr_5_A_Secondary,
  eFMTStr_50_Hz,
  eFMTStr_500_kbps,
};

/* AV Data index */
enum eModbus30000REG {
	eDataAVNone = -1,
	eDataProductCode = 0,		// Modbus REG30000
	eDataHWRev,
	eDataSWRev,
	eDataBuildDate_1,
	eDataBuildDate_2,
	eDataBuildDate_3,
	eDataBuildDate_4,
	eDataBuildDate_5,
	eDataBuildDate_6,
	eDataBuildTime_1,
	eDataBuildTime_2,
	eDataBuildTime_3,
	eDataBuildTime_4,

	eDataSystemTime_1,
	eDataSystemTime_2,
	eDataSystemTime_3,
	eDataSystemTime_4,
	eDataSystemTime_5,
	eDataSystemTime_6,
	eDataSystemTime_7,
	eDataSystemTime_8,
	eDataSystemTime_9,
	eDataSystemTime_10,
	
	eDataDIDirectStatus,
	eDataDIUseStatus,
	eDataDODirectStatus,
	eDataDOUseStatus,

	eDataEncoderPosition1_1,
	eDataEncoderPosition1_2,
	eDataUserPosition1_1,
	eDataUserPosition1_2,
	eDataEncoderPosition2_1,
	eDataEncoderPosition2_2,
	eDataUserPosition2_1,
	eDataUserPosition2_2,
	eDataEncoderPosition3_1,
	eDataEncoderPosition3_2,
	eDataUserPosition3_1,
	eDataUserPosition3_2,
	
	eDataInclinometerOnLine,
	eDataCompassOnLine,
	eDataXInclination_1,
	eDataXInclination_2,
	eDataYInclination_1,
	eDataYInclination_2,

	eDataGpsOnLine,
	eDataLongitude_1,
	eDataLongitude_2,
	eDataLatitude_1,
	eDataLatitude_2,
	eDataCourse_1,
	eDataCourse_2,

	eDataGpsTime_1,
	eDataGpsTime_2,
	eDataGpsTime_3,
	eDataGpsTime_4,
	eDataGpsTime_5,
	eDataGpsTime_6,
	eDataGpsTime_7,
	eDataGpsTime_8,
	eDataGpsTime_9,
	eDataGpsTime_10,	

	eDataSysWarning,
	eDataSysFault,

	eDataAVMax
};

#define LAST_AV_REG eDataReserved32123
#define NUM_AV 1398

/* SP Data index */
enum eModbus40000REG {
	eDataSPNone = -1,
//=======Serial port parameter=========//
	eDataSerEnable1 = 0,
	eDataSerSlaveId1,
	eDataSerProtocol1,
	eDataSerBandRate1,
	eDataSerBitLen1,
	eDataSerStopBit1,
	eDataSerParity1,
	eDataSerEnable2,
	eDataSerSlaveId2,
	eDataSerProtocol2,
	eDataSerBandRate2,
	eDataSerBitLen2,
	eDataSerStopBit2,
	eDataSerParity2,
	eDataSerEnable3,
	eDataSerSlaveId3,
	eDataSerProtocol3,
	eDataSerBandRate3,
	eDataSerBitLen3,
	eDataSerStopBit3,
	eDataSerParity3,
	eDataSerEnable4,
	eDataSerSlaveId4,
	eDataSerProtocol4,
	eDataSerBandRate4,
	eDataSerBitLen4,
	eDataSerStopBit4,
	eDataSerParity4,
	eDataSerEnable5,
	eDataSerSlaveId5,
	eDataSerProtocol5,
	eDataSerBandRate5,
	eDataSerBitLen5,
	eDataSerStopBit5,
	eDataSerParity5,
	eDataSerEnable6,
	eDataSerSlaveId6,
	eDataSerProtocol6,
	eDataSerBandRate6,
	eDataSerBitLen6,
	eDataSerStopBit6,
	eDataSerParity6,
	eDataSerEnable7,
	eDataSerSlaveId7,
	eDataSerProtocol7,
	eDataSerBandRate7,
	eDataSerBitLen7,
	eDataSerStopBit7,
	eDataSerParity7,
	eDataSerEnable8,
	eDataSerSlaveId8,
	eDataSerProtocol8,
	eDataSerBandRate8,
	eDataSerBitLen8,
	eDataSerStopBit8,
	eDataSerParity8,
//=======Serial port parameter=========//
	eDataCanEnable1,
	eDataCanSlaveId1,
	eDataCanProtocol1,
	eDataCanBandRate1,
	eDataCanEnable2,
	eDataCanSlaveId2,
	eDataCanProtocol2,
	eDataCanBandRate2,
//=======Motor parameter=========//
	eAxisEnable1,
	eDataMechType1,	
	eMotDirResvert1,
	eDataDrvCtrlType1,		
	eMasteDrvAddr1,		
	eSlaveDrvAddr1,		
	eDualMotEn1,		
	eMotEncodeLineNum1_1,
	eMotEncodeLineNum1_2,
	eMotGearRatioD1,	
	eMotGearRatioN1,	
	eMotUnitRatio1_1,	
	eMotUnitRatio1_2,		
	eMoveCtrlMode1,	
	eDataSpdCtrlAcclEn1,
	eDataAxis1MaxSpd1,	
	eDataAxis1MinSpd1,		
	eDataAxis1Accel1,		
	eDataAxis1Decel1,	
	eDataTrimDisLimt1,	
	eDataErrTol1,		
	eDataWaitStopTime1,	
	eDataPosSource1,	
	eDataUserOrgPoint1_1,
	eDataUserOrgPoint1_2,
	eDataWholeRangeDis1_1,
	eDataWholeRangeDis1_2,
	eDataWholeRangePulNum1_1,
	eDataWholeRangePulNum1_2,
	ePidKp1,	
	ePidKi1,		
	ePidKd1,
	eIntegralEnRng1,
	eLeftLimitPort1,
	eRightLimitPort1,
	eOriginalPointPort1,
	eDrvFaultPort1,
	eNegLimitVal1_1,
	eNegLimitVal1_2,
	ePosLimitVal1_1,
	ePosLimitVal1_2,
	eEnableStartUpFindOrig1,
	eFindOrigSpd1,
	eFindOrigMode1,
	eOrigOffSet1_1,
	eOrigOffSet1_2,
	eMoveStartCmd1,
	eMoveStopCmd1,	
	eMoveEmgStopCmd1,
	eCleanDrvFaultCmd1,
	eMoveSpdSetPoint1_1,
	eMoveSpdSetPoint1_2,
	eMovePosSetPoint1_1,
	eMovePosSetPoint1_2,
	
	eAxisEnable2,
	eDataMechType2,
	eMotDirResvert2,
	eDataDrvCtrlType2,		
	eMasteDrvAddr2,		
	eSlaveDrvAddr2,		
	eDualMotEn2,		
	eMotEncodeLineNum2_1,
	eMotEncodeLineNum2_2,
	eMotGearRatioD2,	
	eMotGearRatioN2,	
	eMotUnitRatio2_1,	
	eMotUnitRatio2_2,	
	eMoveCtrlMode2,	
	eDataSpdCtrlAcclEn2,
	eDataAxis1MaxSpd2,	
	eDataAxis1MinSpd2,		
	eDataAxis1Accel2,		
	eDataAxis1Decel2,	
	eDataTrimDisLimt2,	
	eDataErrTol2,		
	eDataWaitStopTime2,	
	eDataPosSource2,	
	eDataUserOrgPoint2_1,
	eDataUserOrgPoint2_2,
	eDataWholeRangeDis2_1,
	eDataWholeRangeDis2_2,
	eDataWholeRangePulNum2_1,
	eDataWholeRangePulNum2_2,
	ePidKp2,	
	ePidKi2,		
	ePidKd2,
	eIntegralEnRng2,
	eLeftLimitPort2,
	eRightLimitPort2,
	eOriginalPointPort2,
	eDrvFaultPort2,
	eNegLimitVal2_1,
	eNegLimitVal2_2,
	ePosLimitVal2_1,
	ePosLimitVal2_2,
	eEnableStartUpFindOrig2,
	eFindOrigSpd2,
	eFindOrigMode2,
	eOrigOffSet2_1,
	eOrigOffSet2_2,	
	eMoveStartCmd2,
	eMoveStopCmd2,
	eMoveEmgStopCmd2,
	eCleanDrvFaultCmd2,
	eMoveSpdSetPoint2_1,
	eMoveSpdSetPoint2_2,
	eMovePosSetPoint2_1,
	eMovePosSetPoint2_2,
	
	eAxisEnable3,
	eDataMechType3,
	eMotDirResvert3,
	eDataDrvCtrlType3,		
	eMasteDrvAddr3,		
	eSlaveDrvAddr3,		
	eDualMotEn3,		
	eMotEncodeLineNum3_1,
	eMotEncodeLineNum3_2,
	eMotGearRatioD3,	
	eMotGearRatioN3,	
	eMotUnitRatio3_1,	
	eMotUnitRatio3_2,	
	eMoveCtrlMode3,	
	eDataSpdCtrlAcclEn3,
	eDataAxis1MaxSpd3,	
	eDataAxis1MinSpd3,		
	eDataAxis1Accel3,		
	eDataAxis1Decel3,	
	eDataTrimDisLimt3,	
	eDataErrTol3,		
	eDataWaitStopTime3,	
	eDataPosSource3,	
	eDataUserOrgPoint3_1,
	eDataUserOrgPoint3_2,
	eDataWholeRangeDis3_1,
	eDataWholeRangeDis3_2,
	eDataWholeRangePulNum3_1,
	eDataWholeRangePulNum3_2,
	ePidKp3,	
	ePidKi3,		
	ePidKd3,
	eIntegralEnRng3,
	eLeftLimitPort3,
	eRightLimitPort3,
	eOriginalPointPort3,	
	eDrvFaultPort3,
	eNegLimitVal3_1,
	eNegLimitVal3_2,
	ePosLimitVal3_1,
	ePosLimitVal3_2,
	eEnableStartUpFindOrig3,
	eFindOrigSpd3,
	eFindOrigMode3,	
	eOrigOffSet3_1,
	eOrigOffSet3_2,	
	eMoveStartCmd3,
	eMoveStopCmd3,
	eMoveEmgStopCmd3,
	eCleanDrvFaultCmd3,
	eMoveSpdSetPoint3_1,
	eMoveSpdSetPoint3_2,
	eMovePosSetPoint3_1,
	eMovePosSetPoint3_2,

	eECDEnable1,
	eECDType1,
	eECDSingleTurnBitNum1,
	eECDMultiTurnBitNum1,
	eECDCodeMode1,		
	eECDGearRatioN1,
	eECDGearRatioD1,
	eECDSignEn1,

	eECDEnable2,
	eECDType2,
	eECDSingleTurnBitNum2,
	eECDMultiTurnBitNum2,
	eECDCodeMode2,	
	eECDGearRatioN2,
	eECDGearRatioD2,
	eECDSignEn2,

	eECDEnable3,
	eECDType3,
	eECDSingleTurnBitNum3,
	eECDMultiTurnBitNum3,
	eECDCodeMode3,
	eECDGearRatioN3,
	eECDGearRatioD3,
	eECDSignEn3,

	eInclinometerMode,		
	e3DCompassMode,		
	eLevelCompEn,

	eGpsMode,	
	eSyncTimeWithGpsCmd,
	
	eDataDIInverseCtrl,
	eDataDI1FilterTime,
	eDataDI2FilterTime,
	eDataDI3FilterTime,
	eDataDI4FilterTime,
	eDataDI5FilterTime,
	eDataDI6FilterTime,
	eDataDI7FilterTime,
	eDataDI8FilterTime,
	eDataDI9FilterTime,
	eDataDOInverseCtrl,
	eDataDO1Mode,
	eDataDI1PulseTime,
	eDataDO2Mode,
	eDataDO2PulseTime,
	eDataDO3Mode,
	eDataDO3PulseTime,
	eDataDO4Mode,
	eDataDO4PulseTime,
	eDataDO5Mode,
	eDataDO5PulseTime,
	eDataDO6Mode,
	eDataDO6PulseTime,
	eDataDO7Mode,
	eDataDO7PulseTime,
	eDataDO8Mode,
	eDataDO8PulseTime,
	eDataDO9Mode,
	eDataDO9PulseTime,
	
	eSaveSingleParamCmd,
	eSaveParamId,	
	eSaveAllParamCmd,	
	eSaveParamPercent,	
	eClearSysFaultCmd,

	eDataSPMax
};

#define LAST_SP_REG eDataLastSPCheck
#define NUM_SP 4086

enum eModbus50000REG {
  eDataDebugSimRegNone = -1,
};

#define LAST_DEBUG_SIM_REG eDataLastDebugSimCheck
#define NUM_DEBUG_SIM_REG 11

#endif      /* DB_ENUM_H */
