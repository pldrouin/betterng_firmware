#include "analog_sensor.h"

static uint8_t ids[MAX_ANALOG_SENSORS];
static uint8_t nsensors=0;

int16_t analog_sensor_getValue(const uint8_t index)
{
  if(index>=nsensors) return ANALOG_SENSOR_INVALID_VALUE;

  return adc_getValue(4+ids[index]);
}
