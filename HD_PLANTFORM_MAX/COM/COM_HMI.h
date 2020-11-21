#include <stm32f4xx.h>
#include "UTL_USART.h"
#include "BSP_BasicDataType.h"	


#define PTR2U16(PTR) ((((u8 *)(PTR))[0]<<8)|((u8 *)(PTR))[1])  //从缓冲区取16位数据
#define BEGIN_CMD() TX_8(0XEE)
#define END_CMD() TX_32(0XFFFCFFFF)
#define TX_8(P1) SEND_DATA((P1)&0xFF)  //发送单个字节
#define TX_8N(P,N) SendNU8((u8 *)P,N)  //发送N个字节
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)  //发送16位整数
#define TX_16N(P,N) SendNU16((u16 *)P,N)  //发送N个16位整数
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)  //发送32位整数
#define SEND_DATA(P) SendChar(P)

#define CMD_MAX_SIZE 20   

#define BUTTON_SCAN             0X0001
#define BUTTON_POSITION_MODLE   0X001E
#define BUTTON_STOP             0X0014
#define BUTTON_LINK             0X000F
#define BUTTON_BREAK            0X0010

#define CMD_ZHEN_TOU        0XFE
#define CMD_SET_ANGLE       0X44
#define CMD_SET_SPEED       0X46
#define CMD_CORTRL          0X4C
#define CMD_STOP            0X45
#define CMD_GET_ANGLE       0X41
#define CMD_SPEED_MODLE     0X00
#define CMD_POSITION_MODLE  0X01
#define CMD_FOLLOW_MODLE    0X02
#define CMD_SCAN_MODLE      0X03


#define NOTIFY_MENU                        0X14  //菜单事件通知
#define NOTIFY_TIMER                       0X43  //定时器超时通知
#define NOTIFY_CONTROL                0XB1  //控件更新通知
#define NOTIFY_SCREEN                   0XB2  //画面切换通知
#define NOTIFY_TXT               0X11  //控件更新通知
#define NOTIFY_SLID                  0X10  //画面切换通知


enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //按钮
	kCtrlText,  //文本
	kCtrlProgress,  //进度条
	kCtrlSlider,    //滑动条
	kCtrlMeter,  //仪表
	kCtrlDropList, //下拉列表
	kCtrlAnimation, //动画
	kCtrlRTC, //时间显示
	kCtrlGraph, //曲线图控件
	kCtrlTable, //表格控件
	kCtrlMenu,//菜单控件
	kCtrlSelector,//选择控件
	kCtrlQRCode,//二维码
};


typedef struct
{
	u8    cmd_head;  //帧头

	u8    cmd_type;  //命令类型(UPDATE_CONTROL)	
	u8    ctrl_msg;   //CtrlMsgType-指示消息的类型
	u8   screen_id[2];  //产生消息的画面ID
	u8   control_id[2];  //产生消息的控件ID
	u8    control_type; //控件类型

	u8    param[256];//可变长度参数，最多256个字节

	u8  cmd_tail[4];   //帧尾
}CTRL_MSG,*PCTRL_MSG;


typedef struct tagHMI_ACT_FRAME
{
	UINT8	  m_Head;
	UINT8	  BEGIN_CMD[2];
	UINT8	  m_IdCode[4];
	UINT8	  m_PosAng[6];
	UINT8	  END_CMD[4];
}HMI_ACT_FRAME;


void  SendChar(unsigned char t);
void NotifyButton(u16 screen_id, u16 control_id, u8  state);
void ProcessMessage( PCTRL_MSG msg, u16 size );
void NotifyText(u16 screen_id, u16 control_id, u32 data);
void NotifyMenu(u16 screen_id, u16 control_id, u8  item, u8  state);
void NotifySelector(u16 screen_id, u16 control_id, u8  item);
void NotifyTimer(u16 screen_id, u16 control_id);
void USART_Send(u8 *buffer,u16 LEN);
void NotifyScreen(u16 screen_id);
void SetSliderValue(u16 screen_id,u16 control_id,u32 value);
void Write(u16 screen_id,u16 control_id,u16 length,u8 *_data);
void SendNU8(u8 *pData,u16 nDataLen);
VOID HMIProtocol1Loop(SERIAL_PORT port, UINT8 slvAdd);
VOID HMIActSend(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id);
VOID HMIActSend2(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id);
VOID HMIActSend3(SERIAL_PORT port,UINT8 CMD,UINT16 screen_id,UINT16 control_id);
