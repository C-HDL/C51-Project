#include "wifi_control.h"
#include "uart.h"
#include "timer.h"
//����ȫ�ֱ���
u16 global_OUT1_SEL;
u16 global_OUT2_SEL;
//u16 global_OUT3_SEL;
//��������ܽ�
sbit OUT1=P2^0;
sbit OUT2=P2^1;
//sbit OUT3=P2^2;
//����WIFI��������
#define OUT1_ON_CMD_5min	'1'
#define OUT1_ON_CMD_10min	'2'
#define OUT1_OFF_CMD		'3'
#define OUT2_ON_CMD_5min  	'4'
#define OUT2_ON_CMD_10min  	'5'
#define OUT2_OFF_CMD		'6'
//#define OUT3_ON_CMD_5min	'7'
//#define OUT3_ON_CMD_10min	'8'
//#define OUT3_OFF_CMD		'9'
									 

//ESP8266 WIFI����ATָ��
//pbuf��ATָ��ַ�����ʽ���磺"AT"
void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendData('\r');//�س�
	delay_10us(5);
	UART_SendData('\n');//����
	delay_ms(1000);
}

//ESP8266 WIFI�������ݵ�APP
//pbuf������
void ESP8266_SendData(u8 *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,7");
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	UART_SendData('\n');//����	
//	delay_ms(10);
}

//ESP8266-WIFIģ�鹤��ģʽ��ʼ��
void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+CWMODE=2");//����·����ģʽ 1 staTIonģʽ 2 AP�� ·����ģʽ 3 station+AP���ģʽ
	ESP8266_SendCmd("AT+CWSAP=\"DaChuang\",\"123456789\",11,0"); //����WIFI�ȵ���������
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"");
//	ESP8266_SendCmd("AT+RST");//��������wifiģ��
//	delay_ms(2000);
	ESP8266_SendCmd("AT+CIPMUX=1");	//����������ģʽ�����������ͻ��˽���
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//����TCP/IP �˿�Ϊ8080 ʵ�ֻ����������	
} 

//WIFI���Ƴ�ʼ��
void wifi_control_init(void)
{
	UART_Init();
	time0_init();	
	ESP8266_ModeInit();
	ES=1;//�������ж�
}

//�����жϷ�����
//�����ֻ�APP���͵��źź���ư�����Դ
void UART_IRQn() interrupt 4
{
	static u8 i=0;

	if(RI)
	{
		RI=0;
		//��ȡ���յ�������
		UART_RX_BUF[i]=SBUF;
		//���ݸ�ʽ����Ϊ��+IPD,ID��,�������ݳ���:����	����OUT1_ON_CMD_5min��ָ�+IPD,0,1:1
		if(UART_RX_BUF[0]=='+')i++;//�жϵ�һλ�Ƿ�Ϊ+
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI����
			if(UART_RX_BUF[9]==OUT1_ON_CMD_5min||UART_RX_BUF[9]==OUT1_ON_CMD_10min)//�жϵ�ʮλ������
				{
					global_OUT1_SEL=UART_RX_BUF[9];
					OUT1=1;
				}
			else if(UART_RX_BUF[9]==OUT1_OFF_CMD)
				OUT1=0;	
			if(UART_RX_BUF[9]==OUT2_ON_CMD_5min||UART_RX_BUF[9]==OUT2_ON_CMD_10min)
				{
					global_OUT2_SEL=UART_RX_BUF[9];
					OUT2=1;
				}
			else if(UART_RX_BUF[9]==OUT2_OFF_CMD)
				OUT2=0;	
//			if(UART_RX_BUF[9]==OUT3_ON_CMD_5min||UART_RX_BUF[9]==OUT3_ON_CMD_10min)
//				{
//					global_OUT3_SEL=UART_RX_BUF[9];
//					OUT3=1;
//				}
//			else if(UART_RX_BUF[9]==OUT3_OFF_CMD)
//				OUT3=0;		
		}			
	}	
}

void time0() interrupt 1 //��ʱ��0�жϺ���
{
	static u16 i,j,k;//���徲̬����i
 	TH0=0XFC; //����ʱ������ֵ����ʱ1ms
 	TL0=0X18;
 	if(global_OUT1_SEL==OUT1_ON_CMD_5min)
	{
		if(OUT1==1)
		{
			i++;
 			if(i==300000)
 			{
 				i=0;
 				OUT1=0;
 			}
		}
	}
	else if(global_OUT1_SEL==OUT1_ON_CMD_10min)
	{
		if(OUT1==1)
		{
			i++;
 			if(i==600000)
 			{
 				i=0;
 				OUT1=0;
 			}
		}
	}
	if(global_OUT2_SEL==OUT2_ON_CMD_5min)
	{
		if(OUT2==1)
		{
			j++;
 			if(j==5000)
 			{
 				j=0;
 				OUT2=0;
 			}
		}
	}
	else if(global_OUT2_SEL==OUT2_ON_CMD_10min)
	{
		if(OUT2==1)
		{
			j++;
 			if(j==10000)
 			{
 				j=0;
 				OUT2=0;
 			}
		}
	}
//	if(global_OUT3_SEL==OUT3_ON_CMD_5min)
//	{
//		if(OUT3==1)
//		{
//			k++;
// 			if(k==5000)
// 			{
// 				k=0;
// 				OUT3=0;
// 			}
//		}
//	}
//	else if(global_OUT3_SEL==OUT3_ON_CMD_10min)
//	{
//		if(OUT3==1)
//		{
//			k++;
// 			if(k==10000)
// 			{
// 				k=0;
// 				OUT3=0;
// 			}
//		}
//	}
}