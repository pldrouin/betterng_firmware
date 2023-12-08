#include "adc.h"
#include "fan.h"

volatile static int16_t adc_values[ADC_NCHANNELS];
static uint8_t cur_adc_channel=0;

void initadc(void)
{
  ADC_MUX_SELECT_REG = _BV(ADC_REFERENCE_SELECT_BIT_0)|cur_adc_channel;
  _delay_us(10);
  //ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE);
  ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE) | _BV(ADC_PRESCALE_BIT_2) | _BV(ADC_PRESCALE_BIT_1) | _BV(ADC_PRESCALE_BIT_0);
}

int16_t adc_getValue(const uint8_t index)
{
  return adc_values[index];
}

// Called when ADC conversion completes
ISR(ADC_INTR_FUNC)
{
  adc_values[cur_adc_channel] = ADC_DATA_WORD_REG;

  //Fan control in DC mode. Makes the most sense to have it in sync with ADC
  //readings.
  if(cur_adc_channel < N_MAX_FANS && (fans[cur_adc_channel].mode&FAN_MODE_MASK) == FAN_VOLTAGE_MODE) {

    if(adc_values[cur_adc_channel] > fans[cur_adc_channel].level) set_fan_pin(DC, cur_adc_channel, true);

    else set_fan_pin(DC, cur_adc_channel, false);
  }
  cur_adc_channel=ADC_MUX_SELECT_REG&(ADC_NCHANNELS-1);
  ADC_MUX_SELECT_REG = _BV(ADC_REFERENCE_SELECT_BIT_0) | ((cur_adc_channel+1)%ADC_NCHANNELS);
}
