#include "stm32f10x.h"

#define uint unsigned int
#define uchar unsigned char

#define CLOCK 72/8 //ʱ��=72M

/*------------------------------------------------------------
                         us��ʱ���� 
------------------------------------------------------------*/
void delay_us(unsigned int us)
{
	u8 n;		    
	while(us--)for(n=0;n<CLOCK;n++); 	 
}

/*------------------------------------------------------------
                         ms��ʱ����
------------------------------------------------------------*/
void delay_ms(unsigned int ms)
{
	while(ms--)delay_us(1000);	 
}

/*------------------------------------------------------------
                      ������ʱ�ӼĴ�����λ
------------------------------------------------------------*/
#if 0
void RCC_DeInit(void)
{									   
	RCC->APB2RSTR = 0x00000000;//���踴λ			 
	RCC->APB1RSTR = 0x00000000;   	  
  	RCC->AHBENR = 0x00000014;  //flashʱ��,����ʱ��ʹ��.DMAʱ�ӹر�	  
  	RCC->APB2ENR = 0x00000000; //����ʱ�ӹر�.			   
  	RCC->APB1ENR = 0x00000000;   
	RCC->CR |= 0x00000001;     //ʹ���ڲ�����ʱ��HSION	 															 
	RCC->CFGR &= 0xF8FF0000;   //��λSW[1:0],HPRE[3:0],PPRE1[2:0],PPRE2[2:0],ADCPRE[1:0],MCO[2:0]					 
	RCC->CR &= 0xFEF6FFFF;     //��λHSEON,CSSON,PLLON
	RCC->CR &= 0xFFFBFFFF;     //��λHSEBYP	   	  
	RCC->CFGR &= 0xFF80FFFF;   //��λPLLSRC, PLLXTPRE, PLLMUL[3:0] and USBPRE 
	RCC->CIR = 0x00000000;     //�ر������ж�
}
#endif


/*------------------------------------------------------------
                  �ⲿ8M,��õ�72M��ϵͳʱ��
------------------------------------------------------------*/	
void Stm32_Clock_Init(void)
{
	unsigned char temp=0;
	u8 timeout=0;
	RCC_DeInit();
	RCC->CR|=0x00010000;  //�ⲿ����ʱ��ʹ��HSEON

	timeout=0;
	while(!(RCC->CR>>17)&&timeout<200)timeout++;//�ȴ��ⲿʱ�Ӿ���	 

	//0-24M �ȴ�0;24-48M �ȴ�1;48-72M�ȴ�2;(�ǳ���Ҫ!)	   
	FLASH->ACR|=0x32;//FLASH 2����ʱ����

	RCC->CFGR|=0X001D2400;//APB1/2=DIV2;AHB=DIV1;PLL=9*CLK;HSE��ΪPLLʱ��Դ
	RCC->CR|=0x01000000;  //PLLON

	timeout=0;
	while(!(RCC->CR>>25)&&timeout<200)timeout++;//�ȴ�PLL����

	RCC->CFGR|=0x00000002;//PLL��Ϊϵͳʱ��
	while(temp!=0x02&&timeout<200)     //�ȴ�PLL��Ϊϵͳʱ�����óɹ�
	{   
		temp=RCC->CFGR>>2;
		timeout++;
		temp&=0x03;
	}  
}

