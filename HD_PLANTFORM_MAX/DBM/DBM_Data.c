/*
//######################################################################################################
//
// FILE:	DBM_Data.c
//
// TITLE: System database management.
//
// AUTHOR:	
//
//

//------------------------------------------------------------------------------------------------------
// Created by: 		
// Created date:   	
// Version:
// Descriptions: 	Frist Version
//
//------------------------------------------------------------------------------------------------------

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
#include <stdio.h>

#include "BSP_BasicDataType.h"		// Basic data type define.
#include "BSP_TgtCfg.h"				// Target config.
#define   DBM_DATA_GLOBAL_
#include "DBM_Data.h"

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
/* Actual value core reference table  */
const CoreRefDataDef AVCoreRefData[]={
//							Data Ptr,								Min,	Max,	StepValue,DefaultValue, Attrib, Format
//================================================================================================================================================
// Common data for platform
/*eDataProductCode */	{(UINT16*)&AVD.m_ProductCode,			0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F22},
/*eDataHWRev */		{(UINT16*)&AVD.m_HardwareVersion,		0,		0xFFFF,	1, 		100,	(eAttrib_IsFirst),	eFormat_F3},
/*eDataSWRev */		{(UINT16*)&AVD.m_FirmwareVersion,		0,		0xFFFF,	1, 		100,	(eAttrib_IsFirst),	eFormat_F3},
/*eDataBuildDate1 */		{(UINT16*)&AVD.m_BuildDate,				0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F22},
/*eDataBuildDate2 */		{(((UINT16*)&AVD.m_BuildDate)+1),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildDate3 */		{(((UINT16*)&AVD.m_BuildDate)+2),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildDate4 */		{(((UINT16*)&AVD.m_BuildDate)+3),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildDate5 */		{(((UINT16*)&AVD.m_BuildDate)+4),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildDate6 */		{(((UINT16*)&AVD.m_BuildDate)+5),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildTime1 */		{(UINT16*)&AVD.m_BuildTime,				0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F22},
/*eDataBuildTime2 */		{(((UINT16*)&AVD.m_BuildTime)+1),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildTime3 */		{(((UINT16*)&AVD.m_BuildTime)+2),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},
/*eDataBuildTime4 */		{(((UINT16*)&AVD.m_BuildTime)+3),		0,		1,		1,		0,		(eAttrib_None),	eFormat_F22},

/*eDataSystemTime_1 */	{(UINT16*)&AVD.m_SystemTime,			0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F22},
/*eDataSystemTime_2 */	{(((UINT16*)&AVD.m_SystemTime)+1),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_3 */	{(((UINT16*)&AVD.m_SystemTime)+2),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_4 */	{(((UINT16*)&AVD.m_SystemTime)+3),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_5 */	{(((UINT16*)&AVD.m_SystemTime)+4),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_6 */	{(((UINT16*)&AVD.m_SystemTime)+5),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_7 */	{(((UINT16*)&AVD.m_SystemTime)+6),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_8 */	{(((UINT16*)&AVD.m_SystemTime)+7),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_9 */	{(((UINT16*)&AVD.m_SystemTime)+8),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataSystemTime_10 */{(((UINT16*)&AVD.m_SystemTime)+9),		0,		1,		1,		0,		(eAttrib_None), eFormat_F22},

//================================================================================================================================================
/*eDataDIDirectStatus */	{(UINT16*)&AVD.m_DIDirectStatus,		0,		0xFFFF,	1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataDIUseStatus */		{(UINT16*)&AVD.m_DIUserStatus,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataDODirectStatus */	{(UINT16*)&AVD.m_DODirectStatus,		0,		0xFFFF,	1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataDOUseStatus */		{(UINT16*)&AVD.m_DOUserStatus,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst),	eFormat_F1},

/*eDataEncoderPosition1_1 */{(UINT16*)&AVD.m_EncoderPosition1,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataEncoderPosition1_2 */{(((UINT16*)&AVD.m_EncoderPosition1)+1),	0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition1_1 */	{(UINT16*)&AVD.m_UserPosition1,				0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition1_2 */	{(((UINT16*)&AVD.m_UserPosition1)+1),		0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataEncoderPosition2_1 */{(UINT16*)&AVD.m_EncoderPosition2,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataEncoderPosition2_2 */{(((UINT16*)&AVD.m_EncoderPosition2)+1),	0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition2_1 */	{(UINT16*)&AVD.m_UserPosition2,				0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition2_2 */	{(((UINT16*)&AVD.m_UserPosition2)+1),		0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataEncoderPosition3_1 */{(UINT16*)&AVD.m_EncoderPosition3,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataEncoderPosition3_2 */{(((UINT16*)&AVD.m_EncoderPosition3)+1),	0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition3_1 */	{(UINT16*)&AVD.m_UserPosition3,				0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataUserPosition3_2 */	{(((UINT16*)&AVD.m_UserPosition3)+1),		0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},

/*eDataInclinometerOnLine */	{(UINT16*)&AVD.m_InclinometerOnLine,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataCompassOnLine */	{(UINT16*)&AVD.m_CompassOnLine,				0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataXInclination_1 */		{(UINT16*)&AVD.m_XInclination,				0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataXInclination_2 */		{(((UINT16*)&AVD.m_XInclination)+1),		0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataYInclination_1 */		{(UINT16*)&AVD.m_YInclination,				0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataYInclination_2 */		{(((UINT16*)&AVD.m_YInclination)+1),		0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataGpsOnLine */			{(UINT16*)&AVD.m_GpsOnLine,				0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataLongitude_1 */		{(UINT16*)&AVD.m_Longitude,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataLongitude_2 */		{(((UINT16*)&AVD.m_Longitude)+1),		0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataLatitude_1 */			{(UINT16*)&AVD.m_Latitude, 				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataLatitude_2 */			{(((UINT16*)&AVD.m_Latitude)+1),		0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataCourse_1 */			{(UINT16*)&AVD.m_Course,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataCourse_2 */			{(((UINT16*)&AVD.m_Course)+1),			0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},

/*eDataGpsTime_1 */	{(UINT16*)&AVD.m_GpsTime,						0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F22},
/*eDataGpsTime_2 */	{(((UINT16*)&AVD.m_GpsTime)+1),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_3 */	{(((UINT16*)&AVD.m_GpsTime)+2),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_4 */	{(((UINT16*)&AVD.m_GpsTime)+3),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_5 */	{(((UINT16*)&AVD.m_GpsTime)+4),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_6 */	{(((UINT16*)&AVD.m_GpsTime)+5),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_7 */	{(((UINT16*)&AVD.m_GpsTime)+6),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_8 */	{(((UINT16*)&AVD.m_GpsTime)+7),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_9 */	{(((UINT16*)&AVD.m_GpsTime)+8),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},
/*eDataGpsTime_10 */	{(((UINT16*)&AVD.m_GpsTime)+9),				0,		1,		1,		0,		(eAttrib_None), eFormat_F22},

/*eDataSysWarning */	{(UINT16*)&AVD.m_SysWarning,		0,		0xFFFF,	1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eDataSysFault */		{(UINT16*)&AVD.m_SysFault,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F1},

//================================================================================================================================================
};

/* Set point core reference table  */
const CoreRefDataDef SPCoreRefData[]={
//							Data Ptr,									Min,		Max,	StepValue,DefaultValue, 		Attrib, 				Format
//================================================================================================================================================
// Common data for platform
/*eDataSerEnable1 */		{(UINT16*)&SPD.m_SerEnable1,			0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataSerSlaveId1 */		{(UINT16*)&SPD.m_SerSlaveAddress1,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataSerProtocol1 */		{(UINT16*)&SPD.m_SerProtocolType1,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F101},
/*eDataSerBandRate1 */		{(UINT16*)&SPD.m_SerBrandRate1,			0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F102},
/*eDataSerBitLen1 */			{(UINT16*)&SPD.m_SerBitLen1,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataSerStopBit1 */		{(UINT16*)&SPD.m_SerStopBit1,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataSerParity1 */			{(UINT16*)&SPD.m_SerParity1,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataSerEnable2 */		{(UINT16*)&SPD.m_SerEnable2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId2 */		{(UINT16*)&SPD.m_SerSlaveAddress2,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol2 */		{(UINT16*)&SPD.m_SerProtocolType1,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate2 */		{(UINT16*)&SPD.m_SerBrandRate2, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen2 */			{(UINT16*)&SPD.m_SerBitLen2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit2 */		{(UINT16*)&SPD.m_SerStopBit2,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity2 */			{(UINT16*)&SPD.m_SerParity2,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable3 */		{(UINT16*)&SPD.m_SerEnable3,			0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId3 */		{(UINT16*)&SPD.m_SerSlaveAddress3,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol3 */		{(UINT16*)&SPD.m_SerProtocolType3,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate3 */		{(UINT16*)&SPD.m_SerBrandRate3, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen3 */			{(UINT16*)&SPD.m_SerBitLen3,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit3 */		{(UINT16*)&SPD.m_SerStopBit3,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity3 */			{(UINT16*)&SPD.m_SerParity3,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable4 */		{(UINT16*)&SPD.m_SerEnable4,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId4 */		{(UINT16*)&SPD.m_SerSlaveAddress4,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol4 */		{(UINT16*)&SPD.m_SerProtocolType4,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate4 */		{(UINT16*)&SPD.m_SerBrandRate4, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen4 */			{(UINT16*)&SPD.m_SerBitLen4,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit4 */		{(UINT16*)&SPD.m_SerStopBit4,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity4 */			{(UINT16*)&SPD.m_SerParity4,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable5 */		{(UINT16*)&SPD.m_SerEnable5,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId5 */		{(UINT16*)&SPD.m_SerSlaveAddress5,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol5 */		{(UINT16*)&SPD.m_SerProtocolType5,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate5 */		{(UINT16*)&SPD.m_SerBrandRate5, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen5 */			{(UINT16*)&SPD.m_SerBitLen5,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit5 */		{(UINT16*)&SPD.m_SerStopBit5,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity5 */			{(UINT16*)&SPD.m_SerParity5,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable6 */		{(UINT16*)&SPD.m_SerEnable6,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId6 */		{(UINT16*)&SPD.m_SerSlaveAddress6,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol6 */		{(UINT16*)&SPD.m_SerProtocolType6,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate6 */		{(UINT16*)&SPD.m_SerBrandRate6, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen6 */			{(UINT16*)&SPD.m_SerBitLen6,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit6 */		{(UINT16*)&SPD.m_SerStopBit6,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity6 */			{(UINT16*)&SPD.m_SerParity6,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable7 */		{(UINT16*)&SPD.m_SerEnable7,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId7 */		{(UINT16*)&SPD.m_SerSlaveAddress7,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol7 */		{(UINT16*)&SPD.m_SerProtocolType7,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate7 */		{(UINT16*)&SPD.m_SerBrandRate7, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen7 */			{(UINT16*)&SPD.m_SerBitLen7,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit7 */		{(UINT16*)&SPD.m_SerStopBit7,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity7 */			{(UINT16*)&SPD.m_SerParity7,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerEnable8 */		{(UINT16*)&SPD.m_SerEnable8,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerSlaveId8 */		{(UINT16*)&SPD.m_SerSlaveAddress8,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerProtocol8 */		{(UINT16*)&SPD.m_SerProtocolType8,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F101},
/*eDataSerBandRate8 */		{(UINT16*)&SPD.m_SerBrandRate8, 		0,		4,		1,		4,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F102},
/*eDataSerBitLen8 */			{(UINT16*)&SPD.m_SerBitLen8,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerStopBit8 */		{(UINT16*)&SPD.m_SerStopBit8,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSerParity8 */			{(UINT16*)&SPD.m_SerParity8,			0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
//================================================================================================================================================
/*eDataCanEnable1 */		{(UINT16*)&SPD.m_CanEnable1,			0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataCanSlaveId1 */		{(UINT16*)&SPD.m_CanSlaveAddress1,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataCanProtocol1 */		{(UINT16*)&SPD.m_CanProtocolType1,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F103},
/*eDataCanBandRate1 */		{(UINT16*)&SPD.m_CanBrandRate1, 		0,		4,		1,		3,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F104},
/*eDataCanEnable2 */		{(UINT16*)&SPD.m_CanEnable2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataCanSlaveId2 */		{(UINT16*)&SPD.m_CanSlaveAddress2,		0,		255,	1,		254,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataCanProtocol2 */		{(UINT16*)&SPD.m_CanProtocolType2,		0,		255,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F103},
/*eDataCanBandRate2 */		{(UINT16*)&SPD.m_CanBrandRate2, 		0,		4,		1,		3,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F104},

//================================================================================================================================================
/*eAxisEnable1 */			{(UINT16*)&SPD.m_AxisEnable1,			0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataMechType1*/			{(UINT16*)&SPD.m_MechType1, 			0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotDirResvert1*/			{(UINT16*)&SPD.m_MotDirResvert1, 		0,		1, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataDrvCtrlType1*/		{(UINT16*)&SPD.m_DrvCtrlType1, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMasteDrvAddr1 */			{(UINT16*)&SPD.m_MasteDrvAddr1,			0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eSlaveDrvAddr1 */ 		{(UINT16*)&SPD.m_SlaveDrvAddr1, 		0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDualMotEn1 */			{(UINT16*)&SPD.m_DualMotEn1,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotEncodeLineNum1_1*/	{(UINT16*)&SPD.m_MotEncodeLineNum1, 	0,		0xFFFF, 1,		2000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotEncodeLineNum1_2*/	{(((UINT16*)&SPD.m_MotEncodeLineNum1)+1),0, 	0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioN1*/			{(UINT16*)&SPD.m_MotGearRatioN1, 		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioD1*/			{(UINT16*)&SPD.m_MotGearRatioD1,		0, 		0xFFFF, 1,		546,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio1_1*/		{(UINT16*)&SPD.m_MotUnitRatio1, 		0,		0xFFFF, 1,		36000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio1_2*/		{(((UINT16*)&SPD.m_MotUnitRatio1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMoveCtrlMode1*/			{(UINT16*)&SPD.m_MoveCtrlMode1, 		0,		3, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSpdCtrlAcclEn1*/ 	{(UINT16*)&SPD.m_SpdCtrlAcclEn1,		0,		1, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1MaxSpd1*/		{(UINT16*)&SPD.m_MaxSpd1,				0,		0xFFFF, 1,		1800,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1MinSpd1*/		{(UINT16*)&SPD.m_MinSpd1,				0,		0xFFFF, 1,		50, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1Accel1*/		{(UINT16*)&SPD.m_Accel1,				0,		0xFFFF, 1,		1000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1Decel1*/		{(UINT16*)&SPD.m_Decel1,				0,		0xFFFF, 1,		1000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataTrimDisLimt1*/		{(UINT16*)&SPD.m_TrimDisLimt1,			0,		0xFFFF, 1,		50, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataErrTol1*/			{(UINT16*)&SPD.m_ErrTol1,				0,		0xFFFF, 1,		0, 		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataWaitStopTime1*/		{(UINT16*)&SPD.m_WaitStopTime1, 		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataPosSource1*/ 		{(UINT16*)&SPD.m_PosSource1,			0,		2, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataUserOrgPoint1_1*/	{(UINT16*)&SPD.m_UserOrgPoint1, 		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataUserOrgPoint1_2*/	{(((UINT16*)&SPD.m_UserOrgPoint1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangeDis1_1*/	{(UINT16*)&SPD.m_WholeRangeDis1,		0,		0xFFFF, 1,		36000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangeDis1_2*/	{(((UINT16*)&SPD.m_WholeRangeDis1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangePulNum1_1*/{(UINT16*)&SPD.m_WholeRangePulNum1, 	0,		0xFFFF, 1,		60000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangePulNum1_2*/{(((UINT16*)&SPD.m_WholeRangePulNum1)+1),0, 	0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*ePidKp1*/					{(UINT16*)&SPD.m_PidKp1,				0,		0xFFFF, 1,		1000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKi1*/ 					{(UINT16*)&SPD.m_PidKi1,				0,		0xFFFF, 1,		50,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKd1*/ 				{(UINT16*)&SPD.m_PidKd1,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eIntegralEnRng1*/			{(UINT16*)&SPD.m_IntegralEnRng1, 		0,		0xFFFF, 1,		200,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eNegLimitPort1*/			{(UINT16*)&SPD.m_NegLimitPort1, 		0,		9, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePosLimitPort1*/			{(UINT16*)&SPD.m_PosLimitPort1, 		0,		9, 		1,		2,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOriginalPointPort1*/		{(UINT16*)&SPD.m_OriginalPointPort1, 	0,		9, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDrvFaultPort1*/ 			{(UINT16*)&SPD.m_DrvFaultPort1,			0,		9,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eNegLimitVal1_1*/			{(UINT16*)&SPD.m_NegLimitVal1, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eNegLimitVal1_2*/			{(((UINT16*)&SPD.m_NegLimitVal1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal1_1*/ 			{(UINT16*)&SPD.m_PosLimitVal1,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal1_2*/ 			{(((UINT16*)&SPD.m_PosLimitVal1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eEnableStartUpFindOrig1*/	{(UINT16*)&SPD.m_EnableStartUpFindOrig1,0,		1, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigSpd1*/			{(UINT16*)&SPD.m_FindOrigSpd1,			0,		0xFFFF, 1,		500,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigMode1*/			{(UINT16*)&SPD.m_FindOrigMode1, 		0,		2, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOrigOffSet1_1*/ 			{(UINT16*)&SPD.m_OrigOffSet1,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eOrigOffSet1_2*/ 			{(((UINT16*)&SPD.m_OrigOffSet1)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eMoveStartCmd1 */ 		{(UINT16*)&SPD.m_MoveStartCmd1, 		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveStopCmd1 */		{(UINT16*)&SPD.m_MoveStopCmd1,			0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveEmgStopCmd1 */		{(UINT16*)&SPD.m_MoveEmgStopCmd1,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eCleanDrvFaultCmd1 */ 	{(UINT16*)&SPD.m_ClearDrvFaultCmd1, 	0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveSpdSetPoint1_1*/ 	{(UINT16*)&SPD.m_MoveSpdSetPoint1,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMoveSpdSetPoint1_2*/ 	{(((UINT16*)&SPD.m_MoveSpdSetPoint1)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},
/*eMovePosSetPoint1_1*/ 	{(UINT16*)&SPD.m_MovePosSetPoint1,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMovePosSetPoint1_2*/ 	{(((UINT16*)&SPD.m_MovePosSetPoint1)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},

/*eAxisEnable2 */			{(UINT16*)&SPD.m_AxisEnable2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataMechType2*/			{(UINT16*)&SPD.m_MechType2, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotDirResvert2*/ 		{(UINT16*)&SPD.m_MotDirResvert2,		0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataDrvCtrlType2*/		{(UINT16*)&SPD.m_DrvCtrlType2, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMasteDrvAddr2 */ 		{(UINT16*)&SPD.m_MasteDrvAddr2, 		0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eSlaveDrvAddr2 */ 		{(UINT16*)&SPD.m_SlaveDrvAddr2, 		0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDualMotEn2 */			{(UINT16*)&SPD.m_DualMotEn2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotEncodeLineNum2_1*/	{(UINT16*)&SPD.m_MotEncodeLineNum2, 	0,		0xFFFF, 1,		2000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotEncodeLineNum2_2*/	{(((UINT16*)&SPD.m_MotEncodeLineNum2)+1),0, 	0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioN2*/ 		{(UINT16*)&SPD.m_MotGearRatioN2,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioD2*/ 		{(UINT16*)&SPD.m_MotGearRatioD2,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio2_1*/		{(UINT16*)&SPD.m_MotUnitRatio2, 		0,		0xFFFF, 1,		200,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio2_2*/		{(((UINT16*)&SPD.m_MotUnitRatio2)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMoveCtrlMode2*/			{(UINT16*)&SPD.m_MoveCtrlMode2, 		0,		3, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSpdCtrlAcclEn2*/ 	{(UINT16*)&SPD.m_SpdCtrlAcclEn2,		0,		1, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1MaxSpd2*/		{(UINT16*)&SPD.m_MaxSpd2,				0,		0xFFFF, 1,		1000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1MinSpd2*/		{(UINT16*)&SPD.m_MinSpd2,				0,		0xFFFF, 1,		50, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1Accel2*/		{(UINT16*)&SPD.m_Accel2,				0,		0xFFFF, 1,		100,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataAxis1Decel2*/		{(UINT16*)&SPD.m_Decel2,				0,		0xFFFF, 1,		100,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataTrimDisLimt2*/		{(UINT16*)&SPD.m_TrimDisLimt2,			0,		0xFFFF, 1,		50, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataErrTol2*/			{(UINT16*)&SPD.m_ErrTol2,				0,		0xFFFF, 1,		10, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataWaitStopTime2*/		{(UINT16*)&SPD.m_WaitStopTime2, 		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataPosSource2*/ 		{(UINT16*)&SPD.m_PosSource2,			0,		2, 		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataUserOrgPoint2_1*/	{(UINT16*)&SPD.m_UserOrgPoint2, 		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataUserOrgPoint2_2*/	{(((UINT16*)&SPD.m_UserOrgPoint2)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangeDis2_1*/	{(UINT16*)&SPD.m_WholeRangeDis2,		0,		0xFFFF, 1,		36000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangeDis2_2*/	{(((UINT16*)&SPD.m_WholeRangeDis2)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangePulNum2_1*/{(UINT16*)&SPD.m_WholeRangePulNum2, 	0,		0xFFFF, 1,		60000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eDataWholeRangePulNum2_2*/{(((UINT16*)&SPD.m_WholeRangePulNum2)+1),0, 	0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*ePidKp2*/ 				{(UINT16*)&SPD.m_PidKp2,				0,		0xFFFF, 1,		500,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKi2*/ 					{(UINT16*)&SPD.m_PidKi2,				0,		0xFFFF, 1,		50, (eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKd2*/ 				{(UINT16*)&SPD.m_PidKd2,				0,		0xFFFF, 1,		0,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eIntegralEnRng2*/			{(UINT16*)&SPD.m_IntegralEnRng2,		0,		0xFFFF, 1,		200,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eNegLimitPort2*/ 			{(UINT16*)&SPD.m_NegLimitPort2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePosLimitPort2*/		{(UINT16*)&SPD.m_PosLimitPort2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOriginalPointPort2*/ 		{(UINT16*)&SPD.m_OriginalPointPort2,	0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDrvFaultPort2*/			{(UINT16*)&SPD.m_DrvFaultPort2, 		0,		9,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eNegLimitVal2_1*/ 		{(UINT16*)&SPD.m_NegLimitVal2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eNegLimitVal2_2*/ 		{(((UINT16*)&SPD.m_NegLimitVal2)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal2_1*/ 			{(UINT16*)&SPD.m_PosLimitVal2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal2_2*/ 			{(((UINT16*)&SPD.m_PosLimitVal2)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eEnableStartUpFindOrig2*/ {(UINT16*)&SPD.m_EnableStartUpFindOrig2,0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigSpd2*/			{(UINT16*)&SPD.m_FindOrigSpd2,			0,		0xFFFF, 1,		500,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigMode2*/			{(UINT16*)&SPD.m_FindOrigMode2, 		0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOrigOffSet2_1*/			{(UINT16*)&SPD.m_OrigOffSet2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eOrigOffSet2_2*/			{(((UINT16*)&SPD.m_OrigOffSet2)+1), 	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eMoveStartCmd2 */ 		{(UINT16*)&SPD.m_MoveStartCmd2, 		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveStopCmd2 */		{(UINT16*)&SPD.m_MoveStopCmd2,			0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveEmgStopCmd2 */		{(UINT16*)&SPD.m_MoveEmgStopCmd2,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eCleanDrvFaultCmd2 */ 	{(UINT16*)&SPD.m_ClearDrvFaultCmd2, 	0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveSpdSetPoint2_1*/ 	{(UINT16*)&SPD.m_MoveSpdSetPoint2,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMoveSpdSetPoint2_2*/ 	{(((UINT16*)&SPD.m_MoveSpdSetPoint2)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},
/*eMovePosSetPoint2_1*/ 	{(UINT16*)&SPD.m_MovePosSetPoint2,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMovePosSetPoint2_2*/ 	{(((UINT16*)&SPD.m_MovePosSetPoint2)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},

/*eAxisEnable3 */			{(UINT16*)&SPD.m_AxisEnable3,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataMechType3*/			{(UINT16*)&SPD.m_MechType3, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotDirResvert3*/ 		{(UINT16*)&SPD.m_MotDirResvert3,		0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataDrvCtrlType3*/		{(UINT16*)&SPD.m_DrvCtrlType3, 			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMasteDrvAddr3 */ 		{(UINT16*)&SPD.m_MasteDrvAddr3, 		0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eSlaveDrvAddr3 */ 		{(UINT16*)&SPD.m_SlaveDrvAddr3, 		0,		255,	1,		127,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDualMotEn3 */			{(UINT16*)&SPD.m_DualMotEn3,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eMotEncodeLineNum3_1*/	{(UINT16*)&SPD.m_MotEncodeLineNum3, 	0,		0xFFFF, 1,		2000,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotEncodeLineNum3_2*/	{(((UINT16*)&SPD.m_MotEncodeLineNum3)+1),0, 	0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioN3*/ 		{(UINT16*)&SPD.m_MotGearRatioN3,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotGearRatioD3*/ 		{(UINT16*)&SPD.m_MotGearRatioD3,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio3_1*/		{(UINT16*)&SPD.m_MotUnitRatio3, 		0,		0xFFFF, 1,		200,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eMotUnitRatio3_2*/		{(((UINT16*)&SPD.m_MotUnitRatio3)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F13},
/*eMoveCtrlMode3*/			{(UINT16*)&SPD.m_MoveCtrlMode3, 		0,		3, 		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDataSpdCtrlAcclEn3*/		{(UINT16*)&SPD.m_SpdCtrlAcclEn3,		0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataAxis1MaxSpd3*/		{(UINT16*)&SPD.m_MaxSpd3,				0,		0xFFFF,	1,		1000,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataAxis1MinSpd3*/		{(UINT16*)&SPD.m_MinSpd3,				0,		0xFFFF,	1,		50,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataAxis1Accel3*/		{(UINT16*)&SPD.m_Accel3,				0,		0xFFFF,	1,		100,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataAxis1Decel3*/		{(UINT16*)&SPD.m_Decel3,				0,		0xFFFF,	1,		100,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataTrimDisLimt3*/		{(UINT16*)&SPD.m_TrimDisLimt3,			0,		0xFFFF,	1,		50,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataErrTol3*/			{(UINT16*)&SPD.m_ErrTol3,				0,		0xFFFF,	1,		10,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataWaitStopTime3*/		{(UINT16*)&SPD.m_WaitStopTime3,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataPosSource3*/		{(UINT16*)&SPD.m_PosSource3,			0,		2,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataUserOrgPoint3_1*/	{(UINT16*)&SPD.m_UserOrgPoint3,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataUserOrgPoint3_2*/	{(((UINT16*)&SPD.m_UserOrgPoint3)+1),	0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataWholeRangeDis3_1*/	{(UINT16*)&SPD.m_WholeRangeDis3,		0,		0xFFFF,	1,		36000,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataWholeRangeDis3_2*/	{(((UINT16*)&SPD.m_WholeRangeDis3)+1),	0,		0xFFFF,	1,		0,		(eAttrib_FRAM),	eFormat_F13},
/*eDataWholeRangePulNum3_1*/{(UINT16*)&SPD.m_WholeRangePulNum3,	0,		0xFFFF,	1,		60000,	(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F13},
/*eDataWholeRangePulNum3_2*/{(((UINT16*)&SPD.m_WholeRangePulNum3)+1),0,		0xFFFF,	1,		0,	(eAttrib_FRAM),	eFormat_F13},
/*ePidKp3*/ 				{(UINT16*)&SPD.m_PidKp3,				0,		0xFFFF, 1,		500,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKi3*/ 					{(UINT16*)&SPD.m_PidKi3,				0,		0xFFFF, 1,		50, 	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*ePidKd3*/ 				{(UINT16*)&SPD.m_PidKd3,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eIntegralEnRng3*/			{(UINT16*)&SPD.m_IntegralEnRng3,		0,		0xFFFF, 1,		200,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F13},
/*eLeftLimitPort3*/ 			{(UINT16*)&SPD.m_NegLimitPort3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eRightyLimitPort3*/		{(UINT16*)&SPD.m_PosLimitPort3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOriginalPointPort3*/ 		{(UINT16*)&SPD.m_OriginalPointPort3,	0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eDrvFaultPort3*/			{(UINT16*)&SPD.m_DrvFaultPort3, 		0,		9,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eNegLimitVal3_1*/ 		{(UINT16*)&SPD.m_NegLimitVal3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eNegLimitVal3_2*/ 		{(((UINT16*)&SPD.m_NegLimitVal3)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal3_1*/ 			{(UINT16*)&SPD.m_PosLimitVal3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*ePosLimitVal3_2*/ 			{(((UINT16*)&SPD.m_PosLimitVal3)+1),	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eEnableStartUpFindOrig3*/ {(UINT16*)&SPD.m_EnableStartUpFindOrig3,0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigSpd3*/			{(UINT16*)&SPD.m_FindOrigSpd3,			0,		0xFFFF, 1,		500,	(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eFindOrigMode3*/			{(UINT16*)&SPD.m_FindOrigMode3, 		0,		2,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eOrigOffSet3_1*/			{(UINT16*)&SPD.m_OrigOffSet3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F17},
/*eOrigOffSet3_2*/			{(((UINT16*)&SPD.m_OrigOffSet3)+1), 	0,		0xFFFF, 1,		0,		(eAttrib_FRAM), eFormat_F17},
/*eMoveStartCmd3 */ 		{(UINT16*)&SPD.m_MoveStartCmd3, 		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveStopCmd3 */		{(UINT16*)&SPD.m_MoveStopCmd3,			0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveEmgStopCmd3 */		{(UINT16*)&SPD.m_MoveEmgStopCmd3,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eCleanDrvFaultCmd3 */		{(UINT16*)&SPD.m_ClearDrvFaultCmd3,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eMoveSpdSetPoint3_1*/ 	{(UINT16*)&SPD.m_MoveSpdSetPoint3,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMoveSpdSetPoint3_2*/ 	{(((UINT16*)&SPD.m_MoveSpdSetPoint3)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},
/*eMovePosSetPoint3_1*/ 	{(UINT16*)&SPD.m_MovePosSetPoint3,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F13},
/*eMovePosSetPoint3_2*/ 	{(((UINT16*)&SPD.m_MovePosSetPoint3)+1),0,		0xFFFF, 1,		0,		0, 	eFormat_F13},

//================================================================================================================================================
/*eECDEnable1 */			{(UINT16*)&SPD.m_ECDEnable1,			0,		1,		1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDType1*/				{(UINT16*)&SPD.m_ECDType1,				0,		0xFFFF, 1,		0, 		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSingleTurnBitNum1*/	{(UINT16*)&SPD.m_ECDSingleTurnBitNum1, 	0,		0xFFFF, 1,		17,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDMultiTurnBitNum1*/	{(UINT16*)&SPD.m_ECDMultiTurnBitNum1,	0,		0xFFFF, 1,		12,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDCodeMode1*/			{(UINT16*)&SPD.m_ECDCodeMode1,			0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioN1*/		{(UINT16*)&SPD.m_ECDGearRatioN1,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioD1*/ 		{(UINT16*)&SPD.m_ECDGearRatioD1,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSignEn1*/ 			{(UINT16*)&SPD.m_ECDSignEn1,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},

/*eECDEnable2*/			{(UINT16*)&SPD.m_ECDEnable2,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDType2*/				{(UINT16*)&SPD.m_ECDType2,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSingleTurnBitNum2*/	{(UINT16*)&SPD.m_ECDSingleTurnBitNum2, 	0,		0xFFFF, 1,		14,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDMultiTurnBitNum2*/	{(UINT16*)&SPD.m_ECDMultiTurnBitNum2,	0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDCodeMode2*/			{(UINT16*)&SPD.m_ECDCodeMode2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioN2*/ 		{(UINT16*)&SPD.m_ECDGearRatioN2,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioD2*/ 		{(UINT16*)&SPD.m_ECDGearRatioD2,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSignEn2*/ 			{(UINT16*)&SPD.m_ECDSignEn2,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},

/*eECDEnable3*/			{(UINT16*)&SPD.m_ECDEnable3,			0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDType3*/				{(UINT16*)&SPD.m_ECDType3,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSingleTurnBitNum3*/	{(UINT16*)&SPD.m_ECDSingleTurnBitNum3, 	0,		0xFFFF, 1,		14,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDMultiTurnBitNum3*/	{(UINT16*)&SPD.m_ECDMultiTurnBitNum3,	0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDCodeMode3*/			{(UINT16*)&SPD.m_ECDCodeMode3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioN3*/ 		{(UINT16*)&SPD.m_ECDGearRatioN3,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDGearRatioD3*/ 		{(UINT16*)&SPD.m_ECDGearRatioD3,		0,		0xFFFF, 1,		1,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eECDSignEn3*/ 			{(UINT16*)&SPD.m_ECDSignEn3,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},

/*eInclinometerMode*/ 		{(UINT16*)&SPD.m_InclinometerMode,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*e3DCompassMode*/		{(UINT16*)&SPD.m_3DCompassMode,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eLevelCompEn*/			{(UINT16*)&SPD.m_LevelCompEn,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eGpsMode*/				{(UINT16*)&SPD.m_GpsMode,				0,		0xFFFF, 1,		0,		(eAttrib_IsFirst|eAttrib_FRAM), eFormat_F1},
/*eSyncTimeWithGpsCmd*/	{(UINT16*)&SPD.m_SyncTimeWithGpsCmd,	0,		0xFFFF, 1,		0,		(eAttrib_IsFirst), eFormat_F1},
//================================================================================================================================================
/*eDataDIInverseCtrl */		{(UINT16*)&SPD.m_DIInverseCtrl,			0,		0xFFFF,	1,		3,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI1FilterTime */		{(UINT16*)&SPD.m_DI1FilterTime,			0,		0xFFFF,	1,		10,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI2FilterTime */		{(UINT16*)&SPD.m_DI2FilterTime,			0,		0xFFFF,	1,		10,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI3FilterTime */		{(UINT16*)&SPD.m_DI3FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI4FilterTime */		{(UINT16*)&SPD.m_DI4FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI5FilterTime */		{(UINT16*)&SPD.m_DI5FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI6FilterTime */		{(UINT16*)&SPD.m_DI6FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI7FilterTime */		{(UINT16*)&SPD.m_DI7FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI8FilterTime */		{(UINT16*)&SPD.m_DI8FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDI9FilterTime */		{(UINT16*)&SPD.m_DI9FilterTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDOInverseCtrl */		{(UINT16*)&SPD.m_DOInverseCtrl,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO1Mode */			{(UINT16*)&SPD.m_DO1Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO1PulseTime */		{(UINT16*)&SPD.m_DO1PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO2Mode */			{(UINT16*)&SPD.m_DO2Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO2PulseTime */		{(UINT16*)&SPD.m_DO2PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO3Mode */			{(UINT16*)&SPD.m_DO3Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO3PulseTime */		{(UINT16*)&SPD.m_DO3PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO4Mode */			{(UINT16*)&SPD.m_DO4Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO4PulseTime */		{(UINT16*)&SPD.m_DO4PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO5Mode */			{(UINT16*)&SPD.m_DO5Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO5PulseTime */		{(UINT16*)&SPD.m_DO5PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO6Mode */			{(UINT16*)&SPD.m_DO6Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO6PulseTime */		{(UINT16*)&SPD.m_DO6PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO7Mode */			{(UINT16*)&SPD.m_DO7Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO7PulseTime */		{(UINT16*)&SPD.m_DO7PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO8Mode */			{(UINT16*)&SPD.m_DO8Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO8PulseTime */		{(UINT16*)&SPD.m_DO8PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO9Mode */			{(UINT16*)&SPD.m_DO9Mode,				0,		1,		1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
/*eDataDO9PulseTime */		{(UINT16*)&SPD.m_DO9PulseTime,			0,		0xFFFF,	1,		0,		(eAttrib_IsFirst|eAttrib_FRAM),	eFormat_F1},
//================================================================================================================================================
/*eSaveSingleParamCmd */	{(UINT16*)&SPD.m_SaveSingleParamCmd,	0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eSaveParamId */			{(UINT16*)&SPD.m_SaveParamId,			0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eSaveAllParamCmd */		{(UINT16*)&SPD.m_SaveAllParamCmd,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eSaveParamPercent */		{(UINT16*)&SPD.m_SaveParamPercent,		0,		0xFFFF, 1,		0,		(eAttrib_IsFirst),	eFormat_F1},
/*eClearSysFaultCmd */		{(UINT16*)&SPD.m_ClearSysFaultCmd,		0,		1,		1,		0,		(eAttrib_IsFirst),	eFormat_F1},

};


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
* Description	: Get AV data Point
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID *GetAVDataPoint(UINT16 index)
{
	return (VOID *)AVCoreRefData[index].m_DataPtr;
}

/*
**************************************************************************
* Description	: Get AV default value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetAVDefaultValue(UINT16 index)
{
	return AVCoreRefData[index].m_Default;
}

/*
**************************************************************************
* Description	: Get AV Max value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetAVMaxValue(UINT16 index)
{
	return AVCoreRefData[index].m_Max;
}

/*
**************************************************************************
* Description	: Get AV Min value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetAVMinValue(UINT16 index)
{
	return AVCoreRefData[index].m_Min;
}

/*
**************************************************************************
* Description	: Get AV Step value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetAVStepValue(UINT16 index)
{
	return AVCoreRefData[index].m_StepValue;
}

/*
**************************************************************************
* Description	: Get SP data Point
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
VOID *GetSPDataPoint(UINT16 index)
{
	return (VOID *)SPCoreRefData[index].m_DataPtr;
}

/*
**************************************************************************
* Description	: Get SP default value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetSPDefaultValue(UINT16 index)
{
	return SPCoreRefData[index].m_Default;
}

/*
**************************************************************************
* Description	: Get SP Max value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetSPMaxValue(UINT16 index)
{
	return SPCoreRefData[index].m_Max;
}

/*
**************************************************************************
* Description	: Get AV Min value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetSPMinValue(UINT16 index)
{
	return SPCoreRefData[index].m_Min;
}

/*
**************************************************************************
* Description	: Get AV Step value
* Author	  	: Chris_Shang
* Date			: Aug 15, 2015
**************************************************************************
*/
UINT16 GetSPStepValue(UINT16 index)
{
	return SPCoreRefData[index].m_StepValue;
}

/*
**************************************************************************
* Description	: Get Sp Address
* Author	  	: Chris_Shang
* Date			: Jun 20, 2018
**************************************************************************
*/
VOID* GetSPAdd(UINT16 index)
{
	if(index < eDataSPMax)
	{
		return SPCoreRefData[index].m_DataPtr;
	}
	else
	{
		return NULL;
	}
}

/*
**************************************************************************
* Description	: Get Setpoint size
* Author	  	: Chris_Shang
* Date			: Jun 6, 2018
**************************************************************************
*/
UINT8 GetSPSize(UINT16 index)
{
	UINT8 size;
	size = 2;
	index ++;
	while((index != eDataSPMax))
	{
		if(0 == (SPCoreRefData[index].m_Attrib & eAttrib_IsFirst))
			size += 2;
		else
			break;

		index ++;
	}
	return size;
}

/*
**************************************************************************
* Description	: Is 
* Author	  	: Chris_Shang
* Date			: Jun 6, 2018
**************************************************************************
*/
BOOL GetNonvolatileAttrib(UINT16 index)
{
	if(0 != (SPCoreRefData[index].m_Attrib & eAttrib_FRAM))
		return TRUE;
	else
		return FALSE;
}

/*
**************************************************************************
* Description	: Initialization global data
  	1) Input argument  :
 	2) Modify argument :
 	3) Output argument :
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 10 2014
**************************************************************************
*/
VOID InitGlobalData(VOID)
{
	UINT16 cnt = 0;
	/* Load AV default value */
	for(cnt = 0; cnt < eDataAVMax; cnt ++)
	{
		*(UINT16* )GetAVDataPoint(cnt) = (UINT16)GetAVDefaultValue(cnt);
	}

	/* Load sp default value */
	for(cnt = 0; cnt < eDataSPMax; cnt ++)
	{
		*(UINT16* )GetSPDataPoint(cnt) = (UINT16)GetSPDefaultValue(cnt);
	}

	/* Get build time */
	sprintf((char * )AVD.m_BuildDate,"%s",__DATE__);	// Get Build date
	sprintf((char * )AVD.m_BuildTime,"%s",__TIME__);	// Get Build date

}

/*
**************************************************************************
* Description	: Recover data value to default for volatile data 
  	1) Input argument  :
 	2) Modify argument :
 	3) Output argument :
* Return      	:
* Notes       	:
* Author	  	: Chris_Shang
* Date			: Nov, 10 2014
**************************************************************************
*/
VOID RecoverVolatileData(VOID)
{
	UINT16 cnt = 0;


	/* Load sp default value */
	for(cnt = 0; cnt < eDataSPMax; cnt ++)
	{
		if(FALSE == GetNonvolatileAttrib(cnt))
			*(UINT16* )GetSPDataPoint(cnt) = (UINT16)GetSPDefaultValue(cnt);
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


