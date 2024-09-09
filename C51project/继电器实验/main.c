#include "reg52.h"
#define time 5000
typedef unsigned char u8;
typedef unsigned char u16;
sbit Relay1 = P2^0;	   //Relay¡ª¡ª¼ÌµçÆ÷
sbit Relay2 = P2^1;	
sbit PPP = P1^0;
sbit PP = P3^0;	 

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
	 Relay1=1;
	 PPP=1;
	 PP=1;
	 //delay_ms(time);
	 //Relay=0;
	 while(1)
	 {		 
	 
			  
			 	
	 
		 
	 
	 }
	 }