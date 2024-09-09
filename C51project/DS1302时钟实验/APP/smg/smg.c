#include "smg.h"
unsigned char gsmg_code[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void smg_display(u8 dat[],u8 position)
{	 u8 i=0; 
     u8 position_temp=position-1;
	 	for(i=position_temp;i<8;i++){
	 switch(i){
	  case 0:LSC=1;LSB=1;LSA=1;break;
	  case 1:LSC=1;LSB=1;LSA=0;break;
	  case 2:LSC=1;LSB=0;LSA=1;break;
	  case 3:LSC=1;LSB=0;LSA=0;break;
	  case 4:LSC=0;LSB=1;LSA=1;break;
	  case 5:LSC=0;LSB=1;LSA=0;break;
	  case 6:LSC=0;LSB=0;LSA=1;break;
	  case 7:LSC=0;LSB=0;LSA=0;break;}
	  SMG=dat[i-position_temp]  ;  
	  delay(100)   ;
	  SMG=0x00 ;
	  
	 }
}