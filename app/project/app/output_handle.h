#ifndef _OUTPUT_HANDLE_H
#define _OUTPUT_HANDLE_H
#include "DS201_handle.h"
#define OUTPUT_HANDLE_TIME 5
#define RELAY_OPEN_TIME 60

void output_handle(void);
void air_pwm_control(DS201_Handle *this);
void fan_control(DS201_Handle *this);
void check_sol_over_current(void);
void sol_rpc_control(void);
void sol_sleep_control(void);
void sol_pwm_control(void);
#endif
