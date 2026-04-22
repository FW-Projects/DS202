#include "stdbool.h"
#include "key_handle.h"
#include "beep_handle.h"
#include "DS201_handle.h"

KEY_EVENT key_event[KEY_NUMBER];
static KEY keys[];
static KEY_VALUE get_air_ch1();
static KEY_VALUE get_air_ch2();
static KEY_VALUE get_air_ch3();
static KEY_VALUE get_sol_ch1();
static KEY_VALUE get_sol_ch2();
static KEY_VALUE get_sol_ch3();

static void get_key(void)
{
    static bool set_done = false;
    static uint16_t last_set_vol_value = 0x00;
    static int set_time = 150;

    if (get_air_ch1() == K_RELEASE &&
        get_air_ch2() == K_RELEASE &&
        get_air_ch3() == K_RELEASE &&
		get_sol_ch1() == K_RELEASE &&
        get_sol_ch2() == K_RELEASE &&
        get_sol_ch3() == K_RELEASE)
    {
        set_done = false;
    }

    /* scan key begin */
    for (uint8_t i = 0; i < KEY_NUMBER; i++)
    {
        key_event[i] = key_event_check(&keys[i], LONG_PRESS_TIME);
    }

    /* scan key end */
    if (set_done == false)
    {
        /* get air ch1 */
        if (key_event[0] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.air_set_temp = sFWDS201_t.system_parameter.ch1_air_set_temp;
            sFWDS201_t.system_parameter.air_data = sFWDS201_t.system_parameter.ch1_set_air;
            sFWDS201_t.general_parameter.air_ch = 1;
            sFWDS201_t.display_air_temp_number = DISPLAY_SET;
            sFWDS201_t.display_air_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
            sFWDS201_t.system_parameter.last_air_data = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
        /* get air ch2 */
        else if (key_event[1] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.air_set_temp = sFWDS201_t.system_parameter.ch2_air_set_temp;
            sFWDS201_t.system_parameter.air_data = sFWDS201_t.system_parameter.ch2_set_air;
            sFWDS201_t.general_parameter.air_ch = 2;
            sFWDS201_t.display_air_temp_number = DISPLAY_SET;
            sFWDS201_t.display_air_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
            sFWDS201_t.system_parameter.last_air_data = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
        /* get air ch3 */
        else if (key_event[2] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.air_set_temp = sFWDS201_t.system_parameter.ch3_air_set_temp;
            sFWDS201_t.system_parameter.air_data = sFWDS201_t.system_parameter.ch3_set_air;
            sFWDS201_t.general_parameter.air_ch = 3;
            sFWDS201_t.display_air_temp_number = DISPLAY_SET;
            sFWDS201_t.display_air_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES;
            sFWDS201_t.system_parameter.last_air_data = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.air_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
		
		/* get sol ch1 */
        if (key_event[3] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.sol_set_temp = sFWDS201_t.system_parameter.ch1_sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 1;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.system_parameter.sol_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
        /* get sol ch2 */
        else if (key_event[4] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.sol_set_temp = sFWDS201_t.system_parameter.ch2_sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 2;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.system_parameter.sol_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
        /* get sol ch3 */
        else if (key_event[5] == KE_PRESS)
        {
            sFWDS201_t.system_parameter.sol_set_temp = sFWDS201_t.system_parameter.ch3_sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 3;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SET;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES;
			sFWDS201_t.system_parameter.sol_last_set_temp = 0x00;
			sFWDS201_t.system_parameter.sol_last_set_temp_f_display = 0x00;
            sbeep.cmd = BEEP_SHORT;
            set_done = TRUE;
        }
		
        /* get long air ch1 */
        else if (key_event[0] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch1_air_set_temp = sFWDS201_t.system_parameter.air_set_temp;
            sFWDS201_t.system_parameter.ch1_set_air = sFWDS201_t.system_parameter.air_data;
            sFWDS201_t.general_parameter.air_ch = 1;
            sFWDS201_t.display_air_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.display_air_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_air_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
        /* get long air ch2 */
        else if (key_event[1] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch2_air_set_temp = sFWDS201_t.system_parameter.air_set_temp;
            sFWDS201_t.system_parameter.ch2_set_air = sFWDS201_t.system_parameter.air_data;
            sFWDS201_t.general_parameter.air_ch = 2;
            sFWDS201_t.display_air_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.display_air_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_air_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
        /* get long air ch3 */
        else if (key_event[2] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch3_air_set_temp = sFWDS201_t.system_parameter.air_set_temp;
            sFWDS201_t.system_parameter.ch3_set_air = sFWDS201_t.system_parameter.air_data;
            sFWDS201_t.general_parameter.air_ch = 3;
            sFWDS201_t.display_air_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.display_air_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_air_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.set_air_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_air_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
		
		/* get long sol ch1 */
        else if (key_event[3] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch1_sol_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 1;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_sol_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
        /* get long sol ch2 */
        else if (key_event[4] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch2_sol_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 2;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_sol_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
        /* get long sol ch3 */
        else if (key_event[5] == KE_LONG_PRESS)
        {
            sFWDS201_t.system_parameter.ch3_sol_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
            sFWDS201_t.general_parameter.sol_ch = 3;
            sFWDS201_t.display_sol_temp_number = DISPLAY_SAVE_CH;
            sFWDS201_t.general_parameter.set_sol_temp_time = SET_SHOW_TIMES * 2;
            sFWDS201_t.general_parameter.save_sol_ch_flag = true;
            sbeep.cmd = BEEP_LONG;
            set_done = TRUE;
        }
    }
}

void key_handle(void)
{
    get_key();
	
	if (key_event[0] != KE_NONE ||
		key_event[1] != KE_NONE ||
		key_event[2] != KE_NONE ||
		key_event[3] != KE_NONE ||
		key_event[4] != KE_NONE ||
		key_event[5] != KE_NONE)
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

static KEY_VALUE get_air_ch1()
{
    if (AIRGUN_READ_CH1 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_air_ch2()
{
    if (AIRGUN_READ_CH2 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_air_ch3()
{
    if (AIRGUN_READ_CH3 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_sol_ch1()
{
    if (SOL_READ_CH1 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_sol_ch2()
{
    if (SOL_READ_CH2 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY_VALUE get_sol_ch3()
{
    if (SOL_READ_CH3 == 0)
    {
        return K_PRESS;
    }
    else
    {
        return K_RELEASE;
    }
}

static KEY keys[] =
    {
        {KS_RELEASE, 0, KEY_CYCLE_TIME, get_air_ch1},
        {KS_RELEASE, 0, KEY_CYCLE_TIME, get_air_ch2},
        {KS_RELEASE, 0, KEY_CYCLE_TIME, get_air_ch3},
		{KS_RELEASE, 0, KEY_CYCLE_TIME, get_sol_ch1},
        {KS_RELEASE, 0, KEY_CYCLE_TIME, get_sol_ch2},
        {KS_RELEASE, 0, KEY_CYCLE_TIME, get_sol_ch3},
};

KEY_EVENT key_event[KEY_NUMBER] =
    {
        KE_PRESS,
        KE_RELEASE,
        KE_LONG_PRESS,
        KE_LONG_RELEASE,
        KE_NONE,
};
