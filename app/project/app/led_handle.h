#ifndef __LED_HANDLE_H
#define __LED_HANDLE_H

#include "at32f415.h"

#define DISP_HANDLE_TIME 1

#define GUN_TAMP_BAI_PORT GPIOF
#define GUN_TAMP_BAI_PIN GPIO_PINS_6

#define GUN_TAMP_SHI_PORT GPIOA
#define GUN_TAMP_SHI_PIN GPIO_PINS_12

#define GUN_TAMP_GE_PORT GPIOA
#define GUN_TAMP_GE_PIN GPIO_PINS_11

#define AIR_BAI_PORT GPIOC
#define AIR_BAI_PIN GPIO_PINS_10

#define AIR_SHI_PORT GPIOA
#define AIR_SHI_PIN GPIO_PINS_15

#define AIR_GE_PORT GPIOF
#define AIR_GE_PIN GPIO_PINS_7

#define SOL_TAMP_BAI_PORT GPIOD
#define SOL_TAMP_BAI_PIN GPIO_PINS_2

#define SOL_TAMP_SHI_PORT GPIOC
#define SOL_TAMP_SHI_PIN GPIO_PINS_12

#define SOL_TAMP_GE_PORT GPIOC
#define SOL_TAMP_GE_PIN GPIO_PINS_11

#define LEDA_PORT GPIOB
#define LEDA_PIN GPIO_PINS_3

#define LEDB_PORT GPIOB
#define LEDB_PIN GPIO_PINS_4

#define LEDC_PORT GPIOB
#define LEDC_PIN GPIO_PINS_5

#define LEDD_PORT GPIOB
#define LEDD_PIN GPIO_PINS_6

#define LEDE_PORT GPIOB
#define LEDE_PIN GPIO_PINS_7

#define LEDF_PORT GPIOB
#define LEDF_PIN GPIO_PINS_8

#define LEDG_PORT GPIOB
#define LEDG_PIN GPIO_PINS_9

#define LEDDP_PORT GPIOB
#define LEDDP_PIN GPIO_PINS_11

extern uint16_t show_direct_set_temp_time;

// 状态机定义
typedef enum
{
    STATE_IDLE,
    STATE_DISPLAY,
    STATE_UPDATE
} StateMachine_TypeDef;

void Gpio_out(uint8_t in_pin);
void Gpio_in(uint8_t in_pin);
void Disp_Power_Init(void);
void Display_Scan1(uint16_t in_dat);
void Display_Scan2(uint16_t in_dat);
void Display_Scan3(uint16_t in_dat);
void Display_Scan4(uint16_t in_dat);
void Display_Scan5(void); // 显示%号
void disp_set_temp_bit(void);
void disp_actual_temp_bit(float actual_data);
void Disp_Error(void);
void Disp_sleep(void);
void Disp_chx(uint8_t in_chx);
void Disp_Clean(void);
void Disp_Power(float in_data);
void Disp_flicker(float in_temp, uint8_t in_cf);
void disp_digit_all(void);
void Dips_Px(uint8_t in_px);
void nine(void);
void eight(void);
void seven(void);
void six(void);
void five(void);
void four(void);
void three(void);
void two(void);
void one(void);
void sleep_G(void);

void Display_Handle(uint8_t in_air_temp_num, uint8_t in_sol_temp_num, uint8_t in_air_num);
void Disp_Set_Handle(uint8_t menu_number);
void led_handle(void);

#endif
