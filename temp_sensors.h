#ifndef _TEMP_SENSORS_
#define _TEMP_SENSORS_

#include "lm75a.h"
#include "analog_sensor.h"

#define N_MAX_SOFT_TEMP_SENSORS	2U

#define TEMP_SENSOR_INVALID_VALUE	32767L

struct temp_sensor
{
  int16_t value;
  int16_t a0;
  int16_t a1; //Units are 2^-14
  int16_t a2; //Units are 2^-28
};

extern struct temp_sensor lsensors[LM75A_MAX_SENSORS];
extern struct temp_sensor asensors[MAX_ANALOG_SENSORS];
extern struct temp_sensor ssensors[N_MAX_SOFT_TEMP_SENSORS];

uint8_t get_lm75a_temp_sensor_list(void);
int8_t add_lm75a_temp_sensor(const uint8_t id);
int8_t del_lm75a_temp_sensor(const uint8_t id);
uint8_t get_analog_temp_sensor_list(void);
int8_t add_analog_temp_sensor(const uint8_t id);
int8_t del_analog_temp_sensor(const uint8_t id);
uint8_t get_soft_temp_sensor_list(void);
int8_t add_soft_temp_sensor(const uint8_t id);
int8_t del_soft_temp_sensor(const uint8_t id);
static inline int16_t get_lm75a_sensor_value(const uint8_t id){if(id>=LM75A_MAX_SENSORS) return TEMP_SENSOR_INVALID_VALUE; return lsensors[id].value;}
static inline int16_t get_analog_sensor_value(const uint8_t id){if(id>=MAX_ANALOG_SENSORS) return TEMP_SENSOR_INVALID_VALUE; return asensors[id].value;}
static inline int16_t get_soft_sensor_value(const uint8_t id){if(id>=N_MAX_SOFT_TEMP_SENSORS) return TEMP_SENSOR_INVALID_VALUE; return ssensors[id].value;}
int8_t get_lm75a_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2);
int8_t get_analog_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2);
int8_t set_lm75a_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2);
int8_t set_analog_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2);
int8_t set_soft_temp_sensor_value(const uint8_t id, const int16_t value);

void update_temp_values(void);

#endif
