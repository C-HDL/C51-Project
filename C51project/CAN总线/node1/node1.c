#include <reg52.h>
#include <intrins.h>

#define uchar	unsigned char	//�궨��
#define uint	unsigned int	//�궨��

#define	N_can	13	//N_can����һ֡�ֽ���

bit left_flag = 0;

uchar TX_buffer_1[13]; //���յ�13���ֽ����ݣ�//���͵����ݣ�
uchar RX_buffer_1[13]; //���յ�13���ֽ�����

sbit SJA_CS  = P2^0; //SJA1000Ƭѡ�ܽţ��͵�ƽ��Ч
sbit SJA_RST = P2^7;//SJA1000��λ�ܽ�

sbit left_light = P1^0;//��ת���
sbit backup_light = P1^1;//������
sbit frog_light = P1^2;//���
sbit stop_light   = P1^3;//ɲ����

uchar complete_1;

bit interrupt_flag_1 = 0;

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
void can_txd(void);  //���ʹ�����
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
    SJA_RST = 0;
	delay(10);
    SJA_RST = 1;//CAN���߸�λ�ܽ�
	SJA_CS = 0;		//CAN����Ƭѡ��Ч
    IT0 = 0;	//MCU�ⲿ�ж�INT0����Ϊ �͵�ƽ ����
    EX0 = 1;	//��MCU�ⲿ�ж�INT0
    EA = 1; 	//��MCU���ж�	
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
/*ԭ�д���
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
    ACRR[1] = 0x02;
    ACRR[2] = 0xff;
    ACRR[3] = 0xff;//���մ���Ĵ���

    AMRR[0] = 0xff;
    AMRR[1] = 0X00;
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

}


//	void time0_init()
//{
//	TMOD|=0X10;
//	TH1=0XFC;
//	TL1=0X18;	
//	ET1=1;
//	EA=1;
//	TR1=1;		
//}

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
//		frog_light = 0;
//		delay(10000);
//		frog_light = 1;
//		left_light = 0;//��ת���
//		backup_light = 0;//������
//		stop_light   = 0;
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
//	stop_light = 0;
//	delay(10000);
//	stop_light =1;

	if (control_light == 0x03)
	{	
		if(light_state == 0x01)
		{
			
			
			left_light = 0;//��ת��
//			delay(500000);
//			left_light = 1;
//			delay(500000);
		
			
			interrupt_flag_1 = 0;
			left_flag = 1; 	  }
		
		else
		{
			left_light = 1;//��ת���
			interrupt_flag_1 = 0;
			left_flag = 0;
		}
   		 }
	if (control_light == 0x04)
	{
		if(light_state == 0x01)
		{
			backup_light = 0;//�����ƿ�
			interrupt_flag_1 = 0;
		}else
		{
			backup_light = 1;//�����ƹ�
			interrupt_flag_1 = 0;
		}
	}
	if (control_light == 0x05)
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
	if (control_light == 0x06)
	{
		if(light_state == 0x01)
		{
			stop_light = 0;//ɲ���ƿ�
			interrupt_flag_1 = 0;
		}else
		{
			stop_light = 1;//ɲ���ƹ�
			interrupt_flag_1 = 0;
		}
	}
}
   

///////////////////////////////////////////////
//������can_txd
//˵�������ʹ���������ȡִ��Ч����Ϣ֮�󷵻������Ϣ�����Ŀ��ƽڵ�
//��ڣ���
//���أ���
///////////////////////////////////////////////
void can_txd(void)  //���ʹ�����
{
	uchar state;
	uchar TX_buffer_1[ N_can ] ;	//N_can=13��TX_buffer����Ϊ�����͵�����֡
	//��ʼ����ʾ��ͷ��Ϣ
    TX_buffer_1[0] = 0x82;	//.7=0--��չ֡��.6=0--����֡; .0-.3=100--���ݳ���Ϊ8�ֽ�
    TX_buffer_1[1] = 0x02;	//��֡��Ϣ��ID	  28~21	  �˲�����28~13��
    TX_buffer_1[2] = 0x01;	  //20~13
    TX_buffer_1[3] = 0x10;	   //12~5
    TX_buffer_1[4] = 0x00;	   // 4~0

	complete_1 = 0;
	//��ʼ���������ݵ�Ԫ
	
	TX_buffer_1[5] = 0x01;	//Դ��ַ
	TX_buffer_1[6] = 0x10;
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
	init_mcu();
	init_sja1000();
//	time0_init();
	while(1)
	{
//		if(interrupt_flag_1)
//		{	
	
			can_rxd();
			if(left_flag==1)
			{
				left_light = 0;//��ת��
				delay(500000);
				left_light = 1;
				delay(500000);
				
			}
		}
	
//		 }
}
//	 void time0() interrupt 1 
//{
//	static int i=0;
//	uchar control_light,light_state;
//	control_light = RX_buffer_1[5];	//���ݵ����ֽ������ж��Ǻ��ֵơ�
//	light_state = RX_buffer_1[6];
//	TH1=0XFC;	
//	TL1=0X18;
//							   g
//	if(control_light == 0x04&&light_state == 0x01)
//	{	i++;
//		if(i=1000){
//		backup_light = !backup_light;	
//		i=0;}
//		
//	}						
//}	 
