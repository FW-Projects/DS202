#ifndef _BEEP_H
#define _BEEP_H

#include <stdio.h>
#include "at32f415.h"
/* output */
#define BEEP_PORT             GPIOC
#define BEEP_PIN              GPIO_PINS_3

#define BEEP_LONG_TIME        100
#define BEEP_SHORT_TIME       5
typedef enum
{
    BEEP_IDLE,
    BEEP_STEP1,
	BEEP_STEP2,
} beep_run_e;

typedef enum
{
    BEEP_SHORT,
	BEEP_LONG,
    BEEP_STOP,
} beep_ctr_e;


typedef struct
{
    char *name;
    beep_run_e status;
    beep_ctr_e cmd;	
    void (*on)(void);      
    void (*off)(void);     
} beep_t;

void BeepProc(beep_t *beep);
#endif
