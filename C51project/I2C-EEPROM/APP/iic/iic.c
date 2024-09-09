#include "iic.h"

void iic_start()
{
	IIC_SCL=1;
	IIC_SDA=1;
	delay(1);
	IIC_SDA=0;
	delay(1);
	IIC_SCL=0;
}


void iic_stop()
{	IIC_SDA=0;
	delay(1);
	IIC_SCL=1;	
	delay(1);
	IIC_SDA=1;
	delay(1);
}

void iic_ack()
{	
	IIC_SCL=0;
	IIC_SDA=0;
	delay(1);
	IIC_SCL=1;
	delay(1);
	IIC_SCL=0;
}

void iic_nack()
{	
	IIC_SCL=0;
	IIC_SDA=1;
	delay(1);
	IIC_SCL=1;
	delay(1);
	IIC_SCL=0;
}

u8 iic_wait_ack()
{	u8 temp=0;
	IIC_SCL=1;
	delay(1);
	while(IIC_SDA){
		temp++;
		if(temp>100)
		{
			iic_stop();
			return 1;
		}

	}
	IIC_SCL=0;
	return 0;		
}


void iic_write_byte(u8 dat){
	u8 i=0;
	IIC_SCL=0;
	for(i=0;i<8;i++){
		if((dat&0x80)>0){
			IIC_SDA=1;}
		else
			IIC_SDA=0;	
			
		dat<<=1;
		delay(1);
		IIC_SCL=1;
		delay(1);
		IIC_SCL=0;
		delay(1);
		}
 }

 u8 iic_read_byte(u8 ack){
 	u8 i=0;
	u8 receive=0;
	for(i=0;i<8;i++){
		IIC_SCL=0;
		delay(1);
		IIC_SCL=1;
		
		receive<<=1;
		if(IIC_SDA){
			receive++;
			delay(1);}
				}
		if(!ack) iic_nack();
		else iic_ack();
		
	return receive;
}				