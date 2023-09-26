#ifndef _TEMP_SENSORS_
#define _TEMP_SENSORS_

#include "lm75a.h"
#include "analog_sensor.h"

#define N_MAX_SOFT_TEMP_SENSORS	2
#define LM75A_SENSOR		(0)
#define ANALOG_TEMP_SENSOR	(LM75A_MAX_SENSORS)
#define SOFT_TEMP_SENSOR	(LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS)

#define TEMP_SENSOR_INVALUD_VALUE	32767L

int8_t add_temp_sensor(const uint8_t type, const uint8_t id);
int8_t del_temp_sensor(const uint8_t index);
int8_t set_temp_sensor_calib(const uint16_t index, const float a2, const float a1, const float a0);
void update_temp_values(void);
int8_t set_soft_sensor_value(const uint8_t id, const int16_t value);

#endif
