#include "ds1302.h"
#include "intrins.h"

u8 gREAD_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 	//秒分时日月周年
u8 gWRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

u8 gDS1302_TIME[7] = {0x02, 0x24, 0x15, 0x16, 0x01, 0x01, 0x24};

void ds1302_write_byte(u8 addr,u8 dat)
{
	u8 i=0;
	DS1302_RST=0;
	_nop_();
	DS1302_CLK=0;
	_nop_();
	DS1302_RST=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		DS1302_IO=addr&0x01;
		addr>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
		
	}
	for(i=0;i<8;i++)
	{
		DS1302_IO=dat&0x01;
		dat>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
		
	}
		DS1302_RST=0;
		_nop_();
}

u8 ds1302_read_byte(u8 addr)
{
	u8 i=0;
	u8 temp=0;
	u8 value=0;
	DS1302_RST=0;
	_nop_();
	DS1302_CLK=0;
	_nop_();
	DS1302_RST=1;
	_nop_();
	for(i=0;i<8;i++)
	{
		DS1302_IO=addr&0x01; 
		addr>>=1;
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
		
	}
	for(i=0;i<8;i++)
	{	temp=DS1302_IO;
		//value|=(temp << i);
		value=(temp<<7)|(value>>1);
		DS1302_CLK=1;
		_nop_();
		DS1302_CLK=0;
		_nop_();
	}
		DS1302_RST=0;
		_nop_();
		DS1302_CLK=1;
		_nop_();
		DS1302_IO=0;
		_nop_();
		DS1302_IO=1;
		_nop_();

		return value;

}

void ds1302_init()
{
	u8 i=0;
	ds1302_write_byte(0x8e,0x00);
	for(i=0;i<7;i++)
	{
		ds1302_write_byte(gWRITE_RTC_ADDR[i],gDS1302_TIME[i]);
	}
	ds1302_write_byte(0x8e,0x80);
}

void ds1302_read_time()
{	u8 i=0;
	for(i=0;i<7;i++)
	{
		gDS1302_TIME[i]=ds1302_read_byte(gREAD_RTC_ADDR[i]);
	}	
}