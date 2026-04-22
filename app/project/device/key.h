#ifndef _KEY_H
#define _KEY_H

#include <stdio.h>
#include "at32f415.h"
/* input */
#define AIRGUN_CH1_PORT            GPIOC
#define AIRGUN_CH1_PIN             GPIO_PINS_5
#define AIRGUN_READ_CH1            gpio_input_data_bit_read(AIRGUN_CH1_PORT, AIRGUN_CH1_PIN)

#define AIRGUN_CH2_PORT            GPIOB
#define AIRGUN_CH2_PIN             GPIO_PINS_0
#define AIRGUN_READ_CH2            gpio_input_data_bit_read(AIRGUN_CH2_PORT, AIRGUN_CH2_PIN)

#define AIRGUN_CH3_PORT            GPIOB
#define AIRGUN_CH3_PIN             GPIO_PINS_1
#define AIRGUN_READ_CH3            gpio_input_data_bit_read(AIRGUN_CH3_PORT, AIRGUN_CH3_PIN)

#define SOL_CH1_PORT               GPIOF
#define SOL_CH1_PIN                GPIO_PINS_5
#define SOL_READ_CH1               gpio_input_data_bit_read(SOL_CH1_PORT, SOL_CH1_PIN)

#define SOL_CH2_PORT               GPIOA
#define SOL_CH2_PIN                GPIO_PINS_4
#define SOL_READ_CH2               gpio_input_data_bit_read(SOL_CH2_PORT, SOL_CH2_PIN)

#define SOL_CH3_PORT               GPIOA
#define SOL_CH3_PIN                GPIO_PINS_5
#define SOL_READ_CH3               gpio_input_data_bit_read(SOL_CH3_PORT, SOL_CH3_PIN)

#define LONG_PRESS_TIME          5 
#define KEY_CYCLE_TIME           10
#define KEY_NUMBER               6
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    K_RELEASE,
    K_PRESS,
} KEY_VALUE;

typedef enum {
    KS_RELEASE,
	//KS_LONG_RELEASE,
	KS_CHECK,
    KS_PRESS,
	KS_LONG_PRESS,
} KEY_STATUS;

typedef enum {
    KE_PRESS,
    KE_RELEASE,
    KE_LONG_PRESS,
    KE_LONG_RELEASE,
    KE_NONE,
} KEY_EVENT;

typedef struct {
    KEY_STATUS status;
    int count;
	int key_cycle_time;
    KEY_VALUE (*get)(void);
} KEY;

KEY_EVENT key_event_check(KEY *key, int interval);

#ifdef __cplusplus
}
#endif
#endif


