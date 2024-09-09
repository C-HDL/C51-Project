#ifndef _smg_H
#define _smg_H

#include "public.h"

#define SMG 	P0
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;
extern unsigned char gsmg_code[];

void smg_display(u8 dat[],u8 position);

#endif