#include <stm32f4xx.h>
#include "UTL_USART.h"
#include "BSP_BasicDataType.h"	


#define PTR2U16(PTR) ((((u8 *)(PTR))[0]<<8)|((u8 *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define BEGIN_CMD() TX_8(0XEE)
#define END_CMD() TX_32(0XFFFCFFFF)
#define TX_8(P1) SEND_DATA((P1)&0xFF)  //���͵����ֽ�
#define TX_8N(P,N) SendNU8((u8 *)P,N)  //����N���ֽ�
#define TX_16(P1) TX_8((P1)>>8);TX_8(P1)  //����16λ����
#define TX_16N(P,N) SendNU16((u16 *)P,N)  //����N��16λ����
#define TX_32(P1) TX_16((P1)>>16);TX_16((P1)&0xFFFF)  //����32λ����
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


#define NOTIFY_MENU                        0X14  //�˵��¼�֪ͨ
#define NOTIFY_TIMER                       0X43  //��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL                0XB1  //�ؼ�����֪ͨ
#define NOTIFY_SCREEN                   0XB2  //�����л�֪ͨ
#define NOTIFY_TXT               0X11  //�ؼ�����֪ͨ
#define NOTIFY_SLID                  0X10  //�����л�֪ͨ


enum CtrlType
{
	kCtrlUnknown=0x0,
	kCtrlButton=0x10,  //��ť
	kCtrlText,  //�ı�
	kCtrlProgress,  //������
	kCtrlSlider,    //������
	kCtrlMeter,  //�Ǳ�
	kCtrlDropList, //�����б�
	kCtrlAnimation, //����
	kCtrlRTC, //ʱ����ʾ
	kCtrlGraph, //����ͼ�ؼ�
	kCtrlTable, //���ؼ�
	kCtrlMenu,//�˵��ؼ�
	kCtrlSelector,//ѡ��ؼ�
	kCtrlQRCode,//��ά��
};


typedef struct
{
	u8    cmd_head;  //֡ͷ

	u8    cmd_type;  //��������(UPDATE_CONTROL)	
	u8    ctrl_msg;   //CtrlMsgType-ָʾ��Ϣ������
	u8   screen_id[2];  //������Ϣ�Ļ���ID
	u8   control_id[2];  //������Ϣ�Ŀؼ�ID
	u8    control_type; //�ؼ�����

	u8    param[256];//�ɱ䳤�Ȳ��������256���ֽ�

	u8  cmd_tail[4];   //֡β
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
