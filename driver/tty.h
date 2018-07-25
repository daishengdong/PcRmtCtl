#ifndef tty
#define tty
#include "../FWlib/inc/stm32f10x_usart.h"
#include "../FWlib/inc/stm32f10x_gpio.h"
void tty_init(void);
int tty_command(u8 *cmd);
int tty_printf(u8 *str);
int tty_printf_H(char *mode,u8 *str);
#endif
