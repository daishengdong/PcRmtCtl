/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : main.c
* Author             : Zachary(F.SB)
* Version            : V1.0
* Date               : 12/30/2013
* Description        : main file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x_lib.h"
#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "../driver/tty.h"
#include "../driver/24l01.h"
#include "../driver/Delay.h"
#include "../driver/usart.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(vu32 nCount);

/* Private functions ---------------------------------------------------------*/
#define DEBUG
/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

// 十进制打印数字，调试用
void debug_print(u8 x) {
	s8 i = 0;
	u8 buff[8];

	for (i = 0; i < 8; ++i) {
		buff[i] = 0;
	}

	i = 0;
	while (x != 0) {
		buff[i++] = x % 10;
		x = x / 10;
	}
	for (i = 7; i >= 0; --i) {
		USART_SendData(USART1, buff[i] + '0');
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
	tty_printf("\n");
}

typedef enum {
	KEYBOARD	= 0x01,
	MOUSE		= 0x02,
} Device;
	
#define None_Key	0x00

typedef enum {
	Left_Control	= 0x01,
	Left_Shift		= 0x02,
	Left_Alt		= 0x04,
	Left_GUI		= 0x08,
	Right_Control	= 0x10,
	Right_Shift	= 0x20,
	Right_Alt		= 0x40,
	Right_GUI		= 0x80,
} FunctionKey;

typedef enum {
	Left_Key		= 0x01,
	Right_Key		= 0x02,
	Mid_Kry			= 0x04,
} MouseKey;
 
u8 HID_SendBuffer[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
 
void HID_Send(Device device) {
	HID_SendBuffer[0] = (u8)device;
	
    /*copy mouse position info in ENDP1 Tx Packet Memory Area*/
    UserToPMABufferCopy(HID_SendBuffer, GetEPTxAddr(ENDP1), 9);
    /* enable endpoint for transmission */
    SetEPTxValid(ENDP1);
 
	DelayMs(20);
}
 
void SendKeyboardData(u8 ctrl_shift, u8 key) {
	HID_SendBuffer[1] = ctrl_shift;
	HID_SendBuffer[3] = key;
	HID_Send(KEYBOARD);
}
 
void SendMouseData(u8 key, s8 x, s8 y) {
	HID_SendBuffer[1] = key;
	HID_SendBuffer[2] = (u8)x;
	HID_SendBuffer[3] = (u8)y;
	HID_Send(MOUSE);
}


void mouse_up(void) {
	u8 delta_y = 10;
	u8 delta_x = 0;
	u8 sign = 0;
	
	HID_SendBuffer[0] = MOUSE;
	HID_SendBuffer[1] = 100;
	HID_SendBuffer[2] = 100;
	HID_SendBuffer[3] = 0;

	UserToPMABufferCopy(HID_SendBuffer, GetEPTxAddr(ENDP1), 5);
	SetEPTxCount(ENDP1, 5);
    /* enable endpoint for transmission */
    SetEPTxValid(ENDP1);
 
	DelayMs(20);
}

void mouse_down(void) {
	u8 delta_y = 10;
	u8 delta_x = 0;
	u8 sign = 1;


	HID_SendBuffer[0] = 0x8;
	HID_SendBuffer[1] = 100;
	HID_SendBuffer[2] = 100;
	HID_SendBuffer[3] = 0;

	UserToPMABufferCopy(HID_SendBuffer, GetEPTxAddr(ENDP1), 5);
	SetEPTxCount(ENDP1, 5);
	/* enable endpoint for transmission */
	SetEPTxValid(ENDP1);

	DelayMs(20);

}

void mouse_left(void) {
	u8 delta_y = 0;
	u8 delta_x = 10;
	u8 sign = 1;

}

void mouse_right(void) {
	u8 delta_y = 0;
	u8 delta_x = 10;
	u8 sign = 0;

}

u8 is_led_on = 0;

void set_led(void) {
	if (is_led_on) {
		GPIOB->ODR = 0;			 //全部输出0
		GPIOA->ODR = 0;		
		GPIOC->ODR = 0;
		GPIOD->ODR = 0;
	} else {
		GPIOB->ODR = 0xffffffff;	 //全部输出1 
		GPIOA->ODR = 0xffffffff;	  
		GPIOC->ODR = 0xffffffff;
		GPIOD->ODR = 0xffffffff;
	}
	is_led_on = !is_led_on;
}

void led_init(void) {
	GPIOB->CRH=0X33333333;	//推挽输出
	GPIOB->CRL=0X33333333;	//推挽输出
	GPIOC->CRH=0X33333333;	//推挽输出
	GPIOC->CRL=0X33333333;	//推挽输出
	GPIOD->CRH=0X33333333;	//推挽输出
	GPIOD->CRL=0X33333333;	//推挽输出
	GPIOA->CRH=0X33333333;	//推挽输出
	GPIOA->CRL=0X33333333;	//推挽输出
}

int main(void)
{
	u8 key;

	Set_System();
	Delay_init(72);
	tty_init();
	// usart_init();

	Remote_Init(); //红外接收初始化

	USB_Interrupts_Config();
#ifdef DEBUG
	tty_printf("Interrupts completed\n");
#endif

	Set_USBClock();
#ifdef DEBUG
	tty_printf("USBClock completed\n");
#endif

	USB_Init();
#ifdef DEBUG	
	tty_printf("USB_Init completed\n");
#endif

	// NRF24L01_Init();
#ifdef DEBUG
	tty_printf("this is 2.4G init\n");
#endif

#if 0
	Stm32_Clock_Init();//系统时钟设置
	RCC->APB2ENR |= 0x00000001; //开启afio时钟
	// AFIO->MAPR = (0x00FFFFFF & AFIO->MAPR)|0x04000000;		  //关闭JTAG 

	RCC->APB2ENR|=0X0000001c;//先使能外设IO PORTa,b,c时钟

	led_init();

	// delay_ms(100);
#endif

	while (1) {
		key = Remote_Scan();
		if (key) {
			// debug_print(key);
			switch (key) {
				case 162: // 1
					break;
				case 98: // 2
					// mouse_up();
					break;
				case 226: // 3
					break;
				case 34: // 4
					// mouse_left();
					break;
				case 2: // 5
					// double click
					// Send_MouseKeys(Package.mouse_x,Package.mouse_y,MouseKey);
					break;
				case 194: // 6
					// mouse_right();
					break;
				case 224: // 7
					break;
				case 168: // 8
					// mouse_down();
					break;
				case 144: // 9
					break;
				case 104: // *
					// alt + tab
					// Send_BoardKeys(0xE2, 0x2b, 0x00);
					break;
				case 152: // 0
					break;
				case 176: // #
					break;

				case 24: // up
					// debug_print(key);
					Send_BoardKeys(0x00, 0x52, 0x00);
					break;
				case 74: // down
					Send_BoardKeys(0x00, 0x51, 0x00);
					break;
				case 16: // left
					Send_BoardKeys(0x00, 0x50, 0x00);
					break;
				case 90: // right
					Send_BoardKeys(0x00, 0x4F, 0x00);
					break;
				case 56: // ok
					// debug_print(key);
					// space, play <-> pause
					Send_BoardKeys(0x00, 0x2C, 0x00);
					break;
			}
		} else {
			DelayMs(100);
			// 看板子到底有没有在正常工作
#if 0
			set_led();
#endif
		}
	}
		/*
		status=0;
  while(status==0){
		status=NRF24L01_Check();
		if(status)
				tty_printf("connect ok \n");				
		else
				tty_printf("connect failed \r\n");
		DelayMs(100);
	}
	
	status = NRF_RX_Mode();
#ifdef DEBUG
	tty_printf("Enter RevMode\n");
#endif  
	while (1)
  {
    if ((bDeviceState == CONFIGURED))
    {
				if(con){tty_printf("connected");con=0;}
				status = NRF_Rx_Dat((u8 *)&Package);
				if(status == 0){
					 tty_printf("receive Data:");
#ifdef DEBUG
					 tty_printf_H("%X",(u8*)&Package);
					 tty_printf("\n");
#endif
					 ReveiveCmd(Package);			
				}
    }
  }*/
	
	
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for (; nCount != 0;nCount--);
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


