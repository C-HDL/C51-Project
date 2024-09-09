#include "pwm.h"

u8 ghigh=0;
u8 glow=0;
u8 gscale=0;
u8 gduty=0;
void pwm_init(u8 high,u8 low,u8 scale,u8 duty)				//high和low确定（pwm用定时器）定时器的一个周期（这个时间内产生中断，在里边儿对占空比进行确定），scale（进行倍频放大定时器的周期）来确定pwm的一个周期,duty是占空比
{

	ghigh=high;
	glow=low;
	gscale=scale;
	gduty=duty;
	TMOD|=0X01;
	TH0=ghigh;
	TL0=glow;
	ET0=1;
	EA=1;
	TR0=1;
}
void set_duty(u8 duty)
{
	gduty=duty;
}



void pwm()	interrupt 1
{	static u16 i=0;
	TH0=ghigh;
	TL0=glow;

	i++;

	if(i>=gscale)	i=0;
	if(i<=gduty)	PWM=1;
	else 			PWM=0;	
}