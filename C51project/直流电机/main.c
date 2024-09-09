#include "reg52.h"
#define time 5000
typedef unsigned char u8;
typedef unsigned char u16;
sbit motor = P1^0;
sbit A = P2^0;
void delay(unsigned int x){
     while(x--);
	 }
void delay_ms(u16 ms)
{
u16 i,j;
for(i=ms;i>0;i--)
for(j=110;j>0;j--);
}			

void main()
{	  
     u8  i;
	 A=1;
//	 delay_ms(time);
//	 A=0;

	 while(1)
	 {		 
	 
			  
			 	
	 
		 
	 
	 }
	 }