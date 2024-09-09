#include "public.h"
#include "smg.h"
#include "xpt2046.h"


void main(){
	u16 temp=0;
	float vol;
	u8 buf[3];
 	while(1){
	  temp=xpt2045_read_adc_value(0x94);
	  vol=5.0*temp/4096;
	  temp=vol*10;
	  buf[0]=gsmg_code[temp/10]|0x80;
	  buf[1]=gsmg_code[temp%10];
	  buf[2]=0x3e;
	  smg_display(buf,6);



 }	
	 }