#ifndef _FAN_
#define _FAN_

#include<string.h>
#include<stdint.h>
#include<avr/io.h>

#include "defines.h"
#include "cmd_common.h"
#include "overtemp.h"
#include "timer.h"
#include "adc.h"
#include "temp_sensors.h"
#include "binary_search.h"
#include "buzzer.h"

//Prescaler to 64, 250kHz @ 16 MHz F_CPU
#define FAN_PWM_FREQ (20000UL)
#define FAN_TACH_MEASUREMENT_MAX_TICKS (FAN_PWM_FREQ)
#define FAN_TACH_NO_TICK_TIMEOUT (2*FAN_PWM_FREQ)
#define FAN_TIMER_PRESCALE TIMER_FINE_8BIT_PRESCALE(FAN_PWM_FREQ)
#define FAN_TIMER_ALARM TIMER_FINE_8BIT_ALARM(FAN_PWM_FREQ)

#define FAN_POSITIVE_PRESSURE (1)
#define FAN_NEGATIVE_PRESSURE (-1)
#define FAN_NO_PRESSURE (0)

#define FAN_INVALID_DIRECTION (-2)
#define FAN_INVALID_MAX_RPM (-3)

#define MAX_CURVE_NPOINTS (6U)

#define set_fan_pin(MODE, ID, VALUE) (FAN_ ## MODE ## _PORT = (FAN_ ## MODE ## _PORT & ~_BV(FAN_ ## MODE ## _FIRST_NO+ID)) | ((VALUE!=0)<<(FAN_ ## MODE ## _FIRST_NO+ID)))
#define toggle_fan_pin(MODE, ID) (FAN_ ## MODE ## _PORT ^= _BV(FAN_ ## MODE ## _FIRST_NO+ID))
#define set_fan_pin_as_output(MODE, ID) (sbi(FAN_ ## MODE ## _DDR, FAN_ ## MODE ## _FIRST_NO+ID))
#define set_fan_pin_as_input(MODE, ID) (cbi(FAN_ ## MODE ## _DDR, FAN_ ## MODE ## _FIRST_NO+ID))
#define read_fan_pin(MODE, ID) (bit_is_set(FAN_ ## MODE ## _PIN, FAN_ ## MODE ## _FIRST_NO+ID)!=0)

#define convert_fan_rpm(TACH_TICKS) ((int16_t)((F_CPU/FAN_TIMER_PRESCALE)*(int32_t)30/(FAN_TIMER_ALARM+1)/TACH_TICKS))

#define FAN_DEFAULT_OUTPUT_VALUE (128U)

#define FAN_VNOOUT_DEFAULT_VALUE (FAN_SAFE_WORKING_VOLTAGE)
#define FAN_DVDOUT_DEFAULT_VALUE (((int16_t)ceil((FAN_MAX_VOLTAGE_SCALE - FAN_SAFE_WORKING_VOLTAGE)*(256./UINT8_MAX))))
#define FAN_D2VDOUT2_DEFAULT_VALUE (0)

#define FAN_DCNOOUT_DEFAULT_VALUE (FAN_SAFE_WORKING_DUTY_CYCLE*64)
#define FAN_DDCDOUT_DEFAULT_VALUE (((int16_t)ceil((((uint16_t)UINT8_MAX)*64 - FAN_DCNOOUT_DEFAULT_VALUE)*(256./UINT8_MAX))))
#define FAN_D2DCDOUT2_DEFAULT_VALUE (0)

enum {FAN_LAST_TACH_UP=1U, FAN_LAST_POWER_UP=2U, FAN_TACH_ACCURATE_RPM=4U};

#define FAN_DATA_SAVED_SIZE (21+LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS+N_MAX_SOFT_TEMP_SENSORS+2*MAX_CURVE_NPOINTS)

struct fan
{
  uint16_t vnoout;     //Voltage when fan stops (units are mV)
  int16_t dvdout;     //Voltage derivative * max output (255) (mV)
  int16_t d2vdout2;    //Second voltage derivative with output * max output * max output (mV)
  uint16_t dcnoout;    //PWM duty cycle when fan stops * 64
  int16_t ddcdout;     //PWM duty cycle derivative * max output (255) * 64
  int16_t d2dcdout2;   //PWM second duty cycle derivative * max output * max output * 64
  uint8_t hysterisis;
  uint8_t lsenslist[LM75A_MAX_SENSORS];
  uint8_t nlsensors;
  uint8_t asenslist[MAX_ANALOG_SENSORS];
  uint8_t nasensors;
  uint8_t ssenslist[N_MAX_SOFT_TEMP_SENSORS];
  uint8_t nssensors;
  int8_t curve_temps[MAX_CURVE_NPOINTS];
  uint8_t curve_outputs[MAX_CURVE_NPOINTS];
  uint8_t ncurvepoints;
  uint8_t pwm_to_voltage_output;
  uint8_t voltage_to_pwm_output;
  uint8_t mode;
  uint8_t output; //Only saved for custom mode

  //List of unsaved values;
  int16_t level;       //Calculated target ADC level
  volatile int16_t prev_tach_ticks;
  uint16_t cur_tach_ticks;
  uint16_t cur_tach_phase;
  int8_t last_temp;
  uint8_t pwm_counter_offset;
  uint8_t duty_cycle;  //Calculated PWM duty cycle
  uint8_t last_fan_status;
  uint8_t prev_tach_osc;
};

extern struct fan fans[N_MAX_FANS];
extern uint8_t fanlist[N_MAX_FANS];
extern uint8_t nfans;

//static inline void initfan_data(void);
//static inline void initfans(void);
//static inline uint8_t get_fan_list(void);
//static inline int8_t add_fan(const uint8_t id);
//static inline int8_t del_fan(const uint8_t id);
//static inline uint8_t get_fan_lm75a_temp_sensor_list(const uint8_t fan_id);
//static inline uint8_t get_fan_analog_temp_sensor_list(const uint8_t fan_id);
//static inline uint8_t get_fan_soft_temp_sensor_list(const uint8_t fan_id);
//static inline int8_t add_fan_lm75a_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t add_fan_analog_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t add_fan_soft_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t del_fan_lm75a_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t del_fan_analog_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t del_fan_soft_temp_sensor(const uint8_t fan_id, const uint8_t sens_id);
//static inline int8_t add_fan_curve_point(const uint8_t fan_id, const int8_t temp, const uint8_t output);
//static inline int8_t del_fan_curve_point(const uint8_t fan_id, const uint8_t index);
//static inline int8_t get_fan_n_curve_points(const uint8_t fan_id);
//static inline int8_t get_fan_curve_point(const uint8_t fan_id, const uint8_t index, int8_t* const temp, uint8_t* const output);
//static inline int8_t get_fan_duty_cycle_response(const uint8_t id, uint16_t* const dc_no_out, int16_t* const ddcdout, int16_t* const d2dcdout2);
//static inline int8_t set_fan_duty_cycle_response(const uint8_t id, const uint16_t dc_no_out, const int16_t ddcdout, const int16_t d2dcdout2);
//static inline int8_t get_fan_voltage_response(const uint8_t id, uint16_t* const v_no_out, int16_t* const dvdout, int16_t* const d2vdout2);
//static inline int8_t set_fan_voltage_response(const uint8_t id, const uint16_t v_no_out, const int16_t dvdout, const int16_t d2vdout2);
//static inline int8_t get_fan_mode_transitions(const uint8_t id, uint8_t* const pwm_to_voltage_output, uint8_t* const voltage_to_pwm_output);
//static inline int8_t set_fan_mode_transitions(const uint8_t id, const uint8_t pwm_to_voltage_output, const uint8_t voltage_to_pwm_output);
//static inline uint8_t get_fan_adc_value(const uint8_t id);
//static inline uint8_t get_fan_output(const uint8_t id);
int8_t set_fan_output(const uint8_t id, const uint8_t output);
int8_t set_fan_output_auto(const uint8_t id, const uint8_t output);
//static inline uint8_t get_fan_mode(const uint8_t id);
int8_t switch_fan_control(const uint8_t id, const uint8_t mode);
//static inline int16_t get_fan_rpm(const uint8_t id);
//static inline void update_fans(void);

#include "fan_c.h"

#endif
