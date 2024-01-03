#ifndef _ANALOG_SENSOR_
#define _ANALOG_SENSOR_

#include "adc.h"

#define set_analog_pin(MODE, ID, VALUE) (ANALOG_ ## MODE ## _PORT = (ANALOG_ ## MODE ## _PORT & ~_BV(ANALOG_ ## MODE ## _FIRST_NO+ID)) | ((VALUE!=0)<<(ANALOG_ ## MODE ## _FIRST_NO+ID)))
#define toggle_analog_pin(MODE, ID) (ANALOG_ ## MODE ## _PORT ^= _BV(ANALOG_ ## MODE ## _FIRST_NO+ID))
#define set_analog_pin_as_output(MODE, ID) (sbi(ANALOG_ ## MODE ## _DDR, ANALOG_ ## MODE ## _FIRST_NO+ID))
#define set_analog_pin_as_input(MODE, ID) (cbi(ANALOG_ ## MODE ## _DDR, ANALOG_ ## MODE ## _FIRST_NO+ID))
#define read_analog_pin(MODE, ID) (bit_is_set(ANALOG_ ## MODE ## _PIN, ANALOG_ ## MODE ## _FIRST_NO+ID)!=0)

#define ANALOG_SENSOR_INVALID_VALUE       32767L        // Just an arbritary value outside of the sensor limits
#define MAX_ANALOG_SENSORS	4U

static inline int16_t analog_sensor_get_ADC_value(const uint8_t id)
{
  if(id>=MAX_ANALOG_SENSORS) return ANALOG_SENSOR_INVALID_VALUE;

  return adc_get_value(N_MAX_FANS+id);
}

static inline void initanalog(void)
{
  uint8_t id;

  for(id=0U; id<N_MAX_ANALOG; ++id) {
    set_analog_pin_as_input(ADC, id);
    set_analog_pin(ADC, id, false);
  }
}
#endif
