#include "ds18b20.h"
#include "intrins.h"
void ds18b20_reset(void)
{
	DS18B20_PORT=0;
	delay(75);
	DS18B20_PORT=1;
	delay(2);
}

u8 ds18b20_check(void)
{	u8 time_temp=0;
	while(DS18B20_PORT&&time_temp<20){
		  time_temp++;
		  delay(1);
		  }
	if(time_temp>=20) return 1;
	else time_temp=0;
	while((!DS18B20_PORT)&&time_temp<20){
		  time_temp++;
		  delay(1);
		  }
	if(time_temp>=20) return 1;
	return 0;
}

u8 ds18b20_init(void)
{
	ds18b20_reset();
	return ds18b20_check();
}

void ds18b20_write_byte(u8 dat)
{
	u8 i=0;
	u8 temp=0;
	for(i=0;i<8;i++)
	{
		//temp=dat&0x01;
		//dat>>=1;
		temp=(dat>>i)&0x01;
		if(temp){
		DS18B20_PORT=0;
		_nop_();_nop_();
		DS18B20_PORT=1;
		delay(6);
		}
		else{
			DS18B20_PORT=0;
			delay(6);
			DS18B20_PORT=1;
			_nop_();_nop_();
		}
		
	}
}

u8 ds18b20_read_bit(void)
{
	u8 dat=0;
	DS18B20_PORT=0;
	_nop_();_nop_();
	DS18B20_PORT=1;	
	_nop_();_nop_();
	if(DS18B20_PORT) dat=1;
	else dat=0;
	delay(5);
	return dat;

}
u8 ds18b20_read_byte(void)
{
	u8 i=0;
	u8 temp=0;
	u8 dat=0;
	for(i=0;i<8;i++)
	{
		temp=ds18b20_read_bit();
		dat|=(temp<<i);
		//dat>>=1;
		//dat|=(temp<<7);
		//dat=(temp<<7)|(dat>>1);
	}
	return dat;
}

void ds18b20_start(void)
{
	ds18b20_reset();
	ds18b20_check();
	ds18b20_write_byte(0xcc);
	ds18b20_write_byte(0x44);		
}
float ds18b20_read_temperature(void)
{	u8 dath=0;
	u8 datl=0;
	u16 value=0;
	float temp;

	ds18b20_start();
	ds18b20_reset();
	ds18b20_check();
	ds18b20_write_byte(0xcc);
	ds18b20_write_byte(0xbe);
	datl=ds18b20_read_byte();
	dath=ds18b20_read_byte();
	value=(dath<<8)+datl;

	if((value&0xf800)==0xf800)
	{
		value=(~value)+1;
		temp=value*(-0.0625);
	}
	else{
		temp=value*0.0625;
	}
	return temp;
}	
