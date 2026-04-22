/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f415_wk_config.h
  * @brief    header file of work bench config
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* define to prevent recursive inclusion -----------------------------------*/
#ifndef __AT32F415_WK_CONFIG_H
#define __AT32F415_WK_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* includes -----------------------------------------------------------------------*/
#include "stdio.h"
#include "at32f415.h"

/* private includes -------------------------------------------------------------*/
/* add user code begin private includes */

/* add user code end private includes */

/* exported types -------------------------------------------------------------*/
/* add user code begin exported types */

/* add user code end exported types */

/* exported constants --------------------------------------------------------*/
/* add user code begin exported constants */

/* add user code end exported constants */

/* exported macro ------------------------------------------------------------*/
/* add user code begin exported macro */

/* add user code end exported macro */

/* add user code begin dma define */
/* user can only modify the dma define value */
#define DMA1_CHANNEL1_BUFFER_SIZE   0
#define DMA1_CHANNEL1_MEMORY_BASE_ADDR   0
#define DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR  0

//#define DMA1_CHANNEL2_BUFFER_SIZE        
//#define DMA1_CHANNEL2_MEMORY_BASE_ADDR   
//#define DMA1_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL3_BUFFER_SIZE   0
//#define DMA1_CHANNEL3_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL4_BUFFER_SIZE   0
//#define DMA1_CHANNEL4_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL5_BUFFER_SIZE   0
//#define DMA1_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL5_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL6_BUFFER_SIZE   0
//#define DMA1_CHANNEL6_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL6_PERIPHERAL_BASE_ADDR   0

//#define DMA1_CHANNEL7_BUFFER_SIZE   0
//#define DMA1_CHANNEL7_MEMORY_BASE_ADDR   0
//#define DMA1_CHANNEL7_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL1_BUFFER_SIZE   0
//#define DMA2_CHANNEL1_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL1_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL2_BUFFER_SIZE   0
//#define DMA2_CHANNEL2_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL2_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL3_BUFFER_SIZE   0
//#define DMA2_CHANNEL3_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL3_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL4_BUFFER_SIZE   0
//#define DMA2_CHANNEL4_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL4_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL5_BUFFER_SIZE   0
//#define DMA2_CHANNEL5_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL5_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL6_BUFFER_SIZE   0
//#define DMA2_CHANNEL6_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL6_PERIPHERAL_BASE_ADDR   0

//#define DMA2_CHANNEL7_BUFFER_SIZE   0
//#define DMA2_CHANNEL7_MEMORY_BASE_ADDR   0
//#define DMA2_CHANNEL7_PERIPHERAL_BASE_ADDR   0
/* add user code end dma define */

/* Private defines -------------------------------------------------------------*/
#define JC_COM1_245_PIN    GPIO_PINS_13
#define JC_COM1_245_GPIO_PORT    GPIOC
#define CHECK_REPLACE_PIN    GPIO_PINS_14
#define CHECK_REPLACE_GPIO_PORT    GPIOC
#define CHECK_START_PIN    GPIO_PINS_15
#define CHECK_START_GPIO_PORT    GPIOC
#define LED_CF_PIN    GPIO_PINS_0
#define LED_CF_GPIO_PORT    GPIOC
#define SLEEP_G_PIN    GPIO_PINS_2
#define SLEEP_G_GPIO_PORT    GPIOC
#define BUZZ_PIN    GPIO_PINS_3
#define BUZZ_GPIO_PORT    GPIOC
#define FAN_FG_PIN    GPIO_PINS_0
#define FAN_FG_GPIO_PORT    GPIOA
#define AD_AIRGUN_PIN    GPIO_PINS_1
#define AD_AIRGUN_GPIO_PORT    GPIOA
#define AD_HOTGUN_PIN    GPIO_PINS_2
#define AD_HOTGUN_GPIO_PORT    GPIOA
#define OVER_LOAD_PIN    GPIO_PINS_3
#define OVER_LOAD_GPIO_PORT    GPIOA
#define HOTSOL_PWM_PIN    GPIO_PINS_4
#define HOTSOL_PWM_GPIO_PORT    GPIOF
#define KEY_CH3_PIN    GPIO_PINS_5
#define KEY_CH3_GPIO_PORT    GPIOF
#define KEY_CH2_PIN    GPIO_PINS_4
#define KEY_CH2_GPIO_PORT    GPIOA
#define KEY_CH1_PIN    GPIO_PINS_5
#define KEY_CH1_GPIO_PORT    GPIOA
#define HOTAIR_PWM_PIN    GPIO_PINS_6
#define HOTAIR_PWM_GPIO_PORT    GPIOA
#define JC_COM1_210_PIN    GPIO_PINS_7
#define JC_COM1_210_GPIO_PORT    GPIOA
#define HOT_REALY_PIN    GPIO_PINS_4
#define HOT_REALY_GPIO_PORT    GPIOC
#define KEY_CH23_PIN    GPIO_PINS_5
#define KEY_CH23_GPIO_PORT    GPIOC
#define KEY_CH22_PIN    GPIO_PINS_0
#define KEY_CH22_GPIO_PORT    GPIOB
#define KEY_CH21_PIN    GPIO_PINS_1
#define KEY_CH21_GPIO_PORT    GPIOB
#define FAN_PWM_PIN    GPIO_PINS_10
#define FAN_PWM_GPIO_PORT    GPIOB
#define DP_PIN    GPIO_PINS_11
#define DP_GPIO_PORT    GPIOB
#define EC11_R_HOT_PIN    GPIO_PINS_12
#define EC11_R_HOT_GPIO_PORT    GPIOB
#define EC11_L_HOT_PIN    GPIO_PINS_13
#define EC11_L_HOT_GPIO_PORT    GPIOB
#define KEY_SET3_PIN    GPIO_PINS_14
#define KEY_SET3_GPIO_PORT    GPIOB
#define EC11_R_SOL_PIN    GPIO_PINS_15
#define EC11_R_SOL_GPIO_PORT    GPIOB
#define EC11_L_SOL_PIN    GPIO_PINS_6
#define EC11_L_SOL_GPIO_PORT    GPIOC
#define KEY_SET1_PIN    GPIO_PINS_7
#define KEY_SET1_GPIO_PORT    GPIOC
#define EC11_R_AIR_PIN    GPIO_PINS_8
#define EC11_R_AIR_GPIO_PORT    GPIOC
#define EC11_L_AIR_PIN    GPIO_PINS_9
#define EC11_L_AIR_GPIO_PORT    GPIOC
#define KEY_SET2_PIN    GPIO_PINS_8
#define KEY_SET2_GPIO_PORT    GPIOA
#define DG9_PIN    GPIO_PINS_11
#define DG9_GPIO_PORT    GPIOA
#define DG8_PIN    GPIO_PINS_12
#define DG8_GPIO_PORT    GPIOA
#define DG7_PIN    GPIO_PINS_6
#define DG7_GPIO_PORT    GPIOF
#define DG6_PIN    GPIO_PINS_7
#define DG6_GPIO_PORT    GPIOF
#define DG5_PIN    GPIO_PINS_15
#define DG5_GPIO_PORT    GPIOA
#define DG4_PIN    GPIO_PINS_10
#define DG4_GPIO_PORT    GPIOC
#define DG3_PIN    GPIO_PINS_11
#define DG3_GPIO_PORT    GPIOC
#define DG2_PIN    GPIO_PINS_12
#define DG2_GPIO_PORT    GPIOC
#define DG1_PIN    GPIO_PINS_2
#define DG1_GPIO_PORT    GPIOD
#define A_PIN    GPIO_PINS_3
#define A_GPIO_PORT    GPIOB
#define B_PIN    GPIO_PINS_4
#define B_GPIO_PORT    GPIOB
#define C_PIN    GPIO_PINS_5
#define C_GPIO_PORT    GPIOB
#define D_PIN    GPIO_PINS_6
#define D_GPIO_PORT    GPIOB
#define E_PIN    GPIO_PINS_7
#define E_GPIO_PORT    GPIOB
#define F_PIN    GPIO_PINS_8
#define F_GPIO_PORT    GPIOB
#define G_PIN    GPIO_PINS_9
#define G_GPIO_PORT    GPIOB

/* exported functions ------------------------------------------------------- */
  /* system clock config. */
  void wk_system_clock_config(void);

  /* config periph clock. */
  void wk_periph_clock_config(void);

  /* nvic config. */
  void wk_nvic_config(void);

/* add user code begin exported functions */

/* add user code end exported functions */

#ifdef __cplusplus
}
#endif

#endif
