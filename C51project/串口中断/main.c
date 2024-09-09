
#include "reg52.h"

typedef unsigned int u16;	
typedef unsigned char u8;





void delay(u16 x)
{
	while(x--);	
}



void uart_init(u8 baud)
{	 TMOD|=0X20; 
 	 SCON=0X50; 
	 PCON=0X80; 
	 TH1=baud; 
	 TL1=baud;
	 ES=1; 
	 EA=1; 
	 TR1=1;
		
}


void main()
{	
	uart_init(0xFA);

	while(1)
	{			
							
	}		
}

void uart() interrupt 4 
{		u8 recept;
		
		recept=SBUF;
		SBUF=recept;
		RI=0;
		while(TI==0);
		TI=0;

		
							
}
