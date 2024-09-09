#ifndef _ds1302_H
#define _ds1302_H

#include "public.h"

sbit DS1302_RST=P3^5;
sbit DS1302_CLK=P3^6;
sbit DS1302_IO=P3^4;

void ds1302_write_byte(u8 addr,u8 dat);
u8 ds1302_read_byte(u8 addr);
void ds1302_init();
void ds1302_read_time();
extern u8 gDS1302_TIME[7];

#endif