#include "public.h"

void delay(unsigned int x){
     while(x--);
	 }
void delay_ms(u16 ms)
{
u16 i,j;
for(i=ms;i>0;i--)
for(j=110;j>0;j--);
}			
