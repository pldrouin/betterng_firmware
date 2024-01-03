#ifndef _ADC_
#define _ADC_

#include <avr/interrupt.h>
#include "cmd_common.h"
#include "defines.h"
#include <util/delay.h>

extern volatile int16_t adc_values[ADC_NCHANNELS];
extern uint8_t cur_adc_channel;

static inline int16_t adc_get_value(const uint8_t index)
{
  return adc_values[index];
}

static inline void initadc(void)
{
  ADC_MUX_SELECT_REG = _BV(ADC_REFERENCE_SELECT_BIT_0)|_BV(ADC_REFERENCE_SELECT_BIT_1)|cur_adc_channel;
  _delay_us(10);
  //ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE);
  ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE) | _BV(ADC_PRESCALE_BIT_2) | _BV(ADC_PRESCALE_BIT_1) | _BV(ADC_PRESCALE_BIT_0);
}

#endif
