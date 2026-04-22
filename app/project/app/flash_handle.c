
#include "flash_handle.h"
#include "led_handle.h"
#include "DS201_handle.h"

// ===================== 新增：Flash 忙标志 =====================
uint8_t flash_busy_flag = 0;

void get_data_from_b(void);
void get_data_from_a(void);
void get_reset_data(void);
void check_data_all(void);

uint16_t data_check(uint32_t address)
{
    static uint16_t data;
    data = flash_wred_halfword(address);

    if (data == 0xffff)
    {
        return FALSE;
    }
    else
    {
        return data;
    }
}

uint16_t data_check_len(uint32_t address, uint8_t len)
{
    volatile uint16_t data;
    uint8_t i;

    for (i = 0; i < len; i += DATA_SIZE)
    {
        data = flash_wred_halfword(address + i);

        if (data == 0xFFFF)
        {
            break;
        }
    }

    return data;
}

void FlashProc(void)
{
    static flash_handle_t sflash;
    static uint16_t air_last_set_temp;
    static uint16_t last_set_wind;
    static uint16_t last_ch1_air_set_temp;
    static uint16_t last_ch1_set_wind;
    static uint16_t last_ch2_air_set_temp; // 修复：独立变量
    static uint16_t last_ch2_set_wind; // 修复：独立变量
    static uint16_t last_ch3_air_set_temp; // 修复：独立变量
    static uint16_t last_ch3_set_wind; // 修复：独立变量
	static uint16_t sol_last_set_temp;
	static uint16_t last_ch1_sol_set_temp;
	static uint16_t last_ch2_sol_set_temp;
	static uint16_t last_ch3_sol_set_temp;
    static uint8_t last_temp_unit_mode;
    static uint8_t last_speak_mode;
    static uint16_t last_air_cal_data;
	static uint16_t last_sol_cal_data;
    static uint8_t last_display_lock_mode;
    static uint8_t last_air_sleep_mode;
	static uint8_t last_sol_sleep_time;
    static uint16_t flash_version = 0;
    static uint8_t flash_count = 0;
    static uint16_t a_ver;
    static uint16_t b_ver;

    // ===================== 防抖延时 =====================
    static uint16_t data_change_delay = 0;
    static uint8_t need_save_flash = 0;

    switch (sflash.state)
    {
    case FLASH_START:
        if (sFWDS201_t.init_flag == false)
        {
            if (data_check_len(A_SET_AIR_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF && data_check_len(B_SET_AIR_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                a_ver = flash_wred_halfword(A_FLASH_VERSION_ADDRESS);
                b_ver = flash_wred_halfword(B_FLASH_VERSION_ADDRESS);

                if (a_ver > b_ver)
                {
                    get_data_from_a();
                }
                else
                {
                    get_data_from_b();
                }
            }
            else if (data_check_len(A_SET_AIR_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                get_data_from_a();
            }
            else if (data_check_len(B_SET_AIR_TEMP_ADDRESS, FLASH_MENBER) != 0xFFFF)
            {
                get_data_from_b();
            }
            else
            {
                get_reset_data();
            }

            check_data_all();
            sFWDS201_t.init_flag = TRUE;

            // 初始化上次值
            air_last_set_temp = sFWDS201_t.system_parameter.air_set_temp;
			sol_last_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
            last_set_wind = sFWDS201_t.system_parameter.air_data;
            last_ch1_air_set_temp = sFWDS201_t.system_parameter.ch1_air_set_temp;
			last_ch1_sol_set_temp = sFWDS201_t.system_parameter.ch1_sol_set_temp;
            last_ch1_set_wind = sFWDS201_t.system_parameter.ch1_set_air;
            last_ch2_air_set_temp = sFWDS201_t.system_parameter.ch2_air_set_temp;
			last_ch2_sol_set_temp = sFWDS201_t.system_parameter.ch2_sol_set_temp;
            last_ch2_set_wind = sFWDS201_t.system_parameter.ch2_set_air;
            last_ch3_air_set_temp = sFWDS201_t.system_parameter.ch3_air_set_temp;
			last_ch3_sol_set_temp = sFWDS201_t.system_parameter.ch3_sol_set_temp;
            last_ch3_set_wind = sFWDS201_t.system_parameter.ch3_set_air;
            last_temp_unit_mode = sFWDS201_t.temp_unit;
            last_speak_mode = sFWDS201_t.speak_state;
            last_air_cal_data = sFWDS201_t.system_parameter.air_cal_data;
			last_sol_cal_data = sFWDS201_t.system_parameter.sol_cal_data;
            last_display_lock_mode = sFWDS201_t.display_lock_state;
            last_air_sleep_mode = sFWDS201_t.sleep_state;
			last_sol_sleep_time = sFWDS201_t.system_parameter.set_sleep_time;
        }
        else
        {
            sflash.state = FLASH_HANDLE_DATA;
        }
        break;

    case FLASH_HANDLE_DATA:
        // ===================== 检测数据是否变化 =====================
        if (air_last_set_temp != sFWDS201_t.system_parameter.air_set_temp ||
			sol_last_set_temp != sFWDS201_t.system_parameter.sol_set_temp ||
            last_set_wind != sFWDS201_t.system_parameter.air_data ||
            last_ch1_air_set_temp != sFWDS201_t.system_parameter.ch1_air_set_temp ||
			last_ch1_sol_set_temp != sFWDS201_t.system_parameter.ch1_sol_set_temp ||
            last_ch1_set_wind != sFWDS201_t.system_parameter.ch1_set_air ||
            last_ch2_air_set_temp != sFWDS201_t.system_parameter.ch2_air_set_temp ||
			last_ch2_sol_set_temp != sFWDS201_t.system_parameter.ch2_sol_set_temp ||
            last_ch2_set_wind != sFWDS201_t.system_parameter.ch2_set_air ||
            last_ch3_air_set_temp != sFWDS201_t.system_parameter.ch3_air_set_temp ||
			last_ch3_sol_set_temp != sFWDS201_t.system_parameter.ch3_sol_set_temp ||
            last_ch3_set_wind != sFWDS201_t.system_parameter.ch3_set_air ||
            last_temp_unit_mode != sFWDS201_t.temp_unit ||
            last_speak_mode != sFWDS201_t.speak_state ||
            last_air_cal_data != sFWDS201_t.system_parameter.air_cal_data ||
			last_sol_cal_data != sFWDS201_t.system_parameter.sol_cal_data ||
            last_display_lock_mode != sFWDS201_t.display_lock_state ||
            last_air_sleep_mode != sFWDS201_t.sleep_state ||
			last_sol_sleep_time != sFWDS201_t.system_parameter.set_sleep_time)
        {
            data_change_delay++;
            if (data_change_delay >= 20) // 1秒防抖
            {
                need_save_flash = 1;
                data_change_delay = 0;
            }
        }
        else
        {
            data_change_delay = 0;
        }

        // ===================== 需要保存，进入擦写流程 =====================
        if (need_save_flash == 1)
        {
            need_save_flash = 0;
            flash_busy_flag = 1; // 【关键】标记Flash忙，数码管暂停刷新
            flash_unlock();

            if (flash_count % 2 != FALSE)
            {
                flash_sector_erase(A_SET_AIR_TEMP_ADDRESS);
            }
            else
            {
                flash_sector_erase(B_SET_AIR_TEMP_ADDRESS);
            }
            sflash.state = FLASH_GENERAL_DATA;
        }
        break;

    case FLASH_GENERAL_DATA:
        if (flash_count % 2 != FALSE)
        {
            flash_halfword_program(A_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.air_set_temp);
            flash_halfword_program(A_SET_AIR_ADDRESS, sFWDS201_t.system_parameter.air_data);
            flash_halfword_program(A_CH1_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch1_air_set_temp);
            flash_halfword_program(A_CH1_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch1_set_air);
            flash_halfword_program(A_CH2_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch2_air_set_temp);
            flash_halfword_program(A_CH2_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch2_set_air);
            flash_halfword_program(A_CH3_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch3_air_set_temp);
            flash_halfword_program(A_CH3_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch3_set_air);
			flash_halfword_program(A_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.sol_set_temp);
			flash_halfword_program(A_CH1_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch1_sol_set_temp);
			flash_halfword_program(A_CH2_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch2_sol_set_temp);
			flash_halfword_program(A_CH3_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch3_sol_set_temp);
            flash_halfword_program(A_TEMP_UNIT_ADDRESS, sFWDS201_t.temp_unit);
            flash_halfword_program(A_SPEAK_MODE_ADDRESS, sFWDS201_t.speak_state);
            flash_halfword_program(A_AIR_CAL_DATA_ADDRESS, sFWDS201_t.system_parameter.air_cal_data);
			flash_halfword_program(A_SOL_CAL_DATA_ADDRESS, sFWDS201_t.system_parameter.sol_cal_data);
            flash_halfword_program(A_DISPLAY_LOCK_MODE_ADDRESS, sFWDS201_t.display_lock_state);
            flash_halfword_program(A_AIR_SLEEP_MODE_ADDRESS, sFWDS201_t.sleep_state);
			flash_halfword_program(A_SOL_SLEEP_TIME_ADDRESS, sFWDS201_t.system_parameter.set_sleep_time);
        }
        else
        {
            flash_halfword_program(B_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.air_set_temp);
            flash_halfword_program(B_SET_AIR_ADDRESS, sFWDS201_t.system_parameter.air_data);
            flash_halfword_program(B_CH1_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch1_air_set_temp);
            flash_halfword_program(B_CH1_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch1_set_air);
            flash_halfword_program(B_CH2_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch2_air_set_temp);
            flash_halfword_program(B_CH2_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch2_set_air);
            flash_halfword_program(B_CH3_SET_AIR_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch3_air_set_temp);
            flash_halfword_program(B_CH3_SET_WIND_ADDRESS, sFWDS201_t.system_parameter.ch3_set_air);
			flash_halfword_program(B_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.sol_set_temp);
			flash_halfword_program(B_CH1_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch1_sol_set_temp);
			flash_halfword_program(B_CH2_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch2_sol_set_temp);
			flash_halfword_program(B_CH3_SET_SOL_TEMP_ADDRESS, sFWDS201_t.system_parameter.ch3_sol_set_temp);
            flash_halfword_program(B_TEMP_UNIT_ADDRESS, sFWDS201_t.temp_unit);
            flash_halfword_program(B_SPEAK_MODE_ADDRESS, sFWDS201_t.speak_state);
            flash_halfword_program(B_AIR_CAL_DATA_ADDRESS, sFWDS201_t.system_parameter.air_cal_data);
			flash_halfword_program(B_SOL_CAL_DATA_ADDRESS, sFWDS201_t.system_parameter.sol_cal_data);
            flash_halfword_program(B_DISPLAY_LOCK_MODE_ADDRESS, sFWDS201_t.display_lock_state);
            flash_halfword_program(B_AIR_SLEEP_MODE_ADDRESS, sFWDS201_t.sleep_state);
			flash_halfword_program(B_SOL_SLEEP_TIME_ADDRESS, sFWDS201_t.system_parameter.set_sleep_time);
        }

        // 更新缓存
        air_last_set_temp = sFWDS201_t.system_parameter.air_set_temp;
		sol_last_set_temp = sFWDS201_t.system_parameter.sol_set_temp;
		last_set_wind = sFWDS201_t.system_parameter.air_data;
		last_ch1_air_set_temp = sFWDS201_t.system_parameter.ch1_air_set_temp;
		last_ch1_sol_set_temp = sFWDS201_t.system_parameter.ch1_sol_set_temp;
		last_ch1_set_wind = sFWDS201_t.system_parameter.ch1_set_air;
		last_ch2_air_set_temp = sFWDS201_t.system_parameter.ch2_air_set_temp;
		last_ch2_sol_set_temp = sFWDS201_t.system_parameter.ch2_sol_set_temp;
		last_ch2_set_wind = sFWDS201_t.system_parameter.ch2_set_air;
		last_ch3_air_set_temp = sFWDS201_t.system_parameter.ch3_air_set_temp;
		last_ch3_sol_set_temp = sFWDS201_t.system_parameter.ch3_sol_set_temp;
		last_ch3_set_wind = sFWDS201_t.system_parameter.ch3_set_air;
		last_temp_unit_mode = sFWDS201_t.temp_unit;
		last_speak_mode = sFWDS201_t.speak_state;
		last_air_cal_data = sFWDS201_t.system_parameter.air_cal_data;
		last_sol_cal_data = sFWDS201_t.system_parameter.sol_cal_data;
		last_display_lock_mode = sFWDS201_t.display_lock_state;
		last_air_sleep_mode = sFWDS201_t.sleep_state;
		last_sol_sleep_time = sFWDS201_t.system_parameter.set_sleep_time;
        sflash.state = FLASH_FINSH;
        break;

    case FLASH_FINSH:
        if (flash_count % 2 != FALSE)
        {
            flash_halfword_program(A_FLASH_VERSION_ADDRESS, flash_version);
        }
        else
        {
            flash_halfword_program(B_FLASH_VERSION_ADDRESS, flash_version);
        }

        flash_lock();
        flash_version++;
        flash_count++;
        flash_busy_flag = 0; // 【关键】解除忙标志
        sflash.state = FLASH_HANDLE_DATA;
        break;
    }
}

void get_data_from_a(void)
{
    sFWDS201_t.system_parameter.air_set_temp = flash_wred_halfword(A_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.air_data = flash_wred_halfword(A_SET_AIR_ADDRESS);
    sFWDS201_t.system_parameter.ch1_air_set_temp = flash_wred_halfword(A_CH1_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch1_set_air = flash_wred_halfword(A_CH1_SET_WIND_ADDRESS);
    sFWDS201_t.system_parameter.ch2_air_set_temp = flash_wred_halfword(A_CH2_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch2_set_air = flash_wred_halfword(A_CH2_SET_WIND_ADDRESS);
    sFWDS201_t.system_parameter.ch3_air_set_temp = flash_wred_halfword(A_CH3_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch3_set_air = flash_wred_halfword(A_CH3_SET_WIND_ADDRESS);
	sFWDS201_t.system_parameter.sol_set_temp = flash_wred_halfword(A_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch1_sol_set_temp = flash_wred_halfword(A_CH1_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch2_sol_set_temp = flash_wred_halfword(A_CH2_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch3_sol_set_temp = flash_wred_halfword(A_CH3_SET_SOL_TEMP_ADDRESS);
    sFWDS201_t.temp_unit = flash_wred_halfword(A_TEMP_UNIT_ADDRESS);
    sFWDS201_t.speak_state = flash_wred_halfword(A_SPEAK_MODE_ADDRESS);
    sFWDS201_t.system_parameter.air_cal_data = flash_wred_halfword(A_AIR_CAL_DATA_ADDRESS);
	sFWDS201_t.system_parameter.sol_cal_data = flash_wred_halfword(A_SOL_CAL_DATA_ADDRESS);
    sFWDS201_t.display_lock_state = flash_wred_halfword(A_DISPLAY_LOCK_MODE_ADDRESS);
    sFWDS201_t.sleep_state = flash_wred_halfword(A_AIR_SLEEP_MODE_ADDRESS);
	sFWDS201_t.system_parameter.set_sleep_time = flash_wred_halfword(A_SOL_SLEEP_TIME_ADDRESS);
}

void get_data_from_b(void)
{
    sFWDS201_t.system_parameter.air_set_temp = flash_wred_halfword(B_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.air_data = flash_wred_halfword(B_SET_AIR_ADDRESS);
    sFWDS201_t.system_parameter.ch1_air_set_temp = flash_wred_halfword(B_CH1_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch1_set_air = flash_wred_halfword(B_CH1_SET_WIND_ADDRESS);
    sFWDS201_t.system_parameter.ch2_air_set_temp = flash_wred_halfword(B_CH2_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch2_set_air = flash_wred_halfword(B_CH2_SET_WIND_ADDRESS);
    sFWDS201_t.system_parameter.ch3_air_set_temp = flash_wred_halfword(B_CH3_SET_AIR_TEMP_ADDRESS);
    sFWDS201_t.system_parameter.ch3_set_air = flash_wred_halfword(B_CH3_SET_WIND_ADDRESS);
	sFWDS201_t.system_parameter.sol_set_temp = flash_wred_halfword(B_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch1_sol_set_temp = flash_wred_halfword(B_CH1_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch2_sol_set_temp = flash_wred_halfword(B_CH2_SET_SOL_TEMP_ADDRESS);
	sFWDS201_t.system_parameter.ch3_sol_set_temp = flash_wred_halfword(B_CH3_SET_SOL_TEMP_ADDRESS);
    sFWDS201_t.temp_unit = flash_wred_halfword(B_TEMP_UNIT_ADDRESS);
    sFWDS201_t.speak_state = flash_wred_halfword(B_SPEAK_MODE_ADDRESS);
    sFWDS201_t.system_parameter.air_cal_data = flash_wred_halfword(B_AIR_CAL_DATA_ADDRESS);
	sFWDS201_t.system_parameter.sol_cal_data = flash_wred_halfword(B_SOL_CAL_DATA_ADDRESS);
    sFWDS201_t.display_lock_state = flash_wred_halfword(B_DISPLAY_LOCK_MODE_ADDRESS);
    sFWDS201_t.sleep_state = flash_wred_halfword(B_AIR_SLEEP_MODE_ADDRESS);
	sFWDS201_t.system_parameter.set_sleep_time = flash_wred_halfword(B_SOL_SLEEP_TIME_ADDRESS);
}

void get_reset_data(void)
{
    sFWDS201_t.system_parameter.air_set_temp = 380;
	sFWDS201_t.system_parameter.sol_set_temp = 380;
    sFWDS201_t.system_parameter.air_data = 80;
    sFWDS201_t.system_parameter.ch1_air_set_temp = 320;
	sFWDS201_t.system_parameter.ch1_sol_set_temp = 320;
    sFWDS201_t.system_parameter.ch1_set_air = 50;
    sFWDS201_t.system_parameter.ch2_air_set_temp = 350;
	sFWDS201_t.system_parameter.ch2_sol_set_temp = 350;
    sFWDS201_t.system_parameter.ch2_set_air = 60;
    sFWDS201_t.system_parameter.ch3_air_set_temp = 380;
	sFWDS201_t.system_parameter.ch3_sol_set_temp = 380;
    sFWDS201_t.system_parameter.ch3_set_air = 80;
    sFWDS201_t.temp_unit = CELSIUS;
    sFWDS201_t.speak_state = SPEAKER_OPEN;
    sFWDS201_t.system_parameter.air_cal_data = 0;
	sFWDS201_t.system_parameter.sol_cal_data = 0;
    sFWDS201_t.display_lock_state = UNLOCK;
    sFWDS201_t.sleep_state = SLEEP_OPEN;
	sFWDS201_t.system_parameter.set_sleep_time = 0x00;
}

void check_data_all(void)
{
    if (sFWDS201_t.system_parameter.air_set_temp > MAX_SET_AIR_TEMP || sFWDS201_t.system_parameter.air_set_temp < MIN_SET_AIR_TEMP)
        sFWDS201_t.system_parameter.air_set_temp = 380;
	
	 if (sFWDS201_t.system_parameter.sol_set_temp > MAX_SET_SOL_TEMP || sFWDS201_t.system_parameter.sol_set_temp < MIN_SET_SOL_TEMP)
        sFWDS201_t.system_parameter.sol_set_temp = 380;

    if (sFWDS201_t.system_parameter.air_data > MAX_SET_AIR || sFWDS201_t.system_parameter.air_data < MIN_SET_AIR)
        sFWDS201_t.system_parameter.air_data = 80;

    if (sFWDS201_t.system_parameter.ch1_air_set_temp > MAX_SET_AIR_TEMP || sFWDS201_t.system_parameter.ch1_air_set_temp < MIN_SET_AIR_TEMP)
        sFWDS201_t.system_parameter.ch1_air_set_temp = 320;
    if (sFWDS201_t.system_parameter.ch2_air_set_temp > MAX_SET_AIR_TEMP || sFWDS201_t.system_parameter.ch2_air_set_temp < MIN_SET_AIR_TEMP)
        sFWDS201_t.system_parameter.ch2_air_set_temp = 350;
    if (sFWDS201_t.system_parameter.ch3_air_set_temp > MAX_SET_AIR_TEMP || sFWDS201_t.system_parameter.ch3_air_set_temp < MIN_SET_AIR_TEMP)
        sFWDS201_t.system_parameter.ch3_air_set_temp = 380;
	
	 if (sFWDS201_t.system_parameter.ch1_sol_set_temp > MAX_SET_SOL_TEMP || sFWDS201_t.system_parameter.ch1_sol_set_temp < MIN_SET_SOL_TEMP)
        sFWDS201_t.system_parameter.ch1_sol_set_temp = 320;
    if (sFWDS201_t.system_parameter.ch2_sol_set_temp > MAX_SET_SOL_TEMP || sFWDS201_t.system_parameter.ch2_sol_set_temp < MIN_SET_SOL_TEMP)
        sFWDS201_t.system_parameter.ch2_sol_set_temp = 350;
    if (sFWDS201_t.system_parameter.ch3_sol_set_temp > MAX_SET_SOL_TEMP || sFWDS201_t.system_parameter.ch3_sol_set_temp < MIN_SET_SOL_TEMP)
        sFWDS201_t.system_parameter.ch3_sol_set_temp = 380;

    if (sFWDS201_t.system_parameter.ch1_set_air > MAX_SET_AIR || sFWDS201_t.system_parameter.ch1_set_air < MIN_SET_AIR)
        sFWDS201_t.system_parameter.ch1_set_air = 50;
    if (sFWDS201_t.system_parameter.ch2_set_air > MAX_SET_AIR || sFWDS201_t.system_parameter.ch2_set_air < MIN_SET_AIR)
        sFWDS201_t.system_parameter.ch2_set_air = 60;
    if (sFWDS201_t.system_parameter.ch3_set_air > MAX_SET_AIR || sFWDS201_t.system_parameter.ch3_set_air < MIN_SET_AIR)
        sFWDS201_t.system_parameter.ch3_set_air = 80;

    if (sFWDS201_t.temp_unit != CELSIUS && sFWDS201_t.temp_unit != FAHRENHEIT)
        sFWDS201_t.temp_unit = CELSIUS;

    if (sFWDS201_t.speak_state != SPEAKER_CLOSE && sFWDS201_t.speak_state != SPEAKER_OPEN)
        sFWDS201_t.speak_state = SPEAKER_OPEN;

    if (sFWDS201_t.system_parameter.air_cal_data > MAX_CAL_TEMP || sFWDS201_t.system_parameter.air_cal_data < MIN_CAL_TEMP)
        sFWDS201_t.system_parameter.air_cal_data = 0;
	
	if (sFWDS201_t.system_parameter.sol_cal_data > MAX_CAL_TEMP || sFWDS201_t.system_parameter.sol_cal_data < MIN_CAL_TEMP)
        sFWDS201_t.system_parameter.sol_cal_data = 0;

    if (sFWDS201_t.display_lock_state != LOCK && sFWDS201_t.display_lock_state != UNLOCK)
        sFWDS201_t.display_lock_state = UNLOCK;

    if (sFWDS201_t.sleep_state != SLEEP_CLOSE && sFWDS201_t.sleep_state != SLEEP_OPEN)
        sFWDS201_t.sleep_state = SLEEP_OPEN;
	
	if(sFWDS201_t.system_parameter.set_sleep_time > 999 || sFWDS201_t.system_parameter.set_sleep_time < 0)
		sFWDS201_t.system_parameter.set_sleep_time = 0x00;

}