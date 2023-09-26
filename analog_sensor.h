#ifndef _ANALOG_SENSOR_
#define _ANALOG_SENSOR_

#include "adc.h"

#define ANALOG_SENSOR_INVALID_VALUE       32767L        // Just an arbritary value outside of the sensor limits
#define MAX_ANALOG_SENSORS	4

int8_t add_analog_sensor(const uint8_t id);
int8_t del_analog_sensor(const uint8_t index);
uint8_t get_n_analog_sensors(void);
int16_t analog_sensor_getValue(const uint8_t index);

#endif
