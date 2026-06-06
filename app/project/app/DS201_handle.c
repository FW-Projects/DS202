#include "DS201_Handle.h"
#include <string.h>

DS201_Handle sFWDS201_t;

void FWDS201_Init(DS201_Handle *FWDS201)
{

	/* start the Direct handle init of  */
	FWDS201->system_parameter.air_actual_temp = 0,
	FWDS201->system_parameter.air_set_temp = 380,
	FWDS201->system_parameter.air_set_temp_f_display = 716;
	FWDS201->system_parameter.sol_actual_temp = 0,
	FWDS201->system_parameter.sol_set_temp = 380,
	FWDS201->system_parameter.sol_set_temp_f_display = 716;

	FWDS201->system_parameter.air_error_time = 0,
	FWDS201->system_parameter.air_cal_data = 0,
	FWDS201->system_parameter.air_cal_temp_c_display = 0,
	FWDS201->system_parameter.air_cal_temp_f_display = 0,
	FWDS201->air_handle_position = IN_POSSITION,
	FWDS201->airgun_handle_error_state = HANDLE_OK,
	FWDS201->air_work_handle_state = HANDLE_SLEEP;

	FWDS201->system_parameter.set_sleep_time = 0x00;
	/* end the  Direct handle init of*/

	/* start the general init  of  */
	FWDS201->temp_unit = CELSIUS,
	FWDS201->speak_state = SPEAKER_OPEN,
	FWDS201->display_lock_state = UNLOCK,
	FWDS201->sleep_state = SLEEP_OPEN,
	FWDS201->display_air_temp_number = DISPLAY_SET;
	FWDS201->display_sol_temp_number = DISPLAY_SET;
	FWDS201->display_air_number = DISPLAY_SET;
	
	FWDS201->general_parameter.air_ch = 0,
	FWDS201->general_parameter.sol_ch = 0,
	FWDS201->system_parameter.air_last_pwm_out = 0;
	FWDS201->system_parameter.sol_last_pwm_out = 0;
	/* ch set */
	FWDS201->system_parameter.ch1_air_set_temp = 320,
	FWDS201->system_parameter.ch2_air_set_temp = 350,
	FWDS201->system_parameter.ch3_air_set_temp = 380,
	FWDS201->system_parameter.ch1_set_air = 40,
	FWDS201->system_parameter.ch2_set_air = 50,
	FWDS201->system_parameter.ch3_set_air = 60,
	FWDS201->system_parameter.ch1_sol_set_temp = 320,
	FWDS201->system_parameter.ch2_sol_set_temp = 350,
	FWDS201->system_parameter.ch3_sol_set_temp = 380,
	FWDS201->system_parameter.last_sleep_air_data = 0,
	FWDS201->system_parameter.sleep_air_data = 0,

	/* en working set */
		/* end the general init of*/

		FWDS201->reset_flag = 0;
	FWDS201->system_parameter.air_pwm_out = 0x00;
	FWDS201->system_parameter.cpu_temp = 0x00;

	FWDS201->system_parameter.air_data = 0x32;
	FWDS201->system_parameter.last_air_data = 0x00;
	FWDS201->system_parameter.actual_air = 0x00;
	//	FWDS201->general_parameter.set_air_temp_time = SET_SHOW_TIMES;
	FWDS201->general_parameter.set_air_temp_time = 0x00;
	FWDS201->general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
	FWDS201->general_parameter.set_air_time = 0x00;
	FWDS201->general_parameter.save_air_ch_flag = false;
	FWDS201->general_parameter.save_sol_ch_flag = false;
	FWDS201->general_parameter.air_dp_flash_flag = false;
	FWDS201->general_parameter.sol_dp_flash_flag = false;
	FWDS201->set_flag = false;
	FWDS201->set_interface_number = EXIT;
	FWDS201->fan_run_flag = true;
	FWDS201->relay_open_flag = false;
	FWDS201->air_hot_state = FULL_POWER;
	FWDS201->sol_hot_state = FULL_POWER;
	FWDS201->init_flag = false;
}
