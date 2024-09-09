#include "reg52.h"
#define KEY P1
#define SMG P0
sbit LSA = P2^2;
sbit LSB = P2^3;
sbit LSC = P2^4;

unsigned char gsmg_code[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};



void delay(unsigned int x){
     while(x--);
	 }
unsigned char scan1(){			
      unsigned char value = 0;
	  KEY = 0xf7;
	  if(KEY != 0xf7){
	     delay(1000);
	     switch(KEY){
		     case 0x77:value=1;break;
			 case 0xb7:value=5;break;
  			 case 0xd7:value=9;break;
	 		 case 0xe7:value=13;break;}
			 }
			 //while(KEY != 0xf7);
	  KEY = 0xfb;
	  if(KEY != 0xfb){
	     delay(1000);
	     switch(KEY){
		     case 0x7b:value=2;break;
			 case 0xbb:value=6;break;
  			 case 0xdb:value=10;break;
	 		 case 0xeb:value=14;break;}
			 }
	  KEY = 0xfd;
	  if(KEY != 0xfd){
	     delay(1000);
	     switch(KEY){
		     case 0x7d:value=3;break;
			 case 0xbd:value=7;break;
  			 case 0xdd:value=11;break;
	 		 case 0xed:value=15;break;}
			 }
	  KEY = 0xfe;
	  if(KEY != 0xfe){
	     delay(1000);
	     switch(KEY){
		     case 0x7e:value=4;break;
			 case 0xbe:value=8;break;
  			 case 0xde:value=12;break;
	 		 case 0xee:value=16;break;}
			 }
	  
	  return value;}

unsigned char scan2(){
        static unsigned char value=0;
		KEY=0x0f;
		if(KEY!=0x0f){
		    delay(1000);
			switch(KEY){
			   case 0x07:value=1;break;
			   case 0x0b:value=2;break;
			   case 0x0d:value=3;break;
			   case 0x0e:value=4;break;	}
			   }
	    KEY=0xf0;
		
			switch(KEY){
			   case 0x70:value=value;break;
			   case 0xb0:value=value+4;break;
			   case 0xd0:value=value+8;break;
			   case 0xe0:value=value+12;break;	}
			   
			   
		return value;
		}

void main()
{	 unsigned char key=0; 
     int i;
	 while(1)
	 {
	 
	 key=scan2();
	 if(key!=0)
	 for(i=0;i<8;i++){
	 switch(i){
	  case 0:LSC=1;LSB=1;LSA=1;break;
	  case 1:LSC=1;LSB=1;LSA=0;break;
	  case 2:LSC=1;LSB=0;LSA=1;break;
	  case 3:LSC=1;LSB=0;LSA=0;break;
	  case 4:LSC=0;LSB=1;LSA=1;break;
	  case 5:LSC=0;LSB=1;LSA=0;break;
	  case 6:LSC=0;LSB=0;LSA=1;break;
	  case 7:LSC=0;LSB=0;LSA=0;break;} 
	 SMG= gsmg_code[key-1];
	 delay(100);
	 
	  }	 
	 }
	 }
