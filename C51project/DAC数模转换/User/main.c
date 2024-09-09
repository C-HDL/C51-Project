#include "public.h"
#include "pwm.h"


void main(){
	u8 dir=0;
	u8 duty=0;
	pwm_init(0xff,0xf6,100,0);
 	while(1){
		  if(dir==0)
		  {
		  	duty++;
			if(duty==70)dir=1;
		  }
		  else
		  {
		  	duty--;
			if(duty==0)dir=0;
		  }
	   	  set_duty(duty);
		  delay_ms(1);
 }	
	 }