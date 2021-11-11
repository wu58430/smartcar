/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2019,逐飞科技
* All rights reserved.
* 技术讨论QQ群：一群：179029047(已满)  二群：244861897
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file				SEEKFREE_PRINTF
* @company			成都逐飞科技有限公司
* @author			逐飞科技(QQ3184284598)
* @version			查看doc内version文件 版本说明
* @Software			IAR 8.3 or MDK 5.24
* @Target core		MM32SPIN2XPs
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
********************************************************************************************************************/


#include <stdarg.h>
#include <string.h>
#include "zf_uart.h"
#include "SEEKFREE_PRINTF.h"


uint8 number_conversion_ascii(uint32 dat, int8 *p, uint8 neg_type, uint8 radix)
{
    int32   neg_dat;
    uint32  pos_dat;
    uint8   temp_data = 0;
    uint8   valid_num = 0;

    if(neg_type)
    {
        neg_dat = (int32)dat;
        if(neg_dat < 0)   neg_dat = -neg_dat;
        while(1)
        {
            *p = neg_dat % radix + '0';
            neg_dat = neg_dat / radix;
            valid_num++;

            if(!neg_dat) break;
            p++;
        }
    }
    else
    {
        pos_dat = dat;
        while(1)
        {
            temp_data = pos_dat % radix;
            if(10 <= temp_data)	temp_data += 'A' - 10;
            else				temp_data += '0';

            *p = temp_data;

            pos_dat = pos_dat / radix;
            valid_num++;

            if(!pos_dat) break;
            p++;
        }
    }
    return valid_num;
}

void printf_reverse_order(int8 *d_buff, uint32 len)
{
    uint32 i;
    int8  temp_data;
    for(i = 0; i < len / 2; i++)
    {
        temp_data = d_buff[len - 1 - i];
        d_buff[len - 1 - i] = d_buff[i];
        d_buff[i] = temp_data;
    }
}

uint32 zf_sprintf(int8 *buff, const int8 *format, ...)
{
    uint32 buff_len = 0;
    va_list arg;
    va_start(arg, format);

    while (*format)
    {
        int8 ret = *format;
        if (ret == '%')
        {
            switch (*++format)
            {
            case 'a':// 十六进制p计数法输出浮点数 暂未实现
            {
            }
            break;

            case 'c':// 一个字符
            {
                int8 ch = (int8)va_arg(arg, uint32);
                *buff = ch;
                buff++;
                buff_len++;
            }
            break;

            case 'd':
            case 'i':// 有符号十进制整数
            {
                int8 vstr[33];
                int32 ival = (int32)va_arg(arg, int32);
                uint8 vlen = number_conversion_ascii((uint32)ival, vstr, 1, 10);

                if(ival < 0)
                {
                    vstr[vlen] = '-';
                    vlen++;
                }
                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 'f':// 浮点数，输出小数点后六位  不能指定输出精度
            case 'F':// 浮点数，输出小数点后六位  不能指定输出精度
            {
                int8 vstr[33];
                double ival = (double)va_arg(arg, double);
                uint8 vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);

                if(ival < 0)
                {
                    vstr[vlen] = '-';
                    vlen++;
                }
                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;

                ival = ((double)ival - (int32)ival) * 1000000;
                if(ival)
                {
                    vlen = number_conversion_ascii((uint32)(int32)ival, vstr, 1, 10);
                }
                else
                {
                    vstr[0] = vstr[1] = vstr[2] = vstr[3] = vstr[4] = vstr[5] = '0';
                    vlen = 6;
                }

                while(6 > vlen)
                {
                    vstr[vlen] = '0';
                    vlen++;
                }

                vstr[vlen] = '.';
                vlen++;

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff_len += vlen;
            }
            break;

            case 'u':// 无符号十进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 10);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 'o':// 无符号八进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 8);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;

            }
            break;

            case 'x':// 无符号十六进制整数
            case 'X':// 无符号十六进制整数
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                printf_reverse_order(vstr, vlen);
                memcpy(buff, vstr, vlen);
                buff += vlen;
                buff_len += vlen;
            }
            break;

            case 's':// 字符串
            {
                int8 *pc = va_arg(arg, int8 *);
                while (*pc)
                {
                    *buff = *pc;
                    buff++;
                    buff_len++;
                    pc++;
                }
            }
            break;

            case 'p':// 以16进制形式输出指针
            {
                int8 vstr[33];
                uint32 ival = (uint32)va_arg(arg, uint32);
                uint8 vlen = number_conversion_ascii(ival, vstr, 0, 16);

                printf_reverse_order(vstr, 8);
                memcpy(buff, vstr, 8);
                buff += 8;
                buff_len += 8;
            }
            break;

            case '%':// 输出字符%
            {
                *buff = '%';
                buff++;
                buff_len++;
            }
            break;

            default:
                break;
            }
        }
        else
        {
            *buff = (int8)(*format);
            buff++;
            buff_len++;
        }
        format++;
    }
    va_end(arg);

    return buff_len;
}
