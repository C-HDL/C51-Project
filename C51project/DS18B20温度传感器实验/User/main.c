#include "public.h"
#include "smg.h"
#include "ds18b20.h"

void main(){
	u8 i=0;
	int temp_value=0;
	u8 buf[5];
	ds18b20_init();
 	while(1){
	  	i++;
		if(i%50==0)
		temp_value=ds18b20_read_temperature()*10;
		if(temp_value<0)
		{
			temp_value=-temp_value;
			buf[0]=0x40;

		}
		else buf[0]=0x00;
		buf[1]=gsmg_code[temp_value/1000];
		buf[2]=gsmg_code[temp_value%1000/100];
		buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;
		buf[4]=gsmg_code[temp_value%1000%100%10];
		smg_display(buf,4); 

   }
 } 