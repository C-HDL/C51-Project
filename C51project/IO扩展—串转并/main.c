#include "reg52.h"

typedef unsigned char u8;
typedef unsigned char u16;
sbit SCK=P3^6;
sbit RCK=P3^5;
sbit SER=P3^4;

#define LEDdianzhen_COL_PORT P0
u8 the_data[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};


void delay(unsigned int x){
     while(x--);
	 }
void write_data_595(u8 the_data){
              int i=0;
			  for(i=0;i<8;i++){
			         SER = the_data >> 7;
			  		 the_data <<= 1;
					 SCK=0;
					 delay(1);
					 SCK=1;
			  }
			  RCK=0;
			  delay(1);
			  RCK=1;
			  }			
   

void main()
{	  
     u8  i;
	 LEDdianzhen_COL_PORT=0x00;
	 while(1)
	 {		 for(i=0;i<8;i++){
	            
				write_data_595(the_data[i]);
				delay(50000);
				write_data_595(0x00);}
			 for(i=6;i>=1;i--){
			    
				write_data_595(the_data[i]);
				delay(50000);
				write_data_595(0x00);} 
	 
		 
	 
	 }
	 }