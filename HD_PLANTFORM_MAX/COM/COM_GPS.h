

#include <stm32f4xx.h>
#include "UTL_USART.h"


typedef struct taGPS_SENS_FRAME
{
	float Gpsweek;
	float Heading;   //航向角
	float Pitch;     //俯仰角
	float Altitude;	 //高度
	float Latitude;   //纬度
	float Longitude;  //经度
	unsigned char NSV1;  //前天线卫星数
	unsigned char NSV2;  //后天线卫星数
}GPS_SENS_FRAME;

extern GPS_SENS_FRAME g_GPSSensAnsFrame[SERIAL_PORT_MAX];

BOOL ProceGpsAnsFrame(GPS_SENS_FRAME* pAns, UINT8 len, UINT8 slvAdd);

void GPS_Config(SERIAL_PORT port);