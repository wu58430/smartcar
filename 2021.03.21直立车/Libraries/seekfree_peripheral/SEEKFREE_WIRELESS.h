/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				SEEKFREE_WIRELESS
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
* @note
* 					接线定义：
* 					------------------------------------
* 					模块管脚				单片机管脚
* 					RX						查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_TX宏定义
* 					TX						查看SEEKFREE_WIRELESS.h文件中的WIRELESS_UART_RX宏定义
* 					RTS						查看SEEKFREE_WIRELESS.h文件中的RTS_PIN宏定义
* 					CMD						悬空或者上拉
* 					------------------------------------
********************************************************************************************************************/

#ifndef _SEEKFREE_WIRELESS
#define _SEEKFREE_WIRELESS

#include "common.h"

#define WIRELESS_UART			UART_1												// 无线转串口模块 所使用到的串口
#define WIRELESS_UART_TX		UART1_TX_A09
#define WIRELESS_UART_RX		UART1_RX_A10
#define WIRELESS_UART_BAUD		115200

#define RTS_PIN					B12													// 定义流控位引脚  指示当前模块是否可以接受数据  0可以继续接收  1不可以继续接收

#define WIRELESS_BUFFER_SIZE	5

extern uint8 wireless_rx_buffer[WIRELESS_BUFFER_SIZE];
extern uint16 wireless_rx_index;

void		wireless_uart_callback		(void);
void		seekfree_wireless_init		(void);
uint32		seekfree_wireless_send_buff	(uint8 *buff, uint32 len);


//-------------------------user-----------------------------------//
//帧数据
#define 	COMMUSIZE  			5
#define 	Head0     			0  //帧头0
#define 	Head1     			1  //帧头1 
#define     CarID               2  //ID
#define 	DATA0        		3  //球标志
#define 	SUMVERIFY 			4  //校验
//标识
#define     PedicabID          0x01
#define 	BalanceID          0x02

typedef struct
{
    uint8 InfoState;
    int16 ID;
    int16 ConmmuState;
} CommunicateType;

extern CommunicateType CommuInfo;
extern uint8 CommuTxBuff[COMMUSIZE],
       CommuRxBuff[COMMUSIZE];

void CommunicateInit(void);
uint8 InformationReslove(uint8 *data, CommunicateType *Commu);
uint8 InformationSend(uint8 *data, CommunicateType *Commu);

#endif
