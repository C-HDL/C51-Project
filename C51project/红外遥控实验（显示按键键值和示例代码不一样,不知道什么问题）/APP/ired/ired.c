#include "ired.h"

u8 gired_data[4];

void ired_init()
{
	IT0=1;
	EX0=1;
	EA=1;
	IRED=1;
}

void ired() interrupt 0
{
	u16 time_cnt=0;
	u8 i=0,j=0;
	u8 ired_high_time=0;
	if(IRED==0)
	{
		time_cnt=1000;
		while((!IRED)&&time_cnt)
		{
			  delay(1);
			  time_cnt--;
		
		if(time_cnt==0)return;
		}
	
				if(IRED==1)
			{
						time_cnt=500;
						while(IRED&&time_cnt)
					{
						delay(1);
						time_cnt--;
					
						if(time_cnt==0)return;
					}
									for(i=0;i<4;i++)
								{
											for(j=0;j<8;j++)
										{
													time_cnt=600;
													while((!IRED)&&time_cnt)
										{
			  											delay(1);
			  											time_cnt--;
										
													if(time_cnt==0)return;
										}
													time_cnt=20;
													while(IRED&&time_cnt)
										{
			  										delay(10);
			  										ired_high_time++;
			  										time_cnt--;
										
													if(time_cnt==0)return;
										}
													if(ired_high_time>=8)
														gired_data[i]|=0x80;
													gired_data[i]>>=1;
													ired_high_time=0;
										}
								}
			}
			if(gired_data[2]!=~gired_data[3])
			{
				for(i=0;i<4;i++)
				{
					gired_data[i]=0;
					return;
				}	
			}	 
	} 
}