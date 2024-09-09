#include "xpt2046.h"
#include "intrins.h"

void xpt2046_write_data(u8 dat)
{
	u8 i=0;

	CLK=0;
	_nop_();
	for(i=0;i<8;i++)
	{
		DIN=dat>>7;
		CLK=1;
		_nop_();
		CLK=0;
		_nop_();
		dat<<=1;
			
	}

}

u16 xpt2046_read_data()
{
	u8 i=0;
	u16 temp=0;
	CLK=0;
	_nop_();
	for(i=0;i<12;i++)
	{
		
		CLK=1;
		_nop_();
		CLK=0;
		_nop_();
		temp|=DOUT;
		temp<<=1;
	}
	return temp;
}

u16 xpt2045_read_adc_value(u8 cmd)
{
	u16 adc_value=0;
	u8 i=0;
	CS=0;
	CLK=0;
	xpt2046_write_data(cmd);
	for(i=6;i>0;i--);//—” ±
	CLK=0;	
	_nop_();
	CLK=1;
	_nop_();
	adc_value=xpt2046_read_data();
	CS=1;
	return adc_value;


}