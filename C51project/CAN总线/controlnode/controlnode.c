//#include <reg52.h>
#include <AT89X52.H>
#include <intrins.h>

#define uchar	unsigned char	//宏定义
#define uint	unsigned int	//宏定义

#define	N_can	13	//N_can代表一帧字节数

bit left_flag = 0;

uchar TX_buffer_1[13]; //接收的13个字节数据（//发送的数据）
uchar RX_buffer_1[13]; //接收的13个字节数据

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;

sbit SJA_CS  = P2^0; //SJA1000片选管脚，低电平有效
sbit SJA_RST = P2^7;//SJA1000复位管脚

sbit hight_beam = P2^1;//控制远光灯
sbit dipped_beam = P2^2;//控制近光灯
sbit left_light = P2^3;//左转向灯
sbit backup_light = P2^4;//倒车灯
sbit frog_light = P2^5;//雾灯
sbit stop_light   = P2^6;//刹车灯

uchar hbeam_s;
uchar dbeam_s;
uchar leftl_s;
uchar backupl_s;
uchar frogl_s;
uchar stopl_s;

uchar complete_1;
uchar prenode;

bit interrupt_flag_1 = 0;
bit alivestate = 0;//alive报文状态
bit ringstate = 0;//ring报文状态
bit prealivestate = 0;//前节点alive状态
bit preringstate = 0; //前节点live状态


/*CAN总线SJA1000寄存器地址定义（用的是PeliCAN模式，扩展帧EFF模式）*/
//xdata ----> 可寻址片外ram (64k地址范围:0000H-FFFFH)

uchar   xdata    MODR     _at_   0xFE00;	// 模式寄存器
uchar   xdata    CMR	  _at_   0xFE01;	// 命令寄存器
uchar   xdata    SR	      _at_   0xFE02;	// 状态寄存器
uchar   xdata    IR	  	  _at_   0xFE03;	// 中断寄存器
uchar   xdata    IER	  _at_   0xFE04;	// 中断使能寄存器

uchar   xdata    BTR0	  _at_   0xFE06;	// 总线定时寄存器0 ；总线波特率的选择
uchar   xdata    BTR1	  _at_   0xFE07;	// 总线定时寄存器1 ；总线波特率的选择
uchar   xdata    OCR	  _at_   0xFE08;	// 输出控制寄存器	汇编 1a

	// 验收代码寄存器（16-19）
uchar   xdata    ACR0	  _at_   0xFE10;//16;
uchar   xdata    ACR1	  _at_   0xFE11;//17;
uchar   xdata    ACR2	  _at_   0xFE12;//18;
uchar   xdata    ACR3	  _at_   0xFE13;//19;

	// 验收屏蔽寄存器（20-23）
uchar   xdata    AMR0	  _at_   0xFE14;//20;
uchar   xdata    AMR1	  _at_   0xFE15;//21;
uchar   xdata    AMR2	  _at_   0xFE16;//22;
uchar   xdata    AMR3	  _at_   0xFE17;//23;

uchar   xdata    CDR	  _at_   0xFE1F;//31;	// 时钟分频器		 C8
uchar   xdata    ALC	  _at_   0xFE0B;//11;	// 仲裁丢失捕捉寄存器
uchar   xdata    ECC	  _at_   0xFE0C;//12;	// 误码捕捉寄存器

	// 发送缓冲器地址(16-28)
uchar   xdata    TBSR0	  _at_   0xFE10;//16;//TX帧信息
uchar   xdata    TBSR1	  _at_   0xFE11;//17;//TX识别码1 (ID.21-ID.28)
uchar   xdata    TBSR2	  _at_   0xFE12;//18;//TX识别码2 (ID.13-ID.20)
uchar   xdata    TBSR3	  _at_   0xFE13;//19;//TX识别码3 (ID.5-ID.12)
uchar   xdata    TBSR4	  _at_   0xFE14;//20;//TX识别码4（高5位是ID.0-ID.4）
uchar   xdata    TBSR5	  _at_   0xFE15;//21;//数据1
uchar   xdata    TBSR6	  _at_   0xFE16;//22;//数据2
uchar   xdata    TBSR7	  _at_   0xFE17;//23;//数据3
uchar   xdata    TBSR8	  _at_   0xFE18;//24;//数据4
uchar   xdata    TBSR9	  _at_   0xFE19;//25;//数据5
uchar   xdata    TBSR10	  _at_   0xFE1A;//26;//数据6
uchar   xdata    TBSR11	  _at_   0xFE1B;//27;//数据7
uchar   xdata    TBSR12	  _at_   0xFE1C;//28;//数据8

// 接收缓冲器地址(16-28)
uchar   xdata    RBSR0	  _at_   0xFE10;//16;
uchar   xdata    RBSR1	  _at_   0xFE11;//17;
uchar   xdata    RBSR2	  _at_   0xFE12;//18;
uchar   xdata    RBSR3	  _at_   0xFE13;//19;
uchar   xdata    RBSR4	  _at_   0xFE14;//20;
uchar   xdata    RBSR5	  _at_   0xFE15;//21;
uchar   xdata    RBSR6	  _at_   0xFE16;//22;
uchar   xdata    RBSR7	  _at_   0xFE17;//23;
uchar   xdata    RBSR8	  _at_   0xFE18;//24;
uchar   xdata    RBSR9	  _at_   0xFE19;//25;
uchar   xdata    RBSR10	  _at_   0xFE1A;//26;
uchar   xdata    RBSR11	  _at_   0xFE1B;//27;
uchar   xdata    RBSR12	  _at_   0xFE1C;//28;
  
/*子函数声明*/
void init_mcu(void);  //MCU初始化
void init_sja1000(void);	//SJA1000初始化
void can_txd(uchar node,uchar control_lamp,uchar cmd);  //发送处理函数
void can_rxd(void);	 //接收处理函数
void delay(uchar);	 //延迟	

///////////////////////////////////////////////
//函数：init_mcu
//说明：单片机I/O口初始化
//		主要是各中断寄存器的初始化
//入口：无
//返回：无
///////////////////////////////////////////////
void init_mcu(void)
{
    P1 = 0xFF;
	LED1 = 0;
	delay(1000);
	SJA_RST = 0;
	delay(10);
    SJA_RST = 1;//CAN总线复位管脚
	SJA_CS = 0;		//CAN总线片选有效
    IT0 = 0;	//MCU外部中断INT0设置为 低电平 触发
    EX0 = 1;	//开MCU外部中断INT0
    EA = 1; 	//开MCU总中断
	LED1 = 0;
	delay(300);
	LED1 = 1;	
}

///////////////////////////////////////////////
//函数：init_sja1000
//说明：独立CAN控制器SJA1000的初始化
//入口：无
//返回：无
///////////////////////////////////////////////
void init_sja1000(void)
{
//uchar state;
    uchar ACRR[4];
    uchar AMRR[4];
/*//原有代码
// 接收代码寄存器	                                                     
    ACRR[0] = 0x20;
    ACRR[1] = 0x00;
    ACRR[2] = 0x10;
    ACRR[3] = 0x00;
// 接收屏蔽寄存器
    AMRR[0] = 0x3f;
    AMRR[1] = 0Xff;
    AMRR[2] = 0xf8;
    AMRR[3] = 0x07;
原有代码*/

    ACRR[0] = 0xff;
    ACRR[1] = 0x01;//本节点编号01
    ACRR[2] = 0xff;
    ACRR[3] = 0xff;//接收代码寄存器

    AMRR[0] = 0xff;
    AMRR[1] = 0x00;
    AMRR[2] = 0xff;
    AMRR[3] = 0xff;//接收屏蔽寄存器，只接收发往本节点的报文

	MODR  = 0x09;	// 设置MOD.0=1--进入复位模式，以便设置相应的寄存器



// 对SJA1000部分寄存器进行初始化设置
    CDR  = 0xC8;	// CDR为时钟分频器，CDR.3=1--时钟关闭, CDR.7=0---basic CAN, CDR.7=1---Peli CAN
    BTR0 = 0x01;	// 总线定时寄存器0 ；总线波特率设定
    BTR1 = 0x1B;	// 总线定时寄存器1 ；总线波特率设定
    IER  = 0x01;	// IER.0=1--接收中断使能；  IER.1=0--关闭发送中断使能
    OCR  = 0x1a;	// 配置输出控制寄存器
    CMR  = 0x04;	// 释放接收缓冲器

// 初始化接收代码寄存器
    ACR0 = ACRR[0];
    ACR1 = ACRR[1];	  
    ACR2 = ACRR[2];
    ACR3 = ACRR[3]; 

// 初始化接收屏蔽寄存器
    AMR0 = AMRR[0];
    AMR1 = AMRR[1];
    AMR2 = AMRR[2];
    AMR3 = AMRR[3]; 

	MODR   = 0x08;	//MOD.3=0--双滤波器模式
	LED2 = 0;
	delay(200);
	LED2 = 1;
	delay(200);
	LED2 = 0;
	delay(200);
	LED2 = 1;

}

///////////////////////////////////////////////
//函数：inter1_can_rxd  (外部中断INT0)
//说明：接收数据函数，在中断服务程序中调用
//入口：无
//返回：无
///////////////////////////////////////////////
void inter1_can_rxd( void ) interrupt 0
{
	uchar state;
    uchar ID_M;
    uchar ID_L;
	uchar ID_R;

    EA = 0;		//关CPU中断
    IE0 = 0;	//由于是中断INT0是电平触发方式，所以需要软件将INT0的中断请求标志IE0清零
	ID_R = RBSR1;
    ID_M = RBSR2;
    ID_L = RBSR3;

	state = IR;	//IR为SJA1000的中断寄存器

	if( state & 0x01)
	{
		RX_buffer_1[0] =  RBSR0;
		RX_buffer_1[1] =  RBSR1;
		RX_buffer_1[2] =  RBSR2;
		RX_buffer_1[3] =  RBSR3;
		RX_buffer_1[4] =  RBSR4;
		RX_buffer_1[5] =  RBSR5;
		RX_buffer_1[6] =  RBSR6;
		interrupt_flag_1 = 1; //实际上也就这些寄存器中的数据有用，扩展的其他字节都是填写的无意义数据。
	}

	CMR = 0x04;		//CMR.2=1--接收完毕，释放接收缓冲器
	state = ALC;	//释放仲裁随时捕捉寄存器（读该寄存器即可）
	state = ECC;	//释放错误代码捕捉寄存器（读该寄存器即可）
	IER = 0x01;		// IER.0=1--接收中断使能
	EA = 1;			//重新开启CPU中断
	EX0=1;
	 
}

///////////////////////////////////////////////
//函数：can_rxd
//说明：接收处理函数，根据接收处理子函数信息处理外部显示效果，点亮相应的LED灯。
//入口：无
//返回：无
///////////////////////////////////////////////

void can_rxd(void)	 //接收处理函数
{
	uchar control_light,light_state;
	control_light = RX_buffer_1[5];	//根据第六字节数据判断是何种灯。
	light_state = RX_buffer_1[6];
	if (control_light == 0x01)
	{	
		if(light_state == 0x01)
		{
			hight_beam = 0;//远光灯开
			interrupt_flag_1 = 0;
			left_flag = 1;
		}else
		{
			hight_beam = 1;//远光灯关
			interrupt_flag_1 = 0;
			left_flag = 0;
		}
   	}
	if (control_light == 0x02)
	{
		if(light_state == 0x01)
		{
			dipped_beam = 0;//近光灯开
			interrupt_flag_1 = 0;
		}else
		{
			dipped_beam = 1;//近光灯关
			interrupt_flag_1 = 0;
		}
	}

	
	if (control_light == 0x03)
	{
		if(light_state == 0x01)
		{
			frog_light = 0;//雾灯开
			interrupt_flag_1 = 0;
		}else
		{
			frog_light = 1;//雾灯关
			interrupt_flag_1 = 0;
		}
	}
	if (control_light == 0x04)
	{
		if(light_state == 0x01)
		{
			stop_light = 0;//刹车灯开
			interrupt_flag_1 = 0;
		}else
		{
			frog_light = 1;//刹车灯关
			interrupt_flag_1 = 0;
		}
	}

}


//void can_rxd(void)
//{
//	uchar control_light,light_state,node;
//	control_light = RX_buffer_1[5];	//0100表示alive报文；0101表示ring报文；0102表示aliveack；0103表示ringack。
//	light_state = RX_buffer_1[6];
//	prenode = RX_buffer_1[1];//获取数据来源的源节点编号
//	if (control_light == 0x01)		//收到前节点alive报文
//	{	
//		if(light_state == 0x00)
//		prealivestate = 1;
//	 }
//	if (control_light == 0x01)		//收到前节点ring报文
//	{	
//		if(light_state == 0x01)
//		preringstate = 1;
//	 }
//	if (control_light == 0x01)		//收到后继节点aliveack报文
//	{	
//		if(light_state == 0x02)
//		alivestate = 1;
//	 }
//	if (control_light == 0x01)		//收到后继节点ringack报文
//	{	
//		if(light_state == 0x03)
//		ringstate = 1;
//	 }
//	RX_buffer_1[5] = 0x00;//接收区清零；
//	RX_buffer_1[6] = 0x00;
//}

///////////////////////////////////////////////
//函数：can_txd
//说明：发送处理函数，获取执行效果信息之后返回相关信息给中心控制节点
//入口：control_lamp表示何种灯，cmd表示是开还是关.

//返回：无
///////////////////////////////////////////////
void can_txd(uchar node,uchar control_lamp,uchar cmd)  //发送处理函数
{
	uchar state;
	uchar TX_buffer_1[ N_can ] ;	//N_can=13，TX_buffer数组为待传送的数据帧
	//初始化标示码头信息
    TX_buffer_1[0] = 0x82;	//.7=0--扩展帧；.6=0--数据帧; .0-.3=100--数据长度为8字节
    TX_buffer_1[1] = 0x01;	//本帧信息的ID	  28~21	  滤波器滤28~13的 ；当前节点地址
    TX_buffer_1[2] = node;	  //20~13,目的地址
    TX_buffer_1[3] = 0x10;	   //12~5
    TX_buffer_1[4] = 0x00;	   // 4~0

	complete_1 = 0;
	//初始化发送数据单元
	
	TX_buffer_1[5] = control_lamp;	//报文种类0x01为管理类报文
	TX_buffer_1[6] = cmd;			//报文内容00表示alive，01表示ring，02表示aliveack，03表示ringack

	state = SR;
	if(!(state & 0x10))
	{
		if(state & 0x08)
		{
		   if(state & 0x04)
		   {
		   		TBSR0  = TX_buffer_1[0];
    			TBSR1  = TX_buffer_1[1];
    			TBSR2  = TX_buffer_1[2];
    			TBSR3  = TX_buffer_1[3];
    			TBSR4  = TX_buffer_1[4];
    			TBSR5  = TX_buffer_1[5];
    			TBSR6  = TX_buffer_1[6];
				complete_1 = 1;
    			CMR = 0x01;	//置位发送请求
		   }
		}
	}
}



///////////////////////////////////////////////
//函数：delay
//说明：延时子函数
//入口：uchar time:延时时间time us
//返回：无
///////////////////////////////////////////////
void delay(unsigned char time)
{	
    unsigned char timeee;
    while(time--)
	{
	    timeee=0xFF;
		while(timeee--);
	}
}

///////////////////////////////////////////////
//函数：main
//说明：主函数
//入口：无
//返回：无
///////////////////////////////////////////////
void main(void)
{
	uchar node,light,cmd;

	LED1 = 0;
	delay(10000);

	init_mcu();
	init_sja1000();
	hbeam_s = 0;
	dbeam_s = 0;
	leftl_s = 0;
	backupl_s = 0;
	frogl_s = 0;
	stopl_s = 0;
	P2 = 0xFF;
	node = 0x01;

//	while(1)
//	{	
//		can_rx();
//		while(node<0x04)
//		{
//			if(alivestate == 0)
//			{
//				node = 0x02;//01节点向02节点发alive报文
//				light = 0x01;//0100表示alive报文
//				cmd = 0x00;
//				can_txd(node,light,cmd);
//				delay(500);
//				can_rx();//如果成功接收到02节点发送的aliveack报文，则将alivestate置1，否则依然为0；同时判断前驱ring和alive报文以及后继结点给的ack报文
//				node = node + 0x01;
//			}
//			else
//			node = 0x04;//此时alivestate == 1 即上线，将node置0x04，不再发送alive报文
//			if (preringstate == 1) //收到前驱节点的ring报文
//			{	
//				light = 0x01;//回复ringack报文
//				cmd = 0x03;
//				can_txd(prenode,light,cmd);
//				preringstate = 0;
//			}
//			if (prealivestate == 1) //收到前驱节点的alive报文
//			{	
//				light = 0x01;//回复aliveack报文
//				cmd = 0x02;
//				can_txd(prenode,light,cmd);
//				prealivestate = 0;
//			}
//			if (alivestate == 1) //收到后继节点的aliveack报文
//			{	
//				node = 0x04;
//			} 
//			if (ringstate == 1) //收到后继节点的ringack报文
//			{	
//				light = 0x01;//回复ringack报文
//				cmd = 0x01;
//				can_txd(prenode,light,cmd);//继续发送ring报文
//				preringstate = 0;
//			}
//			delay(200);   
//		}
//		node = 0x02;//01节点向02节点发ring报文
//		light = 0x01;//0101表示ring报文
//		cmd = 0x01;
//		can_txd(node,light,cmd); //第一次发送ring报文
//		delay(200);
//		can_rx();
//		if (ringstate == 1) //收到后继节点的ringack报文
//			{	
//				light = 0x01;//回复ringack报文
//				cmd = 0x01;
//				can_txd(prenode,light,cmd);//继续发送ring报文
//				preringstate = 0;
//			}
//			delay(200);
//	   can_rx();//循环判断发送
//	   node = 0x02;//01节点向02节点发ring报文
//		light = 0x01;//0101表示ring报文
//		cmd = 0x01;
//		can_txd(node,light,cmd); //第二次发送ring报文
//		delay(200);
//		can_rx();
//	   if (ringstate == 0)//连续两次未能获得后继结点发送的ringack报文，证明后继结点掉线。
//	   {
//	   		LED2 = 0;
//			delay(5000);//led2号灯 常亮5秒；
//			LED2 = 1;
//	   }
//	}
////////////////
/*
sbit backup_light = P2^4;//倒车灯
sbit frog_light = P2^5;//雾灯
sbit stop_light   = P2^6;//刹车灯
*/
////////////////////////	
	while(1)
	{
		if(hight_beam == 0)//节点2远光灯开
		{
			if(hbeam_s == 0)
			{
				LED4 = 0;
				node = 0x03;
				light = 0x01;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED4 = 1;
				hbeam_s = 1;
			}
		}else if(hight_beam == 1)
		{
			if(hbeam_s == 1)//节点2远光灯关
			{
				LED4 = 0;
				node = 0x03;
				light = 0x01;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED4 = 1;
				delay(500);
				LED4 = 0;
				delay(500);
				LED4 = 1;
				hbeam_s = 0;
			}
		}
		if(dipped_beam ==0)//节点2近光灯开
		{
		 	if(dbeam_s == 0)
			{
				LED4 = 0;
				node = 0x03;
				light = 0x02;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED4 = 1;
				dbeam_s = 1;
			}
		}else if(dipped_beam ==1)
		{	
			if(dbeam_s == 1)//节点2近光灯关
			{
				LED4 = 0;
				node = 0x03;
				light = 0x02;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED4 = 1;
				delay(500);
				LED4 = 0;
				delay(500);
				LED4 = 1;
				dbeam_s = 0;
			}
		}
		if(left_light ==0)
		{
		 	if(leftl_s == 0)//节点1左转向灯开
			{
				LED3 = 0;
				node = 0x02;
				light = 0x03;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				leftl_s = 1;
			}
		}else if(left_light ==1)
		{	
			if(leftl_s == 1)//节点1左转向灯关
			{
				LED3 = 0;
				node = 0x02;
				light = 0x03;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				delay(500);
				LED3 = 0;
				delay(500);
				LED3 = 1;
				leftl_s = 0;
			}
		}
		if(backup_light ==0)
		{
		 	if(backupl_s == 0)//节点1倒车灯开
			{
				LED3 = 0;
				node = 0x02;
				light = 0x04;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				backupl_s = 1;
			}
		}else if(backup_light ==1)
		{	
			if(backupl_s == 1)//节点1倒车灯关
			{
				LED3 = 0;
				node = 0x02;
				light = 0x04;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				delay(500);
				LED3 = 0;
				delay(500);
				LED3 = 1;
				backupl_s = 0;
			}
		}
		if(frog_light ==0)
		{
		 	if(frogl_s == 0)//节点1雾灯开
			{
				LED3 = 0;
				node = 0x02;
				light = 0x05;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				frogl_s = 1;
			}
		}else if(frog_light ==1)
		{	
			if(frogl_s == 1)//节点1雾灯关
			{
				LED3 = 0;
				node = 0x02;
				light = 0x05;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				delay(500);
				LED3 = 0;
				delay(500);
				LED3 = 1;
				frogl_s = 0;
			}
		}
		if(stop_light ==0)
		{
		 	if(stopl_s == 0)//节点1刹车灯开
			{
				LED3 = 0;
				node = 0x02;
				light = 0x06;
				cmd = 0x01;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				stopl_s = 1;
			}
		}else if(stop_light ==1)
		{	
			if(stopl_s == 1)//节点1刹车灯关
			{
				LED3 = 0;
				node = 0x02;
				light = 0x06;
				cmd = 0x00;
				can_txd(node,light,cmd);
				delay(500);
				LED3 = 1;
				delay(500);
				LED3 = 0;
				delay(500);
				LED3 = 1;
				stopl_s = 0;
			}
		}

	}
		
}
