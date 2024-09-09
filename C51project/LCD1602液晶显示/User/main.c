#include "public.h"
#include "lcd1602.h"
#include "ds18b20.h"
#include "intrins.h"

void main(){

	u8 i=0;
	int temp_value=0;
	u8 buf[7];
	lcd1602_init();
	ds18b20_init();	
 	while(1){

		 i++;
		if(i%50==0)
		temp_value=ds18b20_read_temperature()*10;
		if(temp_value<0)
		{
			temp_value=-temp_value;
			buf[0]='-';

		}
		else buf[0]=' ';
		buf[1]=(temp_value/1000)+0x30;
		buf[2]=(temp_value%1000/100)+0x30;
		buf[3]=(temp_value%1000%100/10)+0x30;
		buf[4]='.';
		buf[5]=(temp_value%1000%100%10)+0x30;
		buf[6]='\0';
		lcd1602_show_string(5,0,buf);
 }	
	 }