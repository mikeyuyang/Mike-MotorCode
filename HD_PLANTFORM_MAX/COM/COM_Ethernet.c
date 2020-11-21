/*
//######################################################################################################
//
// FILE:	COM_Ethernet.c
//
// TITLE:	This function realized the Ethernet communication set up functions(LWIP).
//
// AUTHOR:	Chris.Shang
//
// DATE:	Oct 28, 2017
//
// Description: All communication function for serial ports.
//

//------------------------------------------------------------------------------------------------------
// Created by: 		Chris.Shang
// Created date:   	Oct 28, 2017
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
#include <stm32f4xx.h>
#include <string.h>
#include <stdio.h>
#include "includes.h"

#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "tcpip.h"


#include "BSP_BasicDataType.h"			// Basic data type define.
#include "BSP_TgtCfg.h"					// Target config.

#include "DBM_Data.h"
#include "DBM_Enum.h"
#include "UTL_USART.h"
#include "COM_ModbusRTU.h"

#define	COM_ETHERNET_GLOBAL_
#include "COM_Ethernet.h"

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
#define USE_DHCP	0

#define MAX_DHCP_TRIES 5
#define  LWIP_DHCP_TASK_PRIO    10

/*
********************************************************************************************************
*	 Externals
********************************************************************************************************
*/
/*
********************************************************************************************************
*	 Local data type
********************************************************************************************************
*/

/* COM task information. */
typedef struct
{
	void (*m_TaskFunc)(void*);
	const char *m_TaskName;
	UINT16 m_StackSize;
	void* const m_TaskArg;
	UBaseType_t m_TaskPrio;
	TaskHandle_t* const m_TaskHandler;	
} EthernetTaskInfo;

typedef enum 
{ 
  DHCP_START=0,
  DHCP_WAIT_ADDRESS,
  DHCP_ADDRESS_ASSIGNED,
  DHCP_TIMEOUT
}DHCP_State_TypeDef;

/*
********************************************************************************************************
*	 Local variables
********************************************************************************************************
*/
struct netif xnetif; /* network interface structure */

TaskHandle_t EthComAppTask_Handler;

/*
*********************************************************************************************************
*    Local function prototypes
*********************************************************************************************************
*/
/*
void Task_SerComApp1(void* p_arg);
void Task_SerComApp2(void* p_arg);
void Task_SerComApp3(void* p_arg);
void Task_SerComApp4(void* p_arg);
void Task_SerComApp5(void* p_arg);
void Task_SerComApp6(void* p_arg);
void Task_SerComApp7(void* p_arg);
void Task_SerComApp8(void* p_arg);
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
* Description	: Initilize LwIP Stack
	1) Input argument  : None
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
* Author		: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
VOID InitLwIP(VOID)
{
	ip4_addr_t ipaddr;
	ip4_addr_t netmask;
	ip4_addr_t gw;
#if USE_DHCP 
	uint8_t iptab[4];
	uint8_t iptxt[20];
#endif
	/* Create tcp_ip stack thread */
	tcpip_init( NULL, NULL );	

	/* IP address setting & display on STM32_evalboard LCD*/
#if USE_DHCP
	ipaddr.addr = 0;
	netmask.addr = 0;
	gw.addr = 0;
#else
	IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
	IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
	IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);

#endif

	/* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
	    struct ip_addr *netmask, struct ip_addr *gw,
	    void *state, err_t (* init)(struct netif *netif),
	    err_t (* input)(struct pbuf *p, struct netif *netif))

	Adds your network interface to the netif_list. Allocate a struct
	netif and pass a pointer to this structure as the first argument.
	Give pointers to cleared ip_addr structures when using DHCP,
	or fill them with sane numbers otherwise. The state pointer may be NULL.

	The init function pointer must point to a initialization function for
	your ethernet netif interface. The following code illustrates it's use.*/

	netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

	/*  Registers the default network interface. */
	netif_set_default(&xnetif);

	/*  When the netif is fully configured this function must be called.*/
	netif_set_up(&xnetif); 

	/* Creat DHCP task if needed */
#if USE_DHCP
	xTaskCreate((TaskFunction_t )Task_LwIP_DHCP,			 
		(const char*	)"DHCP_Task",			
		(uint16_t		)configMINIMAL_STACK_SIZE * 2,		 
		(void*			)NULL,					
		(UBaseType_t	)LWIP_DHCP_TASK_PRIO,		
		(TaskHandle_t*	)&EthComAppTask_Handler);  

#endif	
}

#if USE_DHCP
/*
**************************************************************************
* Description	: Initilize LwIP Stack
	1) Input argument  : None
	2) Modify argument : None
	3) Output argument : None
* Return		:
* Notes 		:
* Author		: Chris.Shang
* Date		: Oct 28, 2017
**************************************************************************
*/
void Task_LwIP_DHCP(void* p_arg)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint32_t IPaddress;
  uint8_t iptab[4];
  uint8_t iptxt[20];
  uint8_t DHCP_state;  
  DHCP_state = DHCP_START;

  for (;;)
  {
    switch (DHCP_state)
    {
      case DHCP_START:
      {
        dhcp_start(&xnetif);
        IPaddress = 0;
        DHCP_state = DHCP_WAIT_ADDRESS;
		break;
      }
      case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = xnetif.ip_addr.addr;

        if (IPaddress!=0) 
        {
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
          
          /* Stop DHCP */
          dhcp_stop(&xnetif);

          vTaskDelete(NULL);
        }
        else
        {
          /* DHCP timeout */
          if (xnetif.dhcp->tries > MAX_DHCP_TRIES)
          {
            DHCP_state = DHCP_TIMEOUT;

            /* Stop DHCP */
            dhcp_stop(&xnetif);

            /* Static address used */
            IP4_ADDR(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
            IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
            IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
            netif_set_addr(&xnetif, &ipaddr , &netmask, &gw);

            vTaskDelete(NULL);
          }
        }
		break;
      }
      default: break;
    }

    /* wait 250 ms */
    vTaskDelay(250);
  }   
}
#endif  /* USE_DHCP */

/*
*********************************************************************************************************
*                                           End of file 
*********************************************************************************************************
*/


