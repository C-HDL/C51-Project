//#include <reg52.h>
#include <AT89X52.H>
#include <intrins.h>

#define uchar	unsigned char	//�궨��
#define uint	unsigned int	//�궨��

#define	N_can	13	//N_can����һ֡�ֽ���

bit left_flag = 0;

uchar TX_buffer_1[13]; //���յ�13���ֽ����ݣ�//���͵����ݣ�
uchar RX_buffer_1[13]; //���յ�13���ֽ�����

sbit LED1 = P1^0;
sbit LED2 = P1^1;
sbit LED3 = P1^2;
sbit LED4 = P1^3;

sbit SJA_CS  = P2^0; //SJA1000Ƭѡ�ܽţ��͵�ƽ��Ч
sbit SJA_RST = P2^7;//SJA1000��λ�ܽ�

sbit hight_beam = P2^1;//����Զ���
sbit dipped_beam = P2^2;//���ƽ����
sbit left_light = P2^3;//��ת���
sbit backup_light = P2^4;//������
sbit frog_light = P2^5;//���
sbit stop_light   = P2^6;//ɲ����

uchar hbeam_s;
uchar dbeam_s;
uchar leftl_s;
uchar backupl_s;
uchar frogl_s;
uchar stopl_s;

uchar complete_1;
uchar prenode;

bit interrupt_flag_1 = 0;
bit alivestate = 0;//alive����״̬
bit ringstate = 0;//ring����״̬
bit prealivestate = 0;//ǰ�ڵ�alive״̬
bit preringstate = 0; //ǰ�ڵ�live״̬


/*CAN����SJA1000�Ĵ�����ַ���壨�õ���PeliCANģʽ����չ֡EFFģʽ��*/
//xdata ----> ��ѰַƬ��ram (64k��ַ��Χ:0000H-FFFFH)

uchar   xdata    MODR     _at_   0xFE00;	// ģʽ�Ĵ���
uchar   xdata    CMR	  _at_   0xFE01;	// ����Ĵ���
uchar   xdata    SR	      _at_   0xFE02;	// ״̬�Ĵ���
uchar   xdata    IR	  	  _at_   0xFE03;	// �жϼĴ���
uchar   xdata    IER	  _at_   0xFE04;	// �ж�ʹ�ܼĴ���

uchar   xdata    BTR0	  _at_   0xFE06;	// ���߶�ʱ�Ĵ���0 �����߲����ʵ�ѡ��
uchar   xdata    BTR1	  _at_   0xFE07;	// ���߶�ʱ�Ĵ���1 �����߲����ʵ�ѡ��
uchar   xdata    OCR	  _at_   0xFE08;	// ������ƼĴ���	��� 1a

	// ���մ���Ĵ�����16-19��
uchar   xdata    ACR0	  _at_   0xFE10;//16;
uchar   xdata    ACR1	  _at_   0xFE11;//17;
uchar   xdata    ACR2	  _at_   0xFE12;//18;
uchar   xdata    ACR3	  _at_   0xFE13;//19;

	// �������μĴ�����20-23��
uchar   xdata    AMR0	  _at_   0xFE14;//20;
uchar   xdata    AMR1	  _at_   0xFE15;//21;
uchar   xdata    AMR2	  _at_   0xFE16;//22;
uchar   xdata    AMR3	  _at_   0xFE17;//23;

uchar   xdata    CDR	  _at_   0xFE1F;//31;	// ʱ�ӷ�Ƶ��		 C8
uchar   xdata    ALC	  _at_   0xFE0B;//11;	// �ٲö�ʧ��׽�Ĵ���
uchar   xdata    ECC	  _at_   0xFE0C;//12;	// ���벶׽�Ĵ���

	// ���ͻ�������ַ(16-28)
uchar   xdata    TBSR0	  _at_   0xFE10;//16;//TX֡��Ϣ
uchar   xdata    TBSR1	  _at_   0xFE11;//17;//TXʶ����1 (ID.21-ID.28)
uchar   xdata    TBSR2	  _at_   0xFE12;//18;//TXʶ����2 (ID.13-ID.20)
uchar   xdata    TBSR3	  _at_   0xFE13;//19;//TXʶ����3 (ID.5-ID.12)
uchar   xdata    TBSR4	  _at_   0xFE14;//20;//TXʶ����4����5λ��ID.0-ID.4��
uchar   xdata    TBSR5	  _at_   0xFE15;//21;//����1
uchar   xdata    TBSR6	  _at_   0xFE16;//22;//����2
uchar   xdata    TBSR7	  _at_   0xFE17;//23;//����3
uchar   xdata    TBSR8	  _at_   0xFE18;//24;//����4
uchar   xdata    TBSR9	  _at_   0xFE19;//25;//����5
uchar   xdata    TBSR10	  _at_   0xFE1A;//26;//����6
uchar   xdata    TBSR11	  _at_   0xFE1B;//27;//����7
uchar   xdata    TBSR12	  _at_   0xFE1C;//28;//����8

// ���ջ�������ַ(16-28)
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
  
/*�Ӻ�������*/
void init_mcu(void);  //MCU��ʼ��
void init_sja1000(void);	//SJA1000��ʼ��
void can_txd(uchar node,uchar control_lamp,uchar cmd);  //���ʹ�����
void can_rxd(void);	 //���մ�����
void delay(uchar);	 //�ӳ�	

///////////////////////////////////////////////
//������init_mcu
//˵������Ƭ��I/O�ڳ�ʼ��
//		��Ҫ�Ǹ��жϼĴ����ĳ�ʼ��
//��ڣ���
//���أ���
///////////////////////////////////////////////
void init_mcu(void)
{
    P1 = 0xFF;
	LED1 = 0;
	delay(1000);
	SJA_RST = 0;
	delay(10);
    SJA_RST = 1;//CAN���߸�λ�ܽ�
	SJA_CS = 0;		//CAN����Ƭѡ��Ч
    IT0 = 0;	//MCU�ⲿ�ж�INT0����Ϊ �͵�ƽ ����
    EX0 = 1;	//��MCU�ⲿ�ж�INT0
    EA = 1; 	//��MCU���ж�
	LED1 = 0;
	delay(300);
	LED1 = 1;	
}

///////////////////////////////////////////////
//������init_sja1000
//˵��������CAN������SJA1000�ĳ�ʼ��
//��ڣ���
//���أ���
///////////////////////////////////////////////
void init_sja1000(void)
{
//uchar state;
    uchar ACRR[4];
    uchar AMRR[4];
/*//ԭ�д���
// ���մ���Ĵ���	                                                     
    ACRR[0] = 0x20;
    ACRR[1] = 0x00;
    ACRR[2] = 0x10;
    ACRR[3] = 0x00;
// �������μĴ���
    AMRR[0] = 0x3f;
    AMRR[1] = 0Xff;
    AMRR[2] = 0xf8;
    AMRR[3] = 0x07;
ԭ�д���*/

    ACRR[0] = 0xff;
    ACRR[1] = 0x01;//���ڵ���01
    ACRR[2] = 0xff;
    ACRR[3] = 0xff;//���մ���Ĵ���

    AMRR[0] = 0xff;
    AMRR[1] = 0x00;
    AMRR[2] = 0xff;
    AMRR[3] = 0xff;//�������μĴ�����ֻ���շ������ڵ�ı���

	MODR  = 0x09;	// ����MOD.0=1--���븴λģʽ���Ա�������Ӧ�ļĴ���



// ��SJA1000���ּĴ������г�ʼ������
    CDR  = 0xC8;	// CDRΪʱ�ӷ�Ƶ����CDR.3=1--ʱ�ӹر�, CDR.7=0---basic CAN, CDR.7=1---Peli CAN
    BTR0 = 0x01;	// ���߶�ʱ�Ĵ���0 �����߲������趨
    BTR1 = 0x1B;	// ���߶�ʱ�Ĵ���1 �����߲������趨
    IER  = 0x01;	// IER.0=1--�����ж�ʹ�ܣ�  IER.1=0--�رշ����ж�ʹ��
    OCR  = 0x1a;	// ����������ƼĴ���
    CMR  = 0x04;	// �ͷŽ��ջ�����

// ��ʼ�����մ���Ĵ���
    ACR0 = ACRR[0];
    ACR1 = ACRR[1];	  
    ACR2 = ACRR[2];
    ACR3 = ACRR[3]; 

// ��ʼ���������μĴ���
    AMR0 = AMRR[0];
    AMR1 = AMRR[1];
    AMR2 = AMRR[2];
    AMR3 = AMRR[3]; 

	MODR   = 0x08;	//MOD.3=0--˫�˲���ģʽ
	LED2 = 0;
	delay(200);
	LED2 = 1;
	delay(200);
	LED2 = 0;
	delay(200);
	LED2 = 1;

}

///////////////////////////////////////////////
//������inter1_can_rxd  (�ⲿ�ж�INT0)
//˵�����������ݺ��������жϷ�������е���
//��ڣ���
//���أ���
///////////////////////////////////////////////
void inter1_can_rxd( void ) interrupt 0
{
	uchar state;
    uchar ID_M;
    uchar ID_L;
	uchar ID_R;

    EA = 0;		//��CPU�ж�
    IE0 = 0;	//�������ж�INT0�ǵ�ƽ������ʽ��������Ҫ�����INT0���ж������־IE0����
	ID_R = RBSR1;
    ID_M = RBSR2;
    ID_L = RBSR3;

	state = IR;	//IRΪSJA1000���жϼĴ���

	if( state & 0x01)
	{
		RX_buffer_1[0] =  RBSR0;
		RX_buffer_1[1] =  RBSR1;
		RX_buffer_1[2] =  RBSR2;
		RX_buffer_1[3] =  RBSR3;
		RX_buffer_1[4] =  RBSR4;
		RX_buffer_1[5] =  RBSR5;
		RX_buffer_1[6] =  RBSR6;
		interrupt_flag_1 = 1; //ʵ����Ҳ����Щ�Ĵ����е��������ã���չ�������ֽڶ�����д�����������ݡ�
	}

	CMR = 0x04;		//CMR.2=1--������ϣ��ͷŽ��ջ�����
	state = ALC;	//�ͷ��ٲ���ʱ��׽�Ĵ��������üĴ������ɣ�
	state = ECC;	//�ͷŴ�����벶׽�Ĵ��������üĴ������ɣ�
	IER = 0x01;		// IER.0=1--�����ж�ʹ��
	EA = 1;			//���¿���CPU�ж�
	EX0=1;
	 
}

///////////////////////////////////////////////
//������can_rxd
//˵�������մ����������ݽ��մ����Ӻ�����Ϣ�����ⲿ��ʾЧ����������Ӧ��LED�ơ�
//��ڣ���
//���أ���
///////////////////////////////////////////////

void can_rxd(void)	 //���մ�����
{
	uchar control_light,light_state;
	control_light = RX_buffer_1[5];	//���ݵ����ֽ������ж��Ǻ��ֵơ�
	light_state = RX_buffer_1[6];
	if (control_light == 0x01)
	{	
		if(light_state == 0x01)
		{
			hight_beam = 0;//Զ��ƿ�
			interrupt_flag_1 = 0;
			left_flag = 1;
		}else
		{
			hight_beam = 1;//Զ��ƹ�
			interrupt_flag_1 = 0;
			left_flag = 0;
		}
   	}
	if (control_light == 0x02)
	{
		if(light_state == 0x01)
		{
			dipped_beam = 0;//����ƿ�
			interrupt_flag_1 = 0;
		}else
		{
			dipped_beam = 1;//����ƹ�
			interrupt_flag_1 = 0;
		}
	}

	
	if (control_light == 0x03)
	{
		if(light_state == 0x01)
		{
			frog_light = 0;//��ƿ�
			interrupt_flag_1 = 0;
		}else
		{
			frog_light = 1;//��ƹ�
			interrupt_flag_1 = 0;
		}
	}
	if (control_light == 0x04)
	{
		if(light_state == 0x01)
		{
			stop_light = 0;//ɲ���ƿ�
			interrupt_flag_1 = 0;
		}else
		{
			frog_light = 1;//ɲ���ƹ�
			interrupt_flag_1 = 0;
		}
	}

}


//void can_rxd(void)
//{
//	uchar control_light,light_state,node;
//	control_light = RX_buffer_1[5];	//0100��ʾalive���ģ�0101��ʾring���ģ�0102��ʾaliveack��0103��ʾringack��
//	light_state = RX_buffer_1[6];
//	prenode = RX_buffer_1[1];//��ȡ������Դ��Դ�ڵ���
//	if (control_light == 0x01)		//�յ�ǰ�ڵ�alive����
//	{	
//		if(light_state == 0x00)
//		prealivestate = 1;
//	 }
//	if (control_light == 0x01)		//�յ�ǰ�ڵ�ring����
//	{	
//		if(light_state == 0x01)
//		preringstate = 1;
//	 }
//	if (control_light == 0x01)		//�յ���̽ڵ�aliveack����
//	{	
//		if(light_state == 0x02)
//		alivestate = 1;
//	 }
//	if (control_light == 0x01)		//�յ���̽ڵ�ringack����
//	{	
//		if(light_state == 0x03)
//		ringstate = 1;
//	 }
//	RX_buffer_1[5] = 0x00;//���������㣻
//	RX_buffer_1[6] = 0x00;
//}

///////////////////////////////////////////////
//������can_txd
//˵�������ʹ���������ȡִ��Ч����Ϣ֮�󷵻������Ϣ�����Ŀ��ƽڵ�
//��ڣ�control_lamp��ʾ���ֵƣ�cmd��ʾ�ǿ����ǹ�.

//���أ���
///////////////////////////////////////////////
void can_txd(uchar node,uchar control_lamp,uchar cmd)  //���ʹ�����
{
	uchar state;
	uchar TX_buffer_1[ N_can ] ;	//N_can=13��TX_buffer����Ϊ�����͵�����֡
	//��ʼ����ʾ��ͷ��Ϣ
    TX_buffer_1[0] = 0x82;	//.7=0--��չ֡��.6=0--����֡; .0-.3=100--���ݳ���Ϊ8�ֽ�
    TX_buffer_1[1] = 0x01;	//��֡��Ϣ��ID	  28~21	  �˲�����28~13�� ����ǰ�ڵ��ַ
    TX_buffer_1[2] = node;	  //20~13,Ŀ�ĵ�ַ
    TX_buffer_1[3] = 0x10;	   //12~5
    TX_buffer_1[4] = 0x00;	   // 4~0

	complete_1 = 0;
	//��ʼ���������ݵ�Ԫ
	
	TX_buffer_1[5] = control_lamp;	//��������0x01Ϊ�����౨��
	TX_buffer_1[6] = cmd;			//��������00��ʾalive��01��ʾring��02��ʾaliveack��03��ʾringack

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
    			CMR = 0x01;	//��λ��������
		   }
		}
	}
}



///////////////////////////////////////////////
//������delay
//˵������ʱ�Ӻ���
//��ڣ�uchar time:��ʱʱ��time us
//���أ���
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
//������main
//˵����������
//��ڣ���
//���أ���
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
//				node = 0x02;//01�ڵ���02�ڵ㷢alive����
//				light = 0x01;//0100��ʾalive����
//				cmd = 0x00;
//				can_txd(node,light,cmd);
//				delay(500);
//				can_rx();//����ɹ����յ�02�ڵ㷢�͵�aliveack���ģ���alivestate��1��������ȻΪ0��ͬʱ�ж�ǰ��ring��alive�����Լ���̽�����ack����
//				node = node + 0x01;
//			}
//			else
//			node = 0x04;//��ʱalivestate == 1 �����ߣ���node��0x04�����ٷ���alive����
//			if (preringstate == 1) //�յ�ǰ���ڵ��ring����
//			{	
//				light = 0x01;//�ظ�ringack����
//				cmd = 0x03;
//				can_txd(prenode,light,cmd);
//				preringstate = 0;
//			}
//			if (prealivestate == 1) //�յ�ǰ���ڵ��alive����
//			{	
//				light = 0x01;//�ظ�aliveack����
//				cmd = 0x02;
//				can_txd(prenode,light,cmd);
//				prealivestate = 0;
//			}
//			if (alivestate == 1) //�յ���̽ڵ��aliveack����
//			{	
//				node = 0x04;
//			} 
//			if (ringstate == 1) //�յ���̽ڵ��ringack����
//			{	
//				light = 0x01;//�ظ�ringack����
//				cmd = 0x01;
//				can_txd(prenode,light,cmd);//��������ring����
//				preringstate = 0;
//			}
//			delay(200);   
//		}
//		node = 0x02;//01�ڵ���02�ڵ㷢ring����
//		light = 0x01;//0101��ʾring����
//		cmd = 0x01;
//		can_txd(node,light,cmd); //��һ�η���ring����
//		delay(200);
//		can_rx();
//		if (ringstate == 1) //�յ���̽ڵ��ringack����
//			{	
//				light = 0x01;//�ظ�ringack����
//				cmd = 0x01;
//				can_txd(prenode,light,cmd);//��������ring����
//				preringstate = 0;
//			}
//			delay(200);
//	   can_rx();//ѭ���жϷ���
//	   node = 0x02;//01�ڵ���02�ڵ㷢ring����
//		light = 0x01;//0101��ʾring����
//		cmd = 0x01;
//		can_txd(node,light,cmd); //�ڶ��η���ring����
//		delay(200);
//		can_rx();
//	   if (ringstate == 0)//��������δ�ܻ�ú�̽�㷢�͵�ringack���ģ�֤����̽����ߡ�
//	   {
//	   		LED2 = 0;
//			delay(5000);//led2�ŵ� ����5�룻
//			LED2 = 1;
//	   }
//	}
////////////////
/*
sbit backup_light = P2^4;//������
sbit frog_light = P2^5;//���
sbit stop_light   = P2^6;//ɲ����
*/
////////////////////////	
	while(1)
	{
		if(hight_beam == 0)//�ڵ�2Զ��ƿ�
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
			if(hbeam_s == 1)//�ڵ�2Զ��ƹ�
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
		if(dipped_beam ==0)//�ڵ�2����ƿ�
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
			if(dbeam_s == 1)//�ڵ�2����ƹ�
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
		 	if(leftl_s == 0)//�ڵ�1��ת��ƿ�
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
			if(leftl_s == 1)//�ڵ�1��ת��ƹ�
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
		 	if(backupl_s == 0)//�ڵ�1�����ƿ�
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
			if(backupl_s == 1)//�ڵ�1�����ƹ�
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
		 	if(frogl_s == 0)//�ڵ�1��ƿ�
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
			if(frogl_s == 1)//�ڵ�1��ƹ�
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
		 	if(stopl_s == 0)//�ڵ�1ɲ���ƿ�
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
			if(stopl_s == 1)//�ڵ�1ɲ���ƹ�
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
