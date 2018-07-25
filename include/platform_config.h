/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : platform_config.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Evaluation board specific configuration file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_type.h"


/* Define the STM32F10x hardware depending on the used evaluation board */
  #define USB_DISCONNECT                      GPIOB  
  #define USB_DISCONNECT_PIN                  GPIO_Pin_8
  #define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOB
  #define RCC_APB2Periph_GPIO_KEY             RCC_APB2Periph_GPIOA
  
  #define GPIO_Pin_KEY                        GPIO_Pin_1  /* PA.1 */
  #define GPIO_Pin_UP                         GPIO_Pin_2  /* PA.2 */
  #define GPIO_Pin_DOWN                       GPIO_Pin_3  /* PA.3 */
  #define GPIO_Pin_LEFT                       GPIO_Pin_4  /* PA.4 */
  #define GPIO_Pin_RIGHT                      GPIO_Pin_5  /* PA.5 */
  
  #define RCC_APB2Periph_GPIO_JOY_SET1        RCC_APB2Periph_GPIOA
  #define RCC_APB2Periph_GPIO_JOY_SET2        RCC_APB2Periph_GPIOA

  #define GPIO_RIGHT                          GPIOA
  #define GPIO_LEFT                           GPIOA
  #define GPIO_DOWN                           GPIOA
  #define GPIO_UP                             GPIOA
  #define GPIO_KEY                            GPIOA

                                                
  #define RCC_APB2Periph_ALLGPIO             (RCC_APB2Periph_GPIOA \
                                              | RCC_APB2Periph_GPIOB \
                                              | RCC_APB2Periph_GPIOC \
                                              | RCC_APB2Periph_GPIOD)

  #define GPIO_KEY_PORTSOURCE                 GPIO_PortSourceGPIOA
  #define GPIO_KEY_PINSOURCE                  GPIO_PinSource8
  #define GPIO_KEY_EXTI_Line                  EXTI_Line8

  #define EXTI_KEY_IRQChannel                 EXTI9_5_IRQChannel

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

