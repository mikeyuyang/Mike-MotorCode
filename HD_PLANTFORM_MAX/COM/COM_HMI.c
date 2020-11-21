#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>
#include "COM_HMI.h"
#include "UTL_CRC.h"
#include "UTL_USART.h"
#include "DBM_Data.h"
#include "UTL_EEPROM.h"
#include "MOV_Control.h"
#include "BSP_BasicDataType.h"	


VOID HMIProtocol1Loop(SERIAL_PORT port, UINT8 slvAdd)
{
	USART_COM_HANDLE* pUartHandle;
	USART_BUF* pRxBuf;
	
	
	pUartHandle = GetUSARTPortHandler(port);
	pRxBuf = &pUartHandle->m_Buf.RXBuffer;	
	if((pRxBuf->m_buffer[pRxBuf->m_count-2]==0xFF)&&((pRxBuf->m_buffer[pRxBuf->m_count-1]==0xFF))) 
	{
  ProcessMessage((PCTRL_MSG)pRxBuf, pRxBuf->m_count);
	pRxBuf->m_buffer[pRxBuf->m_count-1]=0;
	pRxBuf->m_buffer[pRxBuf->m_count-2]=0;
	pRxBuf->m_count=0;
	}
}
void ProcessMessage( PCTRL_MSG msg, u16 size )
{
	u8 cmd_type = msg->cmd_type;                   //ָ������
	u8 control_type = msg->control_type;           //�ؼ�����
	u16 screen_id = PTR2U16(&msg->screen_id);       //����ID
	u16 control_id = PTR2U16(&msg->control_id);     //�ؼ�ID
	u32 value=0;
	  if((control_id==0x23)||(control_id==0x24)||(control_id==0x25))
		{
				if(msg->param[0]==0x2d)
				{
					msg->param[1]=msg->param[1]-'0';	
					value =(msg->param[1]);
					if(msg->param[2]==0x2e)
					{
					msg->param[3]=msg->param[3]-'0';		
					value =-(((msg->param[1])*10)+(msg->param[3]))*10;
					}
					else  if(msg->param[3]==0x2e)
					{
						msg->param[2]=msg->param[2]-'0';	
						msg->param[4]=msg->param[4]-'0';	
						value =-(((msg->param[1])*100)+((msg->param[2])*10)+((msg->param[4])))*10;
					}
					else    if(msg->param[4]==0x2e)
					{
						msg->param[2]=msg->param[2]-'0';	
						msg->param[3]=msg->param[3]-'0';
						msg->param[5]=msg->param[5]-'0';	
						value =-(((msg->param[1])*1000)+((msg->param[2])*100)+((msg->param[3])*10)+((msg->param[5])))*10;
					}
				}
				else 
				{
					msg->param[0]=msg->param[0]-'0';	
					value =(msg->param[0]);
					if(msg->param[1]==0x2e)
					{
					msg->param[2]=msg->param[2]-'0';		
					value =(((msg->param[0])*10)+(msg->param[2]))*10;
					}
					else  if(msg->param[2]==0x2e)
					{
						msg->param[1]=msg->param[1]-'0';	
						msg->param[3]=msg->param[3]-'0';	
						value =(((msg->param[0])*100)+((msg->param[1])*10)+((msg->param[3])))*10;
					}
					else    if(msg->param[3]==0x2e)
					{
						msg->param[1]=msg->param[1]-'0';	
						msg->param[2]=msg->param[2]-'0';
						msg->param[4]=msg->param[4]-'0';	
						value =(((msg->param[0])*1000)+((msg->param[1])*100)+((msg->param[2])*10)+((msg->param[4])))*10;
					}
				}
			}
		else
		{
				if(msg->param[0]==0x2d)
				{
					msg->param[1]=msg->param[1]-'0';	
					value =(msg->param[1]);
					if(msg->param[2]==0x2e)
					{
					value =-((msg->param[1])*1000);
					}
					else  if(msg->param[3]==0x2e)
					{
						msg->param[2]=msg->param[2]-'0';	
						value =-((msg->param[1])*10000)+((msg->param[2])*1000);
					}
					else    if(msg->param[4]==0x2e)
					{
						msg->param[2]=msg->param[2]-'0';	
						msg->param[3]=msg->param[3]-'0';
						value =-((msg->param[1])*100000)+((msg->param[2])*10000)+((msg->param[3])*1000);
					}
					else   if(msg->param[5]==0x2e)
					{
						msg->param[2]=msg->param[2]-'0';	
						msg->param[3]=msg->param[3]-'0';	
						msg->param[4]=msg->param[4]-'0';	
						value =-((msg->param[1])*1000000)+((msg->param[2])*100000)+((msg->param[3])*10000)+((msg->param[4])*1000);
					}
				}
				else 
				{
					msg->param[0]=msg->param[0]-'0';	
					value =(msg->param[0]);
					if(msg->param[1]==0x2e)
					{	
					value =((msg->param[0])*1000);
					}
					else  if(msg->param[2]==0x2e)
					{
						msg->param[1]=msg->param[1]-'0';		
						value =((msg->param[0])*10000)+((msg->param[1])*1000);
					}
					else    if(msg->param[3]==0x2e)
					{
						msg->param[1]=msg->param[1]-'0';	
						msg->param[2]=msg->param[2]-'0';
						value =((msg->param[0])*100000)+((msg->param[1])*10000)+((msg->param[2])*1000);
					}
					else   if(msg->param[4]==0x2e)
					{
						msg->param[1]=msg->param[1]-'0';	
						msg->param[2]=msg->param[2]-'0';	
						msg->param[3]=msg->param[3]-'0';	
						value =((msg->param[0])*1000000)+((msg->param[1])*100000)+((msg->param[2])*10000)+((msg->param[3])*1000);
					}
				}	
		}
	switch(cmd_type)
	{
	case NOTIFY_SCREEN://�л�����
		NotifyScreen(screen_id);
		break;
	case NOTIFY_CONTROL:
		{
			switch(control_type)
			{
			case kCtrlButton: //��ť�ؼ�
				NotifyButton(screen_id,control_id,msg->param[1]);
				break;
			case kCtrlText://�ı��ؼ�
				NotifyText(screen_id,control_id,value);
				break;
			case kCtrlMenu://�˵��ؼ�
				NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
				break;
			case kCtrlSelector://ѡ��ؼ�
				NotifySelector(screen_id,control_id,msg->param[0]);
				break;
			case kCtrlRTC://����ʱ�ؼ�
				NotifyTimer(screen_id,control_id);
				break;
			default:
				break;
			}
		}
		break;
	  default:
		break;
	}
}

/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton(u16 screen_id, u16 control_id, u8  state)
{
	switch(control_id)
		{
				case 		0x38:	
				MoveStop(MOTOR_PORT_1);
				break;
				case 		0x39:		
				EEPROMSaveParam(eDataUserOrgPoint1_1);
				EEPROMSaveParam(eDataUserOrgPoint1_2);
				MotorPwmSetOrigin(MOTOR_PORT_1);
				break;
				case 		0x3A:	
				MovePosCtrlSetPoint(MOTOR_PORT_1,0);	
				break;
				case 		0x35:		
				MoveStop(MOTOR_PORT_2);
				break;
				case 		0x36:		
				EEPROMSaveParam(eDataUserOrgPoint1_1);
				EEPROMSaveParam(eDataUserOrgPoint1_2);
				MotorPwmSetOrigin(MOTOR_PORT_2);	
				break;
				case 		0x37:		
				MovePosCtrlSetPoint(MOTOR_PORT_2,0);	
				break;
				case 		0x32:		
				MoveStop(MOTOR_PORT_3);
				break;
				case 		0x33:		
				EEPROMSaveParam(eDataUserOrgPoint1_1);
				EEPROMSaveParam(eDataUserOrgPoint1_2);
				MotorPwmSetOrigin(MOTOR_PORT_3);
				break;
				case 		0x34:		
				MovePosCtrlSetPoint(MOTOR_PORT_3,0);	
				break;
		}

}
/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(u16 screen_id, u16 control_id, u32 data)
{
	switch(control_id)
		{
				case 		0x1E:
				MovePosCtrlSetPoint(MOTOR_PORT_1,data);	
				break;
				case 		0x15:	
				MovePosCtrlSetAction(MOTOR_PORT_1,data);
				break;
				case 		0x1D:
				MovePosCtrlSetPoint(MOTOR_PORT_2,data);	
				break;
				case 		0x13:		
				MovePosCtrlSetAction(MOTOR_PORT_2,data);
				break;
				case 		0x24:		
				MovePosCtrlSetPoint(MOTOR_PORT_3,data);		
				break;
				case 		0x25:		
				MovePosCtrlSetAction(MOTOR_PORT_3,data);
				break;
				case 		0x16:		
				SPD.m_MaxSpd1 = data;
				EEPROMSaveParam(eDataAxis1MaxSpd1);
				break;
				case 		0x1F:		
				SPD.m_MaxSpd2 = data;
				EEPROMSaveParam(eDataAxis1MaxSpd2);
				break;
				case 		0x23:		
				SPD.m_MaxSpd3 = data;
				EEPROMSaveParam(eDataAxis1MaxSpd3);
				break;
	}
}
/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
	//TODO: ����û�����
}
/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(u16 screen_id, u16 control_id)
{
	//TODO: ����û�����
}
/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(u16 screen_id)
{
	//TODO: ����û�����
}
void  SendChar(unsigned char  t)
{
    USART_SendData(USART2,t);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));//�ȴ����ڷ������
}
void SetSliderValue(u16 screen_id,u16 control_id,u32 value)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_32(value);
	END_CMD();
}
void Write(u16 screen_id,u16 control_id,u16 length,u8 *_data)
{
	BEGIN_CMD();
	TX_8(0xB1);
	TX_8(0x10);
	TX_16(screen_id);
	TX_16(control_id);
	TX_8N(_data,length);
	END_CMD();
}
void SendNU8(u8 *pData,u16 nDataLen)
{
	u16 i = 0;
	for (;i<nDataLen;++i)
	{
		TX_8(pData[i]);
	}
}


HMI_ACT_FRAME g_HMIActFrame[SERIAL_PORT_MAX];


VOID HMIActSend(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id)
{
	INT32 i16Temp;
	i16Temp = AVD.m_UserPosition1;
	if(CMD==0X11)
	  {
			if(i16Temp >=0)
					{
						if(i16Temp/1000000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%1000000)/100000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/100000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/100000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);			
						}
						else if(i16Temp/10000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/10000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,2,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/1000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000+'0';
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);			
						}
						else
						{
						g_HMIActFrame[port].m_PosAng[0]=0x30;
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);	
						}
					}
					else
					{
						g_HMIActFrame[port].m_PosAng[0]=0x2d;
						i16Temp=-i16Temp;	
						if(i16Temp/1000000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000000)/100000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[4]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);
	
						}
						else if(i16Temp/100000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/100000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);

						}
						else if(i16Temp/10000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/10000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/1000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000+'0';
						Write(screen_id,control_id,2,g_HMIActFrame[port].m_PosAng);		
						}
						else
						{
						g_HMIActFrame[port].m_PosAng[0]=0x30;
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);	
						}

					}
		}
		else if(CMD==0X13)
		{
			SetSliderValue(screen_id,control_id,i16Temp/1000);
		}
}
VOID HMIActSend2(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id)
{
	INT32 i16Temp;
	i16Temp = AVD.m_UserPosition2;
	if(CMD==0X11)
	  {
			if(i16Temp >=0)
					{
						if(i16Temp/1000000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%1000000)/100000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/100000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/100000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);			
						}
						else if(i16Temp/10000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/10000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,2,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/1000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000+'0';
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);			
						}
						else
						{
						g_HMIActFrame[port].m_PosAng[0]=0x30;
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);	
						}

					}
					else
					{
						g_HMIActFrame[port].m_PosAng[0]=0x2d;
						i16Temp=-i16Temp;	
						if(i16Temp/1000000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000000)/100000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[4]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);
	
						}
						else if(i16Temp/100000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/100000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100000)/10000+'0';
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);

						}
						else if(i16Temp/10000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/10000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+'0';
						Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/1000>=1)
						{
						g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000+'0';
						Write(screen_id,control_id,2,g_HMIActFrame[port].m_PosAng);		
						}
						else
						{
						g_HMIActFrame[port].m_PosAng[0]=0x30;
						Write(screen_id,control_id,1,g_HMIActFrame[port].m_PosAng);	
						}
					}
		}
		else if(CMD==0X13)
		{
			SetSliderValue(screen_id,control_id,i16Temp/1000);
		}
}
VOID HMIActSend3(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id)
{
	INT32 i16Temp;
	i16Temp = AVD.m_UserPosition3;
	if(i16Temp>=0)
	{
	i16Temp=i16Temp%36000;
	}
	if(i16Temp<0)
	{
	i16Temp=-(i16Temp%36000);
	}
		
	if(CMD==0X11)
	  {
			if((i16Temp >=0)&&(i16Temp<18000))
			{
						if(i16Temp/10000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/10000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%10000)/1000+'0';
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000)/100+'0';
						g_HMIActFrame[port].m_PosAng[3]=0x2e;
						g_HMIActFrame[port].m_PosAng[4]=(i16Temp%100)/10+'0';
						Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);		
						}
						else if(i16Temp/1000>=1)
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000+'0';
						g_HMIActFrame[port].m_PosAng[1]=(i16Temp%1000)/100+'0';
						g_HMIActFrame[port].m_PosAng[2]=0x2e;
						g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100)/10+'0';
						Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
						}
						else 
						{
						g_HMIActFrame[port].m_PosAng[0]=i16Temp/100+'0';
						g_HMIActFrame[port].m_PosAng[1]=0x2e;
						g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100)/10+'0'; 
						Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);		
						}
					}
					else  if((i16Temp <0)&&(i16Temp>-18000))
					{
								g_HMIActFrame[port].m_PosAng[0]=0x2d;
								i16Temp=-i16Temp;	
								if(i16Temp/10000>=1)
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/10000+'0';
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+'0';
								g_HMIActFrame[port].m_PosAng[3]=(i16Temp%1000)/100+'0';
								g_HMIActFrame[port].m_PosAng[4]=0x2e;
								g_HMIActFrame[port].m_PosAng[5]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,6,g_HMIActFrame[port].m_PosAng);		
								}
								else if(i16Temp/1000>=1)
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000+'0';
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000)/100+'0';
								g_HMIActFrame[port].m_PosAng[3]=0x2e;
								g_HMIActFrame[port].m_PosAng[4]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);		
								}
								else 
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/100+'0';
								g_HMIActFrame[port].m_PosAng[2]=0x2e;
								g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
								}
					}
					else  if(i16Temp >18000)
					{
								g_HMIActFrame[port].m_PosAng[0]=0x2d;
								i16Temp=36000-i16Temp;	
								if(i16Temp/10000>=1)
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/10000+0x30;
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%10000)/1000+0x30;
								g_HMIActFrame[port].m_PosAng[3]=(i16Temp%1000)/100+0x30;
								g_HMIActFrame[port].m_PosAng[4]=0x2e;
								g_HMIActFrame[port].m_PosAng[5]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,7,g_HMIActFrame[port].m_PosAng);		
								}
								else if(i16Temp/1000>=1)
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/1000+'0';
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000)/100+'0';
								g_HMIActFrame[port].m_PosAng[3]=0x2e;
								g_HMIActFrame[port].m_PosAng[4]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);		
								}
								else 
								{
								g_HMIActFrame[port].m_PosAng[1]=i16Temp/100+'0';
								g_HMIActFrame[port].m_PosAng[2]=0x2e;
								g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
								}
					}
							else  if(i16Temp<-18000 )
							{
								i16Temp=36000+i16Temp;	
								if(i16Temp/10000>=1)
								{
								g_HMIActFrame[port].m_PosAng[0]=i16Temp/10000+0x30;
								g_HMIActFrame[port].m_PosAng[1]=(i16Temp%10000)/1000+0x30;
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%1000)/100+0x30;
								g_HMIActFrame[port].m_PosAng[3]=0x2e;
								g_HMIActFrame[port].m_PosAng[4]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,5,g_HMIActFrame[port].m_PosAng);		
								}
								else if(i16Temp/1000>=1)
								{
								g_HMIActFrame[port].m_PosAng[0]=i16Temp/1000+'0';
								g_HMIActFrame[port].m_PosAng[1]=(i16Temp%1000)/100+'0';
								g_HMIActFrame[port].m_PosAng[2]=0x2e;
								g_HMIActFrame[port].m_PosAng[3]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,4,g_HMIActFrame[port].m_PosAng);		
								}
								else 
								{
								g_HMIActFrame[port].m_PosAng[0]=i16Temp/100+'0';
								g_HMIActFrame[port].m_PosAng[1]=0x2e;
								g_HMIActFrame[port].m_PosAng[2]=(i16Temp%100)/10+'0';
								Write(screen_id,control_id,3,g_HMIActFrame[port].m_PosAng);		
								}
							
							}
		}
		else if(CMD==0X13)
		{
			SetSliderValue(screen_id,control_id,i16Temp/100);
		}
}


