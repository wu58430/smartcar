/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2020,逐飞科技
* All rights reserved.
* 技术讨论QQ群：三群：824575535
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file       		2.0寸IPS屏幕
* @company	   		成都逐飞科技有限公司
* @author     		逐飞科技(QQ3184284598)
* @version    		查看doc内version文件 版本说明
* @Software 		ADS v1.2.2
* @Target core		TC264D
* @Taobao   		https://seekfree.taobao.com/
* @date       		2020-3-23
* @note
					接线定义：
					------------------------------------
						模块管脚            	单片机管脚
						D0       		查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D0_PIN 宏定义
						D1             	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D1_PIN 宏定义
						D2            	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D2_PIN 宏定义
						D3             	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D3_PIN 宏定义
						D4            	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D4_PIN 宏定义
						D5         		查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D5_PIN 宏定义
						D6           	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D6_PIN 宏定义
						D7           	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_D7_PIN 宏定义

						BL          	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_BL_PIN 宏定义
						CS            	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_CS_PIN 宏定义
						RD            	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RD_PIN 宏定义
						WR           	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_WR_PIN 宏定义
						RS          	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RS_PIN 宏定义
						RST            	查看SEEKFREE_IPS200_PARALLEL8.H文件内的IPS200_RST_PIN 宏定义

						电源引脚
						VCC 3.3V电源
						GND 电源地
						最大分辨率240*320
					------------------------------------
********************************************************************************************************************/

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "zf_systick.h"
#include "zf_gpio.h"
#include "zf_fsmc.h"
#include "SEEKFREE_PRINTF.h"
#include "SEEKFREE_IPS200_PARALLEL8.h"

#include "hal_fsmc.h"
#include "hal_rcc.h"


uint16 IPS200_BGCOLOR = WHITE;//背景颜色
uint16 IPS200_PENCOLOR = RED;//画笔颜色


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_w_data(uint8 dat)			//写数据
{
    *(vuint16 *)IPS200_DATA_ADD = ((uint16)dat << 8);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_reg(uint8 command)			//写命令
{
    IPS200_RD(1);
    IPS200_WR(0);
    *(vuint16 *)IPS200_CMD_ADD = ((uint16)command << 8);
    IPS200_WR(1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_data(uint8 dat)			//向液晶屏写一个8位数据
{
    IPS200_RD(1);
    IPS200_WR(0);
    ips200_w_data(dat);
    IPS200_WR(1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_wr_data16(uint16 dat)		//向液晶屏写一个16位数据
{
    IPS200_RD(1);
    IPS200_WR(0);
    ips200_w_data((uint8)(dat >> 8));
    IPS200_WR(1);
    IPS200_WR(0);
    ips200_w_data((uint8)(dat));
    IPS200_WR(1);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_w_reg(uint8 com, uint8 dat)		//写寄存器
{
    ips200_wr_reg(com);
    ips200_wr_data(dat);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      内部调用，用户无需关心
//  @param      void
//  @return
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_address_set(uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    ips200_wr_reg(0x2a);
    ips200_wr_data16(x1);
    ips200_wr_data16(x2);

    ips200_wr_reg(0x2b);
    ips200_wr_data16(y1);
    ips200_wr_data16(y2);

    ips200_wr_reg(0x2c);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      2.0寸 IPS液晶初始化
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips200_init(void)
{
    gpio_init(IPS200_RST_PIN, GPO, 0, GPO_PUSH_PULL);							// LCD_RTS
    gpio_init(IPS200_BL_PIN, GPO, 0, GPO_PUSH_PULL);							// LCD_BL

    gpio_init(IPS200_RD_PIN, GPO, 0, GPO_PUSH_PULL);							// FSMC_NOE LCD_RD
    gpio_init(IPS200_WR_PIN, GPO, 0, GPO_PUSH_PULL);							// FSMC_NWE LCD_WR

    afio_init(IPS200_CS_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_NE1 LCD_CS
    afio_init(IPS200_RS_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_A18 LCD_RS

    afio_init(IPS200_D0_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D0 LCD_D0
    afio_init(IPS200_D1_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D1 LCD_D1

    afio_init(IPS200_D2_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D2 LCD_D2
    afio_init(IPS200_D3_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D3 LCD_D3

    afio_init(IPS200_D4_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D4 LCD_D4
    afio_init(IPS200_D5_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D5 LCD_D5
    afio_init(IPS200_D6_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D6 LCD_D6
    afio_init(IPS200_D7_PIN, GPO, GPIO_AF12, GPO_AF_PUSH_PUL);					// FSMC_D7 LCD_D7

    fsmc_init();

    IPS200_BL(1);
    IPS200_RST(0);
    systick_delay_ms(5);
    IPS200_RST(1);
    systick_delay_ms(120);

    ips200_wr_reg(0x11);
    systick_delay_ms(120);

    ips200_wr_reg(0x36);
    if		(IPS200_DISPLAY_DIR == 0)	ips200_wr_data(0x00);
    else if	(IPS200_DISPLAY_DIR == 1)	ips200_wr_data(0xC0);
    else if	(IPS200_DISPLAY_DIR == 2)	ips200_wr_data(0x70);
    else							ips200_wr_data(0xA0);

    ips200_wr_reg(0x3A);
    ips200_wr_data(0x05);

    ips200_wr_reg(0xB2);
    ips200_wr_data(0x0C);
    ips200_wr_data(0x0C);
    ips200_wr_data(0x00);
    ips200_wr_data(0x33);
    ips200_wr_data(0x33);

    ips200_wr_reg(0xB7);
    ips200_wr_data(0x35);

    ips200_wr_reg(0xBB);
    ips200_wr_data(0x29); //32 Vcom=1.35V

    ips200_wr_reg(0xC2);
    ips200_wr_data(0x01);

    ips200_wr_reg(0xC3);
    ips200_wr_data(0x19); //GVDD=4.8V

    ips200_wr_reg(0xC4);
    ips200_wr_data(0x20); //VDV, 0x20:0v

    ips200_wr_reg(0xC5);
    ips200_wr_data(0x1A);//VCOM Offset Set

    ips200_wr_reg(0xC6);
    ips200_wr_data(0x01F); //0x0F:60Hz

    ips200_wr_reg(0xD0);
    ips200_wr_data(0xA4);
    ips200_wr_data(0xA1);

    ips200_wr_reg(0xE0);
    ips200_wr_data(0xD0);
    ips200_wr_data(0x08);
    ips200_wr_data(0x0E);
    ips200_wr_data(0x09);
    ips200_wr_data(0x09);
    ips200_wr_data(0x05);
    ips200_wr_data(0x31);
    ips200_wr_data(0x33);
    ips200_wr_data(0x48);
    ips200_wr_data(0x17);
    ips200_wr_data(0x14);
    ips200_wr_data(0x15);
    ips200_wr_data(0x31);
    ips200_wr_data(0x34);

    ips200_wr_reg(0xE1);
    ips200_wr_data(0xD0);
    ips200_wr_data(0x08);
    ips200_wr_data(0x0E);
    ips200_wr_data(0x09);
    ips200_wr_data(0x09);
    ips200_wr_data(0x15);
    ips200_wr_data(0x31);
    ips200_wr_data(0x33);
    ips200_wr_data(0x48);
    ips200_wr_data(0x17);
    ips200_wr_data(0x14);
    ips200_wr_data(0x15);
    ips200_wr_data(0x31);
    ips200_wr_data(0x34);

    ips200_wr_reg(0x21);

    ips200_wr_reg(0x29);
    ips200_clear(IPS200_BGCOLOR);	//初始化为白屏
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶清屏函数
//  @param      color     		颜色设置
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_clear(YELLOW);// 全屏设置为黄色
//-------------------------------------------------------------------------------------------------------------------
void ips200_clear(uint16 color)
{
    uint16 i, j;
    ips200_address_set(0, 0, IPS200_X_MAX - 1, IPS200_Y_MAX - 1);
    for(i = 0; i < IPS200_X_MAX; i++)
    {
        for (j = 0; j < IPS200_Y_MAX; j++)
        {
            ips200_wr_data16(color);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶画点
//  @param      x     	        坐标x方向的起点
//  @param      y     	        坐标y方向的起点
//  @param      dat       	    需要显示的颜色
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_drawpoint(0,0,RED);  //坐标0,0画一个红色的点
//-------------------------------------------------------------------------------------------------------------------
void ips200_drawpoint(int16 x, int16 y, uint16 color)
{
    //x   240	y	320
    if(x < 0)
    {
        x = 0;
    }
    if(x >= 240)
    {
        x = 239;
    }
    if(y < 0)
    {
        y = 0;
    }
    if(y >= 320)
    {
        y = 319;
    }
    ips200_address_set(x, y, x, y);
    ips200_wr_data16(color);
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符
//  @param      x     	        坐标x方向的起点 参数范围 0 - (IPS200_X_MAX-1)
//  @param      y     	        坐标y方向的起点 参数范围 0 - (IPS200_Y_MAX/16-1)
//  @param      dat       	    需要显示的字符
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showchar(0,0,'x');//坐标0,0写一个字符x
//-------------------------------------------------------------------------------------------------------------------
void ips200_showchar(uint16 x, uint16 y, const int8 dat)
{
    uint8 i, j;
    uint8 temp;

    for(i = 0; i < 16; i++)
    {
        ips200_address_set(x, y + i, x + 7, y + i);
        temp = tft_ascii[(uint16)dat - 32][i]; //减32因为是取模是从空格开始取得 空格在ascii中序号是32
        for(j = 0; j < 8; j++)
        {
            if(temp & 0x01)	ips200_wr_data16(IPS200_PENCOLOR);
            else			ips200_wr_data16(IPS200_BGCOLOR);
            temp >>= 1;
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示字符串
//  @param      x     	        坐标x方向的起点  参数范围 0 - (IPS200_X_MAX-1)
//  @param      y     	        坐标y方向的起点  参数范围 0 - (IPS200_Y_MAX/16-1)
//  @param      dat       	    需要显示的字符串
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips200_showstr(uint16 x, uint16 y, const int8 dat[])
{
    uint16 j;

    j = 0;
    while(dat[j] != '\0')
    {
        ips200_showchar(x + 8 * j, y * 16, dat[j]);
        j++;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位有符号
//  @param      x     	        坐标x方向的起点  参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点  参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint8(0,0,x);//x为int8类型
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint8(uint16 x, uint16 y, int8 dat)
{
    uint8 a[3];
    uint8 i;
    uint8 Is0;

    if(dat < 0)
    {
        ips200_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else	ips200_showchar(x, y * 16, ' ');

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while(i < 3)
    {
        if(a[i] != 0) Is0 ++;
        if(a[i] == 0 && Is0 == 0)
            ips200_showchar(x + (8 * (i + 1)), y * 16, ' ');
        else
            ips200_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint8
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showuint8(0,0,x);//x为uint8类型
//-------------------------------------------------------------------------------------------------------------------
void ips200_showuint8(uint16 x, uint16 y, uint8 dat)
{
    uint8 a[3];
    uint8 i;
    uint8 Is0 = 0;

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while(i < 3)
    {
        if(a[i] != 0) Is0 ++;
        if(a[i] == 0 && Is0 == 0)
            ips200_showchar(x + (8 * (i + 1)), y * 16, ' ');
        else
            ips200_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位有符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型int16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint16(0,0,x);//x为int16类型
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint16(uint16 x, uint16 y, int16 dat)
{
    uint8 a[5];
    uint8 i;
    uint8 Is0 = 0;
    if(dat < 0)
    {
        ips200_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else	ips200_showchar(x, y * 16, ' ');

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while(i < 5)
    {
        if(a[i] != 0) Is0 ++;
        if(a[i] == 0 && Is0 == 0)
            ips200_showchar(x + (8 * (i + 1)), y * 16, ' ');
        else
            ips200_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示16位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint16
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showuint16(0,0,x);//x为uint16类型
//-------------------------------------------------------------------------------------------------------------------
void ips200_showuint16(uint16 x, uint16 y, uint16 dat)
{
    uint8 a[5];
    uint8 i;
    uint8 Is0 = 0;

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while(i < 5)
    {
        if(a[i] != 0) Is0 ++;
        if(a[i] == 0 && Is0 == 0)
            ips200_showchar(x + (8 * (i + 1)), y * 16, ' ');
        else
            ips200_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示32位有符号(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型uint32
//  @param      num       	    需要显示的位数 最高10位  不包含正负号
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showint32(0,0,x,3);//x可以为int32 uint16 int16 uint8 int8类型
//  Sample usage:               负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void ips200_showint32(uint16 x, uint16 y, int dat, uint8 num)
{
    int8    buff[34];
    uint32   length;

    if(10 < num)      num = 10;


    num++;
    if(0 > dat)   length = zf_sprintf(&buff[0], "%d", dat); //负数
    else
    {
        buff[0] = ' ';
        length = zf_sprintf(&buff[1], "%d", dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    ips200_showstr(x, y, buff);	//显示数字
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示浮点数(去除整数部分无效的0)
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      dat       	    需要显示的变量，数据类型float或double
//  @param      num       	    整数位显示长度   最高10位
//  @param      pointnum        小数位显示长度   最高6位
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showfloat(0,0,x,2,3);//显示浮点数   整数显示2位   小数显示三位
//  @note                       特别注意当发现小数部分显示的值与你写入的值不一样的时候，
//                              可能是由于浮点数精度丢失问题导致的，这并不是显示函数的问题，
//                              有关问题的详情，请自行百度学习   浮点数精度丢失问题。
//                              负数会显示一个 ‘-’号   正数显示一个空格
//-------------------------------------------------------------------------------------------------------------------
void ips200_showfloat(uint16 x, uint16 y, double dat, int8 num, int8 pointnum)
{
    uint32   length;
    int8    buff[34];
    int8    start, end, point;

    if(6 < pointnum)  pointnum = 6;
    if(10 < num)      num = 10;

    if(0 > dat)   length = zf_sprintf( &buff[0], "%f", dat); //负数
    else
    {
        length = zf_sprintf( &buff[1], "%f", dat);
        length++;
    }
    point = (int8)(length - 7);         //计算小数点位置
    start = point - num - 1;    //计算起始位
    end = point + pointnum + 1; //计算结束位
    while(0 > start) //整数位不够  末尾应该填充空格
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if(0 > dat)   buff[start] = '-';
    else        buff[start] = ' ';

    buff[end] = '\0';

    ips200_showstr(x, y, buff);	//显示数字
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      液晶显示8位无符号
//  @param      x     	        坐标x方向的起点 参数范围 0 -（IPS200_X_MAX-1）
//  @param      y     	        坐标y方向的起点 参数范围 0 -（IPS200_Y_MAX/16-1）
//  @param      w       	    图像宽度
//  @param      l       	    图像高度
//  @param      *p       	    图像数组地址
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_showimage(0,0,10,20,&image);//图像起点(0,0)。宽10，高20。
//-------------------------------------------------------------------------------------------------------------------
void ips200_showimage(uint16 x, uint16 y, uint16 w, uint16 l, const unsigned char *p)
{
    int i;
    unsigned char picH, picL;
    ips200_address_set(x, y, x + w - 1, y + l - 1);
    for(i = 0; i < w * l; i++)
    {
        picL = *(p + i * 2);
        picH = *(p + i * 2 + 1);
        ips200_wr_data16(picH << 8 | picL);
    }
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      总钻风(灰度摄像头)液晶缩放显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @param      start_x         设置显示起点的x轴坐标
//  @param      start_y     	设置显示起点的y轴坐标
//  @param      dis_width       图像显示宽度  1 -（TFT_X_MAX）
//  @param      dis_height      图像显示高度  1 -（TFT_Y_MAX）
//  @return     void
//  @since      v1.0
//  Sample usage:               ips200_displayimage032_zoom1(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, 0, 0, MT9V03X_CSI_W, MT9V03X_CSI_H)//显示灰度摄像头 图像
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y, uint16 dis_width, uint16 dis_height)
{
    uint32 i, j;

    uint16 color = 0;
    uint16 temp = 0;

    //检查设置的参数是否超过屏幕的分辨率
    //	if((start_x+dis_width)>IPS200_X_MAX)	ZF_ASSERT(0);
    //	if((start_y+dis_height)>IPS200_Y_MAX)	ZF_ASSERT(0);

    ips200_address_set(start_x, start_y, start_x + dis_width - 1, start_y + dis_height - 1); //设置显示区域

    for(j = 0; j < dis_height; j++)
    {
        for(i = 0; i < dis_width; i++)
        {
            temp = *(p + (j * height / dis_height) * width + i * width / dis_width); //读取像素点
            color = (0x001f & ((temp) >> 3)) << 11;
            color = color | (((0x003f) & ((temp) >> 2)) << 5);
            color = color | (0x001f & ((temp) >> 3));
            ips200_wr_data16(color);
        }
    }
}




//-------------------------------------------------------------------------------------------------------------------
//  @brief      小钻风(二值化摄像头)液晶显示函数
//  @param      *p     			图像数组地址
//  @param      width     	    图像宽度
//  @param      height     	    图像高度
//  @return     void
//  @since      v1.0
//  @note      	图像分辨率需要设置在120(高)*160(宽)以下
//-------------------------------------------------------------------------------------------------------------------
void ips200_displayimage7725(uint8 *p, uint16 width, uint16 height)
{
    int i, j;

    uint16 temp = 0;
    ips200_address_set(0, 0, width - 1, height - 1);
    for(i = 0; i < (height * width / 8); i++)
    {
        temp = *p;
        p++;
        for(j = 0; j < 8; j++)
        {
            if( (temp << j) & 0x80 )	ips200_wr_data16(WHITE);
            else					ips200_wr_data16(BLACK);
        }
    }
}


//小钻风可选位置显示
void ips200_displayimage7725Zoom(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y)
{
    int i, j;
    uint16 temp = 0;
    ips200_address_set(start_x, start_y, start_x + width - 1, start_y + height - 1); //设置显示区域
    for(i = 0; i < (height * width / 8); i++)
    {
        temp = *p;
        p++;
        for(j = 0; j < 8; j++)
        {
            if( (temp << j) & 0x80 )	ips200_wr_data16(WHITE);
            else					ips200_wr_data16(BLACK);
        }
    }
}

//小钻风可选位置显示  显示的是16位的颜色
void ips200_displayimage7725SetPosi(uint16 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y)
{
    int i;
    ips200_address_set(start_x, start_y, start_x + width - 1, start_y + height - 1); //设置显示区域
    for(i = 0; i < (height * width); i++)
    {
        p ++;
        ips200_wr_data16(*p);
    }
}


//小钻风可选位置显示 黑白翻转
void ips200_displayimage7725SetPosi0(uint8 *p, uint16 width, uint16 height, uint16 start_x, uint16 start_y)
{
    int i, j;
    uint16 temp = 0;
    ips200_address_set(start_x, start_y, start_x + width - 1, start_y + height - 1); //设置显示区域
    for(i = 0; i < (height * width / 8); i++)
    {
        temp = *p;
        p++;
        for(j = 0; j < 8; j++)
        {
            if( (temp << j) & 0x80 )	ips200_wr_data16(BLACK);
            else					ips200_wr_data16(WHITE);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      汉字显示
//  @param      x       横坐标 0-（IPS200_X_MAX-1）
//  @param      y       纵坐标 0-（IPS200_Y_MAX-1）
//  @param      size    取模的时候设置的汉字字体大小，也就是一个汉字占用的点阵长宽为多少个点，取模的时候需要长宽是一样的。
//  @param      *p      需要显示的汉字数组
//  @param      number  需要显示多少位
//  @param      color   显示颜色
//  @return     void
//  @since      v1.0
//  Sample usage:		ips200_display_chinese(0,0,16,chinese_test[0],4,RED);//显示font文件里面的 示例
//  @Note       使用PCtoLCD2002软件取模		    阴码、逐行式、顺向   16*16
//-------------------------------------------------------------------------------------------------------------------
void ips200_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k;
    uint8 temp, temp1, temp2;
    const uint8 *p_data;

    temp2 = size / 8;

    ips200_address_set(x, y, number * size - 1 + x, y + size - 1);

    for(i = 0; i < size; i++)
    {
        temp1 = number;
        p_data = p + i * temp2;
        while(temp1--)
        {
            for(k = 0; k < temp2; k++)
            {
                for(j = 8; j > 0; j--)
                {
                    temp = (*p_data >> (j - 1)) & 0x01;
                    if(temp)    ips200_wr_data16(color);
                    else        ips200_wr_data16(IPS200_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }
    }
}


//-----------------------------------user--------------------------------------------//
void ips200_drawline(uint16 x1, uint16 y1, uint16 x2, uint16 y2, uint16 color)
{
    float x;
    //制作y = kx + p直线
    float k = 0.0000f;
    float p = 0.0000f;
    if(x1 == x2)
    {
        for(int i = MIN(y1, y2); i < MAX(y1, y2); i ++)
        {
            ips200_drawpoint(x1, i, color);
        }
    }
    else if(y1 == y2)
    {
        for(int i = MIN(x1, x2); i < MAX(x1, x2); i ++)
        {
            ips200_drawpoint(i, y1, color);
        }
    }
    else
    {
        k = (float)(y2 - y1) / (float)(x2 - x1);
        p = y1 - k * (float)x1;
        for(int i = MIN(y1, y2); i < MAX(y1, y2); i ++)
        {
            x = (float)(i - p) / k;
            ips200_drawpoint((int16)x, i, color);
        }
    }
}



