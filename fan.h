#ifndef _FAN_
#define _FAN_

#include<stdint.h>

#include "defines.h"
#include "adc.h"
#include "temp_sensors.h"

#define FAN_POSITIVE_PRESSURE (1)
#define FAN_NEGATIVE_PRESSURE (-1)
#define FAN_NO_PRESSURE (0)

#define FAN_INVALID_DIRECTION (-2)
#define FAN_INVALID_MAX_RPM (-3)

void initfans(void);
int8_t add_fan(const uint8_t id);
int8_t del_fan(const uint8_t id);
int8_t set_fan_specs(const uint8_t id, const uint16_t max_flow, const uint16_t max_rpm, const int8_t direction);
int8_t switch_fan_to_pwm_control(const uint8_t id);
int8_t switch_fan_to_voltage_control(const uint8_t id);
int8_t fan_adc_calibration(const uint8_t id);
uint8_t* get_fan_data(const uint8_t id);

#endif
