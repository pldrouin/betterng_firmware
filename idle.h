#ifndef _IDLE_
#define _IDLE_

#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "defines.h"

void idle_init(void);

#define set_idle_delay_freq(freq) ({\
    IDLE_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM(freq);\
    IDLE_TIMER_CONTROL_REG = _BV(IDLE_TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING(freq);\
})

#define set_idle_delay_millis(millis) ({\
    IDLE_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM_MILLIS(millis);\
    IDLE_TIMER_CONTROL_REG = _BV(IDLE_TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING_MILLIS(millis);\
})

#define set_idle_delay_micros(micros) ({\
    IDLE_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM_MICROS(micros);\
    IDLE_TIMER_CONTROL_REG = _BV(IDLE_TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING_MICROS(micros);\
})

#endif

#define idle_delay_block_maximum_freq_start(freq) (\
    IDLE_TIMER_REG = 0;\
    set_idle_delay_freq(freq);\
)

#define idle_delay_block_minimum_millis_start(millis) ({\
    IDLE_TIMER_REG = 0;\
    set_idle_delay_millis(millis);\
})

#define idle_delay_block_minimum_micros_start(micros) ({\
    IDLE_TIMER_REG = 0;\
    set_idle_delay_micros(micros);\
})

void idle_delay_block_end(void);

#define idle_delay_millis(millis) ({idle_delay_block_minimum_millis_start(millis); idle_delay_block_end();})

#define idle_delay_micros(micros) ({idle_delay_block_minimum_millis_start(micros); idle_delay_block_end();})
