#include "ec11_handle.h"
#include "beep_handle.h"
#include "DS201_handle.h"

// 事件处理结束标记
#define AIR_TEMP_EC11_EVENT_END AIR_TEMP_EC11_END_EVENT
#define SOL_TEMP_EC11_EVENT_END SOL_TEMP_EC11_END_EVENT
#define AIR_EC11_EVENT_END AIR_EC11_END_EVENT

void air_temp_ec11_event_handle(void);
void sol_temp_ec11_event_handle(void);
void air_ec11_event_handle(void);

void air_temp_ec11_get_event(EC11_AnalyzeResult state);
void sol_temp_ec11_get_event(EC11_AnalyzeResult state);
void air_ec11_get_event(EC11_AnalyzeResult state);

static air_temp_ec11_event_e air_temp_ec11_event = AIR_TEMP_EC11_END_EVENT;
static sol_temp_ec11_event_e sol_temp_ec11_event = SOL_TEMP_EC11_END_EVENT;
static air_ec11_event_e g_air_ec11_event = AIR_EC11_END_EVENT;

static bool air_temp_long_press_handled = false;
static bool sol_temp_long_press_handled = false;


EC11_AnalyzeResult air_temp_ec11_analyze_result;
EC11_AnalyzeResult sol_temp_ec11_analyze_result;
EC11_AnalyzeResult g_air_ec11_analyze_result;

/************************** EC11总处理 **************************/
/**
 * @brief 
 * @note  
 */
void ec11_handle(void)
{
	// 风枪温度EC11处理
	EC11_ScanResult air_temp_scan_result = EC11_Scan(&air_temp_ec11);
	air_temp_ec11_analyze_result = EC11_Analyze(&air_temp_ec11, air_temp_scan_result);
	air_temp_ec11_get_event(air_temp_ec11_analyze_result);
	air_temp_ec11_event_handle();
	
	// 烙铁温度EC11处理
	EC11_ScanResult sol_temp_scan_result = EC11_Scan(&sol_temp_ec11);
	sol_temp_ec11_analyze_result = EC11_Analyze(&sol_temp_ec11, sol_temp_scan_result);
	sol_temp_ec11_get_event(sol_temp_ec11_analyze_result);
	sol_temp_ec11_event_handle();

	// 风量EC11处理
	EC11_ScanResult air_scan_result = EC11_Scan(&air_ec11);
	g_air_ec11_analyze_result = EC11_Analyze(&air_ec11, air_scan_result);
	air_ec11_get_event(g_air_ec11_analyze_result);
	air_ec11_event_handle();
	
	
}


void air_ec11_get_event(EC11_AnalyzeResult state)
{
	static int disp_vision_time = 0x00;
	switch (state)
	{
	case EC11_ANALYZE_CW:
		if (sFWDS201_t.set_flag == false)
		{
			g_air_ec11_event = AIR_ADD;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_CCW:
		if (sFWDS201_t.set_flag == false)
		{
			g_air_ec11_event = AIR_REDUCE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CW:
		if (sFWDS201_t.set_flag == false)
		{
			g_air_ec11_event = AIR_ADD_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CCW:
		if (sFWDS201_t.set_flag == false)
		{
			g_air_ec11_event = AIR_REDUCE_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_SHORT_CLICK:
		break;
	case EC11_ANALYZE_DOUBLE_CLICK:
		break;
	case EC11_ANALYZE_LONG_PRESS:
		break;
	case EC11_ANALYZE_LONG_RELEASE:
		break;
	case EC11_ANALYZE_KEY_CW:
		break;
	case EC11_ANALYZE_KEY_CCW:
		break;
	default:
		break;
	}
}


void air_temp_ec11_get_event(EC11_AnalyzeResult state)
{
	static int set_time = SET_TIME;
	static int reset_time = RESET_TIME;
	switch (state)
	{
	case EC11_ANALYZE_CW:
		if (sFWDS201_t.set_flag == true)
		{
			sFWDS201_t.set_interface_number++;
			if (sFWDS201_t.set_interface_number > 10)
				sFWDS201_t.set_interface_number = 0;
			set_time = SET_TIME;
		}
		else
		{
			air_temp_ec11_event = AIR_TEMP_ADD;
		}
		
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_CCW:
		if (sFWDS201_t.set_flag == true)
		{
			if (sFWDS201_t.set_interface_number == 0)
			{
				sFWDS201_t.set_interface_number = 10;
			}
			else
				sFWDS201_t.set_interface_number--;
			set_time = SET_TIME;
		}
		else
		{
			air_temp_ec11_event = AIR_TEMP_REDUCE;
		}
		
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CW:
		if (sFWDS201_t.set_flag == false)
		{
			air_temp_ec11_event = AIR_TEMP_ADD_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CCW:
		if (sFWDS201_t.set_flag == false)
		{
			air_temp_ec11_event = AIR_TEMP_REDUCE_FIVE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_SHORT_CLICK:
//		if (sFWDS201_t.set_interface_number == EXIT)
//		{
//			air_temp_ec11_event = EXIT_SET;
//		}
//		else if (sFWDS201_t.set_interface_number == RESET_RUN)
//		{
//			air_temp_ec11_event = RESET_DS01_VALUE;
//		}
//		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_DOUBLE_CLICK:

		break;
	case EC11_ANALYZE_LONG_PRESS:
		// 长按进入设置界面
		if (!air_temp_long_press_handled)
		{
			air_temp_long_press_handled = true;
			if (sFWDS201_t.set_flag == false)
			{
				sFWDS201_t.set_flag = true;
				sFWDS201_t.set_interface_number = EXIT;
				set_time = SET_TIME;
			}
			else
			{
				sFWDS201_t.set_interface_number = EXIT;
				air_temp_ec11_event = EXIT_SET;
			}
		}
		sbeep.cmd = BEEP_LONG;
		break;
	case EC11_ANALYZE_LONG_RELEASE:
		air_temp_long_press_handled = false;
		break;
	case EC11_ANALYZE_KEY_CW:
		break;
	case EC11_ANALYZE_KEY_CCW:
		break;
	default:
		break;
	}
	if (sFWDS201_t.set_flag == false)
	{
		sFWDS201_t.system_parameter.air_cal_temp_c_display = sFWDS201_t.system_parameter.air_set_temp;
		sFWDS201_t.system_parameter.air_cal_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display;
		
		sFWDS201_t.system_parameter.sol_cal_temp_c_display = sFWDS201_t.system_parameter.sol_set_temp;
		sFWDS201_t.system_parameter.sol_cal_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display;
	}
	else
	{
		if (state == EC11_ANALYZE_NO_ACTION)
		{
			set_time--;
			if (set_time < 0)
			{
				set_time = SET_TIME;
				air_temp_ec11_event = EXIT_SET;
			}
		}
		if (sFWDS201_t.reset_flag == true)
		{
			reset_time--;
			if (reset_time < 0)
			{
				reset_time = RESET_TIME;
				air_temp_ec11_event = EXIT_SET;
				sFWDS201_t.reset_flag = false;
			}
		}
	}
}

void sol_temp_ec11_get_event(EC11_AnalyzeResult state)
{
	static int set_time = SET_TIME;
	static int reset_time = RESET_TIME;
	switch (state)
	{
	case EC11_ANALYZE_CW:
		if (sFWDS201_t.set_flag == true)
		{
			switch (sFWDS201_t.set_interface_number)
			{
			case EXIT:
				break;
			case AIR_TEMP_CAL:
				sol_temp_ec11_event = AIR_TEMP_CAL_ADD;
				break;
			case SOL_TEMP_CAL:
				sol_temp_ec11_event = SOL_TEMP_CAL_ADD;
				break;
			case TEMP_UNIT:
				sol_temp_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				sol_temp_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				sol_temp_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				sol_temp_ec11_event = SET_SLEEP_STATE;
				break;
			case SET_SLEEP_TIME:
				sol_temp_ec11_event = SOL_SLEEP_TIME_ADD;
				break;
			case SOFTWARE_VERSION:
				break;
			case HARDWARE_VERSION:
				break;
			case RESET_RUN:
				break;
			}
		}
		else
		{
			sol_temp_ec11_event = SOL_TEMP_ADD;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_CCW:
		if (sFWDS201_t.set_flag == true)
		{
			switch (sFWDS201_t.set_interface_number)
			{
			case EXIT:
				break;
			case AIR_TEMP_CAL:
				sol_temp_ec11_event = AIR_TEMP_CAL_REDUCE;
				break;
			case SOL_TEMP_CAL:
				sol_temp_ec11_event = SOL_TEMP_CAL_REDUCE;
			case TEMP_UNIT:
				sol_temp_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				sol_temp_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				sol_temp_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				sol_temp_ec11_event = SET_SLEEP_STATE;
				break;
			case SET_SLEEP_TIME:
				sol_temp_ec11_event = SOL_SLEEP_TIME_REDUCE;
				break;
			case SOFTWARE_VERSION:
				break;
			case HARDWARE_VERSION:
				break;
			case RESET_RUN:
				break;
			}
		}
		else
		{
			sol_temp_ec11_event = SOL_TEMP_REDUCE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CW:
		if (sFWDS201_t.set_flag == false)
		{
			sol_temp_ec11_event = SOL_TEMP_ADD_FIVE;
		}
		else
		{
			switch (sFWDS201_t.set_interface_number)
			{
			case EXIT:
				break;
			case AIR_TEMP_CAL:
				sol_temp_ec11_event = AIR_TEMP_CAL_ADD_FIVE;
				break;
			case SOL_TEMP_CAL:
				sol_temp_ec11_event = SOL_TEMP_CAL_ADD_FIVE;
			case TEMP_UNIT:
				sol_temp_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				sol_temp_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				sol_temp_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				sol_temp_ec11_event = SET_SLEEP_STATE;
				break;
			case SET_SLEEP_TIME:
				sol_temp_ec11_event = SOL_SLEEP_TIME_ADD_FIVE;
				break;
			case SOFTWARE_VERSION:
				break;
			case HARDWARE_VERSION:
				break;
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_FAST_CCW:
		if (sFWDS201_t.set_flag == false)
		{
			sol_temp_ec11_event = SOL_TEMP_REDUCE_FIVE;
		}
		else
		{
			switch (sFWDS201_t.set_interface_number)
			{
			case EXIT:
				break;
			case AIR_TEMP_CAL:
				sol_temp_ec11_event = AIR_TEMP_CAL_REDUCE_FIVE;
				break;
			case SOL_TEMP_CAL:
				sol_temp_ec11_event = SOL_TEMP_CAL_REDUCE_FIVE;
			case TEMP_UNIT:
				sol_temp_ec11_event = SET_UNIT;
				break;
			case SPEAK_STATE:
				sol_temp_ec11_event = SET_SPEAK;
				break;
			case DISPLAY_LOCK:
				sol_temp_ec11_event = SET_DISPLAY_LOCK;
				break;
			case SLEEP_STATE:
				sol_temp_ec11_event = SET_SLEEP_STATE;
				break;
			case SET_SLEEP_TIME:
				sol_temp_ec11_event = SOL_SLEEP_TIME_REDUCE_FIVE;
				break;
			case SOFTWARE_VERSION:
				break;
			case HARDWARE_VERSION:
				break;
			
			case RESET_RUN:
				break;
			}
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_SHORT_CLICK:
		if (sFWDS201_t.set_interface_number == EXIT)
		{
			air_temp_ec11_event = EXIT_SET;
		}
		else if (sFWDS201_t.set_interface_number == RESET_RUN)
		{
			air_temp_ec11_event = RESET_DS01_VALUE;
		}
		sbeep.cmd = BEEP_SHORT;
		break;
	case EC11_ANALYZE_DOUBLE_CLICK:

		break;
	case EC11_ANALYZE_LONG_PRESS:
		break;
	case EC11_ANALYZE_LONG_RELEASE:
		break;
	case EC11_ANALYZE_KEY_CW:
		break;
	case EC11_ANALYZE_KEY_CCW:
		break;
	default:
		break;
	}
	if(state != EC11_ANALYZE_NO_ACTION)
	{
		if (sFWDS201_t.sol_work_handle_state == HANDLE_SLEEP)
		{
			sFWDS201_t.sol_work_handle_state = HANDLE_WAKEN;
		}
		else if (sFWDS201_t.sol_work_handle_state == HANDLE_WAKEN)
		{
			sFWDS201_t.system_parameter.waken_time_count = 0;
		}
	}
}

void sol_temp_ec11_event_handle(void)
{
	static int8_t delay_time = 0;
	switch (sol_temp_ec11_event)
	{
		case SOL_TEMP_ADD:
			sbeep.cmd = BEEP_SHORT;
			sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp >= MAX_SET_SOL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp = 0;
					sFWDS201_t.system_parameter.sol_set_temp = MAX_SET_SOL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp++;
				}

				sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp_f_display >= MAX_SET_SOL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0;
					sFWDS201_t.system_parameter.sol_set_temp_f_display = MAX_SET_SOL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp_f_display++;
				}

				sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
			}
			sFWDS201_t.general_parameter.sol_ch = 0;
			break;
		case SOL_TEMP_ADD_FIVE:
			sbeep.cmd = BEEP_SHORT;
			sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp + 10 >= MAX_SET_SOL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp = 0;
					sFWDS201_t.system_parameter.sol_set_temp = MAX_SET_SOL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp += 10;
				}

				sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp_f_display + 50 >= MAX_SET_SOL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0;
					sFWDS201_t.system_parameter.sol_set_temp_f_display = MAX_SET_SOL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp_f_display += 50;
				}

				sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
			}
			sFWDS201_t.general_parameter.sol_ch = 0;
			break;
		case SOL_TEMP_REDUCE:
			sbeep.cmd = BEEP_SHORT;
			sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp <= MIN_SET_SOL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp = 0;
					sFWDS201_t.system_parameter.sol_set_temp = MIN_SET_SOL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp--;
				}

				sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp_f_display <= MIN_SET_SOL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0;
					sFWDS201_t.system_parameter.sol_set_temp_f_display = MIN_SET_SOL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp_f_display--;
				}

				sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
			}
			sFWDS201_t.general_parameter.sol_ch = 0;
			break;
		case SOL_TEMP_REDUCE_FIVE:
			sbeep.cmd = BEEP_SHORT;
			sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp - 10 <= MIN_SET_SOL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp = 0;
					sFWDS201_t.system_parameter.sol_set_temp = MIN_SET_SOL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp -= 10;
				}

				sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_set_temp_f_display <= MIN_SET_SOL_TEMP_F + 41)
				{
					sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0;
					sFWDS201_t.system_parameter.sol_set_temp_f_display = MIN_SET_SOL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_set_temp_f_display -= 41;
				}

				sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
			}
			sFWDS201_t.general_parameter.sol_ch = 0;
			break;
		break;
		case AIR_TEMP_CAL_ADD:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_c_display >= sFWDS201_t.system_parameter.air_set_temp + MAX_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display = sFWDS201_t.system_parameter.air_set_temp + MAX_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display++;
				}

				sFWDS201_t.system_parameter.air_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_f_display >= sFWDS201_t.system_parameter.air_set_temp_f_display + MAX_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display + MAX_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display++;
				}

				sFWDS201_t.system_parameter.air_cal_temp_c_display = (sFWDS201_t.system_parameter.air_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case AIR_TEMP_CAL_ADD_FIVE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_c_display + 10 >= sFWDS201_t.system_parameter.air_set_temp + MAX_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display = sFWDS201_t.system_parameter.air_set_temp + MAX_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display += 10;
				}

				sFWDS201_t.system_parameter.air_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_f_display + 10 >= sFWDS201_t.system_parameter.air_set_temp_f_display + MAX_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display + MAX_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display += 10;
				}

				sFWDS201_t.system_parameter.air_cal_temp_c_display = (sFWDS201_t.system_parameter.air_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case AIR_TEMP_CAL_REDUCE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_c_display <= sFWDS201_t.system_parameter.air_set_temp + MIN_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display = sFWDS201_t.system_parameter.air_set_temp + MIN_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display--;
				}

				sFWDS201_t.system_parameter.air_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_f_display <= sFWDS201_t.system_parameter.air_set_temp_f_display + MIN_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display + MIN_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display--;
				}

				sFWDS201_t.system_parameter.air_cal_temp_c_display = (sFWDS201_t.system_parameter.air_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case AIR_TEMP_CAL_REDUCE_FIVE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_c_display - 10 <= sFWDS201_t.system_parameter.air_set_temp + MIN_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display = sFWDS201_t.system_parameter.air_set_temp + MIN_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_c_display -= 10;
				}

				sFWDS201_t.system_parameter.air_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.air_cal_temp_f_display - 10 <= sFWDS201_t.system_parameter.air_set_temp_f_display + MIN_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display = sFWDS201_t.system_parameter.air_set_temp_f_display + MIN_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.air_cal_temp_f_display -= 10;
				}

				sFWDS201_t.system_parameter.air_cal_temp_c_display = (sFWDS201_t.system_parameter.air_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
			
		case SOL_TEMP_CAL_ADD:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_c_display >= sFWDS201_t.system_parameter.sol_set_temp + MAX_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display = sFWDS201_t.system_parameter.sol_set_temp + MAX_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display++;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_f_display >= sFWDS201_t.system_parameter.sol_set_temp_f_display + MAX_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display + MAX_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display++;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_c_display = (sFWDS201_t.system_parameter.sol_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case SOL_TEMP_CAL_ADD_FIVE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_c_display + 10 >= sFWDS201_t.system_parameter.sol_set_temp + MAX_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display = sFWDS201_t.system_parameter.sol_set_temp + MAX_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display += 10;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_f_display + 10 >= sFWDS201_t.system_parameter.sol_set_temp_f_display + MAX_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display + MAX_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display += 10;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_c_display = (sFWDS201_t.system_parameter.sol_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case SOL_TEMP_CAL_REDUCE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_c_display <= sFWDS201_t.system_parameter.sol_set_temp + MIN_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display = sFWDS201_t.system_parameter.sol_set_temp + MIN_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display--;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_f_display <= sFWDS201_t.system_parameter.sol_set_temp_f_display + MIN_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display + MIN_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display--;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_c_display = (sFWDS201_t.system_parameter.sol_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
		case SOL_TEMP_CAL_REDUCE_FIVE:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_c_display - 10 <= sFWDS201_t.system_parameter.sol_set_temp + MIN_CAL_TEMP)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display = sFWDS201_t.system_parameter.sol_set_temp + MIN_CAL_TEMP;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_c_display -= 10;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				if (sFWDS201_t.system_parameter.sol_cal_temp_f_display - 10 <= sFWDS201_t.system_parameter.sol_set_temp_f_display + MIN_CAL_TEMP_F)
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display = sFWDS201_t.system_parameter.sol_set_temp_f_display + MIN_CAL_TEMP_F;
				}
				else
				{
					sFWDS201_t.system_parameter.sol_cal_temp_f_display -= 10;
				}

				sFWDS201_t.system_parameter.sol_cal_temp_c_display = (sFWDS201_t.system_parameter.sol_cal_temp_f_display - 32) * 5 / 9;
			}
			break;
			
		case SET_UNIT:
			if (sFWDS201_t.temp_unit == CELSIUS)
			{
				sFWDS201_t.temp_unit = FAHRENHEIT;
				sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
				sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;

			}
			else if (sFWDS201_t.temp_unit == FAHRENHEIT)
			{
				sFWDS201_t.temp_unit = CELSIUS;
				sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
				sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
			}
			
			break;
		case SET_SPEAK:
			if (sFWDS201_t.speak_state == SPEAKER_CLOSE)
			{
				sFWDS201_t.speak_state = SPEAKER_OPEN;
			}
			else if (sFWDS201_t.speak_state == SPEAKER_OPEN)
			{
				sFWDS201_t.speak_state = SPEAKER_CLOSE;
			}
			break;
		case SET_DISPLAY_LOCK:
			if (sFWDS201_t.display_lock_state == LOCK)
			{
				sFWDS201_t.display_lock_state = UNLOCK;
			}
			else if (sFWDS201_t.display_lock_state == UNLOCK)
			{
				sFWDS201_t.display_lock_state = LOCK;
			}
			break;
		case SET_SLEEP_STATE:
			if (sFWDS201_t.sleep_state == SLEEP_CLOSE)
			{
				sFWDS201_t.sleep_state = SLEEP_OPEN;
			}
			else if (sFWDS201_t.sleep_state == SLEEP_OPEN)
			{
				sFWDS201_t.sleep_state = SLEEP_CLOSE;
			}
			break;
		case SOL_SLEEP_TIME_ADD:
				if (sFWDS201_t.system_parameter.set_sleep_time >= MAX_SLEEP_TIME)
			{
				sFWDS201_t.system_parameter.set_sleep_time = MAX_SLEEP_TIME;
			}
			else 
			{
				sFWDS201_t.system_parameter.set_sleep_time += 1;
			}
			sFWDS201_t.system_parameter.sleep_time_count = sFWDS201_t.system_parameter.set_sleep_time;
			break;
		case SOL_SLEEP_TIME_ADD_FIVE:
			delay_time--;
			if (delay_time <= 0)
			{
				delay_time = KEY_LONG_PRESS_TIME;
				sbeep.cmd = BEEP_SHORT;
				if (sFWDS201_t.system_parameter.set_sleep_time >= MAX_SLEEP_TIME - 5)
				{
					sFWDS201_t.system_parameter.set_sleep_time = MAX_SLEEP_TIME;
				}
				else
				{
					sFWDS201_t.system_parameter.set_sleep_time += 5;
				}
			}
			sFWDS201_t.system_parameter.sleep_time_count = sFWDS201_t.system_parameter.set_sleep_time;
			break;
		case SOL_SLEEP_TIME_REDUCE:
			if (sFWDS201_t.system_parameter.set_sleep_time <= MIN_SLEEP_TIME)
			{
				sFWDS201_t.system_parameter.set_sleep_time = MIN_SLEEP_TIME;
			}
			else
			{
				sFWDS201_t.system_parameter.set_sleep_time -= 1;
			}
			sFWDS201_t.system_parameter.sleep_time_count = sFWDS201_t.system_parameter.set_sleep_time;
			break;
		case SOL_SLEEP_TIME_REDUCE_FIVE:
			delay_time--;
			if (delay_time <= 0)
			{
				delay_time = KEY_LONG_PRESS_TIME;
				sbeep.cmd = BEEP_SHORT;
				if (sFWDS201_t.system_parameter.set_sleep_time <= MIN_SLEEP_TIME + 5)
				{
					sFWDS201_t.system_parameter.set_sleep_time = MIN_SLEEP_TIME;
				}
				else
				{
					sFWDS201_t.system_parameter.set_sleep_time -= 5;
				}
			}
			sFWDS201_t.system_parameter.sleep_time_count = sFWDS201_t.system_parameter.set_sleep_time;
			break;
		case SOL_TEMP_EC11_EVENT_END: // 无事件
		default:
			break;
	}

	// 事件处理完成，复位事件标记
	sol_temp_ec11_event = SOL_TEMP_EC11_EVENT_END;
}


void air_temp_ec11_event_handle(void)
{
	switch (air_temp_ec11_event)
	{
	case AIR_TEMP_ADD:
		sbeep.cmd = BEEP_SHORT;
		sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_temp_number = DISPLAY_SET;
		if (sFWDS201_t.temp_unit == CELSIUS)
		{
			if (sFWDS201_t.system_parameter.air_set_temp >= MAX_SET_AIR_TEMP)
			{
				sFWDS201_t.system_parameter.air_last_set_temp = 0;
				sFWDS201_t.system_parameter.air_set_temp = MAX_SET_AIR_TEMP;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp++;
			}

			sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
		}
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS201_t.system_parameter.air_set_temp_f_display >= MAX_SET_AIR_TEMP_F)
			{
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0;
				sFWDS201_t.system_parameter.air_set_temp_f_display = MAX_SET_AIR_TEMP_F;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp_f_display++;
			}

			sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_TEMP_ADD_FIVE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_temp_number = DISPLAY_SET;
		if (sFWDS201_t.temp_unit == CELSIUS)
		{
			if (sFWDS201_t.system_parameter.air_set_temp + 10 >= MAX_SET_AIR_TEMP)
			{
				sFWDS201_t.system_parameter.air_last_set_temp = 0;
				sFWDS201_t.system_parameter.air_set_temp = MAX_SET_AIR_TEMP;
			} 
			else
			{
				sFWDS201_t.system_parameter.air_set_temp += 10;
			}

			sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
		}
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS201_t.system_parameter.air_set_temp_f_display + 50 >= MAX_SET_AIR_TEMP_F)
			{
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0;
				sFWDS201_t.system_parameter.air_set_temp_f_display = MAX_SET_AIR_TEMP_F;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp_f_display += 50;
			}

			sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_TEMP_REDUCE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_temp_number = DISPLAY_SET;
		if (sFWDS201_t.temp_unit == CELSIUS)
		{
			if (sFWDS201_t.system_parameter.air_set_temp <= MIN_SET_AIR_TEMP)
			{
				sFWDS201_t.system_parameter.air_last_set_temp = 0;
				sFWDS201_t.system_parameter.air_set_temp = MIN_SET_AIR_TEMP;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp--;
			}

			sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
		}
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS201_t.system_parameter.air_set_temp_f_display <= MIN_SET_AIR_TEMP_F)
			{
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0;
				sFWDS201_t.system_parameter.air_set_temp_f_display = MIN_SET_AIR_TEMP_F;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp_f_display--;
			}

			sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_TEMP_REDUCE_FIVE:
		sbeep.cmd = BEEP_SHORT;
		sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_temp_number = DISPLAY_SET;
		if (sFWDS201_t.temp_unit == CELSIUS)
		{
			if (sFWDS201_t.system_parameter.air_set_temp - 10 <= MIN_SET_AIR_TEMP)
			{
				sFWDS201_t.system_parameter.air_last_set_temp = 0;
				sFWDS201_t.system_parameter.air_set_temp = MIN_SET_AIR_TEMP;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp -= 10;
			}

			sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;
		}
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
		{
			if (sFWDS201_t.system_parameter.air_set_temp_f_display <= MIN_SET_AIR_TEMP_F + 41)
			{
				sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0;
				sFWDS201_t.system_parameter.air_set_temp_f_display = MIN_SET_AIR_TEMP_F;
			}
			else
			{
				sFWDS201_t.system_parameter.air_set_temp_f_display -= 41;
			}

			sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case EXIT_SET:
	{
		sFWDS201_t.set_flag = false;

		sFWDS201_t.system_parameter.air_cal_data += sFWDS201_t.system_parameter.air_set_temp - sFWDS201_t.system_parameter.air_cal_temp_c_display;
		if (sFWDS201_t.system_parameter.air_cal_data >= MAX_CAL_TEMP)
		{
			sFWDS201_t.system_parameter.air_cal_data = MAX_CAL_TEMP;
		}
		if (sFWDS201_t.system_parameter.air_cal_data <= MIN_CAL_TEMP)
		{
			sFWDS201_t.system_parameter.air_cal_data = MIN_CAL_TEMP;
		}
		
		sFWDS201_t.system_parameter.sol_cal_data += sFWDS201_t.system_parameter.sol_set_temp - sFWDS201_t.system_parameter.sol_cal_temp_c_display;
		if (sFWDS201_t.system_parameter.sol_cal_data >= MAX_CAL_TEMP)
		{
			sFWDS201_t.system_parameter.sol_cal_data = MAX_CAL_TEMP;
		}
		if (sFWDS201_t.system_parameter.sol_cal_data <= MIN_CAL_TEMP)
		{
			sFWDS201_t.system_parameter.sol_cal_data = MIN_CAL_TEMP;
		}

		sbeep.cmd = BEEP_LONG;
	}
	break;
	case RESET_DS01_VALUE:
		sFWDS201_t.reset_flag = true;
		/* reset value */
		/* reset handle data */
		sFWDS201_t.system_parameter.air_set_temp = 380;
		sFWDS201_t.system_parameter.sol_set_temp = 380;
		sFWDS201_t.system_parameter.air_data = 40;
		sFWDS201_t.system_parameter.air_cal_data = 0;
		sFWDS201_t.system_parameter.sol_cal_data = 0;
		sFWDS201_t.system_parameter.ch1_air_set_temp = 380;
		sFWDS201_t.system_parameter.ch2_air_set_temp = 400;
		sFWDS201_t.system_parameter.ch3_air_set_temp = 420;
		sFWDS201_t.system_parameter.ch1_sol_set_temp = 380;
		sFWDS201_t.system_parameter.ch2_sol_set_temp = 400;
		sFWDS201_t.system_parameter.ch3_sol_set_temp = 420;
		sFWDS201_t.system_parameter.ch1_set_air = 40;
		sFWDS201_t.system_parameter.ch2_set_air = 50;
		sFWDS201_t.system_parameter.ch3_set_air = 60;
		sFWDS201_t.general_parameter.air_ch = 0;
		sFWDS201_t.general_parameter.sol_ch = 0;
		sFWDS201_t.system_parameter.set_sleep_time = 0;
		/* reset function data */

		sFWDS201_t.temp_unit = CELSIUS;

		sFWDS201_t.speak_state = SPEAKER_OPEN;

		sFWDS201_t.display_lock_state = LOCK;

		sFWDS201_t.sleep_state = SLEEP_OPEN;

		sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
		sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
		
		if (sFWDS201_t.temp_unit == CELSIUS)
		{
//			sFWDS201_t.temp_unit = FAHRENHEIT;
			sFWDS201_t.system_parameter.sol_set_temp_f_display = 9 * sFWDS201_t.system_parameter.sol_set_temp / 5 + 32;
			sFWDS201_t.system_parameter.air_set_temp_f_display = 9 * sFWDS201_t.system_parameter.air_set_temp / 5 + 32;

		}
		else if (sFWDS201_t.temp_unit == FAHRENHEIT)
		{
//			sFWDS201_t.temp_unit = CELSIUS;
			sFWDS201_t.system_parameter.sol_set_temp = (sFWDS201_t.system_parameter.sol_set_temp_f_display - 32) * 5 / 9;
			sFWDS201_t.system_parameter.air_set_temp = (sFWDS201_t.system_parameter.air_set_temp_f_display - 32) * 5 / 9;
		}
	case AIR_TEMP_EC11_EVENT_END: // 无事件
	default:
		break;
	}

	// 事件处理完成，复位事件标记
	air_temp_ec11_event = AIR_TEMP_EC11_EVENT_END;
}

/************************** 电流EC11事件处理（调节电流值） **************************/
void air_ec11_event_handle(void)
{
	switch (g_air_ec11_event)
	{
	case AIR_ADD:
		sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_number = DISPLAY_SET;
		if (sFWDS201_t.system_parameter.air_data + 1 > MAX_SET_AIR)
		{
			sFWDS201_t.system_parameter.air_data = MAX_SET_AIR;
			sFWDS201_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS201_t.system_parameter.air_data++;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_ADD_FIVE:
		sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_number = DISPLAY_SET;
		if (sFWDS201_t.system_parameter.air_data + 5 > MAX_SET_AIR)
		{
			sFWDS201_t.system_parameter.air_data = MAX_SET_AIR;
			sFWDS201_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS201_t.system_parameter.air_data += 5;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_REDUCE:
		sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_number = DISPLAY_SET;
		if (sFWDS201_t.system_parameter.air_data - 1 < MIN_SET_AIR)
		{
			sFWDS201_t.system_parameter.air_data = MIN_SET_AIR;
			sFWDS201_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS201_t.system_parameter.air_data--;
		}
		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_REDUCE_FIVE:
		sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
		sFWDS201_t.display_air_number = DISPLAY_SET;
		if (sFWDS201_t.system_parameter.air_data - 5 < MIN_SET_AIR)
		{
			sFWDS201_t.system_parameter.air_data = MIN_SET_AIR;
			sFWDS201_t.system_parameter.last_air_data = 0;
		}
		else
		{
			sFWDS201_t.system_parameter.air_data -= 5;
		}

		sFWDS201_t.general_parameter.air_ch = 0;
		break;
	case AIR_EC11_EVENT_END: // 无事件
	default:
		break;
	}

	// 事件处理完成，复位事件标记
	g_air_ec11_event = AIR_EC11_EVENT_END;
}
