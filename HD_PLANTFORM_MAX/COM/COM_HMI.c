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
	u8 cmd_type = msg->cmd_type;                   //指令类型
	u8 control_type = msg->control_type;           //控件类型
	u16 screen_id = PTR2U16(&msg->screen_id);       //画面ID
	u16 control_id = PTR2U16(&msg->control_id);     //控件ID
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
	case NOTIFY_SCREEN://切换画面
		NotifyScreen(screen_id);
		break;
	case NOTIFY_CONTROL:
		{
			switch(control_type)
			{
			case kCtrlButton: //按钮控件
				NotifyButton(screen_id,control_id,msg->param[1]);
				break;
			case kCtrlText://文本控件
				NotifyText(screen_id,control_id,value);
				break;
			case kCtrlMenu://菜单控件
				NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
				break;
			case kCtrlSelector://选择控件
				NotifySelector(screen_id,control_id,msg->param[0]);
				break;
			case kCtrlRTC://倒计时控件
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
 *  \brief  按钮控件通知
 *  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param state 按钮状态：0弹起，1按下
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
 *  \brief  文本控件通知
 *  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param str 文本控件内容
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
 *  \brief  菜单控件通知
 *  \details  当菜单项按下或松开时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 菜单项索引
 *  \param state 按钮状态：0松开，1按下
 */
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state)
{
	//TODO: 添加用户代码
}

/*! 
 *  \brief  选择控件通知
 *  \details  当选择控件变化时，执行此函数
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 *  \param item 当前选项
 */
void NotifySelector(u16 screen_id, u16 control_id, u8  item)
{
	//TODO: 添加用户代码
}
/*! 
 *  \brief  定时器超时通知处理
 *  \param screen_id 画面ID
 *  \param control_id 控件ID
 */
void NotifyTimer(u16 screen_id, u16 control_id)
{
	//TODO: 添加用户代码
}
/*! 
 *  \brief  画面切换通知
 *  \details  当前画面改变时(或调用GetScreen)，执行此函数
 *  \param screen_id 当前画面ID
 */
void NotifyScreen(u16 screen_id)
{
	//TODO: 添加用户代码
}
void  SendChar(unsigned char  t)
{
    USART_SendData(USART2,t);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET));//等待串口发送完毕
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


