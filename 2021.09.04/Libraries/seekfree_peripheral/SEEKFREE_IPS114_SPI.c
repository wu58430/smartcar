/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2018,��ɿƼ�
* All rights reserved.
* ��������QQȺ��һȺ��179029047(����)  ��Ⱥ��244861897
*
* �����������ݰ�Ȩ������ɿƼ����У�δ����������������ҵ��;��
* ��ӭ��λʹ�ò������������޸�����ʱ���뱣����ɿƼ��İ�Ȩ������
*
* @file				SEEKFREE_IPS114_SPI
* @company			�ɶ���ɿƼ����޹�˾
* @author			��ɿƼ�(QQ3184284598)
* @version			�鿴doc��version�ļ� �汾˵��
* @Software			IAR 8.3 or MDK 5.24
* @Taobao			https://seekfree.taobao.com/
* @date				2020-11-23
* @note
* 					���߶��壺
* 					------------------------------------
* 					ģ��ܽ�				��Ƭ���ܽ�
* 					SCL					�鿴IPS114_SCL_PIN�궨�������	Ӳ��SPI���Ų��������л�
* 					SDA					�鿴IPS114_SDA_PIN�궨�������	Ӳ��SPI���Ų��������л�
* 					RES					�鿴IPS114_REST_PIN�궨�������
* 					DC					�鿴IPS114_DC_PIN�궨�������
* 					CS					�鿴IPS114_CS_PIN�궨�������		Ӳ��SPI���Ų��������л�
* 					��Դ����
* 					BL  3.3V��Դ������������ţ�Ҳ���Խ�PWM���������ȣ�
* 					VCC 3.3V��Դ
* 					GND ��Դ��
* 					���ֱ���135*240
* 					------------------------------------
********************************************************************************************************************/

#include "zf_spi.h"
#include "zf_gpio.h"
#include "zf_systick.h"
#include "SEEKFREE_PRINTF.h"
#include "SEEKFREE_IPS114_SPI.h"


///////////////////////////////////////////////////////////////
uint16 IPS114_PENCOLOR = WHITE;
uint16 IPS114_BGCOLOR = BLACK;


//-------------------------------------------------------------------------------------------------------------------
// @brief		д����
// @param		dat				����
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips114_writeIndex (uint8 dat)
{
    IPS114_CS(0);
    IPS114_DC(0);
    spi_mosi(IPS114_SPIN, &dat, NULL, 1);
    IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		д����
// @param		dat				����
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips114_writeData (uint8 dat)
{
    IPS114_CS(0);
    IPS114_DC(1);
    spi_mosi(IPS114_SPIN, &dat, NULL, 1);
    IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		��Һ����дһ��16λ����
// @param		dat				����
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips114_writedata_16bit (uint16 dat)
{
    uint8 dat1[2];
    IPS114_CS(0);
    dat1[0] = dat >> 8;
    dat1[1] = (uint8)dat;

    IPS114_DC(1);
    spi_mosi(IPS114_SPIN, dat1, NULL, 2);									// д������  ��λ��ǰ  ��λ�ں�
    IPS114_CS(1);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		������ʾ����
// @param		x1				��ʼx������
// @param		y1				��ʼy������
// @param		x2				����x������
// @param		y2				����y������
// @return		void
// @since		v1.0
// Sample usage:
// @note		�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips114_set_region (uint16 x1, uint16 y1, uint16 x2, uint16 y2)
{
    if(IPS114_DISPLAY_DIR == 0)
    {
        ips114_writeIndex(0x2a);											// �е�ַ����
        ips114_writedata_16bit(x1 + 52);
        ips114_writedata_16bit(x2 + 52);
        ips114_writeIndex(0x2b);											// �е�ַ����
        ips114_writedata_16bit(y1 + 40);
        ips114_writedata_16bit(y2 + 40);
        ips114_writeIndex(0x2c);											// ������д
    }
    else if(IPS114_DISPLAY_DIR == 1)
    {
        ips114_writeIndex(0x2a);											// �е�ַ����
        ips114_writedata_16bit(x1 + 53);
        ips114_writedata_16bit(x2 + 53);
        ips114_writeIndex(0x2b);											// �е�ַ����
        ips114_writedata_16bit(y1 + 40);
        ips114_writedata_16bit(y2 + 40);
        ips114_writeIndex(0x2c);											// ������д
    }
    else if(IPS114_DISPLAY_DIR == 2)
    {
        ips114_writeIndex(0x2a);											// �е�ַ����
        ips114_writedata_16bit(x1 + 40);
        ips114_writedata_16bit(x2 + 40);
        ips114_writeIndex(0x2b);											// �е�ַ����
        ips114_writedata_16bit(y1 + 53);
        ips114_writedata_16bit(y2 + 53);
        ips114_writeIndex(0x2c);											// ������д
    }
    else
    {
        ips114_writeIndex(0x2a);											// �е�ַ����
        ips114_writedata_16bit(x1 + 40);
        ips114_writedata_16bit(x2 + 40);
        ips114_writeIndex(0x2b);											// �е�ַ����
        ips114_writedata_16bit(y1 + 52);
        ips114_writedata_16bit(y2 + 52);
        ips114_writeIndex(0x2c);											// ������д
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		1.14�� IPSҺ����ʼ��
// @return		void
// @since		v1.0
// Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void ips114_init(void)
{
    spi_init(IPS114_SPIN, IPS114_SCL_PIN, IPS114_SDA_PIN, SPI_MISO_NULL, SPI_NSS_NULL, 0, SystemCoreClock / 2);	//Ӳ��SPI��ʼ��

    gpio_init(IPS114_CS_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);
    gpio_init(IPS114_DC_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_REST_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);
    gpio_init(IPS114_BL_PIN, GPO, GPIO_HIGH, GPO_PUSH_PULL);

    IPS114_REST(0);
    systick_delay_ms(200);

    IPS114_REST(1);
    systick_delay_ms(100);

    ips114_writeIndex(0x36);
    systick_delay_ms(100);
    if		(IPS114_DISPLAY_DIR == 0)	ips114_writeData(0x00);
    else if (IPS114_DISPLAY_DIR == 1)	ips114_writeData(0xC0);
    else if (IPS114_DISPLAY_DIR == 2)	ips114_writeData(0x70);
    else							ips114_writeData(0xA0);

    ips114_writeIndex(0x3A);
    ips114_writeData(0x05);

    ips114_writeIndex(0xB2);
    ips114_writeData(0x0C);
    ips114_writeData(0x0C);
    ips114_writeData(0x00);
    ips114_writeData(0x33);
    ips114_writeData(0x33);

    ips114_writeIndex(0xB7);
    ips114_writeData(0x35);

    ips114_writeIndex(0xBB);
    ips114_writeData(0x37);

    ips114_writeIndex(0xC0);
    ips114_writeData(0x2C);

    ips114_writeIndex(0xC2);
    ips114_writeData(0x01);

    ips114_writeIndex(0xC3);
    ips114_writeData(0x12);

    ips114_writeIndex(0xC4);
    ips114_writeData(0x20);

    ips114_writeIndex(0xC6);
    ips114_writeData(0x0F);

    ips114_writeIndex(0xD0);
    ips114_writeData(0xA4);
    ips114_writeData(0xA1);

    ips114_writeIndex(0xE0);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0D);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2B);
    ips114_writeData(0x3F);
    ips114_writeData(0x54);
    ips114_writeData(0x4C);
    ips114_writeData(0x18);
    ips114_writeData(0x0D);
    ips114_writeData(0x0B);
    ips114_writeData(0x1F);
    ips114_writeData(0x23);

    ips114_writeIndex(0xE1);
    ips114_writeData(0xD0);
    ips114_writeData(0x04);
    ips114_writeData(0x0C);
    ips114_writeData(0x11);
    ips114_writeData(0x13);
    ips114_writeData(0x2C);
    ips114_writeData(0x3F);
    ips114_writeData(0x44);
    ips114_writeData(0x51);
    ips114_writeData(0x2F);
    ips114_writeData(0x1F);
    ips114_writeData(0x1F);
    ips114_writeData(0x20);
    ips114_writeData(0x23);

    ips114_writeIndex(0x21);

    ips114_writeIndex(0x11);
    systick_delay_ms(120);

    ips114_writeIndex(0x29);
    ips114_clear(IPS114_BGCOLOR);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����������
// @param		color			��ɫ����
// @return		void
// @since		v1.0
// Sample usage:				ips114_clear(YELLOW);// ȫ������Ϊ��ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips114_clear(uint16 color)
{
    uint16 i, j;
    ips114_set_region(0, 0, IPS114_X_MAX - 1, IPS114_Y_MAX - 1);
    for(i = 0; i < IPS114_X_MAX; i++)
    {
        for (j = 0; j < IPS114_Y_MAX; j++)
        {
            ips114_writedata_16bit(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ������
// @param		x				����x��������
// @param		y				����y��������
// @param		dat				��Ҫ��ʾ����ɫ
// @return		void
// @since		v1.0
// Sample usage:				ips114_drawpoint(0,0,RED);  //����0,0��һ����ɫ�ĵ�
//-------------------------------------------------------------------------------------------------------------------
void ips114_drawpoint(uint16 x, uint16 y, uint16 color)
{
    x >= 240 ? x = 239 : x ;
    y >= 135 ? y = 134 : y;
    ips114_set_region(x, y, x, y);
    ips114_writedata_16bit(color);
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ�ַ�
// @param		x				����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ���ַ�
// @return		void
// @since		v1.0
// Sample usage:				ips114_showchar(0,0,'x');//����0,0дһ���ַ�x
//-------------------------------------------------------------------------------------------------------------------
void ips114_showchar(uint16 x, uint16 y, const int8 dat)
{
    uint8 i, j;
    uint8 temp;

    for(i = 0; i < 16; i++)
    {
        ips114_set_region(x, y + i, x + 7, y + i);
        temp = tft_ascii[dat - 32][i];										// ��32��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո���ascii�������32
        for(j = 0; j < 8; j++)
        {
            if(temp & 0x01)
                ips114_writedata_16bit(IPS114_PENCOLOR);
            else
                ips114_writedata_16bit(IPS114_BGCOLOR);
            temp >>= 1;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ�ַ���
// @param		x				����x��������  ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y��������  ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ���ַ���
// @return		void
// @since		v1.0
// Sample usage:				ips114_showstr(0,0,"seekfree");
//-------------------------------------------------------------------------------------------------------------------
void ips114_showstr(uint16 x, uint16 y, const uint8 dat[])
{
    uint16 j;

    j = 0;
    while(dat[j] != '\0')
    {
        ips114_showchar(x + 8 * j, y * 16, dat[j]);
        j++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ8λ�з���
// @param		x				����x��������  ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y��������  ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ�ı�������������int8
// @return		void
// @since		v1.0
// Sample usage:				ips114_showint8(0,0,x);//xΪint8����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint8(uint16 x, uint16 y, int8 dat)
{
    uint8 a[3];
    uint8 i;
    if(dat < 0)
    {
        ips114_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else	ips114_showchar(x, y * 16, ' ');

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while(i < 3)
    {
        ips114_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ8λ�޷���
// @param		x				����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ�ı�������������uint8
// @return		void
// @since		v1.0
// Sample usage:				ips114_showuint8(0,0,x);//xΪuint8����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint8(uint16 x, uint16 y, uint8 dat)
{
    uint8 a[3];
    uint8 i;

    a[0] = dat / 100;
    a[1] = dat / 10 % 10;
    a[2] = dat % 10;
    i = 0;
    while(i < 3)
    {
        ips114_showchar(x + (8 * i), y * 16, '0' + a[i]);
        i++;
    }

}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ16λ�з���
// @param		x		����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y		����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat		��Ҫ��ʾ�ı�������������int16
// @return		void
// @since		v1.0
// Sample usage:		ips114_showint16(0,0,x);//xΪint16����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint16(uint16 x, uint16 y, int16 dat)
{
    uint8 a[5];
    uint8 i;
    if(dat < 0)
    {
        ips114_showchar(x, y * 16, '-');
        dat = -dat;
    }
    else	ips114_showchar(x, y * 16, ' ');

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while(i < 5)
    {
        ips114_showchar(x + (8 * (i + 1)), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ16λ�޷���
// @param		x				����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ�ı�������������uint16
// @return		void
// @since		v1.0
// Sample usage:				ips114_showuint16(0,0,x);//xΪuint16����
//-------------------------------------------------------------------------------------------------------------------
void ips114_showuint16(uint16 x, uint16 y, uint16 dat)
{
    uint8 a[5];
    uint8 i;

    a[0] = dat / 10000;
    a[1] = dat / 1000 % 10;
    a[2] = dat / 100 % 10;
    a[3] = dat / 10 % 10;
    a[4] = dat % 10;

    i = 0;
    while(i < 5)
    {
        ips114_showchar(x + (8 * i), y * 16, '0' + a[i]);
        i++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ32λ�з���(ȥ������������Ч��0)
// @param		x				����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ�ı�������������uint32
// @param		num				��Ҫ��ʾ��λ�� ���10λ  ������������
// @return		void
// @since		v1.0
// Sample usage:				ips114_showuint32(0,0,x,3);//x����Ϊint32 uint16 int16 uint8 int8����
// Sample usage:				��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_showint32(uint16 x, uint16 y, int32 dat, uint8 num)
{
    int8    buff[34];
    uint8   length;

    if(10 < num)      num = 10;

    num++;
    if(0 > dat)   length = zf_sprintf( &buff[0], "%d", dat);					// ����
    else
    {
        buff[0] = ' ';
        length = zf_sprintf( &buff[1], "%d", dat);
        length++;
    }
    while(length < num)
    {
        buff[length] = ' ';
        length++;
    }
    buff[num] = '\0';

    ips114_showstr(x, y, buff);												// ��ʾ����
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		Һ����ʾ������(ȥ������������Ч��0)
// @param		x				����x�������� ������Χ 0 -��IPS114_X_MAX-1��
// @param		y				����y�������� ������Χ 0 -��IPS114_Y_MAX/16-1��
// @param		dat				��Ҫ��ʾ�ı�������������float��double
// @param		num				����λ��ʾ����   ���10λ
// @param		pointnum		С��λ��ʾ����   ���6λ
// @return		void
// @since		v1.0
// Sample usage:				ips114_showfloat(0,0,x,2,3);//��ʾ������   ������ʾ2λ   С����ʾ��λ
// @note						�ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
// 								���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
// 								�й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
// 								��������ʾһ�� ��-����   ������ʾһ���ո�
//-------------------------------------------------------------------------------------------------------------------
void ips114_showfloat(uint16 x, uint16 y, double dat, uint8 num, uint8 pointnum)
{
    uint8	length;
    int8	buff[34];
    int8	start, end, point;

    if(6 < pointnum)
        pointnum = 6;
    if(10 < num)
        num = 10;

    if(0 > dat)   length = zf_sprintf( &buff[0], "%f", dat);					// ����
    else
    {
        length = zf_sprintf( &buff[1], "%f", dat);
        length++;
    }
    point = length - 7;														// ����С����λ��
    start = point - num - 1;												// ������ʼλ
    end = point + pointnum + 1;												// �������λ
    while(0 > start)															// ����λ����  ĩβӦ�����ո�
    {
        buff[end] = ' ';
        end++;
        start++;
    }

    if(0 > dat)
        buff[start] = '-';
    else
        buff[start] = ' ';

    buff[end] = '\0';

    ips114_showstr(x, y, buff);	//��ʾ����
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		�����(�Ҷ�����ͷ)Һ��������ʾ����
// @param		*p				ͼ�������ַ
// @param		width			ͼ�����
// @param		height			ͼ��߶�
// @param		dis_width		ͼ����ʾ����  1 -��IPS114_X_MAX��
// @param		dis_height		ͼ����ʾ�߶�  1 -��IPS114_Y_MAX��
// @return		void
// @since		v1.0
// Sample usage:		ips114_displayimage032_zoom(mt9v03x_csi_image[0], MT9V03X_CSI_W, MT9V03X_CSI_H, MT9V03X_CSI_W, MT9V03X_CSI_H)//��ʾ�Ҷ�����ͷ ͼ��
// @note				ͼ��Ŀ����������Һ���Ŀ��ȣ����Զ�����������ʾ������������ʾȫ��Ұ
//-------------------------------------------------------------------------------------------------------------------
void ips114_displayimage032_zoom(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i, j;

    uint16 color = 0;
    uint16 temp = 0;

    ips114_set_region(0, 0, dis_width - 1, dis_height - 1); //������ʾ����

    for(j = 0; j < dis_height; j++)
    {
        for(i = 0; i < dis_width; i++)
        {
            temp = *(p + (j * height / dis_height) * width + i * width / dis_width);		// ��ȡ���ص�
            color = (0x001f & ((temp) >> 3)) << 11;
            color = color | (((0x003f) & ((temp) >> 2)) << 5);
            color = color | (0x001f & ((temp) >> 3));
            ips114_writedata_16bit(color);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------
// @brief		С���(��ֵ������ͷ)Һ����ʾ����
// @param		*p				ͼ�������ַ
// @param		width			ͼ�����
// @param		height			ͼ��߶�
// @return		void
// @since		v1.0
// @note		ͼ��ֱ�����Ҫ������120(��)*160(��)����
//-------------------------------------------------------------------------------------------------------------------
void ips114_displayimage7725(uint8 *p, uint16 width, uint16 height)
{
    int i, j, k;

    uint16 temp = 0;
    ips114_set_region(0, 0, width - 1, height - 1);
    for(i = 0; i < height - 60; i++)
    {
        for(k = 0; k < width / 8; k ++)
        {
            temp = *p;
            p++;
            for(j = 0; j < 8; j++)
            {
                if( (temp << j) & 0x80 )
                    ips114_writedata_16bit(BLACK);
                else
                    ips114_writedata_16bit(WHITE);
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// @brief		������ʾ
// @param		x				������ 0-��IPS114_X_MAX-1��
// @param		y				������ 0-��IPS114_Y_MAX-1��
// @param		size			ȡģ��ʱ�����õĺ��������С��Ҳ����һ������ռ�õĵ��󳤿�Ϊ���ٸ��㣬ȡģ��ʱ����Ҫ������һ���ġ�
// @param		*p				��Ҫ��ʾ�ĺ�������
// @param		number			��Ҫ��ʾ����λ
// @param		color			��ʾ��ɫ
// @return		void
// @since		v1.0
// Sample usage:				ips114_display_chinese(0,0,16,chinese_test[0],4,RED);//��ʾfont�ļ������ ʾ��
// @Note		ʹ��PCtoLCD2002����ȡģ	���롢����ʽ��˳��	16*16
//-------------------------------------------------------------------------------------------------------------------
void ips114_display_chinese(uint16 x, uint16 y, uint8 size, const uint8 *p, uint8 number, uint16 color)
{
    int i, j, k;
    uint8 temp, temp1, temp2;
    const uint8 *p_data;

    temp2 = size / 8;

    ips114_set_region(x, y, number * size - 1 + x, y + size - 1);

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
                    if(temp)
                        ips114_writedata_16bit(color);
                    else
                        ips114_writedata_16bit(IPS114_BGCOLOR);
                }
                p_data++;
            }
            p_data = p_data - temp2 + temp2 * size;
        }
    }
}


//==============================================USER==================================================//




/***************************************************************
 * @team      : nefu_fengying
 * @editor    : zhaoyanguo
 * @name      : void ips114_display_myimage(uint8 *p, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
 * @param     : NULL
 * @return    : NULL
 * @function  : NULL
 * @invocation: ips114_display_myimage(&ImageUaed,80,160,80,160);
***************************************************************/
void ips114_display_myimage(uint16 *p, uint16 posi_x, uint16 posi_y, uint16 width, uint16 height, uint16 dis_width, uint16 dis_height)
{
    uint32 i, j;

    ips114_set_region(posi_x, posi_y, posi_x + dis_width - 1, posi_y + dis_height - 1); //������ʾ����

    for(j = 0; j < dis_height; j++)
    {
        for(i = 0; i < dis_width; i++)
        {
            ips114_writedata_16bit(*(p + (j * height / dis_height)*width + i * width / dis_width));
        }
    }
}


//���ƴ�ֱ��X���ֱ�� size�ǰ����м������ȥ�ľ���
void ips114_DrawXLine(int16 x, uint16 color, uint16 size)
{
    int16 _y = 0;
    for(_y = 0; _y < 135; _y ++)
    {
        if(size == 1)
            ips114_drawpoint(x, _y, color);
        else if(size == 3)
        {
            ips114_drawpoint(x - 1, _y, color);
            ips114_drawpoint(x, _y, color);
            ips114_drawpoint(x + 1, _y, color);
        }
        else if(size == 5)
        {
            ips114_drawpoint(x - 2, _y, color);
            ips114_drawpoint(x - 1, _y, color);
            ips114_drawpoint(x, _y, color);
            ips114_drawpoint(x + 1, _y, color);
            ips114_drawpoint(x + 2, _y, color);
        }
    }
}

//����һ����ֱ��Y���ֱ��
void ips114_DrawYLine(int16 y, uint16 color, uint16 size)
{
    int16 _x = 0;
    for(_x = 0; _x < 240; _x ++)
    {
        if(size == 1)
            ips114_drawpoint(_x, y, color);
        else if(size == 3)
        {
            ips114_drawpoint(_x, y - 1, color);
            ips114_drawpoint(_x, y, color);
            ips114_drawpoint(_x, y + 1, color);
        }
        else if(size == 5)
        {
            ips114_drawpoint(_x, y - 2, color);
            ips114_drawpoint(_x, y - 1, color);
            ips114_drawpoint(_x, y, color);
            ips114_drawpoint(_x, y + 1, color);
            ips114_drawpoint(_x, y + 2, color);
        }
    }
}




