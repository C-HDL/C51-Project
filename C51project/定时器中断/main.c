
#include "reg52.h"

typedef unsigned int u16;	
typedef unsigned char u8;


sbit LED1=P2^0;
sbit LED2=P2^1;




void delay(u16 x)
{
	while(x--);	
}



void time0_init()
{
	TMOD|=0X01;
	TH0=0XFC;
	TL0=0X18;	
	ET0=1;
	EA=1;
	TR0=1;		
}


void main()
{	
	time0_init();

	while(1)
	{			
							
	}		
}

void time0() interrupt 1 
{
	static u16 i=0;
	TH0=0XFC;	
	TL0=0X18;
	i++;
	if(i==1000)
	{	LED1=!LED1;	
		LED2=!LED2;		
		i=0;
		
	}						
}
