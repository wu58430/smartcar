/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file             board
* @company          成都逐飞科技有限公司
* @author           逐飞科技(QQ3184284598)
* @version          查看doc内version文件 版本说明
* @Software         IAR 8.3 or MDK 5.24
* @Target core      MM32F3277
* @Taobao           https://seekfree.taobao.com/
* @date             2021-02-22
********************************************************************************************************************/

#include "board.h"
#include "zf_uart.h"
#include "headfile.h"

uint8 dbg_tx_buff[100];
uint8 dbg_rx_buff = 0;




//-------------------------------------------------------------------------------------------------------------------
// @brief       核心板初始化
// @param       debug_enable    是否开启默认 debug 输出 默认 UART1
// @return      void
// Sample usage:                board_init(TRUE);
//-------------------------------------------------------------------------------------------------------------------
void board_init(bool debug_enable)
{
    if (debug_enable)
        uart_init(DEBUG_UART, DEBUG_UART_BAUD, DEBUG_UART_TX, DEBUG_UART_RX);                       // 默认初始化 UART1 用以支持 printf 输出

}


void DebugUart_PrintImage(void)
{
    if (ov7725_finish_flag)
    {
        //seekfree_sendimg_7725(DEBUG_UART, ov7725_image_bin0[0], 160*80);
        ov7725_finish_flag = 0;
    }
}



