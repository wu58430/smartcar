/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				SEEKFREE_FONT
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
* @note
********************************************************************************************************************/

#ifndef _SEEKFREE_FONT_h
#define _SEEKFREE_FONT_h

#include "common.h"

//-------常用颜色----------
#define   RED		0xF800		  // 红色
#define   BLUE		0x001F		  // 蓝色
#define   YELLOW	0xFFE0		  // 黄色
#define   GREEN		0x07E0		  // 绿色
#define   WHITE		0xFFFF		  // 白色
#define   BLACK		0x0000	      // 黑色 
#define   GRAY		0X8430	      // 灰色
#define   BROWN		0XBC40		  // 棕色
#define   PURPLE	0XF81F		  // 紫色
#define   PINK		0XFE19	      // 粉色   
#define   NAVY      0x000F        // 深蓝色  
#define   DGREEN    0x03E0        // 深绿色  
#define   DCYAN     0x03EF        // 深青色  
#define   MAROON    0x7800        // 深红色       
#define   OLIVE     0x7BE0        // 橄榄绿      
#define   LGRAY     0xC618        // 灰白色
#define   DGRAY     0x7BEF        // 深灰色               
#define   CYAN      0x07FF        // 青色        
#define   MAGENTA   0xF81F        // 品红     
#define   CAMEOBROWN 0xC779		  // 豆沙色



extern const uint8 tft_ascii[95][16];
extern const uint8 chinese_test[8][16];

#endif
