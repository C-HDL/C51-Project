#include "reg52.h"
#define time 5000
typedef unsigned char u8;
typedef unsigned char u16;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
sbit LED1 = P2^0;
sbit LED2 = P2^1;

void delay(unsigned int x){
     while(x--);
	 }
void delay_ms(u16 ms)
{
u16 i,j;
for(i=ms;i>0;i--)
for(j=110;j>0;j--);
}			
void exti0_init(){
     IT0=1;
	 EX0=1;
	 EA=1;
}
void exti1_init(){
     IT1=1;
	 EX1=1;
	 EA=1;
}
void main()
{	  
   
			
			
	 while(1)
	 {exti0_init();		 
	  exti1_init();
			  
	 }
	 }
void  exti0() interrupt 0
{       	delay(10000);
			if(KEY3==0){
			   LED1=!LED1;	
			
			}
}
void  exti1() interrupt 2
{       	delay(10000);
			if(KEY4==0){
			   LED2=!LED2;	
			
			}
}