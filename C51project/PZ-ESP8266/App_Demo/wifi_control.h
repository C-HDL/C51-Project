#ifndef _wifi_control_H
#define _wifi_control_H

#include "public.h"


void wifi_control_init(void);
void wifi_control(void);
//定义全局变量
extern u16 global_OUT1_SEL;
extern u16 global_OUT2_SEL;
extern u16 global_OUT3_SEL;
#endif
