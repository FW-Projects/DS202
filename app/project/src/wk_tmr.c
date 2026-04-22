/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_tmr.c
  * @brief    work bench config program
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
#include "wk_tmr.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init tmr2 function.
  * @param  none
  * @retval none
  */
void wk_tmr2_init(void)
{
  /* add user code begin tmr2_init 0 */

  /* add user code end tmr2_init 0 */

  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_output_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin tmr2_init 1 */

  /* add user code end tmr2_init 1 */

  /* configure the CH3 pin */
  gpio_init_struct.gpio_pins = FAN_PWM_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(FAN_PWM_GPIO_PORT, &gpio_init_struct);

  /* GPIO PIN remap */
  gpio_pin_remap_config(TMR2_GMUX_010, TRUE); 

  /* configure counter settings */
  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR2, TMR_CLOCK_DIV1);
  tmr_period_buffer_enable(TMR2, FALSE);
  tmr_base_init(TMR2, 255, 19);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR2, FALSE);
  tmr_primary_mode_select(TMR2, TMR_PRIMARY_SEL_RESET);

  /* configure channel 3 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_3, &tmr_output_struct);
  tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, 0);
  tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_3, FALSE);

  tmr_output_channel_immediately_set(TMR2, TMR_SELECT_CHANNEL_3, FALSE);


  tmr_counter_enable(TMR2, TRUE);

  /**
   * Users need to configure TMR2 interrupt functions according to the actual application.
   * 1. Call the below function to enable the corresponding TMR2 interrupt.
   *     --tmr_interrupt_enable(...)
   * 2. Add the user's interrupt handler code into the below function in the at32f415_int.c file.
   *     --void TMR2_GLOBAL_IRQHandler(void)
   */

  /* add user code begin tmr2_init 2 */
tmr_interrupt_enable(TMR2, TMR_C1_INT, TRUE);

  /* add user code end tmr2_init 2 */
}

/**
  * @brief  init tmr3 function.
  * @param  none
  * @retval none
  */
void wk_tmr3_init(void)
{
  /* add user code begin tmr3_init 0 */

  /* add user code end tmr3_init 0 */

  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_output_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin tmr3_init 1 */

  /* add user code end tmr3_init 1 */

  /* configure the CH1 pin */
  gpio_init_struct.gpio_pins = HOTAIR_PWM_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(HOTAIR_PWM_GPIO_PORT, &gpio_init_struct);

  /* configure counter settings */
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR3, TMR_CLOCK_DIV4);
  tmr_period_buffer_enable(TMR3, FALSE);
  tmr_base_init(TMR3, 59999, 149);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR3, FALSE);
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_RESET);

  /* configure channel 1 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, 0);
  tmr_output_channel_buffer_enable(TMR3, TMR_SELECT_CHANNEL_1, FALSE);

  tmr_output_channel_immediately_set(TMR3, TMR_SELECT_CHANNEL_1, TRUE);


  tmr_counter_enable(TMR3, TRUE);

  /**
   * Users need to configure TMR3 interrupt functions according to the actual application.
   * 1. Call the below function to enable the corresponding TMR3 interrupt.
   *     --tmr_interrupt_enable(...)
   * 2. Add the user's interrupt handler code into the below function in the at32f415_int.c file.
   *     --void TMR3_GLOBAL_IRQHandler(void)
   */

  /* add user code begin tmr3_init 2 */
tmr_interrupt_enable(TMR3,TMR_OVF_INT,TRUE);
  /* add user code end tmr3_init 2 */
}

/**
  * @brief  init tmr5 function.
  * @param  none
  * @retval none
  */
void wk_tmr5_init(void)
{
  /* add user code begin tmr5_init 0 */

  /* add user code end tmr5_init 0 */

  gpio_init_type gpio_init_struct;
  tmr_output_config_type tmr_output_struct;

  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin tmr5_init 1 */

  /* add user code end tmr5_init 1 */

  /* configure the CH1 pin */
  gpio_init_struct.gpio_pins = HOTSOL_PWM_PIN;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init(HOTSOL_PWM_GPIO_PORT, &gpio_init_struct);

  /* GPIO PIN remap */
  gpio_pin_remap_config(TMR5_GMUX_001, TRUE); 

  /* configure counter settings */
  tmr_cnt_dir_set(TMR5, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR5, TMR_CLOCK_DIV1);
  tmr_period_buffer_enable(TMR5, FALSE);
  tmr_base_init(TMR5, 9999, 287);

  /* configure primary mode settings */
  tmr_sub_sync_mode_set(TMR5, FALSE);
  tmr_primary_mode_select(TMR5, TMR_PRIMARY_SEL_RESET);

  /* configure channel 1 output settings */
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_B;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.oc_idle_state = FALSE;
  tmr_output_struct.occ_idle_state = FALSE;
  tmr_output_channel_config(TMR5, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR5, TMR_SELECT_CHANNEL_1, 0);
  tmr_output_channel_buffer_enable(TMR5, TMR_SELECT_CHANNEL_1, FALSE);

  tmr_output_channel_immediately_set(TMR5, TMR_SELECT_CHANNEL_1, FALSE);

  tmr_counter_enable(TMR5, TRUE);

  /* add user code begin tmr5_init 2 */
	tmr_interrupt_enable(TMR5,TMR_OVF_INT,TRUE);
  /* add user code end tmr5_init 2 */
}

/* add user code begin 1 */

/* add user code end 1 */
