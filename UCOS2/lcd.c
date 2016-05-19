#include "font_8x16.h"
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "s5pv210.h"
#include "bmp.h"

extern unsigned char __CHS[];
extern unsigned char __VGA[];

// FB��ַ
int gFB_Addr = 0x23000000;
int gRow,gCol;
int gLeftTopX, gLeftTopY, gRightBotX, gRightBotY;
unsigned char *gImage_bmp;


// ��ʼ��LCD
void Lcd_Init(int lcd_type)
{	
	int hspw,hbpd,hfpd,vspw,vbpd,vfpd,vsync,hsync; 			

	// ������������LCD����
	GPF0CON = 0x22222222;
	GPF1CON = 0x22222222;
	GPF2CON = 0x22222222;
	GPF3CON = 0x22222222;

	// �򿪱���
	GPD0CON &= ~(0xf<<4);
	GPD0CON |= (1<<4);
	GPD0DAT |= (1<<1);

	// 10: RGB=FIMD I80=FIMD ITU=FIMD
	DISPLAY_CONTROL = 2<<0;

	// bit[26~28]:ʹ��RGB�ӿ�
	// bit[18]:RGB ����
	// bit[2]:ѡ��ʱ��ԴΪHCLK_DSYS=166MHz
	VIDCON0 &= ~( (3<<26)|(1<<18)|(1<<2) );

	// bit[1]:ʹ��lcd������
	// bit[0]:��ǰ֡������ʹ��lcd������
	VIDCON0 |= ( (1<<0)|(1<<1) );

	// bit[6]:ѡ����Ҫ��Ƶ
	// bit[6~13]:��Ƶϵ��Ϊ15����VCLK = 166M/(6+1)
	VIDCON0 |= 6<<6 | 1<<4;

	gLeftTopX = 0;
	gLeftTopY = 0;

	switch(lcd_type)
	{
		// h43
		case 0x13:
		case 0xe:
		{
			gImage_bmp = gImage_480272;

			gRow=272;
			gCol=480;	
			gRightBotX  = gCol-1;
			gRightBotY  = gRow-1;

			hspw = (0);
			hbpd = (40 - 1);
			hfpd = (5 - 1);
			vspw = (0);
			vbpd = (8 - 1);
			vfpd = (8 - 1);

			vsync = 1;
			hsync = 1;
			
			break;
		}
		// n43
		case 0x1:
		{
			gImage_bmp = gImage_480272;

			gRow=272;
			gCol=480;	
			gRightBotX  = gCol-1;
			gRightBotY  = gRow-1;

			hspw = 0x6;
			hbpd = 0x2d;
			hfpd = 0x4;
			vspw = 0x2;
			vbpd = 0x3;
			vfpd = 0x2;

			vsync = 0;
			hsync = 0;
			
			break;
		}
		// s70
		case 0x3:
		{
			gImage_bmp = gImage_800480;

			gRow=480;
			gCol=800;	
			gRightBotX  = gCol-1;
			gRightBotY  = gRow-1;

			hspw = 0x2;
			hbpd = 0x2c;
			hfpd = 0xd2;
			vspw = 0x02;
			vbpd = 0x15;
			vfpd = 0x16;

			vsync = 1;
			hsync = 1;
			
			break;
		}
		// A70
		case 0x2:
		{
			gImage_bmp = gImage_800480;

			gRow=480;
			gCol=800;	
			gRightBotX  = gCol-1;
			gRightBotY  = gRow-1;

			hspw = 0x30;
			hbpd = 0x28;
			hfpd = 0x28;
			vspw = 0x18;
			vbpd = 0x1d;
			vfpd = 0x11;

			vsync = 1;
			hsync = 1;
			
			break;
		}
		// W50
		case 0x4:
		{
			gImage_bmp = gImage_800480;

			gRow=480;
			gCol=800;	
			gRightBotX  = gCol-1;
			gRightBotY  = gRow-1;

			hspw = 48;
			hbpd = 40;
			hfpd = 40;
			vspw = 12;
			vbpd = 20;
			vfpd = 20;

			vsync = 1;
			hsync = 1;
			
			break;
		}
	}

	// ����ʱ��
	VIDCON1 |= vsync<<5 | hsync<<6;
	VIDTCON0 = vbpd<<16 | vfpd<<8 | vspw<<0;
	VIDTCON1 = hbpd<<16 | hfpd<<8 | hspw<<0;
	// ���ó���
	VIDTCON2 = (gRightBotY << 11) | (gRightBotX << 0);
	
	// ����windows1
	// bit[0]:ʹ��
	// bit[2~5]:24bpp
	WINCON0 |= 1<<0;
	WINCON0 &= ~(0xf << 2);
	WINCON0 |= (0xB<<2) | (1<<15);

	// ����windows1����������
	VIDOSD0A = (gLeftTopX<<11) | (gLeftTopY << 0);
	VIDOSD0B = (gRightBotX<<11) | (gRightBotY << 0);
	VIDOSD0C = (gRightBotY + 1) * (gRightBotX + 1);

	// ����fb�ĵ�ַ
	VIDW00ADD0B0 = gFB_Addr;
	VIDW00ADD1B0 = (((gRightBotX + 1)*4 + 0) * (gRightBotY + 1)) & (0xffffff);

	// ʹ��channel 0��������
	SHADOWCON = 0x1;
}


// ���
void lcd_draw_pixel(int row, int col, int color)
{
	unsigned long * pixel = (unsigned long  *)gFB_Addr;

	*(pixel + row * gCol + col) = color;

}

// ����
void lcd_clear_screen(int color)
{
	int i, j;

	for (i = 0; i < gRow; i++)
		for (j = 0; j < gCol; j++)
			lcd_draw_pixel(i, j, color);

}

// ������
void lcd_draw_hline(int row, int col1, int col2, int color)
{
	int j;

	// ���row�У���j��
	for (j = col1; j <= col2; j++)
		lcd_draw_pixel(row, j, color);

}

// ������
void lcd_draw_vline(int col, int row1, int row2, int color)
{
	int i;
	// ���i�У���col��
	for (i = row1; i <= row2; i++)
		lcd_draw_pixel(i, col, color);

}

// ��ʮ��
void lcd_draw_cross(int row, int col, int halflen, int color)
{
	lcd_draw_hline(row, col-halflen, col+halflen, color);
	lcd_draw_vline(col, row-halflen, row+halflen, color);
}

// ���ַ�
void lcd_draw_char(unsigned char c)
{
	// �����Ǿ�̬����
	static int x = 0;	// �ڼ���
	static int y = 0;	// �ڼ���

	int i,j;
	unsigned char line_dots;

	// �����ģ
	unsigned char *char_dots = (unsigned char *) (fontdata_8x16 + c * 16);


	// �Ƿ���Ҫ�س�����
	if (c == '\n')
	{
		y += 16;
		if (y > gRow)
			y = 0;
		return ;
	}
	else if (c == '\r')
	{
		x = 0;
		return;
	}

	for (i = 0; i < 16; i++)
	{
		line_dots = char_dots[i];
		for (j = 0; j < 8; j++)
		{
			// Ϊ1,��������
			if (line_dots & (0x80 >> j))
			{
				lcd_draw_pixel(y+i, x+j, 0xff);
			}
		}
	}

	// ����ƶ�����һ��8*16��λ��
	x += 8;
	if (x > gCol)
	{
		x = 0;
		y += 16;
		if (y > gRow)
			y = 0;
	}
}


//extern int gLCD_Type;
void Lcd_Draw_Bmp(const unsigned char gImage_bmp[], int lcd_type)
{
	int i, j, offset;
	unsigned char *p = (unsigned char *)gImage_bmp;
	int blue, green, red;
	int color;

	lcd_clear_screen(0x0);

	if(lcd_type == 0x3 || lcd_type == 0x4)
		offset = 2;
	else
		offset = 0;
		
	for (i = 16; i < gRow-16-offset; i++)
		for (j = 0; j < gCol; j++)
		{
			blue  = *p++;
			green = *p++;
			red   = *p++;
		
			color = red << 16 | green << 8 | blue << 0;
			lcd_draw_pixel(i, j, color);
		}

}

/**************************************************************
��LCD��Ļ��ָ�������дASCII��
**************************************************************/
void Lcd_PutASCII(unsigned int x,unsigned int y,unsigned char ch,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;
	
	

	pZK = &__VGA[ch*16];
	for( i = 0 ; i < 16 ; i++ )
	{
		mask = 0x80;
		buf = pZK[i];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                lcd_draw_pixel(x+i,y+j,c);
            }else
            {
                if( !st )
                {
                    lcd_draw_pixel(x+i,y+j,bk_c);
                }
            }
            
            mask = mask >> 1;
        }
	}
}


/**************************************************************
��LCD��Ļ��ָ�������д����
**************************************************************/
void Lcd_PutHZ(unsigned int x,unsigned int y,unsigned short int QW,unsigned int c,unsigned int bk_c,unsigned int st)
{
	unsigned short int i,j;
	unsigned char *pZK,mask,buf;

	pZK = &__CHS[ (  ( (QW >> 8) - 1 )*94 + (QW & 0x00FF)- 1 )*32 ];
	for( i = 0 ; i < 16 ; i++ )
	{
		//��
		mask = 0x80;
        buf = pZK[i*2];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                lcd_draw_pixel(x+i,y+j,c);
            }else
            {
                if( !st )
                {
                    lcd_draw_pixel(x+i,y+j,bk_c);
                }
            }
            mask = mask >> 1;
        } 
        
		//��
		mask = 0x80;
        buf = pZK[i*2 + 1];
        for( j = 0 ; j < 8 ; j++ )
        {
            if( buf & mask )
            {
                lcd_draw_pixel(x+i ,y+j+8, c);
            }else
            {
                if( !st )
                {
                    lcd_draw_pixel(x+i ,y+j+8,bk_c);
                }
            }
            mask = mask >> 1;
        }                 
	}
}


void Lcd_Printf(unsigned int x,unsigned int y,unsigned int c,unsigned int bk_c,unsigned int st,char *fmt,...)
{
    char __LCD_Printf_Buf[256];
    va_list ap;
	unsigned char *pStr = (unsigned char *)__LCD_Printf_Buf;
    unsigned int i = 0;

    va_start(ap,fmt);
    vsprintf(__LCD_Printf_Buf,fmt,ap);
    va_end(ap);
	 
    while(*pStr != 0 )
	{
		switch(*pStr)
		{
			case '\n' :
				{
			
                    break;
				}

			default:
				{
					if( *pStr > 0xA0 & *(pStr+1) > 0xA0 )  //�������
                    {
                        Lcd_PutHZ( x , y , (*pStr - 0xA0)*0x0100 + *(pStr+1) - 0xA0 , c , bk_c , st);

                        pStr++;
                        i++;

                        y += 16;
                    }else               //Ӣ�����
                    {
                        Lcd_PutASCII( x , y , *pStr , c , bk_c , st );

                        y += 8;

                    }

                    break;
				}
		}
		
		pStr++;
        i++;		

        if( i > 256 ) break;
	}
   
}


