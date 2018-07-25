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
        SysTick->LOAD=nus*9; //时间加载                       
        SysTick->VAL=0x00;                //清空计数器  
        SysTick->CTRL=0x01 ;            //Enable Systick，开始倒数       
        do  
        {  
                temp=SysTick->CTRL;  
        }while(temp&0x01&&!(temp&(1<<16)));//等待时间到达        
        SysTick->CTRL=0x00;              //Disable Systick 
        SysTick->VAL =0X00;              //清空计数器          
}

**/
static u8  fac_us=0;//us延时倍乘数			   
static u16 fac_ms=0;//ms延时倍乘数,在ucos下,代表每个节拍的ms数
void Delay_init(u8 SYSCLK)
{
 	SysTick->CTRL&=~(1<<2);	//SYSTICK使用外部时钟源	 
	fac_us=SYSCLK/8;		//不论是否使用ucos,fac_us都需要使用	  
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   

}								    
	    								   
void DelayUs(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void DelayMs(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	  	    
} 
