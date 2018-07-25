#ifndef __REMOTE_H
#define __REMOTE_H

#include "../FWlib/inc/stm32f10x_tim.h"

//λ������,ʵ��51���Ƶ�GPIO���ƹ���
//����ʵ��˼��,�ο�<<CM3Ȩ��ָ��>>������(87ҳ~92ҳ).
//IO�ڲ����궨��
#define BITBAND(addr, bitnum) ((addr & 0xF0000000) + 0x2000000 + ((addr & 0xFFFFF) << 5) + (bitnum << 2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 

#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //���� 

#define RDATA   PBin(9)		//�������������

//����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
//����ѡ�õ�ң����ʶ����Ϊ0
#define REMOTE_ID 0      		   

extern u8 RmtCnt;	        //�������µĴ���

void Remote_Init(void);     //���⴫��������ͷ���ų�ʼ��
u8 Remote_Scan(void);
#endif
