#include "reg52.h"
sbit KEY1 = P3^1;
sbit KEY2 = P3^0;
sbit KEY3 = P3^2;
sbit KEY4 = P3^3;
sbit LED1 = P2^0;
sbit LED3 = P2^2;
sbit LED5 = P2^4;
sbit LED7 = P2^6;
#define key1_p 	1
#define key2_p 	2
#define key3_p 	3
#define key4_p 	4
#define nopress 5



void delay(unsigned int x){
     while(x--);
	 }
unsigned char scan(unsigned char mode){
     static unsigned char key=1;
	 if(mode==1){
	 key=1;}
	 
	 if(key==1&&KEY1==0){
	 delay(1000);
	 key=0;	
	 return key1_p;} 
	 else if(key==1&&KEY2==0){
	 delay(100000);
	 key=0;
	 return key2_p;}
	 else if(key==1&&KEY3==0){
	 delay(1000);
	 key=0;
	 return key3_p;}
	 else if(key==1&&KEY4==0){
	 delay(1000);
	 key=0;
	 return key4_p;}   
 
 /*	if(key==1&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0)){
	 delay(1000);
	 key=0;
	 if(KEY1==0)
            return key1_p;
     else if(KEY2==0)
		    return key2_p;
	 else if(KEY3==0)
			return key3_p;
	 else if(KEY4==0)
			return key4_p;}	*/ 
	 else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)
	 { delay(1000);
	 key=1;
	   return nopress;	}
	 }
	  
void main()
{	 unsigned char key=0; 
     while(1)
	 {key=scan(0);
	 if(key==key2_p){
	    LED1=!LED1;
		LED3=!LED3;
		LED5=!LED5;
		LED7=!LED7;
	 }	 
	 }
}