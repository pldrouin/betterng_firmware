#include "adc.h"
#include "fan.h"

volatile int16_t adc_values[ADC_NCHANNELS];
uint8_t cur_adc_channel=0;

// Called when ADC conversion completes
ISR(ADC_INTR_FUNC)
{
  adc_values[cur_adc_channel] = ADC_DATA_WORD_REG;

  //Fan control in DC mode. Makes the most sense to have it in sync with ADC
  //readings.
  if(cur_adc_channel < N_MAX_FANS && (fans[cur_adc_channel].mode&(FAN_MODE_MASK|FAN_STARTING_FLAG)) == FAN_VOLTAGE_MODE) {

    if(adc_values[cur_adc_channel] > fans[cur_adc_channel].level) set_fan_pin(DC, cur_adc_channel, true);

    else set_fan_pin(DC, cur_adc_channel, false);
  }
  cur_adc_channel=ADC_MUX_SELECT_REG&(ADC_NCHANNELS-1);
  ADC_MUX_SELECT_REG = _BV(ADC_REFERENCE_SELECT_BIT_0) | ((cur_adc_channel+1)%ADC_NCHANNELS);
}
