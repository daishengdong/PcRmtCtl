#include "usart.h"
#include "../include/Head.h"
#include "tty.h"

//#include "stm32f10x.h"

/*******************
*author:Zachary
*file:usart.c
********************/
#define  usart_pin_tx	GPIO_Pin_2
#define  usart_pin_rx GPIO_Pin_3
#define  Buffer_Size 20
//Receive Buffer
u8 Rev_Buffer[Buffer_Size];

void usart_init(void){
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	// config pin
	GPIO_InitStructure.GPIO_Pin = usart_pin_tx;	         //tx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = usart_pin_rx;	         //rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// usart configure
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate =115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	 
	USART_Init(USART2, &USART_InitStructure);
	// irq fun
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	// open usart2
	USART_Cmd(USART2, ENABLE);
}

void USART2_IRQHandler(void){
	static u8 i=0;
	u16 temp;
	static HeadForRecv Head;
	static CMD_PKG     cmd;
	static PackFormat package;
	if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Rev_Buffer[i]=USART_ReceiveData(USART2);
	}
	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET){
		USART_ClearFlag(USART2,USART_FLAG_ORE);
		USART_ReceiveData(USART2);
	}

	return;
	
	if(i>10){
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
		i=0;
	}
	i++;
}

void usart_send(u8 *data){
	while(*data){
		USART_SendData(USART2,*data++);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}


