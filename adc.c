#include "adc.h"

static uint16_t adc_values[ADC_NCHANNELS];

void initadc(void)
{
  ADC_MUX_SELECT_REG = _BV(ADC_REFERENCE_SELECT_BIT_0);
  //ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE);
  ADC_CTRL_STATUS_REG = _BV(ADC_START_CONVERSION) | _BV(ENABLE_ADC) | _BV(ENABLE_ADC_AUTO_TRIGGER) | _BV(ADC_INTERRUPT_ENABLE) | _BV(ADC_PRESCALE_BIT_2) | _BV(ADC_PRESCALE_BIT_1) | _BV(ADC_PRESCALE_BIT_0);
}

uint16_t adc_getValue(const uint8_t index)
{
  return adc_values[index];
}

// Called when ADC conversion completes
ISR(ADC_INTR_FUNC)
{
  uint8_t adc_channel = ADC_MUX_SELECT_REG & (ADC_NCHANNELS-1);
  ADC_MUX_SELECT_REG = (ADC_MUX_SELECT_REG & ~(ADC_NCHANNELS-1)) | (adc_channel+1)%ADC_NCHANNELS; 
  adc_values[adc_channel] = ADC_DATA_LOW_REG;
  adc_values[adc_channel]|=(((uint16_t)ADC_DATA_HIGH_REG)<<8);
}
