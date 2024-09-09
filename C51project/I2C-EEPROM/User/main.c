#include "public.h"
#include "key.h"
#include "smg.h"
#include "at24c02.h"
#define EEPROM_ADDRESS 0

void main(){
	u16 save_value=0;
	u8 key=0;
	u8 save_buf[3];
 	while(1){
	   key=scan(0);
	 if(key==key1_p){
	    at24c02_write_byte(EEPROM_ADDRESS,save_value);
	}
	 else if(key==key2_p) {
	 save_value=at24c02_read_byte(EEPROM_ADDRESS);
	 }
	 else if(key==key3_p) {
	 save_value++;
	 if(save_value>=255)
	 save_value=255;	 }
	 else if(key==key4_p) {save_value=0;  }

	 save_buf[0]=save_value/100;
	 save_buf[1]=save_value%100/10;
	 save_buf[2]=save_value%100%10;
	 smg_display(save_buf,6);
	
   }
 }