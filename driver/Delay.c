//#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "Delay.h"

/******************
function:Delay function initialize
	important task is initialize timer3
author:Zachary
time:2013/8/12
*****************
void DelayMs_init(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=500-1;
	TIM_TimeBaseStructure.TIM_Prescaler=(72-1);
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}

void DelayMs(u32 i){
	u32 m;
	for(m=0;m<i;m++){
		TIM_Cmd(TIM2,ENABLE);
		while(TIM_GetITStatus(TIM2,TIM_IT_Update)==RESET);
					TIM_ClearFlag(TIM2,TIM_FLAG_Update);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
					TIM_Cmd(TIM2,DISABLE);
	}
}

void DelayUs(u32 nus)  
{                
        u32 temp;                            
        SysTick->LOAD=nus*9; //ʱ�����                       
        SysTick->VAL=0x00;                //��ռ�����  
        SysTick->CTRL=0x01 ;            //Enable Systick����ʼ����       
        do  
        {  
                temp=SysTick->CTRL;  
        }while(temp&0x01&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��        
        SysTick->CTRL=0x00;              //Disable Systick 
        SysTick->VAL =0X00;              //��ռ�����          
}

**/
static u8  fac_us=0;//us��ʱ������			   
static u16 fac_ms=0;//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
void Delay_init(u8 SYSCLK)
{
 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��	  
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   

}								    
	    								   
void DelayUs(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void DelayMs(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 
