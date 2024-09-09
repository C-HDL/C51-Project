#include "uart.h"

#define RELOAD_COUNT 0xFA //�궨�岨���ʷ�����������ֵ 9600

void UART_Init(void)
{
	SCON|=0X50;			//����Ϊ������ʽ1
	TMOD|=0X20;			//���ö�ʱ��������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=RELOAD_COUNT;	//��������ʼֵ����
	TL1=TH1;
	ES=0;				//�رս����ж�
	EA=1;				//�����ж�
	TR1=1;				//�򿪶�ʱ��
//	TI=1;          //�����жϱ��λ�����ʹ��printf�����ı�������	
}

void UART_SendData(u8 dat)
{
	ES=0; //�رմ����ж�
	TI=0; //�巢������ж������־λ
	SBUF=dat; //����
	while(TI==0); //�ȴ��������
	TI=0; //�巢������ж������־λ
	ES=1; //�������ж�
}

void UART_SendString(u8 *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}

u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART_RX_STA=0;       //����״̬���	


//void UART_IRQn() interrupt 4
//{
//	u8 r;
//	static u8 i=0;
//	
//	if(RI)
//	{
//		RI=0;
//		UART_RX_BUF[i]=SBUF;//��ȡ���յ�������
//		if(UART_RX_BUF[0]=='+')i++;
//		else i=0;
//		if(i==10)
//		{
//			i=0;
//		}		  		
//	}	
//}

