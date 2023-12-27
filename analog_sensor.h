#ifndef _ANALOG_SENSOR_
#define _ANALOG_SENSOR_

#include "adc.h"

#define ANALOG_SENSOR_INVALID_VALUE       32767L        // Just an arbritary value outside of the sensor limits
#define MAX_ANALOG_SENSORS	4U

static inline int16_t analog_sensor_getValue(const uint8_t id)
{
  if(id>=MAX_ANALOG_SENSORS) return ANALOG_SENSOR_INVALID_VALUE;

  return adc_getValue(N_MAX_FANS+id);
}
#endif
