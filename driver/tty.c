#include "tty.h"
#include "../include/hw_config.h"
//#include "../init/stm32f10x_it.h"

#define  tty_pin_tx	GPIO_Pin_9
#define  tty_pin_rx GPIO_Pin_10

void tty_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	//①串口时钟使能， GPIO 时钟使能，复用时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1|RCC_APB2Periph_AFIO, ENABLE);

	//②串口复位
	USART_DeInit(USART1); //复位串口 1

	//③GPIO 端口模式设置
	GPIO_InitStructure.GPIO_Pin = tty_pin_tx;	         //tx
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		   

	GPIO_InitStructure.GPIO_Pin = tty_pin_rx;	         //rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	// GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//④串口参数初始化
	// usart configure
	// USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 
	USART_Init(USART1, &USART_InitStructure);

#define EN_USART1_RX	1
#if EN_USART1_RX //如果使能了接收
	//⑤初始化 NVIC
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ; //抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //子优先级 3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure); //中断优先级初始化
	//⑤开启中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //开启中断
#endif

	//⑥使能串口	
	USART_Cmd(USART1, ENABLE);
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#if 0
void USART1_IRQHandler(void)
{
	u8 rev;
	rev=USART_ReceiveData(USART1);
	tty_printf(&rev);
	//ReveiveCmd(rev);
}
#endif

#include "head.h"

#define  Buffer_Size 20
//Receive Buffer
static u8 Rev_Buffer[Buffer_Size];


void USART1_IRQHandler(void) {
	static u8 i=0;
	u16 temp;
	static HeadForRecv Head;
	static CMD_PKG     cmd;
	static PackFormat package;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		Rev_Buffer[i]=USART_ReceiveData(USART2);
	}
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) == SET) {
		USART_ClearFlag(USART1, USART_FLAG_ORE);
		USART_ReceiveData(USART1);
	}

	return;
	
	if (i > 10) {
		tty_printf(Rev_Buffer);
		//package.FB=MouseLeftButton;
		//package.mouse_x=200;
		//package.mouse_y=300;
		package.mouse_enanle='0';
		//package.FB = CtrlAltDelete;
		package.FB = CtrlAltDelete;
		ReveiveCmd(package);
		DelayMs(400);
		package.FB = InputPassWord;
		ReveiveCmd(package);
		ReveiveCmd(package);
		i = 0;
	}
	i++;
}

int tty_command(u8 *cmd){
		return 1;
}

int tty_printf(u8 *str){
	while(*str){
		USART_SendData(USART1,*str++);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
	return 1;
}
int tty_printf_H(char *mode,u8 *str){
	u8 tmp;
	while(*str){
		if(mode[0]=='%'&&mode[1]=='X'){
			tmp=*str;
			tmp=tmp>>4;
			if(tmp>=10){
				tmp=tmp-10+'A';
			}else{tmp+='0';}
			USART_SendData(USART1,tmp);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
			tmp=*str++;
			tmp=tmp&0x0f;
			if(tmp>=10){
				tmp=tmp-10+'A';
			}else{tmp+='0';}
			USART_SendData(USART1,tmp);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		}else{
			USART_SendData(USART1,*str++);
			while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		}
	}
	return 1;
}
