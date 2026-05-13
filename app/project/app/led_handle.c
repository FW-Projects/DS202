#include "led_handle.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "wk_system.h"
#include "at32f415.h"
#include "DS201_Handle.h"

// 引脚定义  功率数码管
#define PIN1 0
#define PIN2 1
#define PIN3 2
#define PIN4 3
#define PIN5 4

#define ON 1
#define OFF 0

static uint8_t buffer_disp_set[3] = {0};
static uint8_t buffer_disp_air_temp[3] = {0};
static uint8_t buffer_disp_air[3] = {0};
static uint8_t buffer_disp_sol_temp[3] = {0};

void nine(void);
void eight(void);
void seven(void);
void six(void);
void five(void);
void four(void);
void three(void);
void two(void);
void one(void);
void zero_0(void);
void sleep_G(void);
void C(void);
void E(void);
void F(void);
void H(void);
void n(void);
void P(void);
void celsius(void);
void fahrenheit(void);
void nothing(void);
void r(void);
void Clear(void);

static void (*TAST_BUFF[])() = {zero_0, one, two, three, four, five, six, seven, eight, nine,
								 //   0     1    2    3      4     5        6    7        8           9
								 C, E, F, H, n, sleep_G, P, celsius, fahrenheit, nothing, r};
	//   10    11   12   13     14    15       16   17       18          19        20

								 
static void (*TAST[])() = {zero_0, one, two, three, four, five, six, seven, eight, nine,
//   0     1    2    3      4     5        6    7        8           9
C, E, F, H, n, sleep_G, P, celsius, fahrenheit, nothing, r};
//   10    11   12   13     14    15       16   17       18          19        20
								 
static volatile bool buffer_update_flag = false; // 缓冲区更新标志


void zero_0(void)
{
	gpio_bits_set(GPIOB, LEDG_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN);
}

void one(void)
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN);
}

void two(void)
{
	gpio_bits_set(GPIOB, LEDC_PIN | LEDF_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDD_PIN | LEDE_PIN | LEDG_PIN);
}

void three(void)
{
	gpio_bits_set(GPIOB, LEDF_PIN | LEDDP_PIN | LEDE_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDG_PIN);
}
void four(void)
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN | LEDF_PIN | LEDG_PIN);
}

void five(void)
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDE_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDG_PIN);
}

void six(void)
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void seven(void)
{
	gpio_bits_set(GPIOB, LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN);
}

void eight(void)
{
	gpio_bits_set(GPIOB, LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void nine(void)
{
	gpio_bits_set(GPIOB, LEDE_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDG_PIN);
}

void r(void) // 10
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDF_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDE_PIN | LEDG_PIN);
}

void C(void) // 12
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDG_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN);
}

void E(void) // 14
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void F(void) // 15
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void H(void) // 16
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDD_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDB_PIN | LEDC_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void n(void) // 17
{
	gpio_bits_set(GPIOB, LEDD_PIN | LEDG_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDE_PIN | LEDF_PIN);
}

void sleep_G(void) // 19
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDG_PIN);
}
void Clear(void)
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}
void P(void) // 20
{
	gpio_bits_set(GPIOB, LEDC_PIN | LEDD_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDB_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void celsius(void) // ℃
{
	gpio_bits_set(GPIOB, LEDB_PIN | LEDC_PIN | LEDDP_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN);
}

void fahrenheit(void) // ℉
{
	gpio_bits_set(GPIOB, LEDD_PIN | LEDB_PIN | LEDC_PIN);
	gpio_bits_reset(GPIOB, LEDA_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}

void nothing(void) // 全灭
{
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);
}

// 统一扫描：温度3位 + 风量3位
void disp_digit_all(void)
{
	static uint8_t step_n = 0; // 全局只有一个步数
	

	// ===================== 统一消隐（必须）=====================
	gpio_bits_set(SOL_TAMP_BAI_PORT, SOL_TAMP_BAI_PIN);
	gpio_bits_set(SOL_TAMP_SHI_PORT, SOL_TAMP_SHI_PIN);
	gpio_bits_set(SOL_TAMP_GE_PORT, SOL_TAMP_GE_PIN);	
								 
	gpio_bits_set(AIR_BAI_PORT, AIR_BAI_PIN);
	gpio_bits_set(AIR_SHI_PORT, AIR_SHI_PIN);
	gpio_bits_set(AIR_GE_PORT, AIR_GE_PIN);
								 
	gpio_bits_set(GUN_TAMP_BAI_PORT, GUN_TAMP_BAI_PIN);
	gpio_bits_set(GUN_TAMP_SHI_PORT, GUN_TAMP_SHI_PIN);
	gpio_bits_set(GUN_TAMP_GE_PORT, GUN_TAMP_GE_PIN);
							 
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);

	// ===================== 6位轮流显示 =====================
	switch (step_n)
	{
		// 烙铁温度 百位
		case 0:
		{
			TAST_BUFF[buffer_disp_sol_temp[0]]();
			gpio_bits_reset(SOL_TAMP_BAI_PORT, SOL_TAMP_BAI_PIN);
			break;
		}
		// 烙铁温度 十位
		case 1:
		{
			TAST_BUFF[buffer_disp_sol_temp[1]]();
			gpio_bits_reset(SOL_TAMP_SHI_PORT, SOL_TAMP_SHI_PIN);
			break;
		}
		// 烙铁温度 个位
		case 2:
		{
			TAST_BUFF[buffer_disp_sol_temp[2]]();
			gpio_bits_reset(SOL_TAMP_GE_PORT, SOL_TAMP_GE_PIN);
			break;
		}
		
		// 风量 百位
		case 3:
		{
			TAST_BUFF[buffer_disp_air[0]]();
			gpio_bits_reset(AIR_BAI_PORT, AIR_BAI_PIN);
			break;
		}
		// 风量 十位
		case 4:
		{
			TAST_BUFF[buffer_disp_air[1]]();
			gpio_bits_reset(AIR_SHI_PORT, AIR_SHI_PIN);
			break;
		}
		// 风量 个位
		case 5:
		{
			TAST_BUFF[buffer_disp_air[2]]();
			gpio_bits_reset(AIR_GE_PORT, AIR_GE_PIN);
			break;
		}
		
		// 风枪温度 百位
		case 6:
		{
			TAST_BUFF[buffer_disp_air_temp[0]]();
			gpio_bits_reset(GUN_TAMP_BAI_PORT, GUN_TAMP_BAI_PIN);
			break;
		}
		// 风枪温度 十位
		case 7:
		{
			TAST_BUFF[buffer_disp_air_temp[1]]();
			gpio_bits_reset(GUN_TAMP_SHI_PORT, GUN_TAMP_SHI_PIN);
			break;
		}
		// 风枪温度 个位
		case 8:
		{
			TAST_BUFF[buffer_disp_air_temp[2]]();
			if (sFWDS201_t.air_handle_position == NOT_IN_POSSITION &&
				sFWDS201_t.air_work_handle_state == HANDLE_WORKING &&
				sFWDS201_t.airgun_handle_error_state == HANDLE_OK)
			{
				if (sFWDS201_t.hot_state == FULL_POWER)
				{
					gpio_bits_reset(GPIOB, LEDDP_PIN);
				}
				else if (sFWDS201_t.hot_state == ZERO_POWER)
				{
					gpio_bits_set(GPIOB, LEDDP_PIN);
				}
				else if (sFWDS201_t.hot_state == PID_POWER)
				{
					if (sFWDS201_t.general_parameter.dp_flash_flag)
						gpio_bits_set(GPIOB, LEDDP_PIN);
					else
						gpio_bits_reset(GPIOB, LEDDP_PIN);
				}
			}
			else
				gpio_bits_set(GPIOB, LEDDP_PIN);
			gpio_bits_reset(GUN_TAMP_GE_PORT, GUN_TAMP_GE_PIN);
			break;
		}	
	}
	step_n = (step_n + 1) % 9;
}


void Display_Handle(uint8_t in_air_temp_num, uint8_t in_sol_temp_num, uint8_t in_air_num)
{
	static char disp_times = 100;
	static char disp_times2 = 100;
	static float air_last_set_temp = 0, air_last_actual_temp = 0;
	static uint16_t temp_val = 0;
	static uint16_t show_air_temp = 0, show_air = 0, show_sol_temp = 0;
								 
	switch (in_sol_temp_num)
	{
		case DISPLAY_SET:
		{ // 显示设置温度
			if (sFWDS201_t.system_parameter.sol_last_set_temp != sFWDS201_t.system_parameter.sol_set_temp ||
				sFWDS201_t.system_parameter.sol_last_set_temp_f_display != sFWDS201_t.system_parameter.sol_set_temp_f_display)
			{
				if (sFWDS201_t.temp_unit == CELSIUS)
					show_sol_temp = (uint16_t)sFWDS201_t.system_parameter.sol_set_temp;
				else if (sFWDS201_t.temp_unit == FAHRENHEIT) // 华氏度
					show_sol_temp = (uint16_t)sFWDS201_t.system_parameter.sol_set_temp_f_display;

				sFWDS201_t.system_parameter.sol_last_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
				sFWDS201_t.system_parameter.sol_last_set_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display;
			}
			buffer_disp_sol_temp[0] = show_sol_temp / 100;
			buffer_disp_sol_temp[1] = (show_sol_temp / 10) % 10;
			buffer_disp_sol_temp[2] = show_sol_temp % 10;
		}
		break;
		
		case DISPLAY_REAL:
		{
			disp_times2--;
			if (disp_times2 <= 0)
			{
				if (sFWDS201_t.temp_unit == CELSIUS)
					show_sol_temp = sFWDS201_t.system_parameter.sol_actual_temp;
				else if (sFWDS201_t.temp_unit == FAHRENHEIT)
					show_sol_temp = (sFWDS201_t.system_parameter.sol_actual_temp * 9 / 5) + 32;
			}
			buffer_disp_sol_temp[0] = show_sol_temp / 100;
			buffer_disp_sol_temp[1] = (show_sol_temp / 10) % 10;
			buffer_disp_sol_temp[2] = show_sol_temp % 10;
		}
		break;
		case DISPLAY_ERR:
		{
			buffer_disp_sol_temp[0] = 11;
			buffer_disp_sol_temp[1] = 20;
			buffer_disp_sol_temp[2] = sFWDS201_t.sol_handle_error_state;
		}
		break;
		case DISPLAY_SLP:
		{
			buffer_disp_sol_temp[0] = 15;
			buffer_disp_sol_temp[1] = 15;
			buffer_disp_sol_temp[2] = 15;
		}
		break;
		case DISPLAY_SAVE_CH:
		{
			buffer_disp_sol_temp[0] = 10;
			buffer_disp_sol_temp[1] = 13;
			buffer_disp_sol_temp[2] = sFWDS201_t.general_parameter.sol_ch;
		}
		break;
		default:
			break;
	}
								 
	switch (in_air_temp_num)
	{
		case DISPLAY_SET:
		{
			if (sFWDS201_t.system_parameter.air_last_set_temp != sFWDS201_t.system_parameter.air_set_temp ||
				sFWDS201_t.system_parameter.air_last_set_temp_f_display != sFWDS201_t.system_parameter.air_set_temp_f_display)
			{
				if (sFWDS201_t.temp_unit == CELSIUS)
					show_air_temp = (uint16_t)sFWDS201_t.system_parameter.air_set_temp;
				else if (sFWDS201_t.temp_unit == FAHRENHEIT) // 华氏度
					show_air_temp = (uint16_t)sFWDS201_t.system_parameter.air_set_temp_f_display;

				sFWDS201_t.system_parameter.air_last_set_temp = sFWDS201_t.system_parameter.air_set_temp;
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display;
			}
			buffer_disp_air_temp[0] = show_air_temp / 100;
			buffer_disp_air_temp[1] = (show_air_temp / 10) % 10;
			buffer_disp_air_temp[2] = show_air_temp % 10;
			
		}break;
		case DISPLAY_REAL:
		{
			disp_times--;
			if (disp_times <= 0)
			{
			if (sFWDS201_t.temp_unit == CELSIUS)
				show_air_temp = sFWDS201_t.system_parameter.air_actual_temp;
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
				show_air_temp = (sFWDS201_t.system_parameter.air_actual_temp * 9 / 5) + 32;
			}
			buffer_disp_air_temp[0] = show_air_temp / 100;
			buffer_disp_air_temp[1] = (show_air_temp / 10) % 10;
			buffer_disp_air_temp[2] = show_air_temp % 10;
		}
		break;
		case DISPLAY_ERR:
		{
		buffer_disp_air_temp[0] = 11;
		buffer_disp_air_temp[1] = 20;
		buffer_disp_air_temp[2] = sFWDS201_t.airgun_handle_error_state;
		}
		break;
		case DISPLAY_SLP:
		{
		buffer_disp_air_temp[0] = 15;
		buffer_disp_air_temp[1] = 15;
		buffer_disp_air_temp[2] = 15;
		}
		break;
		case DISPLAY_SAVE_CH:
		{
		buffer_disp_air_temp[0] = 10;
		buffer_disp_air_temp[1] = 13;
		buffer_disp_air_temp[2] = sFWDS201_t.general_parameter.air_ch;
		}
		break;
		default:
		break;
		}

	switch (in_air_num)
	{
	case DISPLAY_SET:
		if (sFWDS201_t.system_parameter.last_air_data != sFWDS201_t.system_parameter.air_data)
		{
			sFWDS201_t.system_parameter.last_air_data = sFWDS201_t.system_parameter.air_data;
			show_air = sFWDS201_t.system_parameter.air_data;
			buffer_disp_air[0] = show_air / 100;
			buffer_disp_air[1] = (show_air / 10) % 10;
			buffer_disp_air[2] = show_air % 10;
		}
		break;
	case DISPLAY_REAL:
		if (sFWDS201_t.system_parameter.last_sleep_air_data != sFWDS201_t.system_parameter.sleep_air_data)
		{
			sFWDS201_t.system_parameter.last_sleep_air_data = sFWDS201_t.system_parameter.sleep_air_data;
			show_air = sFWDS201_t.system_parameter.sleep_air_data;
			buffer_disp_air[0] = show_air / 100;
			buffer_disp_air[1] = (show_air / 10) % 10;
			buffer_disp_air[2] = show_air % 10;
		}
		break;
	case DISPLAY_ERR:
		break;
	case DISPLAY_SLP:
		buffer_disp_air[0] = 15;
		buffer_disp_air[1] = 15;
		buffer_disp_air[2] = 15;
		break;
	case DISPLAY_SAVE_CH:
		buffer_disp_air[0] = 10;
		buffer_disp_air[1] = 13;
		buffer_disp_air[2] = sFWDS201_t.general_parameter.air_ch;
		break;
	default:
		break;
	}

	disp_digit_all();

	if (sFWDS201_t.general_parameter.set_air_temp_time != 0x00)
	{
		sFWDS201_t.general_parameter.set_air_temp_time--;
		if (sFWDS201_t.general_parameter.save_air_ch_flag == false)
			sFWDS201_t.display_air_temp_number = DISPLAY_SET;
		if (sFWDS201_t.general_parameter.set_air_temp_time <= 0x00)
		{
			sFWDS201_t.general_parameter.set_air_temp_time = 0x00;
			sFWDS201_t.display_air_temp_number = DISPLAY_REAL;
			if (sFWDS201_t.general_parameter.save_air_ch_flag == true)
				sFWDS201_t.general_parameter.save_air_ch_flag = false;
			
		}
	}

	if (sFWDS201_t.general_parameter.set_sol_temp_time != 0x00)
	{
		sFWDS201_t.general_parameter.set_sol_temp_time--;
		if (sFWDS201_t.general_parameter.save_sol_ch_flag == false)
			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
		if (sFWDS201_t.general_parameter.set_sol_temp_time <= 0x00)
		{
			sFWDS201_t.general_parameter.set_sol_temp_time = 0x00;
			sFWDS201_t.display_sol_temp_number = DISPLAY_REAL;
			if (sFWDS201_t.general_parameter.save_sol_ch_flag == true)
				sFWDS201_t.general_parameter.save_sol_ch_flag = false;
		}
	}
	
	if (sFWDS201_t.general_parameter.set_air_time != 0x00)
	{
		sFWDS201_t.general_parameter.set_air_time--;
		sFWDS201_t.display_air_number = DISPLAY_SET;
		if (sFWDS201_t.general_parameter.set_air_time <= 0x00)
		{
			sFWDS201_t.general_parameter.set_air_time = 0x00;
			sFWDS201_t.display_air_number = DISPLAY_SET;
		}
	}
}

/* 设置界面显示 */
void dispmenu_pageoption(uint8_t in_number)
{
	static uint8_t step_n = 0; // 全局只有一个步数

	// ===================== 统一消隐（必须）=====================
	gpio_bits_set(SOL_TAMP_BAI_PORT, SOL_TAMP_BAI_PIN);
	gpio_bits_set(SOL_TAMP_SHI_PORT, SOL_TAMP_SHI_PIN);
	gpio_bits_set(SOL_TAMP_GE_PORT, SOL_TAMP_GE_PIN);	
								 
	gpio_bits_set(AIR_BAI_PORT, AIR_BAI_PIN);
	gpio_bits_set(AIR_SHI_PORT, AIR_SHI_PIN);
	gpio_bits_set(AIR_GE_PORT, AIR_GE_PIN);
								 
	gpio_bits_set(GUN_TAMP_BAI_PORT, GUN_TAMP_BAI_PIN);
	gpio_bits_set(GUN_TAMP_SHI_PORT, GUN_TAMP_SHI_PIN);
	gpio_bits_set(GUN_TAMP_GE_PORT, GUN_TAMP_GE_PIN);
								 
	gpio_bits_set(GPIOB, LEDA_PIN | LEDB_PIN | LEDC_PIN | LEDD_PIN | LEDE_PIN | LEDF_PIN | LEDG_PIN | LEDDP_PIN);

	// ===================== 6位轮流显示 =====================
	switch (step_n)
	{
		/* 烙铁温度数码管 */
		case 0:
		{
			TAST[buffer_disp_set[0]]();
			if (sFWDS201_t.set_interface_number == SOFTWARE_VERSION || sFWDS201_t.set_interface_number == HARDWARE_VERSION)
			{
				gpio_bits_reset(GPIOB, LEDDP_PIN);
			}
			gpio_bits_reset(SOL_TAMP_BAI_PORT, SOL_TAMP_BAI_PIN);
			break;
		}
		case 1:
		{
			TAST[buffer_disp_set[1]]();
			if (sFWDS201_t.set_interface_number == SOFTWARE_VERSION || sFWDS201_t.set_interface_number == HARDWARE_VERSION)
			{
				gpio_bits_reset(GPIOB, LEDDP_PIN);
			}
			gpio_bits_reset(SOL_TAMP_SHI_PORT, SOL_TAMP_SHI_PIN);
			break;
		}
		case 2:
		{
			TAST[buffer_disp_set[2]]();
			gpio_bits_reset(SOL_TAMP_GE_PORT, SOL_TAMP_GE_PIN);
			break;
		}
		
		/* 风量数码管 */
		case 3:
		{
			TAST[15]();
			gpio_bits_reset(AIR_BAI_PORT, AIR_BAI_PIN);
			break;
		}
		case 4:
		{
			TAST[15]();
			gpio_bits_reset(AIR_SHI_PORT, AIR_SHI_PIN);
			break;
		}
		case 5:
		{
			TAST[15]();
			gpio_bits_reset(AIR_GE_PORT, AIR_GE_PIN);
			break;
		}
		
		/* 风枪温度数码管 */
		case 6:
		{
			TAST[16]();
			gpio_bits_reset(GUN_TAMP_BAI_PORT, GUN_TAMP_BAI_PIN);
			break;
		}
		case 7:
		{
			if(in_number == 0x0c)
				TAST[1]();
			else
				TAST[0]();
			gpio_bits_reset(GUN_TAMP_SHI_PORT, GUN_TAMP_SHI_PIN);
			break;
		}
		case 8:
		{
			if(in_number == 0x0c)
				TAST[0]();
			else
				TAST[in_number]();
			gpio_bits_reset(GUN_TAMP_GE_PORT, GUN_TAMP_GE_PIN);
			break;
		}
		
		
	}

	step_n = (step_n + 1) % 9;
}

void Disp_Set_Handle(uint8_t menu_number)
{
	static float last_air_cal_data = 0;
	static uint16_t temp_val = 0,sleep_time = 0;
	switch (menu_number)
	{
		case EXIT: // 退出设置
			buffer_disp_set[0] = 15;
			buffer_disp_set[1] = 15;
			buffer_disp_set[2] = 15;
			break;
		case AIR_TEMP_CAL: // 温度校准
			if (sFWDS201_t.temp_unit == CELSIUS)
				temp_val = (uint16_t)sFWDS201_t.system_parameter.air_cal_temp_c_display;
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
				temp_val = (uint16_t)sFWDS201_t.system_parameter.air_cal_temp_f_display;
			buffer_disp_set[0] = temp_val / 100;	   // 百位
			buffer_disp_set[1] = (temp_val / 10) % 10; // 十位
			buffer_disp_set[2] = temp_val % 10;		   // 个位
			break;
		case SOL_TEMP_CAL: // 温度校准
		if (sFWDS201_t.temp_unit == CELSIUS)
			temp_val = (uint16_t)sFWDS201_t.system_parameter.sol_cal_temp_c_display;
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			temp_val = (uint16_t)sFWDS201_t.system_parameter.sol_cal_temp_f_display;
		buffer_disp_set[0] = temp_val / 100;	   // 百位
		buffer_disp_set[1] = (temp_val / 10) % 10; // 十位
		buffer_disp_set[2] = temp_val % 10;		   // 个位
	
		break;
		case TEMP_UNIT: // 华氏度摄氏度
			if (sFWDS201_t.temp_unit == CELSIUS)
				buffer_disp_set[1] = 10; // C
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
				buffer_disp_set[1] = 12; // F

			buffer_disp_set[0] = 15; //-
			buffer_disp_set[2] = 15; //-
			break;
		case SPEAK_STATE: // 显示锁定
			if (sFWDS201_t.speak_state == SPEAKER_OPEN)
			{
				buffer_disp_set[0] = 19;
				buffer_disp_set[1] = 0;	 // O
				buffer_disp_set[2] = 14; // N
			}
			else if (sFWDS201_t.speak_state == SPEAKER_CLOSE)
			{
				buffer_disp_set[0] = 0;	 // O
				buffer_disp_set[1] = 12; // F
				buffer_disp_set[2] = 12; // F
			}
			break;
		case DISPLAY_LOCK: // 声音开关
			if (sFWDS201_t.display_lock_state == LOCK)
			{
				buffer_disp_set[0] = 19;
				buffer_disp_set[1] = 0;	 // O
				buffer_disp_set[2] = 14; // N
			}
			else if (sFWDS201_t.display_lock_state == UNLOCK)
			{
				buffer_disp_set[0] = 0;	 // O
				buffer_disp_set[1] = 12; // F
				buffer_disp_set[2] = 12; // F
			}
			break;
		case SLEEP_STATE:
			if (sFWDS201_t.sleep_state == SLEEP_OPEN)
			{
				buffer_disp_set[0] = 19;
				buffer_disp_set[1] = 0;	 // O
				buffer_disp_set[2] = 14; // N
			}
			else if (sFWDS201_t.sleep_state == SLEEP_CLOSE)
			{
				buffer_disp_set[0] = 0;	 // O
				buffer_disp_set[1] = 12; // F
				buffer_disp_set[2] = 12; // F
			}
			break;
		case SET_SLEEP_TIME:
			sleep_time = sFWDS201_t.system_parameter.set_sleep_time;
			buffer_disp_set[0] = sleep_time / 100;	 
			buffer_disp_set[1] = (sleep_time / 10) % 10; 
			buffer_disp_set[2] = sleep_time % 10; 
			break;
		case SOFTWARE_VERSION:
			buffer_disp_set[0] = SOFTWARE_VERSION_BAI;
			buffer_disp_set[1] = SOFTWARE_VERSION_SHI;
			buffer_disp_set[2] = SOFTWARE_VERSION_GE;
			break;
		case HARDWARE_VERSION:
			buffer_disp_set[0] = HARDWARE_VERSION_BAI;
			buffer_disp_set[1] = HARDWARE_VERSION_SHI;
			buffer_disp_set[2] = HARDWARE_VERSION_GE;
			break;
		case RESET_RUN:
			buffer_disp_set[0] = 15;
			buffer_disp_set[1] = 20;
			buffer_disp_set[2] = 15;
			break;
		default:
			break;
	}
	dispmenu_pageoption(sFWDS201_t.set_interface_number);
}


void led_handle(void)
{
	if(sFWDS201_t.set_flag == false)
		Display_Handle(sFWDS201_t.display_air_temp_number,sFWDS201_t.display_sol_temp_number,sFWDS201_t.display_air_number);
	else
		Disp_Set_Handle(sFWDS201_t.set_interface_number);
	if(sFWDS201_t.temp_unit == CELSIUS)
	{
		gpio_bits_set(GPIOC,GPIO_PINS_0);
	}
	else if(sFWDS201_t.temp_unit == FAHRENHEIT)
	{
		gpio_bits_reset(GPIOC,GPIO_PINS_0);
	}
	
}