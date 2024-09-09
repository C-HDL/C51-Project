#include "key.h"

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
	 //while(KEY2!=1);
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
	  
