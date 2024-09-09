#include "wifi_control.h"
#include "uart.h"
#include "timer.h"
//定义全局变量
u16 global_OUT1_SEL;
u16 global_OUT2_SEL;
//u16 global_OUT3_SEL;
//定义输出管脚
sbit OUT1=P2^0;
sbit OUT2=P2^1;
//sbit OUT3=P2^2;
//定义WIFI控制命令
#define OUT1_ON_CMD_5min	'1'
#define OUT1_ON_CMD_10min	'2'
#define OUT1_OFF_CMD		'3'
#define OUT2_ON_CMD_5min  	'4'
#define OUT2_ON_CMD_10min  	'5'
#define OUT2_OFF_CMD		'6'
//#define OUT3_ON_CMD_5min	'7'
//#define OUT3_ON_CMD_10min	'8'
//#define OUT3_OFF_CMD		'9'
									 

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
	ESP8266_SendCmd("AT+CWSAP=\"DaChuang\",\"123456789\",11,0"); //设置WIFI热点名及密码
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
	ESP8266_ModeInit();
	ES=1;//允许串口中断
}

//串口中断服务函数
//接收手机APP发送的信号后控制板载资源
void UART_IRQn() interrupt 4
{
	static u8 i=0;

	if(RI)
	{
		RI=0;
		//读取接收到的数据
		UART_RX_BUF[i]=SBUF;
		//数据格式内容为：+IPD,ID号,发送数据长度:数据	控制OUT1_ON_CMD_5min的指令：+IPD,0,1:1
		if(UART_RX_BUF[0]=='+')i++;//判断第一位是否为+
		else i=0;
		if(i==10)
		{
			i=0;
			//WIFI控制
			if(UART_RX_BUF[9]==OUT1_ON_CMD_5min||UART_RX_BUF[9]==OUT1_ON_CMD_10min)//判断第十位的数据
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

void time0() interrupt 1 //定时器0中断函数
{
	static u16 i,j,k;//定义静态变量i
 	TH0=0XFC; //给定时器赋初值，定时1ms
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