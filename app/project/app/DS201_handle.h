#ifndef _DS01_HANDLE_H
#define _DS01_HANDLE_H
#include "at32f415.h"
#include "stdbool.h"

#define SOFTWARE_VERSION_BAI 1
#define SOFTWARE_VERSION_SHI 0
#define SOFTWARE_VERSION_GE  5

#define HARDWARE_VERSION_BAI 1
#define HARDWARE_VERSION_SHI 0
#define HARDWARE_VERSION_GE  1
#define MAX_SLEEP_TIME 999
#define MIN_SLEEP_TIME 0

#define RESET_TIME 300

#define SAVE_CH_TIME 300

#define RESET_VALUE 255

#define MAX_SET_AIR_TEMP 500
#define MIN_SET_AIR_TEMP 100
#define MAX_SET_AIR_TEMP_F 932
#define MIN_SET_AIR_TEMP_F 212

#define MAX_SET_SOL_TEMP 450
#define MIN_SET_SOL_TEMP 200
#define MAX_SET_SOL_TEMP_F 842
#define MIN_SET_SOL_TEMP_F 392


#define MAX_CAL_TEMP 100
#define MIN_CAL_TEMP -100
#define MAX_CAL_TEMP_F 212
#define MIN_CAL_TEMP_F -212

#define PID_RANGE 100

#define MAX_SET_AIR 120
#define MIN_SET_AIR 5

#define MAX_ACTUAL_AIR 534
#define MIN_ACTUAL_AIR 100

#define MAX_ACTUAL_TEMP 600
#define MIN_ACTUAL_TEMP 70

#define ERROR_TIME 600

#define MAX_AIR_PWM_OUTPUT 48000
#define MAX_SOL115_PWM_OUTPUT 5000
#define MAX_SOL210_PWM_OUTPUT 9999



#define ACTUAL_TEMP_REFRESH_TIME 200

#define SET_SHOW_TIMES 250

#define SET_TIME 10000;

#define LOCK_RANGE 20

#define SLEEP_TIME 10

#define SLEEP_FAN_DATA 100

#define WAKEN_TIMES 3000

#define POWER_TEMP 50


typedef enum
{
	HANDLE_SLEEP = 0,
	HANDLE_WORKING,
	HANDLE_WAKEN,
	HANDLE_RPC,
} handle_state_e;

typedef enum
{
	HANDLE_OK = 0,
	HANDLE_LOW_TEMP_ERR,
	HANDLE_OVER_TEMP_ERR,
	HANDLE_FAN_ERROR,
	HANDLE_OVER_CURRENT,
	HANDLE_NO_ERR,
} handle_error_state_e;

typedef enum
{
	NOT_IN_POSSITION = 0,
	IN_POSSITION,
} handle_position_e;

typedef enum
{
    SOL_HANDLE_210 = 0,
    SOL_HANDLE_NO,
} sol_handle_state_e;

typedef enum
{
    NOT_IN_RPC = 0,
    IN_RPC,
} sol_handle_rpc_e;


typedef enum
{
	CELSIUS = 0,
	FAHRENHEIT,
} temp_unit_e;

typedef enum
{
	SPEAKER_CLOSE = 0,
	SPEAKER_OPEN,
} speaker_state_e;

typedef enum
{
	UNLOCK = 0,
	LOCK,
} temp_lock_state_e;

typedef enum
{
	SLEEP_OPEN = 0,
	SLEEP_CLOSE,
} sleep_state_e;

typedef enum
{
	FULL_POWER = 0,
	ZERO_POWER,
	PID_POWER,
} hot_state_e;

typedef enum
{
	EXIT = 0,
	AIR_TEMP_CAL,
	SOL_TEMP_CAL,
	TEMP_UNIT,
	SPEAK_STATE,
	DISPLAY_LOCK,
	SLEEP_STATE,
	SET_SLEEP_TIME,
	SOFTWARE_VERSION,
	HARDWARE_VERSION,
	RESET_RUN,
} set_interface_number_e;

typedef enum
{
	DISPLAY_SET = 0,
	DISPLAY_REAL,
	DISPLAY_ERR,
	DISPLAY_SLP,
	DISPLAY_SAVE_CH,
} display_number_e;

typedef struct
{
	/* actual temp */
	float air_actual_temp;
	float air_actual_temp_f_display;
	float air_last_actual_temp;
	float air_last_actual_temp_f_display;
	float sol_actual_temp;
	float sol_actual_temp_f_display;
	float sol_last_actual_temp;
	float sol_last_actual_temp_f_display;
	
	/* set temp */
	float air_set_temp;
	float air_last_set_temp;
	float air_set_temp_f_display;
	float air_last_set_temp_f_display;
	float sol_set_temp;
	float sol_last_set_temp;
	float sol_set_temp_f_display;
	float sol_last_set_temp_f_display;
	
	/* compensation temp */
	float air_cal_data;
	float air_cal_temp_c_display;
	float air_last_cal_temp_c_display;
	float air_cal_temp_f_display;
	float air_last_cal_temp_f_display;
	
	float sol_cal_data;
	float sol_cal_temp_c_display;
	float sol_last_cal_temp_c_display;
	float sol_cal_temp_f_display;
	float sol_last_cal_temp_f_display;
	
	float air_pwm_out;
	float air_last_pwm_out;
	float sol_pwm_out;
	float sol_last_pwm_out;
	
	
	float current_data;
	float cpu_temp;
	float set_sleep_time;
	float last_set_sleep_time;
	float sleep_time_count;
	float waken_time_count;
	uint16_t air_error_time;
	uint16_t sol_error_time;
	
	float ch1_air_set_temp;
	float ch2_air_set_temp;
	float ch3_air_set_temp;
	float ch1_sol_set_temp;
	float ch2_sol_set_temp;
	float ch3_sol_set_temp;

	float ch1_set_air;
	float ch2_set_air;
	float ch3_set_air;
	
	float air_data;
	float last_air_data;

	float actual_air;
	float sleep_air_data;
	float last_sleep_air_data;
	uint16_t sleep_time;
	int fan_icon_show_times;
	uint8_t fan_icon_show_step;
	float temp_linear_data;

	float last_ch1_air_set_temp;
	float last_ch2_air_set_temp;
	float last_ch3_air_set_temp;

} handle_parameter_t;

typedef struct
{
	uint8_t air_ch;
	uint8_t sol_ch;
	uint8_t last_air_ch;
	uint8_t last_sol_ch;
	int set_air_temp_time;
	int set_sol_temp_time;
	int set_air_time;
	bool save_air_ch_flag;
	bool save_sol_ch_flag;
	bool air_dp_flash_flag;
	bool sol_dp_flash_flag;
	bool key_setting_flag;
	bool ec_setting_flag;
} system_state_t;

typedef struct
{
	handle_parameter_t system_parameter;
	system_state_t general_parameter;
	
	handle_state_e air_work_handle_state;
	handle_state_e air_last_work_handle_state;
	handle_state_e sol_work_handle_state;
	handle_state_e sol_last_work_handle_state;
	
	handle_position_e air_handle_position;
	handle_position_e air_last_handle_position;
	handle_position_e sol_handle_position;
	handle_position_e sol_last_handle_position;
	
	handle_error_state_e airgun_handle_error_state;
	handle_error_state_e airgun_last_handle_error_state;
	handle_error_state_e sol_handle_error_state;
	handle_error_state_e sol_last_handle_error_state;

	sol_handle_state_e sol_handle_state;
	sol_handle_rpc_e sol_handle_rpc_state;
	
	temp_unit_e temp_unit;
	temp_unit_e last_temp_unit;

	speaker_state_e speak_state;
	speaker_state_e last_speak_state;

	temp_lock_state_e display_lock_state;
	temp_lock_state_e last_display_lock_state;

	sleep_state_e sleep_state;
	sleep_state_e last_sleep_state;

	display_number_e display_air_temp_number;
	display_number_e display_air_number;
	display_number_e display_sol_temp_number;


	bool set_flag;
	set_interface_number_e set_interface_number;
	bool reset_flag;
	bool init_flag;
	bool fan_run_flag;
	bool relay_open_flag;
	hot_state_e air_hot_state;
	hot_state_e sol_hot_state;

} DS201_Handle;

extern DS201_Handle sFWDS201_t;

void FWDS201_Init(DS201_Handle *FWDS201);

#endif