
#include <stm32f4xx.h>
#include <string.h>
#include "includes.h"

#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "UTL_USART.h"
//#include "UTL_OCTimer.h"
#include "UTL_EEPROM.h"
//#include "APP_Control.h"
#include "SYS_Application.h"
#include "COM_GPS.h"

//GPS数据示例:$GPHPD, GPSWeek, GPSTime, Heading, Pitch, Track, Latitude, Longitude, Altitude, Ve , Vn, Vu , Baseline, NSV1, NSV2*cs<CR><LF>
/*$GPHPD,			协议帧头
1451,				当前的星期数
368123.30,			星期内的毫秒数
90.01,				偏航角
0.132,				俯仰角
90.11,				真北夹角
34.1966004,			经度
108.8551924,		纬度
394.98,				高度
-0.157,				东向速度
0.019,				北向速度
-0.345,				天向速度
3.898,				基线长度
6,					前天线星
6,					后天线星
2					定位定向
*6A				

*/

#define GPS_SENS_FRAME_LEN_MIN 32
const char GPS_SET_SEND[32]="$cmd,output,com1,GPHPD,1.0*ff\r\n";


void GPS_Config(SERIAL_PORT port)
{
	USARTSendnByte((SERIAL_PORT)port, (UCHAR*)&GPS_SET_SEND[port], 32);
		
}

/*
**************************************************************************
* Description	: Process serial data frame
	1) Input argument  : pQueue - Point of queue that wait for process.
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
**************************************************************************
*/
BOOL GPS_ReceiverDispose(SERIAL_PORT port, UINT8 slvAdd)
{
//	CHAR buffer[64];
	GPS_SENS_FRAME	GPS_DATA;
	BOOL ret = FALSE;
	USART_COM_HANDLE* pUartHandle;
	USART_BUF* pRxBuf;
	UINT16 cnt;
	UINT16 readLen;
	UINT8 *pFrame;
	UINT8 x=0;
	static unsigned char section_num=0,byte_num=0;//,cher=0;
	static unsigned char GPSWeek[8], Heading[8],Pitch[8],Altitude[8],Latitude[16],Longitude[16],NSV1[3],NSV2[3];
	/* Get uart handle */
	pUartHandle = GetUSARTPortHandler(port);
	/* Create the semaphore used by frame notify */
//	sprintf((char *)buffer, "ComPortSem%d", port);
//	pSem = &pUartHandle->m_Sem;
//	*pSem = xEventGroupCreate();

	/* Get start address of buffers */
	pRxBuf = &pUartHandle->m_Buf.RXBuffer;
	x = pRxBuf->m_buffer[cnt++];					
	if(x=='$')
	 {
		 section_num=0;
		 byte_num=0;
		 GPSWeek[0]='\0';
		 Heading[0]='\0';
		 Pitch[0]='\0';
		 Altitude[0]='\0';
		 Latitude[0]='\0';
		 Longitude[0]='\0';
		 
	 }else if( x==',')
	 {
		 ++section_num;
		 byte_num=0;
	 }
	 if(section_num==1)
	 {
		 GPSWeek[byte_num++]=x;
		 GPSWeek[byte_num]='\0';			 
	 }
	 if(section_num==3)
	 {
		 Heading[byte_num++]=x;
		 Heading[byte_num]='\0';		 
	 }else if(section_num==4)
	 {
		 Pitch[byte_num++]=x;
		 Pitch[byte_num]='\0';		 
	 }else if(section_num==6)
	 {
		 Latitude[byte_num++]=x;
		 Latitude[byte_num]='\0';		 
	 }else if(section_num==7)
	 {
		 Longitude[byte_num++]=x;
		 Longitude[byte_num]='\0';		 
	 }else if(section_num==8)
	 {
		 Altitude[byte_num++]=x;
		 Altitude[byte_num]='\0';		 
	 }else if(section_num==13)
	 {
		 NSV1[byte_num++]=x;
		 NSV1[byte_num]='\0';		 
	 }else if(section_num==14)
	 {
		 NSV2[byte_num++]=x;
		 NSV2[byte_num]='\0';		 
	 }
	 if(x=='*')
	 {
		 GPS_DATA.Gpsweek	= atof((const char *)&GPSWeek[1]);
		 GPS_DATA.Heading	= atof((const char *)&Heading[1]);
		 GPS_DATA.Pitch 	= atof((const char *)&Pitch[1]);
		 GPS_DATA.Altitude  = atof((const char *)&Altitude[1]);
		 GPS_DATA.Latitude  = atof((const char *)&Latitude[1]);
		 GPS_DATA.Longitude   = atof((const char *)&Longitude[1]);  //经度
		 GPS_DATA.NSV1        = atoi((const char *)&NSV1[1]);
		 GPS_DATA.NSV2        = atoi((const char *)&NSV2[1]);  
	}
		return ret;
	

}
/**************************************************************************
*/
BOOL ProceGpsAnsFrame(GPS_SENS_FRAME* pAns, UINT8 len, UINT8 slvAdd)
{

	UINT8 cnt;
	UINT8* pFrameData;
	UINT8 chkSum = 0;

#if(JZ_SENS_ENABLE_CHECK_SUM)
	/* calc check sum */
	for(cnt = 0; cnt < (len - 1); cnt ++)
	{
		chkSum += *(((UINT8*)pAns) + cnt);
	}
	/* Check sum validation */
	if(chkSum != *(((UINT8*)pAns) + cnt))
	{
		return FALSE;
	}
#endif

	/* Check slave address. */
#if(JZ_SENS_CHK_ADDR_EN) 
	if((slvAdd != pAns->m_Addr) && (JZ_SENS_BROADCAST_ADDR != pAns->m_Addr))
	{
		return FALSE;
	}
#endif
#if 0
	for(cnt = 0; cnt < JZ_SENS_PORT_MAX; cnt ++)
	{
		if(JZSensorCtrlTable[cnt].m_Addr == pAns->m_Addr)
		{
			break;
		}
	}

	if(cnt == JZ_SENS_PORT_MAX)
	{
		return FALSE;
	}
#endif

		/* Get data */
		U8_TO_F32 dataTemp;
		/* Get x inclination */

	

}

