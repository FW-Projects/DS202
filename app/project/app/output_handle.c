#include "output_handle.h"
#include "beep_handle.h"
#include "PID_handle.h"
#include "adc_filter.h"

// #include "perf_counter.h"
static void get_air_handle_position(DS201_Handle *this);
static void get_air_handle_work_state(DS201_Handle *this);
static void get_air_handle_error_state(DS201_Handle *this);

static void get_sol_handle_state(void);

static void get_sol_handle_error_state(void);

// static voidtrol(DS201_Handle *this);
static void relay_control(DS201_Handle *this);
static void relay_control(DS201_Handle *this);

int8_t off_set_buff[41] = {22, 21, 21, 21, 21, 21, 21, 21, 20, 20,
						   21, 21, 21, 20, 19, 19, 19, 19, 18, 17,
						   17, 16, 16, 15, 14, 13, 12, 11, (11), (10 - 2),
						   (9), (9), (8), 12, 15, 17, 21, 24, 26, 28, 33};

int8_t wind_set_20_buff[41] = {22 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 21 - 10, 20 - 10, 20 - 10,
							   21 - 10, 21 - 10, 21 - 10, 20 - 10, 19 - 10, 19 - 10, 19 - 10, 19 - 10, 18 - 10, 17 - 10,
							   17 - 10, 16 - 10, 16 - 10, 15 - 10, 14 - 10, 13 - 10, 12 - 10, 11 - 10, (11 - 30), (10 - 2 - 10),
							   (9 - 10), (9 - 10), (8 - 10), 12 - 10, 15 - 10, 17 - 10, 21 - 10, 24 - 10, 26 - 10, 28 - 10, 33 - 10};

void output_handle(void)
{
	static uint8_t in_first = false;

	/* air gun handle */
	get_air_handle_position(&sFWDS201_t);
	get_air_handle_work_state(&sFWDS201_t);
	//    get_air_handle_error_state(&sFWDS201_t);
	relay_control(&sFWDS201_t);
	
	
	/* sol iron handle */
	get_sol_handle_state();
//	get_sol_handle_error_state();
	
	
}




static void get_air_handle_position(DS201_Handle *this)
{
	if (gpio_input_data_bit_read(GPIOC, GPIO_PINS_2))
	{
		this->air_handle_position = NOT_IN_POSSITION;
	}
	else
	{
		this->air_handle_position = IN_POSSITION;
	}
}

static void get_air_handle_work_state(DS201_Handle *this)
{
	static uint8_t first_state = false;
	switch (this->air_work_handle_state)
	{
	case HANDLE_SLEEP:
		if (this->sleep_state == SLEEP_OPEN)
		{
			if (this->air_handle_position == NOT_IN_POSSITION)
			{
				this->air_work_handle_state = HANDLE_WORKING;
				sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
				sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
				sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
				sFWDS201_t.system_parameter.last_air_data = 0x00;
			}
			else if (this->air_handle_position == IN_POSSITION)
			{
				this->air_work_handle_state = HANDLE_SLEEP;
				if (sFWDS201_t.general_parameter.set_air_temp_time == 0x00)
					sFWDS201_t.display_air_temp_number = DISPLAY_SLP;
				if (sFWDS201_t.general_parameter.set_air_time == 0x00)
					sFWDS201_t.display_air_number = DISPLAY_SLP;
			}
		}
		else if (this->sleep_state == SLEEP_CLOSE)
		{
			this->air_work_handle_state = HANDLE_WORKING;
			sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
			sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
			sFWDS201_t.system_parameter.last_air_data = 0x00;
		}
		this->system_parameter.air_error_time = 0;
		break;

	case HANDLE_WORKING:

		if (this->air_handle_position == IN_POSSITION &&
			this->sleep_state == SLEEP_OPEN)
		{
			if (this->system_parameter.air_actual_temp <= 70)
			{
				this->system_parameter.sleep_time++;

				if (this->system_parameter.sleep_time > SLEEP_TIME)
				{
					this->air_work_handle_state = HANDLE_SLEEP;

					this->system_parameter.sleep_time = 0;
					sFWDS201_t.fan_run_flag = false;
					//                    sbeep.cmd = BEEP_LONG;
					break;
				}
			}
		}
		else if (this->sleep_state == SLEEP_CLOSE || this->air_handle_position == NOT_IN_POSSITION)
		{
			this->air_work_handle_state = HANDLE_WORKING;
			if (first_state == false)
			{
				first_state = true;
				sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
				sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
				sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
				sFWDS201_t.system_parameter.last_air_data = 0x00;
			}
		}
		break;
	case HANDLE_WAKEN:
		if (sFWDS201_t.system_parameter.waken_time_count != 0)
		{
			if (this->air_handle_position == NOT_IN_POSSITION)
			{
				this->air_work_handle_state = HANDLE_WORKING;
				sFWDS201_t.system_parameter.waken_time_count = 0;
			}
		}
		else
		{
			if (this->air_handle_position == NOT_IN_POSSITION)
				this->air_work_handle_state = HANDLE_WORKING;
			else if (this->air_handle_position == IN_POSSITION)
				this->air_work_handle_state = HANDLE_SLEEP;
		}
		break;
	default:
		break;
	}
}

static void get_air_handle_error_state(DS201_Handle *this)
{
	static uint8_t get_step = 0;
	uint8_t err_type = 0; // 0:无错误 1:超温 2:低温 3:风机错误

	switch (get_step)
	{
	// ==================== 状态0：正常监测 ====================
	case 0:
		// 判断是否允许检测错误（手柄不在位 / 在位但未休眠）
		if ((this->air_handle_position == NOT_IN_POSSITION) ||
			(this->air_handle_position == IN_POSSITION && this->sleep_state == SLEEP_CLOSE))
		{
			// ============== 分步判断错误类型 ==============
			if (this->system_parameter.air_actual_temp >= MAX_ACTUAL_TEMP)
			{
				err_type = 1; // 超温错误
			}
			else if (this->system_parameter.air_actual_temp <= MIN_ACTUAL_TEMP)
			{
				err_type = 2; // 低温错误
			}
			else if (this->system_parameter.actual_air < MIN_ACTUAL_AIR)
			{
				err_type = 3; // 风机错误
			}

			// ============== 有错误 → 计时 ==============
			if (err_type != 0)
			{
				this->system_parameter.air_error_time++;

				// 超温：500ms 触发
				if (err_type == 1 && this->system_parameter.air_error_time > ERROR_TIME / 2)
				{
					this->airgun_handle_error_state = HANDLE_OVER_TEMP_ERR;
					sFWDS201_t.display_air_temp_number = DISPLAY_ERR;
					sFWDS201_t.display_air_number = DISPLAY_ERR;
					this->system_parameter.air_error_time = 0;
					get_step = 1;
				}
				// 低温/风机：1000ms 触发
				else if ((err_type == 2 || err_type == 3) &&
						 this->system_parameter.air_error_time > ERROR_TIME * 2)
				{
					if (err_type == 2)
						this->airgun_handle_error_state = HANDLE_LOW_TEMP_ERR;
					else
						this->airgun_handle_error_state = HANDLE_FAN_ERROR;

					sFWDS201_t.display_air_temp_number = DISPLAY_ERR;
					sFWDS201_t.display_air_number = DISPLAY_ERR;
					this->system_parameter.air_error_time = 0;
					get_step = 1;
				}
			}
			// ============== 无错误 → 清零计时 + 恢复显示 ==============
			else
			{
				this->system_parameter.air_error_time = 0;
			}
		}
		// ============== 不满足检测条件 → 直接恢复 ==============
		else
		{
			this->system_parameter.air_error_time = 0;
		}
		break;

	// ==================== 状态1：错误锁定，等待恢复条件 ====================
	case 1:
		// 手柄在位 + 已休眠 → 错误解除
		if (this->air_handle_position == IN_POSSITION && this->sleep_state == SLEEP_OPEN)
		{
			this->airgun_handle_error_state = HANDLE_OK;
			this->system_parameter.air_error_time = 0;
			sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
			sFWDS201_t.system_parameter.last_air_data = 0x00;
			get_step = 0;
		}
		break;

	default:
		get_step = 0;
		break;
	}
}

void fan_control(DS201_Handle *this)
{
	static uint8_t control_step = 0;
	static bool fan_run_flag = false;

	switch (control_step)
	{
	case 0:
		if (this->airgun_handle_error_state != HANDLE_OK)
		{
			control_step = 1;
			break;
		}
		
		if (this->air_work_handle_state == HANDLE_SLEEP)
		{
			/* keep fan output until the temp below 60 */
			if (this->system_parameter.air_actual_temp >= 60 && this->system_parameter.air_actual_temp < 70)
			{
				if (fan_run_flag == false)
				{
					tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, 80);
				}
			}
			else
			{
				if (fan_run_flag == false)
				{
					fan_run_flag = true;
					/* close fan output */
					tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, 0);
				}
			}
		}
		else if (this->air_work_handle_state == HANDLE_WORKING &&
				 this->air_handle_position == IN_POSSITION &&
				 this->sleep_state == SLEEP_OPEN)
		{
			sFWDS201_t.display_air_number = DISPLAY_REAL;
			/* wait for next time open */
			if (fan_run_flag)
			{
				if (this->relay_open_flag)
				{
					fan_run_flag = false;
				}
			}
			if (this->system_parameter.air_actual_temp >= 250)
			{
				this->system_parameter.sleep_air_data = SLEEP_FAN_DATA;

				if (fan_run_flag == false)
				{
					/* open fan output with a half of max set val*/
					tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, this->system_parameter.sleep_air_data * 0.87 + 81);
				}
			}
			else if (this->system_parameter.air_actual_temp >= 70 && this->system_parameter.air_actual_temp < 250)
			{
				if (fan_run_flag == false)
				{
					/* open fan output with actual temp change*/
					this->system_parameter.sleep_air_data = this->system_parameter.air_actual_temp * 0.4;
					tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, this->system_parameter.sleep_air_data * 0.87 + 81);
				}
			}
			else
			{
				/* close fan output */
				tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, 0);
			}
		}
		else if ((this->air_work_handle_state == HANDLE_WORKING &&
				  this->air_handle_position == NOT_IN_POSSITION) ||
				 this->sleep_state == SLEEP_CLOSE)
		{
			sFWDS201_t.display_air_number = DISPLAY_SET;
			sFWDS201_t.system_parameter.last_air_data = 0x00;
			/* wait for next time open */
			if (fan_run_flag == false)
			{
				if (this->relay_open_flag)
				{
					fan_run_flag = false;
				}
			}

			/* open fan output with user set val */
			tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, this->system_parameter.air_data * 0.87 + 81);
		}

		break;

	case 1:
		/* open fan output with max set val */
		tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_3, MAX_SET_AIR * 0.87 + 81);

		if (this->airgun_handle_error_state == HANDLE_OK)
		{
			control_step = 0;
			break;
		}

		break;

	default:
		break;
	}
}

int8_t linear_correction(uint16_t user_set_temp)
{
	volatile uint8_t n = 0;
	volatile uint16_t check_start = 0;
	volatile uint8_t check_value = 0;

	if (sFWDS201_t.system_parameter.air_data > 20)
	{
		if (user_set_temp < 300)
		{
			check_value = 0;
			check_start = 100;

			for (n = 0; n < 20; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (off_set_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}
		else
		{
			check_value = 0;
			check_start = 300;

			for (n = 20; n < 40; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (off_set_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}

		return (off_set_buff[n]);
	}
	else if (sFWDS201_t.system_parameter.air_data <= 20 && sFWDS201_t.system_parameter.air_data > 0)
	{
		if (user_set_temp < 300)
		{
			check_value = 0;
			check_start = 100;

			for (n = 0; n < 20; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (wind_set_20_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}
		}
		else
		{
			check_value = 0;
			check_start = 300;

			for (n = 20; n < 40; n++)
			{
				if (user_set_temp <= (check_start + check_value))
				{
					return (wind_set_20_buff[n]);
				}
				else
				{
					check_start += 10;
				}
			}

			return (wind_set_20_buff[n]);
		}
	}

	return 0;
}

static void relay_control(DS201_Handle *this)
{
	static uint8_t control_step = 0;
	static uint16_t open_relay_delay_time = RELAY_OPEN_TIME;

	if (this->air_work_handle_state == HANDLE_SLEEP || this->airgun_handle_error_state != HANDLE_OK)
	{
		/* close relay */
		gpio_bits_reset(GPIOC, GPIO_PINS_4);
		/* reset relay open flag */
		this->relay_open_flag = FALSE;
		open_relay_delay_time = RELAY_OPEN_TIME;
	}
	else if (this->air_work_handle_state == HANDLE_WORKING &&
			 this->airgun_handle_error_state == HANDLE_OK &&
			 this->air_handle_position == NOT_IN_POSSITION)
	{
		/* open relay */
		gpio_bits_set(GPIOC, GPIO_PINS_4);

		/* wait 300ms*/
		if (this->relay_open_flag == false)
		{
			open_relay_delay_time--;
		}

		/* relay opened done */
		if (0 == open_relay_delay_time)
		{
			this->relay_open_flag = true;
			open_relay_delay_time = RELAY_OPEN_TIME;
		}
	}
}

void air_pwm_control(DS201_Handle *this)
{
	static float temp = 0;
	static float air_set_temp = 0;
	static float air_last_set_temp = 0;
	static uint16_t delay_time = 0;
	static bool change_flag = false;
	//    static uint32_t hight_kd = 35000;
	static uint32_t hight_kd = 35000;
	static uint32_t low_kd = 5000;

	if (change_flag)
	{
		if (air_handle_pid.Kd <= low_kd)
			air_handle_pid.Kd = low_kd;
	}
	else
	{
		air_handle_pid.Kd = hight_kd;
	}

	air_set_temp = this->system_parameter.air_set_temp;

	if (air_last_set_temp != air_set_temp)
	{
		air_handle_pid.SumError = air_handle_pid.SumError / 2;
		this->system_parameter.temp_linear_data = linear_correction(air_set_temp);
		air_last_set_temp = air_set_temp;
	}

	this->system_parameter.air_actual_temp = move_average_filter(&air_temp) >> 2;
	//    temp = this->system_parameter.air_actual_temp + this->system_parameter.temp_linear_data - this->system_parameter.air_cal_data;
	temp = this->system_parameter.air_actual_temp - this->system_parameter.air_cal_data;
	if ((this->air_handle_position == NOT_IN_POSSITION || this->sleep_state == SLEEP_CLOSE))
	{
		if (this->relay_open_flag == true)
		{
			if (temp <= (air_set_temp + PID_RANGE) && temp >= (air_set_temp - PID_RANGE))
			{
				if (temp <= (air_set_temp + 5) && temp >= (air_set_temp - 5))
				{
					sFWDS201_t.hot_state = PID_POWER;
					delay_time++;
					if (delay_time >= 15)
					{
						if (air_handle_pid.Kd - 5000 <= low_kd)
						{
							air_handle_pid.Kd = low_kd;
						}
						else
						{
							air_handle_pid.Kd -= 5000;
						}

						delay_time = 0;
						change_flag = true;
					}
				}
				else
				{
					change_flag = false;
					delay_time = 0;
					sFWDS201_t.hot_state = FULL_POWER;
				}
				this->system_parameter.air_pwm_out = PID_Position_Calc(&air_handle_pid, air_set_temp, temp);
			}
			else if (temp > (air_set_temp + PID_RANGE))
			{
				this->system_parameter.air_pwm_out = 0;
				sFWDS201_t.hot_state = ZERO_POWER;
			}
			else if (temp < (air_set_temp - PID_RANGE))
			{
				this->system_parameter.air_pwm_out = MAX_AIR_PWM_OUTPUT;
				sFWDS201_t.hot_state = FULL_POWER;
			}
		}
	}
	else if ((this->air_handle_position == IN_POSSITION && this->sleep_state == SLEEP_OPEN))
	{
		
		change_flag = false;
		delay_time = 0;
		this->system_parameter.air_pwm_out = 0;
		PID_Clear(&air_handle_pid);
	}

	if (this->system_parameter.air_pwm_out <= 0)
	{
		this->system_parameter.air_pwm_out = 0;
	}

	if (this->airgun_handle_error_state != HANDLE_OK)
	{
		change_flag = false;
		delay_time = 0;
		this->system_parameter.air_pwm_out = 0;
		PID_Clear(&air_handle_pid);
	}
}

void sol_pwm_control(void)
{
	static sol_handle_state_e last_handle_type = RESET_VALUE;
	static float last_set_temp = 0;
	static float temp = 0;
	static uint16_t hot_temp = 0;
	static uint8_t v_num = 0;
	// 新增：恒温标志位，防止重复响铃
	static bool constant_temp_flag = false;

	temp = sFWDS201_t.system_parameter.sol_actual_temp - sFWDS201_t.system_parameter.sol_cal_data;

	if (last_set_temp != sFWDS201_t.system_parameter.sol_set_temp)
	{
		PID_Clear_I(&sol_210_handle_pid);
		last_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
		constant_temp_flag = false; // 设定温度改变，重置恒温标志
	}

	/* get temp */
	sFWDS201_t.system_parameter.sol_actual_temp = move_average_filter(&sol_temp) >> 2;

	/* pid control */
	if (sFWDS201_t.sol_handle_error_state == HANDLE_OK && sFWDS201_t.sol_work_handle_state == HANDLE_WORKING)
	{
		if (temp <= (sFWDS201_t.system_parameter.sol_set_temp + 30) &&
			temp >= (sFWDS201_t.system_parameter.sol_set_temp - 30))
		{
			sFWDS201_t.system_parameter.sol_pwm_out = PID_Position_Calc(&sol_210_handle_pid, sFWDS201_t.system_parameter.sol_set_temp, temp);
			
			// 新增：进入恒温区，且未响过蜂鸣器 → 响一声
			if(!constant_temp_flag && (temp <= (sFWDS201_t.system_parameter.sol_set_temp + 10) &&
			temp >= (sFWDS201_t.system_parameter.sol_set_temp - 10)))
			{
				constant_temp_flag = true;
				sbeep.cmd = BEEP_SHORT; 
			}
		}
		else if (temp > (sFWDS201_t.system_parameter.sol_set_temp + 30))
		{
			// 全功率减温
			PID_Clear_I(&sol_210_handle_pid);
			sFWDS201_t.system_parameter.sol_pwm_out = 0;
			constant_temp_flag = false; // 超温，重置恒温标志
		}
		else
		{
			PID_Clear_I(&sol_210_handle_pid);
			sFWDS201_t.system_parameter.sol_pwm_out = MAX_SOL210_PWM_OUTPUT;
			constant_temp_flag = false; // 升温中，重置恒温标志
		}
	}
	else if (sFWDS201_t.sol_work_handle_state == HANDLE_SLEEP)
	{
		sFWDS201_t.system_parameter.sol_pwm_out = 0;
		constant_temp_flag = false; // 休眠重置标志
	}
	else if(sFWDS201_t.sol_work_handle_state == HANDLE_RPC)
	{
		sFWDS201_t.system_parameter.sol_pwm_out = 0;
		constant_temp_flag = false; // RPC重置标志
	}
	if (sFWDS201_t.sol_handle_error_state != HANDLE_OK)
	{
		sFWDS201_t.system_parameter.sol_pwm_out = 0x00;
		constant_temp_flag = false; // 异常重置标志
	}
}
void sol_sleep_control(void)
{
	static bool wake_up_flag = false;
	static bool first_in = false;
	static uint8_t first_sleep = false;
	static uint16_t time_count_ms = 0;
	static uint8_t time_count_s = 0;
	
	// 立即休眠，但显示 SLP 要等温度 < 200℃
	bool temp_below_200 = (sFWDS201_t.system_parameter.sol_actual_temp < 200);

	if (sFWDS201_t.sol_handle_position == IN_POSSITION)
	{
		first_in = false;
		if(first_sleep == false)
		{
			first_sleep = true;
			// 立即进入休眠状态（PWM 停止加热）
			sFWDS201_t.sol_work_handle_state = HANDLE_SLEEP;
			sFWDS201_t.display_sol_temp_number = DISPLAY_SLP;
//			// 只有温度 <200 才显示 SLP
//			if (temp_below_200)
//			{
//				sFWDS201_t.display_sol_temp_number = DISPLAY_SLP;
//			}
		}
		else
		{
			if (sFWDS201_t.system_parameter.sleep_time_count)
			{
				sFWDS201_t.sol_work_handle_state = HANDLE_WORKING;
				time_count_ms++;
				if (time_count_ms >= 930)
				{
					time_count_ms = 0;
					time_count_s++;
					if(time_count_s >= 6)
					{
						sFWDS201_t.system_parameter.sleep_time_count-=0.1;
						if(sFWDS201_t.system_parameter.sleep_time_count <= 0)
						sFWDS201_t.system_parameter.sleep_time_count = 0;
						time_count_s = 0;
					}
				}
			}
			else
			{
				if (sFWDS201_t.sol_work_handle_state == HANDLE_WORKING)
				{
					// 立即休眠
					sFWDS201_t.sol_work_handle_state = HANDLE_SLEEP;

					// 温度低于200才显示SLP
					if (temp_below_200)
					{
						sFWDS201_t.display_sol_temp_number = DISPLAY_SLP;
					}
				}
			}
		}
	}
	else if (sFWDS201_t.sol_handle_position == NOT_IN_POSSITION)
	{
		first_sleep = false;
		if (first_in == false)
		{
			first_in = true;
			sFWDS201_t.sol_work_handle_state = HANDLE_WORKING;
			sFWDS201_t.system_parameter.sleep_time_count = sFWDS201_t.system_parameter.set_sleep_time;
			sFWDS201_t.display_sol_temp_number = DISPLAY_REAL;
			time_count_ms = 0;
			time_count_s = 0;
		}
	}

	/* check base waken time */
	if (sFWDS201_t.sol_work_handle_state == HANDLE_WAKEN)
	{
		wake_up_flag = true;
	}

	if (wake_up_flag)
	{
		sFWDS201_t.system_parameter.waken_time_count++;
		if (sFWDS201_t.system_parameter.waken_time_count >= 3000)
		{
			wake_up_flag = false;
			sFWDS201_t.system_parameter.waken_time_count = 0;

			if (sFWDS201_t.sol_handle_position == IN_POSSITION)
			{
				sFWDS201_t.sol_work_handle_state = HANDLE_SLEEP;

				// 温度低于200才显示SLP
				if (temp_below_200)
				{
					sFWDS201_t.display_sol_temp_number = DISPLAY_SLP;
				}
			}
			else
			{
				sFWDS201_t.sol_work_handle_state = HANDLE_WORKING;
			}
		}
		else
		{
			if(sFWDS201_t.general_parameter.save_sol_ch_flag == true)
			{
				sFWDS201_t.display_sol_temp_number = DISPLAY_SAVE_CH;
			}
			else
	 			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
		}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	}
}

void sol_rpc_control(void)
{
	static uint8_t rpc_times = 0x00;
	if(gpio_input_data_bit_read(GPIOC,GPIO_PINS_14) == false)
	{
		rpc_times++;
		if(rpc_times > 20)
		{
			rpc_times = 0x00;
			sFWDS201_t.sol_work_handle_state = HANDLE_RPC;
			sFWDS201_t.sol_handle_rpc_state = IN_RPC;
		}
	}
	else if(gpio_input_data_bit_read(GPIOB,GPIO_PINS_6) == true)
	{
		rpc_times = 0x00;
		sFWDS201_t.sol_handle_rpc_state = NOT_IN_RPC;
		sFWDS201_t.sol_work_handle_state = HANDLE_WORKING;
	}
}

void check_sol_over_current(void)
{
	static uint8_t over_times = 0x00;
	/*over current check*/
	sFWDS201_t.system_parameter.current_data = get_adcval(ADC_CHANNEL_3);

	if(sFWDS201_t.sol_handle_state == SOL_HANDLE_210)
	{
		if(sFWDS201_t.system_parameter.current_data > 2300)
		{
			sFWDS201_t.sol_handle_error_state = HANDLE_OVER_CURRENT;
			sFWDS201_t.display_sol_temp_number = DISPLAY_ERR;
		}
	}
}

void get_sol_handle_state(void)
{
	static bool select_flag = false;
	static uint8_t rpc_times = 0;
	static uint8_t last_position_state = NOT_IN_POSSITION;
	static uint8_t sleep_times = 0, no_sleep_times = 0;
	
	/* check sleep */
	if (gpio_input_data_bit_read(GPIOC, GPIO_PINS_15) == true)
	{
		no_sleep_times++;
		sleep_times = 0;
		if (no_sleep_times > 3)
		{
			no_sleep_times = 0; 
			sFWDS201_t.sol_handle_position = NOT_IN_POSSITION;
			last_position_state = sFWDS201_t.sol_handle_position;
		}
	}
	else
	{
		sleep_times++;
		no_sleep_times = 0;
		if (sleep_times > 3)
		{
			sleep_times = 0;
			sFWDS201_t.sol_handle_position = IN_POSSITION;
			last_position_state = sFWDS201_t.sol_handle_position;
		}
	}
	
	/* check handle type */
	if (gpio_input_data_bit_read(GPIOA, GPIO_PINS_7) == false)
	{
		sFWDS201_t.sol_handle_state = SOL_HANDLE_210;
	}
	else 
	{
		sFWDS201_t.sol_handle_state = SOL_HANDLE_NO;
	}
}

static void get_sol_handle_error_state(void)
{
	if (sFWDS201_t.sol_work_handle_state == HANDLE_WORKING)
	{
		if (sFWDS201_t.system_parameter.sol_actual_temp > MAX_ACTUAL_TEMP || sFWDS201_t.system_parameter.sol_actual_temp < MIN_ACTUAL_TEMP || sFWDS201_t.sol_handle_state == SOL_HANDLE_NO)
		{
			sFWDS201_t.system_parameter.sol_error_time++;
			if(sFWDS201_t.system_parameter.sol_error_time == 1)
			{
				if (sFWDS201_t.sol_handle_state == SOL_HANDLE_NO)
				{
					sFWDS201_t.sol_handle_error_state = HANDLE_NO_ERR;
					sFWDS201_t.display_sol_temp_number = DISPLAY_ERR;
				}
			}
			
			if (sFWDS201_t.system_parameter.sol_error_time >= ERROR_TIME)
			{
				if (sFWDS201_t.sol_handle_state == SOL_HANDLE_NO)
				{
					sFWDS201_t.sol_handle_error_state = HANDLE_NO_ERR;
					sFWDS201_t.display_sol_temp_number = DISPLAY_ERR;
				}
				else if (sFWDS201_t.system_parameter.sol_actual_temp > MAX_ACTUAL_TEMP)
				{
					sFWDS201_t.sol_handle_error_state = HANDLE_OVER_TEMP_ERR;
					sFWDS201_t.display_sol_temp_number = DISPLAY_ERR;
				}
				else if (sFWDS201_t.system_parameter.sol_actual_temp < MIN_ACTUAL_TEMP &&
						sFWDS201_t.sol_handle_position == NOT_IN_POSSITION &&
						sFWDS201_t.sol_handle_error_state != HANDLE_OVER_CURRENT)
				{
					sFWDS201_t.sol_handle_error_state = HANDLE_LOW_TEMP_ERR;
					sFWDS201_t.display_sol_temp_number = DISPLAY_ERR;
				}
			}
		}
		else
		{
			sFWDS201_t.system_parameter.sol_error_time = 0;
			sFWDS201_t.sol_handle_error_state = HANDLE_OK;
		}
	}
	else 
	{
		sFWDS201_t.system_parameter.sol_error_time = 0;
		if(sFWDS201_t.sol_handle_error_state  != HANDLE_OVER_CURRENT)
			sFWDS201_t.sol_handle_error_state = HANDLE_OK;
	}
}