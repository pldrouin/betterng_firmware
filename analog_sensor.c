#include "analog_sensor.h"

static uint8_t ids[MAX_ANALOG_SENSORS];
static uint8_t nsensors=0;

int8_t add_analog_sensor(const uint8_t id)
{
  if(nsensors==MAX_ANALOG_SENSORS || id>=MAX_ANALOG_SENSORS) return -1;

  uint8_t i;

  for(i=0; i<nsensors; ++i)

    if(ids[i]==id) return -1;

  ids[nsensors]=id;
  return nsensors++;
}

int8_t del_analog_sensor(const uint8_t index)
{
  if(index>=nsensors) return -1;

  int8_t i;
  --nsensors;

  for(i=index; i<nsensors; ++i) ids[i]=ids[i+1];
  return 0;
}

uint8_t get_n_analog_sensors(void)
{
  return nsensors;
}

int16_t analog_sensor_getValue(const uint8_t index)
{
  if(index>=nsensors) return ANALOG_SENSOR_INVALID_VALUE;

  return adc_getValue(ids[index]<<2);
}
