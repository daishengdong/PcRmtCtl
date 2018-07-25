/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Hardware Configuration & Setup
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
#include "hw_config.h"
#include "stm32f10x_it.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "platform_config.h"
#include "usb_pwr.h"
#include "../driver/Delay.h"
#include <math.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/

  /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* On STICE the PLL output clock is fixed to 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }
  /* enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_USART2, ENABLE);

  /* Set all the GPIOs to AIN */
  //GPIO_AINConfig();
	/* Enable GPIOA, GPIOD and USART1 clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
                         | RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);


  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,DISABLE);//stop JATG

}



/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_AINConfig
* Description    : Configures all IOs as AIN to reduce the power consumption.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void GPIO_AINConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable all GPIOs Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALLGPIO, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  /* Disable all GPIOs Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALLGPIO, DISABLE);
}



/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}
#if 0
/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;

  /* Clear EXTI Line18 pending bit */
  EXTI_ClearITPendingBit(GPIO_KEY_EXTI_Line);

  /* Request to enter STOP mode with regulator in low power mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Enable HSE */
  //RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  //HSEStartUpStatus = RCC_WaitForHSEStartUp();

  /* Enable HSE */
  //RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  //while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  //{}

  /* Enable PLL */
  //RCC_PLLCmd(ENABLE);

  /* Wait till PLL is ready */
  //while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  //{}

  /* Select PLL as system clock source */
  //RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

  /* Wait till PLL is used as system clock source */
  //while (RCC_GetSYSCLKSource() != 0x08)
  //{}

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}
#endif
/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Set the Vector Table base address at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);

  /* 1 bit for pre-emption priority, 3 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;//USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the USB Wake-up interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn; //USBWakeUp_IRQChannel;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);

  /* Enable USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//Channel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init(&NVIC_InitStructure);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); 	 //设置优先级分组：先占优先级和从优先级
  NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;          //通道 使能或者失能指定的IRQ 通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;// 该参数设置了成员NVIC_IRQChannel  中的先占优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority=0; //   该参数设置了成员NVIC_IRQChannel  中的从优先级
  NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 		 //通道开启
  NVIC_Init(&NVIC_InitStructure); 
  /* Enable the Key EXTI line Interrupt */
  //NVIC_InitStructure.NVIC_IRQChannel = EXTI_KEY_IRQChannel;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  //NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  //NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}



/*****************************************
Function:Send keyboard Keys
********************************************/
void Send_BoardKeys(u8 Key1,u8 Key2,u8 Key3){
		u8 KeyBoard_Buffer[16] = {0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0};
		KeyBoard_Buffer[0] = 0x01;
		KeyBoard_Buffer[1] = Key1;
		KeyBoard_Buffer[3] = Key2;
		KeyBoard_Buffer[4] = Key3;
		/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
		UserToPMABufferCopy(KeyBoard_Buffer, GetEPTxAddr(ENDP1), 9);
		SetEPTxCount(ENDP1, 9);
		/* enable endpoint for transmission */
		SetEPTxValid(ENDP1);
		DelayMs(60);
		KeyBoard_Buffer[1] = 0;
		KeyBoard_Buffer[3] = 0;
		KeyBoard_Buffer[4] = 0;
		/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
		UserToPMABufferCopy(KeyBoard_Buffer, GetEPTxAddr(ENDP1), 9);
		SetEPTxCount(ENDP1, 9);
		/* enable endpoint for transmission */
		SetEPTxValid(ENDP1);
}

/*****************************************
Function:Send Mouse Keys
Key :0x01,left,0x02,right,0x04,middle
********************************************/
void Send_MouseKeys(short X,short Y,u8 Key){
		u8 Mouse_Buffer[5] = {0, 0, 0, 0,0};
		short max_beishu,tmp,tmpx,tmpy;
		u8 i;
		s8 mouse_x,mouse_y;
		/* prepare buffer to send */
		Mouse_Buffer[0] = 0x02;
		Mouse_Buffer[1] = Key;
		if(tmpx<0)tmpx=-tmpx;
		if(tmpy<0)tmpy=-tmpy;
		tmp=tmpx>tmpy?tmpx:tmpy;
		max_beishu=tmp/CURSOR_STEP;
		mouse_x=X/max_beishu;
		mouse_y=Y/max_beishu;
		for(i=0;i<max_beishu;i++){
			Mouse_Buffer[2] = mouse_x;
			Mouse_Buffer[3] = mouse_y;
			/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
			UserToPMABufferCopy(Mouse_Buffer, GetEPTxAddr(ENDP1), 5);
			SetEPTxCount(ENDP1, 5);
			/* enable endpoint for transmission */
			SetEPTxValid(ENDP1);
			DelayMs(20);
		}
		Mouse_Buffer[2] = X-mouse_x*max_beishu;
		Mouse_Buffer[3] = Y-mouse_y*max_beishu;
		/*copy mouse position info in ENDP1 Tx Packet Memory Area*/
		UserToPMABufferCopy(Mouse_Buffer, GetEPTxAddr(ENDP1), 5);
		SetEPTxCount(ENDP1, 5);
		/* enable endpoint for transmission */
		SetEPTxValid(ENDP1);
		DelayMs(20);
}
/*********************************************
*input ascii return usb code
*********************************************/
/*0-9 a-z*/
uchar keymap[37]={
0x27,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26, //0-9
0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11,
0x12,0x13,0x14,0x15,0x16,0x17,0x018,0x19,0x1A,0x1B,0x1C,0x1D
};
uchar KeyMap(uchar n){
	if('0'<= n && n<='9'){
		return keymap[n-'0'];
	}
	if('a'<=n && n<='z'){
		return keymap[n-'a'+10];
	}
}
/*******************************************************************************
* Function Name : ReveiveCmd
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void ReveiveCmd(PackFormat Package)
{
	u8 Keys,MouseKey=0;
	Keys=Package.FB;
	Package.FB=0;
	switch(Keys)
	{
		default:break;
		case MouseLeftButton: MouseKey=USB_MouseLeftButton;break;
		case MouseRightButton: MouseKey=USB_MouseRightButton;break;
		case EnterPaintButton: Send_BoardKeys(USB_MSEnterPaintButton);return;
		case LeavePaintButton: Send_BoardKeys(USB_LeavePaintButton);return;
		case ErasePaintButton: Send_BoardKeys(USB_ErasePaintButton);return;
		case EnterFullScreen: Send_BoardKeys(USB_EnterFullScreen);return;
		case ExitFillScreen: Send_BoardKeys(USB_ExitFillScreen);return;
		case PageUpButton: Send_BoardKeys(USB_PageUpButton);return;
		case PageDownButton: 	 Send_BoardKeys(USB_PageDownButton);return;
		case CtrlAltDelete: Send_BoardKeys(USB_CtrlAltDelete);return;
		case InputPassWord: {
			DelayMs(100);
			Send_BoardKeys(0x00,KeyMap('n'),0x00);
			DelayMs(100);
			Send_BoardKeys(0x00,KeyMap('i'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('h'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('a'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('o'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('8'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('6'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,KeyMap('4'),0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,0x28,0x00);
			DelayMs(80);
			Send_BoardKeys(0x00,0x28,0x00);
			DelayMs(80);
			return;
		}
		case ModeSwitchButton: return;
	}
	DelayMs(10);
	if(Package.mouse_enanle=='1'){
		Send_MouseKeys(Package.mouse_x,Package.mouse_y,MouseKey);
	}else{
		Send_MouseKeys(0,0,0);
	}
}


/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(u32*)(0x1FFFF7E8);
  Device_Serial1 = *(u32*)(0x1FFFF7EC);
  Device_Serial2 = *(u32*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Joystick_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
    Joystick_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
    Joystick_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
    Joystick_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

    Joystick_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
    Joystick_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
    Joystick_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
    Joystick_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

    Joystick_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
    Joystick_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
    Joystick_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
    Joystick_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
