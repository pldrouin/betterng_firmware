#ifndef _TEMP_SENSORS_
#define _TEMP_SENSORS_

#include "lm75a.h"
#include "analog_sensor.h"

#define N_MAX_SOFT_TEMP_SENSORS	2

#define TEMP_SENSOR_INVALID_VALUE	32767L

int8_t add_lm75a_temp_sensor(const uint8_t id);
int8_t del_lm75a_temp_sensor(const uint8_t id);
int8_t add_analog_temp_sensor(const uint8_t id);
int8_t del_analog_temp_sensor(const uint8_t id);
int8_t add_soft_temp_sensor(const uint8_t id);
int8_t del_soft_temp_sensor(const uint8_t id);
int8_t set_lm75a_temp_sensor_calib(const uint16_t id, const float a2, const float a1, const float a0);
int8_t set_analog_temp_sensor_calib(const uint16_t id, const float a2, const float a1, const float a0);
int8_t set_soft_temp_sensor_calib(const uint16_t id, const float a2, const float a1, const float a0);
void update_temp_values(void);
int8_t set_soft_sensor_value(const uint8_t id, const int16_t value);

#endif
