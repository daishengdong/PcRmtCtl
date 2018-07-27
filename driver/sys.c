#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char

#define CLOCK 72/8 //时钟=72M

/*------------------------------------------------------------
                         us延时函数 
------------------------------------------------------------*/
void delay_us(unsigned int us)
{
	u8 n;		    
	while(us--)for(n=0;n<CLOCK;n++); 	 
}

/*------------------------------------------------------------
                         ms延时函数
------------------------------------------------------------*/
void delay_ms(unsigned int ms)
{
	while(ms--)delay_us(1000);	 
}

/*------------------------------------------------------------
                      把所有时钟寄存器复位
------------------------------------------------------------*/
#if 0
void RCC_DeInit(void)
{									   
	RCC->APB2RSTR = 0x00000000;//外设复位			 
	RCC->APB1RSTR = 0x00000000;   	  
  	RCC->AHBENR = 0x00000014;  //flash时钟,闪存时钟使能.DMA时钟关闭	  
  	RCC->APB2ENR = 0x00000000; //外设时钟关闭.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //使能内部高速时钟HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //复位SW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //复位HSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //复位HSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //复位PLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //关闭所有中断
}
#endif


/*------------------------------------------------------------
                  外部8M,则得到72M的系统时钟
------------------------------------------------------------*/	
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
	u8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //外部高速时钟使能HSEON

	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//等待外部时钟就绪	 

	//0-24M 等待0;24-48M 等待1;48-72M等待2;(非常重要!)	   
	FLASH->ACR|=0x32;//FLASH 2个延时周期

	RCC->CFGR|=0X001D2400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE作为PLL时钟源
	RCC->CR|=0x01000000;  //PLLON

	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//等待PLL锁定

	RCC->CFGR|=0x00000002;//PLL作为系统时钟
	while(temp!=0x02&&timeout<200)     //等待PLL作为系统时钟设置成功
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
}

