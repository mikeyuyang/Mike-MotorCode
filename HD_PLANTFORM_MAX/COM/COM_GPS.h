

#include <stm32f4xx.h>
#include "UTL_USART.h"


typedef struct taGPS_SENS_FRAME
{
	float Gpsweek;
	float Heading;   //�����
	float Pitch;     //������
	float Altitude;	 //�߶�
	float Latitude;   //γ��
	float Longitude;  //����
	unsigned char NSV1;  //ǰ����������
	unsigned char NSV2;  //������������
}GPS_SENS_FRAME;

extern GPS_SENS_FRAME g_GPSSensAnsFrame[SERIAL_PORT_MAX];

BOOL ProceGpsAnsFrame(GPS_SENS_FRAME* pAns, UINT8 len, UINT8 slvAdd);

void GPS_Config(SERIAL_PORT port);