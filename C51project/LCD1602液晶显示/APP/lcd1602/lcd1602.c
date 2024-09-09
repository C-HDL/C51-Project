#include "lcd1602.h"

void lcd1602_write_cmd(u8 cmd)
{
	LCD1602_RS=0;
	LCD1602_RW=0;
	LCD1602_E=0;
	LEC1602_DATAPORT=cmd;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}

void lcd1602_write_data(u8 dat)
{
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_E=0;
	LEC1602_DATAPORT=dat;
	delay_ms(1);
	LCD1602_E=1;
	delay_ms(1);
	LCD1602_E=0;
}
void lcd1602_init()
{
	 lcd1602_write_cmd(0x38);
	 lcd1602_write_cmd(0x0c);
	 lcd1602_write_cmd(0x06);
	 lcd1602_write_cmd(0x01);
}

void lcd1602_show_string(u8 col,u8 row,u8 *str)		 
{
	u8 i=0;
	if(row>1||col>15)return;
	if(row<1)
	{
		while(*str!='\0')
		{
			if(i<16-col)
				lcd1602_write_cmd(0x80+col+i);
			else
				lcd1602_write_cmd(0x80+0x40+col+i-16);
			lcd1602_write_data(*str);
			str++;
			i++;
			
		}
	}
	else
	{
			while(*str!='\0')
		{
			if(i<16-col)
				lcd1602_write_cmd(0x80+0x40+col+i);
			else
				lcd1602_write_cmd(0x80+col+i-16);
			lcd1602_write_data(*str);
			str++;
			i++;
			
		}
	}	
}
void lcd1602_clear(void)
{
	lcd1602_write_cmd(0x01);
}
