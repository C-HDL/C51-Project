#include "reg52.h"

typedef unsigned char u8;
typedef unsigned char u16;
sbit SCK=P3^6;
sbit RCK=P3^5;
sbit SER=P3^4;

#define LEDdianzhen_COL_PORT P0
u8 the_data[]={0x01,0xE5,0xA9,0xF5,0xAA,0xF4,0xA8,0xE0,
				0x21,0x2A,0xAC,0xBB,0xEA,0xAA,0xF3,0x20,
				0x7F,0x80,0x80,0x7F,0x00,0xFF,0x11,0x1F		};
u8 LEDdianzhen_COL_PORT_shuzu[]={0x7f,0xbf,0xdf,0xef,0xf7,0xfb,0xfd,0xfe};

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
	 
	 while(1)
	 {		 for(i=0;i<8;i++){
	            
				LEDdianzhen_COL_PORT=LEDdianzhen_COL_PORT_shuzu[i];
				write_data_595(the_data[i]);
				write_data_595(0x00);
				delay(100);
				}

			 delay(1000000000);
			 for(i=0;i<8;i++){
	            
				LEDdianzhen_COL_PORT=LEDdianzhen_COL_PORT_shuzu[i];
				write_data_595(the_data[i+8]);
				write_data_595(0x00);
				delay(100);}
				delay(1000000000);
				for(i=0;i<8;i++){
	            
				LEDdianzhen_COL_PORT=LEDdianzhen_COL_PORT_shuzu[i];
				write_data_595(the_data[i+16]);
				write_data_595(0x00);
				delay(100);}
				delay(1000000000);  
			 	
	 
		 
	 
	 }
	 }