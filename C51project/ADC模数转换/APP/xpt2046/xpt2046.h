#ifndef _xpt2046_H
#define _xpt2046_H

#include "public.h"

sbit DOUT=P3^7;
sbit CLK=P3^6;
sbit DIN=P3^4;
sbit CS=P3^5;

void xpt2046_write_data(u8 dat);
u16 xpt2045_read_adc_value(u8 cmd);

#endif