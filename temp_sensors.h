#ifndef _TEMP_SENSORS_
#define _TEMP_SENSORS_

#include "lm75a.h"
#include "analog_sensor.h"
#include "buzzer.h"

#define N_MAX_SOFT_TEMP_SENSORS	2U

#define TEMP_SENSOR_INVALID_VALUE	32767L

#define ANALOG_SENSOR_DEFAULT_A0_VALUE (6.65105805949316E-06)
#define ANALOG_SENSOR_DEFAULT_A1_VALUE (8.1338284535818E-07)
#define ANALOG_SENSOR_DEFAULT_A2_VALUE (4.20385254613906E-08)
#define ANALOG_SENSOR_DEFAULT_SHIFT_VALUE (0)

#define LM75A_TEMP_SENSOR_DATA_SAVED_SIZE (2*4)
#define ANALOG_TEMP_SENSOR_DATA_SAVED_SIZE (4*3+2*2)

#define TEMP_SENSOR_DEFAULT_ALARM_VALUE (80<<8)

struct lm75a_temp_sensor
{
  int16_t a0;
  int16_t a1; //Units are 2^-14
  int16_t a2; //Units are 2^-28
  int16_t alarm_value;
  int16_t value;
};

struct analog_temp_sensor
{
  float a0;
  float a1;
  float a2;
  int16_t shift;
  int16_t alarm_value;
  int16_t value;
};

extern struct lm75a_temp_sensor lsensors[LM75A_MAX_SENSORS];
extern struct analog_temp_sensor asensors[MAX_ANALOG_SENSORS];
extern int16_t ssensors_alarm_values[N_MAX_SOFT_TEMP_SENSORS];
extern int16_t ssensors_values[N_MAX_SOFT_TEMP_SENSORS];

extern uint8_t lsenslist[LM75A_MAX_SENSORS];
extern uint8_t nlsensors;
extern uint8_t asenslist[MAX_ANALOG_SENSORS];
extern uint8_t nasensors;

//static inline void init_temp_sensor_data(void);

//static inline uint8_t get_lm75a_temp_sensor_list(void);
//static inline int8_t add_lm75a_temp_sensor(const uint8_t id);
//static inline int8_t del_lm75a_temp_sensor(const uint8_t id);
//static inline uint8_t get_analog_temp_sensor_list(void);
//static inline int8_t add_analog_temp_sensor(const uint8_t id);
//static inline int8_t del_analog_temp_sensor(const uint8_t id);
//static inline int16_t get_lm75a_temp_sensor_value(const uint8_t id);
//static inline int16_t get_analog_temp_sensor_value(const uint8_t id);
//static inline int16_t get_soft_temp_sensor_value(const uint8_t id);
//static inline int8_t get_lm75a_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2);
//static inline int8_t get_analog_temp_sensor_calib0(const uint8_t id, float* a0, float* a1);
//static inline int8_t get_analog_temp_sensor_calib1(const uint8_t id, float* a2, int16_t* shift);
//static inline int8_t set_lm75a_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2);
//static inline int8_t set_analog_temp_sensor_calib0(const uint8_t id, const float a0, const float a1);
//static inline int8_t set_analog_temp_sensor_calib1(const uint8_t id, const float a2, const int16_t shift);
//static inline int8_t set_soft_temp_sensor_value(const uint8_t id, const int16_t value);
//static inline int16_t get_lm75a_temp_sensor_alarm_value(const uint8_t id);
//static inline int16_t get_analog_temp_sensor_alarm_value(const uint8_t id);
//static inline int16_t get_soft_temp_sensor_alarm_value(const uint8_t id);
//static inline int8_t set_lm75a_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value);
//static inline int8_t set_analog_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value);
//static inline int8_t set_soft_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value);

//static inline void update_temp_values(void);

#include "temp_sensors_c.h"

#endif
