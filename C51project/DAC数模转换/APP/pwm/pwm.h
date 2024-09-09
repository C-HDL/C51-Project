#ifndef _pwm_H
#define _pwm_H

#include "public.h"
sbit PWM=P2^1;
void pwm_init(u8 high,u8 low,u8 scale,u8 duty);
void set_duty(u8 duty);

#endif