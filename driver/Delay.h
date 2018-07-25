#ifndef __DELAY_H
#define __DELAY_H 
#define u32 unsigned int

void Delay_init(u8 SYSCLOCK);
void DelayMs(u16 i);
void DelayUs(u32 nus);
#endif
