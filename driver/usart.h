#ifndef	 USART
#define	 USART
#include "../FWlib/inc/stm32f10x_usart.h"
#include "../FWlib/inc/stm32f10x_gpio.h"
#include "../FWlib/inc/stm32f10x_conf.h"
void usart_init(void);
void usart_send(u8 *data);
void NVIC_Config(void);
#endif

