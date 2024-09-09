#include "timer.h"

void time0_init(void)
{
	TMOD|=0X01;//选择为定时器0模式，工作方式1
 	TH0=0XFC; //给定时器赋初值，定时1ms
 	TL0=0X18;
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器
}

