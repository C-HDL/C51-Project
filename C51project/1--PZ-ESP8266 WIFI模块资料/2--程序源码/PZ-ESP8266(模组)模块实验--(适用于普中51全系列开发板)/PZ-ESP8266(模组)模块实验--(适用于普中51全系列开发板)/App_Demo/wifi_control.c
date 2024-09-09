#include "wifi_control.h"
#include "ds18b20.h"
#include "uart.h"
#include "smg.h"
#include "timer.h"

//定义LED1管脚
sbit LED1=P2^0;
sbit LED2=P2^1;
sbit LED3=P2^2;

//定义WIFI控制命令
#define LED1_ON_CMD			'1'
#define LED1_OFF_CMD		'2'
#define LED2_ON_CMD			'3'
#define LED2_OFF_CMD		'4'
#define LED3_ON_CMD			'5'
#define LED3_OFF_CMD		'6'


//ESP8266 WIFI发送AT指令
//pbuf：AT指令，字符串格式，如："AT"
void ESP8266_SendCmd(u8 *pbuf)
{
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	delay_10us(5);
	UART_SendData('\r');//回车
	delay_10us(5);
	UART_SendData('\n');//换行
	delay_ms(1000);
}

//ESP8266 WIFI发送数据到APP
//pbuf：数据
void ESP8266_SendData(u8 *pbuf)
{
	ESP8266_SendCmd("AT+CIPSEND=0,7");
	while(*pbuf!='\0') //遇到空格跳出循环	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
	UART_SendData('\n');//换行	
//	delay_ms(10);
}

//ESP8266-WIFI模块工作模式初始化
void ESP8266_ModeInit(void)
{
	ESP8266_SendCmd("AT+CWMODE=2");//设置路由器模式 1 staTIon模式 2 AP点 路由器模式 3 station+AP混合模式
	ESP8266_SendCmd("AT+CWSAP=\"PRECHIN\",\"prechin168\",11,0"); //设置WIFI热点名及密码
	ESP8266_SendCmd("AT+CIPAP=\"192.168.4.1\"");
//	ESP8266_SendCmd("AT+RST");//重新启动wifi模块
//	delay_ms(2000);
	ESP8266_SendCmd("AT+CIPMUX=1");	//开启多连接模式，允许多个各客户端接入
	ESP8266_SendCmd("AT+CIPSERVER=1,8080");	//启动TCP/IP 端口为8080 实现基于网络控制	
} 

//WIFI控制初始化
void wifi_control_init(void)
{
	UART_Init();
	time0_init();
	ds18b20_init();//初始化DS18B20
	ESP8266_ModeInit();
	ES=1;//允许串口中断
}

//WIFI控制
void wifi_control(void)
{
	u16 i=0;
   	int temp_value;
	u8 temp_buf[5];
	u8 wifi_send_buf[7];

	while(1)
	{
		i++;
		if(i%50==0)//间隔一段时间读取温度值，间隔时间要大于温度传感器转换温度时间
			temp_value=ds18b20_read_temperture()*10;//保留温度值小数后一位
		if(temp_value<0)//负温度
		{
			temp_value=-temp_value;
			temp_buf[0]=0x40;//显示负号
			wifi_send_buf[0]='-';	
		}
		else
		{
			temp_buf[0]=0x00;//不显示
			wifi_send_buf[0]='+';
		}		
		temp_buf[1]=gsmg_code[temp_value/1000];//百位
		temp_buf[2]=gsmg_code[temp_value%1000/100];//十位
		temp_buf[3]=gsmg_code[temp_value%1000%100/10]|0x80;//个位+小数点
		temp_buf[4]=gsmg_code[temp_value%1000%100%10];//小数点后一位
		smg_display(temp_buf,4);
		if(i%100==0)
		{
			wifi_send_buf[1]=temp_value/1000+0x30;
			wifi_send_buf[2]=temp_value%1000/100+0x30;
			wifi_send_buf[3]=temp_value%1000%100/10+0x30;
			wifi_send_buf[4]='.';
			wifi_send_buf[5]=temp_value%1000%100%10+0x30;
			wifi_send_buf[6]='\0';
			ESP8266_SendData(wifi_send_buf);//通过串口发送温度数据到APP	
		}		
	}	
}

//串口中断服务函数
//接收手机APP发送的信号后控制板载资源
void UART_IRQn() interrupt 4
{
	static u8 i=0;

	if(RI)
	{
		RI=0;
		UART_RX_BUF[i]=SBUF;//读取接收到的数据
		if(UART_RX_BUF[0]=='+')i++;
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI控制
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

void time0() interrupt 1 //定时器0中断函数
{
	static u16 i;//定义静态变量i
 	TH0=0XFC; //给定时器赋初值，定时1ms
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
