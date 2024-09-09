#include "public.h"
#include "smg.h"
#include "key.h"
#include "ds1302.h"
#include "ds18b20.h"


/*void main(){
	u8 time_buf[8];
	ds1302_init();
 	while(1){
		int i=2000000;
		
		while(i--){
		ds1302_read_time();
		time_buf[0]=gsmg_code[gDS1302_TIME[2]/16];
		time_buf[1]=gsmg_code[gDS1302_TIME[2]%16];
		time_buf[2]=0x40;
		time_buf[3]=gsmg_code[gDS1302_TIME[1]/16];
		time_buf[4]=gsmg_code[gDS1302_TIME[1]%16];
		time_buf[5]=0x40;
		time_buf[6]=gsmg_code[gDS1302_TIME[0]/16];
		time_buf[7]=gsmg_code[gDS1302_TIME[0]%16];
		smg_display(time_buf,1);}
		delay_ms(500);
		i=2000;
		while(i--){
		ds1302_read_time();
		time_buf[0]=gsmg_code[gDS1302_TIME[6]/16];
		time_buf[1]=gsmg_code[gDS1302_TIME[6]%16];
		time_buf[2]=0x40;
		time_buf[3]=gsmg_code[gDS1302_TIME[4]/16];
		time_buf[4]=gsmg_code[gDS1302_TIME[4]%16];
		time_buf[5]=0x40;
		time_buf[6]=gsmg_code[gDS1302_TIME[3]/16];
		time_buf[7]=gsmg_code[gDS1302_TIME[3]%16];
		smg_display(time_buf,1);}
		delay_ms(500);

 }
 }*/
 void main(){
	int temp_value=0;
	u8 i=0;
	u8 temperature_buf[5];
	
	u8 time_buf[8];
	u8 key=0;
	
	ds18b20_init();
	ds1302_init();
 	while(1){		
	
	if(key==0||key==5)
		key=scan(0);
			/*if(key==key3_p)
	{	
		while((key!=key1_p)&&(key!=key2_p))		
		{
			i++;
			if(i%50==0)
			temp_value=ds18b20_read_temperature()*10;

			if(temp_value<0)
		{
			temp_value=-temp_value;
			temperature_buf[0]=0x40;

		}
			else temperature_buf[0]=0x00;
			temperature_buf[1]=gsmg_code[temp_value/1000];
			temperature_buf[2]=gsmg_code[temp_value%1000/100];
			temperature_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;
			temperature_buf[4]=gsmg_code[temp_value%1000%100%10];
			smg_display(temperature_buf,4);
			key=scan(0);
		}
		
	}*/ //不显示温度，不知道为什么	
		
 
 
			if(key==key1_p)
	{	while((key!=key3_p)&&(key!=key2_p)){
		
		ds1302_read_time();
		time_buf[0]=gsmg_code[gDS1302_TIME[2]/16];
		time_buf[1]=gsmg_code[gDS1302_TIME[2]%16];
		time_buf[2]=0x40;
		time_buf[3]=gsmg_code[gDS1302_TIME[1]/16];
		time_buf[4]=gsmg_code[gDS1302_TIME[1]%16];
		time_buf[5]=0x40;
		time_buf[6]=gsmg_code[gDS1302_TIME[0]/16];
		time_buf[7]=gsmg_code[gDS1302_TIME[0]%16];
		smg_display(time_buf,1);
		key=scan(0);
	}}	
		
		if(key==key2_p)
	{	while((key!=key1_p)&&(key!=key3_p)){
		
		ds1302_read_time();
		time_buf[0]=gsmg_code[gDS1302_TIME[6]/16];
		time_buf[1]=gsmg_code[gDS1302_TIME[6]%16];
		time_buf[2]=0x40;
		time_buf[3]=gsmg_code[gDS1302_TIME[4]/16];
		time_buf[4]=gsmg_code[gDS1302_TIME[4]%16];
		time_buf[5]=0x40;
		time_buf[6]=gsmg_code[gDS1302_TIME[3]/16];
		time_buf[7]=gsmg_code[gDS1302_TIME[3]%16];
		smg_display(time_buf,1);
		key=scan(0);
	}}	
	}	
	 }