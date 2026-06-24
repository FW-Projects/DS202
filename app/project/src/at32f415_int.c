/* add user code begin Header */
/**
  **************************************************************************
  * @file     at32f415_int.c
  * @brief    main interrupt service routines.
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

/* includes ------------------------------------------------------------------*/
#include "at32f415_int.h"
#include "wk_system.h"

/* private includes ----------------------------------------------------------*/
/* add user code begin private includes */
#include <math.h>
#include "tmt.h"

#include "at32_Usart.h"
#include "EventRecorder.h"
#include "DS201_handle.h"
#include "output_handle.h"
#include "adc_filter.h"
#include "led_handle.h"
#include "perf_counter.h"

/* add user code end private includes */

/* private typedef -----------------------------------------------------------*/
/* add user code begin private typedef */

/* add user code end private typedef */

/* private define ------------------------------------------------------------*/
/* add user code begin private define */
#define    TIME_1S        1000
#define    TIME_500MS     500
#define    TIME_1MS       1

#define ADC_VREF (3.29)
#define ADC_TEMP_BASE (1.26)
#define ADC_TEMP_SLOPE (-0.00423)

float direct_handle_pid_out;

/* add user code end private define */

/* private macro -------------------------------------------------------------*/
/* add user code begin private macro */

/* add user code end private macro */

/* private variables ---------------------------------------------------------*/
/* add user code begin private variables */

__IO float tmr2_ch1_readvalue = 0;
__IO uint16_t adc1_ordinary_value = 0;

/* add user code end private variables */

/* private function prototypes --------------------------------------------*/
/* add user code begin function prototypes */

/* add user code end function prototypes */

/* private user code ---------------------------------------------------------*/
/* add user code begin 0 */

/* add user code end 0 */

/* external variables ---------------------------------------------------------*/
/* add user code begin external variables */

/* add user code end external variables */

/**
  * @brief  this function handles nmi exception.
  * @param  none
  * @retval none
  */
void NMI_Handler(void)
{
  /* add user code begin NonMaskableInt_IRQ 0 */
    /* add user code end NonMaskableInt_IRQ 0 */

  /* add user code begin NonMaskableInt_IRQ 1 */
    /* add user code end NonMaskableInt_IRQ 1 */
}

/**
  * @brief  this function handles hard fault exception.
  * @param  none
  * @retval none
  */
void HardFault_Handler(void)
{
  /* add user code begin HardFault_IRQ 0 */

    /* add user code end HardFault_IRQ 0 */
  /* go to infinite loop when hard fault exception occurs */
  while (1)
  {
    /* add user code begin W1_HardFault_IRQ 0 */
        /* add user code end W1_HardFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles memory manage exception.
  * @param  none
  * @retval none
  */
void MemManage_Handler(void)
{
  /* add user code begin MemoryManagement_IRQ 0 */

    /* add user code end MemoryManagement_IRQ 0 */
  /* go to infinite loop when memory manage exception occurs */
  while (1)
  {
    /* add user code begin W1_MemoryManagement_IRQ 0 */
        /* add user code end W1_MemoryManagement_IRQ 0 */
  }
}

/**
  * @brief  this function handles bus fault exception.
  * @param  none
  * @retval none
  */
void BusFault_Handler(void)
{
  /* add user code begin BusFault_IRQ 0 */

    /* add user code end BusFault_IRQ 0 */
  /* go to infinite loop when bus fault exception occurs */
  while (1)
  {
    /* add user code begin W1_BusFault_IRQ 0 */
        /* add user code end W1_BusFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles usage fault exception.
  * @param  none
  * @retval none
  */
void UsageFault_Handler(void)
{
  /* add user code begin UsageFault_IRQ 0 */

    /* add user code end UsageFault_IRQ 0 */
  /* go to infinite loop when usage fault exception occurs */
  while (1)
  {
    /* add user code begin W1_UsageFault_IRQ 0 */
        /* add user code end W1_UsageFault_IRQ 0 */
  }
}

/**
  * @brief  this function handles svcall exception.
  * @param  none
  * @retval none
  */
void SVC_Handler(void)
{
  /* add user code begin SVCall_IRQ 0 */
    /* add user code end SVCall_IRQ 0 */
  /* add user code begin SVCall_IRQ 1 */
    /* add user code end SVCall_IRQ 1 */
}

/**
  * @brief  this function handles debug monitor exception.
  * @param  none
  * @retval none
  */
void DebugMon_Handler(void)
{
  /* add user code begin DebugMonitor_IRQ 0 */
    /* add user code end DebugMonitor_IRQ 0 */
  /* add user code begin DebugMonitor_IRQ 1 */
    /* add user code end DebugMonitor_IRQ 1 */
}

/**
  * @brief  this function handles pendsv_handler exception.
  * @param  none
  * @retval none
  */
void PendSV_Handler(void)
{
  /* add user code begin PendSV_IRQ 0 */
    /* add user code end PendSV_IRQ 0 */
  /* add user code begin PendSV_IRQ 1 */
    /* add user code end PendSV_IRQ 1 */
}

/**
  * @brief  this function handles systick handler.
  * @param  none
  * @retval none
  */
void SysTick_Handler(void)
{
  /* add user code begin SysTick_IRQ 0 */
	
	static uint16_t get_fan_speed_time  = TIME_1S;
	static bool first_in = false;
	static uint8_t sol_pid_run_time = 10;
	static uint8_t fan_run_time = 1;
	static int dp_time = TIME_500MS;
	
	dp_time--;
	if(dp_time < 0)
	{
		dp_time  = TIME_500MS;
		sFWDS201_t.general_parameter.air_dp_flash_flag = !sFWDS201_t.general_parameter.air_dp_flash_flag;
	    sFWDS201_t.general_parameter.sol_dp_flash_flag = !sFWDS201_t.general_parameter.sol_dp_flash_flag;
	}
    tmt.tick();
    USART1_TimeOutCounter();
    /* add user code end SysTick_IRQ 0 */

  wk_timebase_handler();

  /* add user code begin SysTick_IRQ 1 */
	
//	if(sFWDS201_t.air_work_handle_state == HANDLE_SLEEP)
//	{
//		fan_control(&sFWDS201_t);
//		sFWDS201_t.fan_run_flag = true;
//	}
//	else
//	{
//		fan_run_time--;
//		if(fan_run_time == 0)
//		{
//			fan_run_time = 5;
//			fan_control(&sFWDS201_t);
////			sFWHA01_t.fan_run_flag = false;
//		}
//	}
	
	
	led_handle();
	
	sol_rpc_control();

	sol_sleep_control();
	
	sol_pid_run_time--;
	if (!sol_pid_run_time)
	{
		tmr_channel_value_set(TMR5, TMR_SELECT_CHANNEL_1, 0);
		delay_us(100);
		check_sol_over_current();
		sol_pwm_control();
		tmr_channel_value_set(TMR5, TMR_SELECT_CHANNEL_1, sFWDS201_t.system_parameter.sol_pwm_out);
		tmr_counter_enable(TMR5, TRUE);
		sol_pid_run_time = 10;
	}
	    /* get fan speed */
    get_fan_speed_time--;

    if (0 == get_fan_speed_time)
    {
        sFWDS201_t.system_parameter.actual_air = tmr2_ch1_readvalue;
        tmr2_ch1_readvalue = 0;
        get_fan_speed_time = TIME_1S;
    }
	
	/* get mcu temp */
    if (first_in == false)
    {
        adc1_ordinary_value = get_adcval(ADC_CHANNEL_16);
        sFWDS201_t.system_parameter.cpu_temp = (ADC_TEMP_BASE - (double)adc1_ordinary_value * ADC_VREF / 4096) / ADC_TEMP_SLOPE + 25;
      
        if (sFWDS201_t.system_parameter.cpu_temp <= 50 && sFWDS201_t.system_parameter.cpu_temp > 0)
        {
			sFWDS201_t.system_parameter.air_actual_temp = sFWDS201_t.system_parameter.cpu_temp;
            first_in = true;
        }
    }
    /* add user code end SysTick_IRQ 1 */
}

/**
  * @brief  this function handles TMR2 handler.
  * @param  none
  * @retval none
  */
void TMR2_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR2_GLOBAL_IRQ 0 */
	//air_pwm
	if (tmr_interrupt_flag_get(TMR2, TMR_C1_FLAG) != RESET)
    {
        tmr2_ch1_readvalue++;
        tmr_flag_clear(TMR2, TMR_C1_FLAG);
    }
  /* add user code end TMR2_GLOBAL_IRQ 0 */


  /* add user code begin TMR2_GLOBAL_IRQ 1 */

  /* add user code end TMR2_GLOBAL_IRQ 1 */
}

/**
  * @brief  this function handles TMR3 handler.
  * @param  none
  * @retval none
  */
void TMR3_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR3_GLOBAL_IRQ 0 */

	//airhot_pwm
	
	static char pid_run_time = 2; 
	if(tmr_flag_get(TMR3,TMR_OVF_FLAG) != RESET)
	{ 
		
		if(sFWDS201_t.init_flag)
		{
			air_pwm_control(&sFWDS201_t);
		} 
		else
			sFWDS201_t.system_parameter.air_pwm_out = 0;

		
		if(sFWDS201_t.system_parameter.air_pwm_out <= 0)
		{
			sFWDS201_t.system_parameter.air_pwm_out = 0;
		}
		
		
		/* PWM output */
		tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, sFWDS201_t.system_parameter.air_pwm_out);
			
		tmr_counter_enable(TMR3, TRUE);
        tmr_flag_clear(TMR3, TMR_OVF_FLAG);
	}
  /* add user code end TMR3_GLOBAL_IRQ 0 */


  /* add user code begin TMR3_GLOBAL_IRQ 1 */
	
	
  /* add user code end TMR3_GLOBAL_IRQ 1 */
}

/* add user code begin 1 */



/* add user code end 1 */
