#ifndef _TIMER_
#define _TIMER_

#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "defines.h"

extern volatile bool timer_itr_set;

static inline void timer_init(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sbi(TIMER_INTR_MASK_REG, TIMER_ENABLE_COMPARE_MATCH_INTR);
}

#define set_timer_freq(freq) ({\
    TIMER_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM(freq);\
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING(freq);\
})

#define set_timer_millis(millis) ({\
    TIMER_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM_MILLIS(millis);\
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING_MILLIS(millis);\
})

#define set_timer_micros(micros) ({\
    TIMER_OUTPUT_COMPARE_REG = TIMER_16BIT_ALARM_MICROS(micros);\
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_16BIT_PRESCALER_SETTING_MICROS(micros);\
})

#define timer_maximum_freq_start(freq) (\
    TIMER_REG = 0;\
    set_timer_freq(freq);\
)

#define timer_minimum_millis_start(millis) ({\
    TIMER_REG = 0;\
    set_timer_millis(millis);\
})

#define timer_minimum_micros_start(micros) ({\
    TIMER_REG = 0;\
    set_timer_micros(micros);\
})

static inline bool timer_delay_verify(void)
{
  if(timer_itr_set) {
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    timer_itr_set = false;
    return true;
  }
  return false;
}

static inline bool timer_delay_end(void)
{
  if(timer_itr_set) {
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    timer_itr_set = false;
    return true;
  }
  TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
  timer_itr_set = false;
  return false;
}

void idle_timer_delay_apply(void);

#define idle_timer_delay_millis(millis) ({timer_minimum_millis_start(millis); idle_timer_delay_apply();})

#define idle_timer_delay_micros(micros) ({timer_minimum_millis_start(micros); idle_timer_delay_apply();})
#endif

