#include "public.h"
#include "smg.h"
#include "ired.h"


void main(){
	u8 buf[3];
	ired_init();
	
 	while(1){

		buf[0]=gsmg_code[gired_data[2]/16];
		buf[1]=gsmg_code[gired_data[2]%16];
		buf[2]=0x76;
		smg_display(buf,6);
 
 }	
	 }