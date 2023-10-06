#include "analog_sensor.h"

int16_t analog_sensor_getValue(const uint8_t id)
{
  if(id>=MAX_ANALOG_SENSORS) return ANALOG_SENSOR_INVALID_VALUE;

  return adc_getValue(N_MAX_FANS+id);
}
