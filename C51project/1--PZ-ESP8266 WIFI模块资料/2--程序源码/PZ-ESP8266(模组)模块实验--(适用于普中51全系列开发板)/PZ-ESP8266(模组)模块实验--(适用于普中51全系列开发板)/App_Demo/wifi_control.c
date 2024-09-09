#include "wifi_control.h"
#include "ds18b20.h"
#include "uart.h"
#include "smg.h"
#include "timer.h"

//����LED1�ܽ�
sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;

//����WIFI��������
#define LED1_ON_CMD			'1'
#define LED1_OFF_CMD		'2'
#define LED2_ON_CMD			'3'
#define LED2_OFF_CMD		'4'
#define LED3_ON_CMD			'5'
#define LED3_OFF_CMD		'6'


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
	ESP8266_SendCmd("AT+CWSAP=\"PRECHIN\",\"prechin168\",11,0"); //����WIFI�ȵ���������
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
	ds18b20_init();//��ʼ��DS18B20
	ESP8266_ModeInit();
	ES=1;//�������ж�
}

//WIFI����
void wifi_control(void)
{
	u16 i=0;
   	int temp_value;
	u8 temp_buf[5];
	u8 wifi_send_buf[7];

	while(1)
	{
		i++;
		if(i%50==0)//���һ��ʱ���ȡ�¶�ֵ�����ʱ��Ҫ�����¶ȴ�����ת���¶�ʱ��
			temp_value=ds18b20_read_temperture()*10;//�����¶�ֵС����һλ
		if(temp_value<0)//���¶�
		{
			temp_value=-temp_value;
			temp_buf[0]=0x40;//��ʾ����
			wifi_send_buf[0]='-';	
		}
		else
		{
			temp_buf[0]=0x00;//����ʾ
			wifi_send_buf[0]='+';
		}		
		temp_buf[1]=gsmg_code[temp_value/1000];//��λ
		temp_buf[2]=gsmg_code[temp_value%1000/100];//ʮλ
		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//��λ+С����
		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//С�����һλ
		smg_display(temp_buf,4);
		if(i%100==0)
		{
			wifi_send_buf[1]=temp_value/1000+0x30;
			wifi_send_buf[2]=temp_value%1000/100+0x30;
			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
			wifi_send_buf[4]='.';
			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
			wifi_send_buf[6]='\0';
			ESP8266_SendData(wifi_send_buf);//ͨ�����ڷ����¶����ݵ�APP	
		}		
	}	
}

//�����жϷ�����
//�����ֻ�APP���͵��źź���ư�����Դ
void UART_IRQn() interrupt 4
{
	static u8 i=0;

	if(RI)
	{
		RI=0;
		UART_RX_BUF[i]=SBUF;//��ȡ���յ�������
		if(UART_RX_BUF[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI����
			if(UART_RX_BUF[9]==LED1_ON_CMD)
				LED1=1;
			else if(UART_RX_BUF[9]==LED1_OFF_CMD)
				LED1=0;	
			if(UART_RX_BUF[9]==LED2_ON_CMD)
				LED2=1;
			else if(UART_RX_BUF[9]==LED2_OFF_CMD)
				LED2=0;	
			if(UART_RX_BUF[9]==LED3_ON_CMD)
				LED3=1;
			else if(UART_RX_BUF[9]==LED3_OFF_CMD)
				LED3=0;		
		}			
	}	
}

void time0() interrupt 1 //��ʱ��0�жϺ���
{
	static u16 i;//���徲̬����i
 	TH0=0XFC; //����ʱ������ֵ����ʱ1ms
 	TL0=0X18;
 	if(LED1==1)
	{
		i++;
 		if(i==5000)
 		{
 			i=0;
 			LED1=0;
 		}
	}
}
