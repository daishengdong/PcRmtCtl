#ifndef __REMOTE_H
#define __REMOTE_H

#include "../FWlib/inc/stm32f10x_tim.h"

//位带操作,实现51类似的GPIO控制功能
//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 

#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define RDATA   PBin(9)		//红外数据输入脚

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;	        //按键按下的次数

void Remote_Init(void);     //红外传感器接收头引脚初始化
u8 Remote_Scan(void);
#endif
