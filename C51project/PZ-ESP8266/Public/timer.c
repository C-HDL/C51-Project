#include "timer.h"

void time0_init(void)
{
	TMOD|=0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1
 	TH0=0XFC; //����ʱ������ֵ����ʱ1ms
 	TL0=0X18;
	ET0=1;//�򿪶�ʱ��0�ж�����
	EA=1;//�����ж�
	TR0=1;//�򿪶�ʱ��
}

