/* add user code begin Header */
/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
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

/* Includes ------------------------------------------------------------------*/
#include "at32f415_wk_config.h"
#include "wk_adc.h"
#include "wk_crc.h"
#include "wk_debug.h"
#include "wk_tmr.h"
#include "wk_usart.h"
#include "wk_wdt.h"
#include "wk_dma.h"
#include "wk_gpio.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include "tmt.h"
#include "EventRecorder.h"
#include "iap.h"
#include "PC_comm_handle.h"
#include "beep_handle.h"
#include "ec11_handle.h"
#include "flash_handle.h"
#include "key_handle.h"
#include "led_handle.h"
#include "output_handle.h"
#include "DS201_handle.h"
#include "PID_handle.h"
#include "adc_filter.h"
#include "perf_counter.h"
/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

#define FEED_DOG_HANDLE_TIME   100

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */



/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */
void iap_task(void);
void feed_dog_task(void);
void pc_comm_task(void);
void output_task(void);
void led_task(void);
void key_task(void);
void flash_task(void);
void ec11_task(void);
void beep_task(void);

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  /* add user code begin 1 */
    /* config vector table offset */
    nvic_vector_table_set(NVIC_VECTTAB_FLASH, 0x4000);
    /* add user code end 1 */

  /* system clock config. */
  wk_system_clock_config();

  /* config periph clock. */
  wk_periph_clock_config();

  /* init debug function. */
  wk_debug_config();

  /* nvic config. */
  wk_nvic_config();

  /* timebase config. */
  wk_timebase_init();

  /* init gpio function. */
  wk_gpio_config();

  /* init dma1 channel1 */
  wk_dma1_channel1_init();
  /* config dma channel transfer parameter */
  /* user need to modify define values DMAx_CHANNELy_XXX_BASE_ADDR and DMAx_CHANNELy_BUFFER_SIZE in at32xxx_wk_config.h */
  wk_dma_channel_config(DMA1_CHANNEL1, 
                        DMA1_CHANNEL1_PERIPHERAL_BASE_ADDR, 
                        DMA1_CHANNEL1_MEMORY_BASE_ADDR, 
                        DMA1_CHANNEL1_BUFFER_SIZE);
  dma_channel_enable(DMA1_CHANNEL1, TRUE);

  /* init usart1 function. */
  wk_usart1_init();

  /* init adc1 function. */
  wk_adc1_init();

  /* init tmr2 function. */
  wk_tmr2_init();

  /* init tmr3 function. */
  wk_tmr3_init();

  /* init tmr5 function. */
  wk_tmr5_init();

  /* init crc function. */
  wk_crc_init();

  /* init wdt function. */
  wk_wdt_init();

  /* add user code begin 2 */
    tmt_init();
    tmt.create(iap_task,        IAP_HANDLE_TIME);
    tmt.create(feed_dog_task,   FEED_DOG_HANDLE_TIME);
    tmt.create(pc_comm_task,    PC_HANDLE_TIME);
	tmt.create(beep_task,    BEEP_TASK_TIME);
	tmt.create(ec11_task,    EC11_TASK_TIME);
	tmt.create(flash_task,    FLASH_TASK_TIME);
	tmt.create(key_task,    KEY_TASK_TIME);
	tmt.create(led_task,    DISP_HANDLE_TIME);
	tmt.create(output_task,    OUTPUT_HANDLE_TIME);
    iap_init();
    BSP_UsartInit();
	filter_init(&air_temp, ADC_CHANNEL_1);
	filter_init(&sol_temp, ADC_CHANNEL_2);
	filter_init(&over_load, ADC_CHANNEL_3);
	PID_Init(&air_handle_pid,800,4,20000, MAX_AIR_PWM_OUTPUT);
	PID_Init(&sol_115_handle_pid,30,1,10, MAX_SOL115_PWM_OUTPUT);
	PID_Init(&sol_210_handle_pid,50,1,10, MAX_SOL210_PWM_OUTPUT);
	FWDS201_Init(&sFWDS201_t);
	init_cycle_counter(true);
    EventRecorderInitialize(0, 1);
    /* add user code end 2 */

  while(1)
  {
    /* add user code begin 3 */
        tmt.run();
        /* add user code end 3 */
  }
}

  /* add user code begin 4 */
void iap_task(void)
{
    iap_command_handle();
	
}


void feed_dog_task(void)
{
    static bool first_in = false;

    if (first_in == false)
    {
        first_in = true;
        /* if enabled, please feed the dog through wdt_counter_reload() function */
        wdt_enable();
    }

    wdt_counter_reload();
}


void pc_comm_task(void)
{
    pc_comm_handle();
}

void beep_task(void)
{
	static uint8_t warning_time = 0;
    if (sFWDS201_t.speak_state == SPEAKER_OPEN)
    {
        beep_handle();
    }
    else if (sFWDS201_t.speak_state == SPEAKER_CLOSE)
    {
        sbeep.off();
    }
	
	if(sFWDS201_t.airgun_handle_error_state != HANDLE_OK || sFWDS201_t.sol_handle_error_state != HANDLE_OK)
	{
	     if(warning_time++ == 50)
		 {
		     warning_time = 0;
			 sbeep.cmd = BEEP_LONG;
		 }
	}
}

void ec11_task(void)
{
	ec11_handle();
}

void flash_task(void)
{
	FlashProc();
}

void key_task(void)
{
	key_handle();
}

void led_task(void)
{
#if 0
	led_handle();
	
#endif
	
}

void output_task(void)
{
	output_handle();
	
}


/* add user code end 4 */
