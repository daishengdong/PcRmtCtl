#ifndef __SYS_H
#define __SYS_H	   

//us��ʱ���� 
void delay_us(unsigned int us);
//ms��ʱ����
void delay_ms(unsigned int ms);
//������ʱ�ӼĴ�����λ
void RCC_DeInit(void);
//�ⲿ8M,��õ�72M��ϵͳʱ��	
void Stm32_Clock_Init(void);
#endif
