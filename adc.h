#ifndef _ADC_
#define _ADC_

#include <avr/interrupt.h>
#include "defines.h"
#include <util/delay.h>
#include "cmd_common.h"

void initadc(void);
int16_t adc_getValue(const uint8_t index);

#endif
