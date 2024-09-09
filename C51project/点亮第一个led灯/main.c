#include "reg52.h"
#include "intrins.h"

//#define LED		P2
sbit led9999 = P2^1	 ;
unsigned char led[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

void delay(unsigned int x){
     while(x--)	 ;
	 }
void main()
{	 int i;
     /*while(1){
	   for(i=0;i<8;i++){
	   LED=led[i];
	   delay(50000);
	   }
	 } */
     /*
     while(1)
	 { LED1=0;
	   delay(50000)	;
	   LED1=1;
	   delay(50000);
	   LED2=0;
	   delay(50000)	;
	   LED2=1;
	   delay(50000);
	   LED3=0;
	   delay(50000)	;
	   LED3=1;
	   delay(50000);
	   LED4=0;
	   delay(50000)	;
	   LED4=1;
	   delay(50000);
	   LED5=0;
	   delay(50000)	;
	   LED5=1;
	   delay(50000);
	   LED6=0;
	   delay(50000)	;
	   LED6=1;
	   delay(50000);
	   LED7=0;
	   delay(50000)	;
	   LED7=1;
	   delay(50000);
	   LED8=0;
	   delay(50000)	;
	   LED8=1;
	   delay(50000);
	 }	  */
	 /*while(1){
	 for(i=0;i<8;i++){
	 LED = ~(0x01<<i);
	 delay(50000);
	 LED = 0xff;
	 delay(50000);}} */ //×óÒÆ
//	 LED =0xfe;
	 while(1){
	 led9999=0;
//	 for(i=0;i<7;i++){
//	 LED = _crol_ (LED,1);
//
//	 delay(50000);}
//
//	 for(i=0;i<7;i++){
//	 LED = _cror_ (LED,1);
//
//	 delay(50000);}
}		//²ÉÓÃº¯Êý£¬Ñ­»·×óÒÆÓÒÒÆ
	 
	 
	 	
}	  