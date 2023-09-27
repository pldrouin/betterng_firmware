#ifndef _ANALOG_SENSOR_
#define _ANALOG_SENSOR_

#include "adc.h"

#define ANALOG_SENSOR_INVALID_VALUE       32767L        // Just an arbritary value outside of the sensor limits
#define MAX_ANALOG_SENSORS	4

int16_t analog_sensor_getValue(const uint8_t index);

#endif