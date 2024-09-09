#ifndef _at24c02_H
#define _at24c02_H

#include "public.h"

void at24c02_write_byte(u8 address,u8 dat);
u8 at24c02_read_byte(u8 address);
#endif