#ifndef _lcd1602_H
#define _lcd1602_H

#include "public.h"

sbit LCD1602_RS=P2^6;
sbit LCD1602_RW=P2^5;
sbit LCD1602_E=P2^7;
#define LEC1602_DATAPORT	P0
void lcd1602_write_cmd(u8 cmd);
void lcd1602_write_data(u8 dat);
void lcd1602_init();
void lcd1602_show_string(u8 col,u8 row,u8 *str)	;
void lcd1602_clear(void);

#endif