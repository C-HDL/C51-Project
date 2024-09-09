#ifndef _key_H
#define _key_H

#include "public.h"

sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
sbit LED1 = P2^0;
sbit LED7 = P2^6;
sbit LED8 = P2^7;

#define key1_p 	1
#define key2_p 	2
#define key3_p 	3
#define key4_p 	4
#define nopress 5

unsigned char scan(unsigned char mode);

#endif