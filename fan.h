#ifndef _FAN_
#define _FAN_

#include<stdint.h>
#include<avr/io.h>

#include "defines.h"
#include "cmd_common.h"
#include "timer.h"
#include "adc.h"
#include "temp_sensors.h"

//Prescaler to 64, 250kHz @ 16 MHz F_CPU
#define FAN_PWM_FREQ (22000UL)
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

#define convert_fan_rpm(TACH_PWM_TICKS) ((uint16_t)((F_CPU/FAN_TIMER_PRESCALE)*(uint32_t)60/(FAN_TIMER_ALARM+1)/TACH_PWM_TICKS))

#define FAN_DEFAULT_OUTPUT_VALUE (128U)

#define FAN_VNOOUT_DEFAULT_VALUE (FAN_SAFE_WORKING_VOLTAGE)
#define FAN_DVDOUT_DEFAULT_VALUE ((int16_t)(FAN_MAX_VOLTAGE_SCALE-FAN_SAFE_WORKING_VOLTAGE))
#define calc_d2vdout2(v_no_out, dvdout) ((((int16_t)FAN_MAX_VOLTAGE_SCALE) - v_no_out - dvdout))
#define FAN_D2VDOUT2_DEFAULT_VALUE (calc_d2vdout2(FAN_VNOOUT_DEFAULT_VALUE,FAN_DVDOUT_DEFAULT_VALUE))

struct fan
{
  //uint16_t max_flow;
  int16_t prev_tach_pwm_ticks;
  int16_t cur_tach_pwm_ticks; //Negative until tach line turns back low again
  int16_t off_level; //Measured (maximum) ADC voltage when MOSFET does not conduct (channel specific)
  //uint16_t diff_level;//Measured ADC maximum voltage range
  uint16_t vnoout;     //Voltage when fan stops (units are mV)
  int16_t dvdout;     //Voltage derivative * max output (255) (mV)
  int16_t d2vdout2;    //Second voltage derivative with output * max output * max output (mV)
  int16_t last_temp;
  uint16_t hysterisis;
  int16_t level; //Calculated target ADC level
  uint16_t voltage;
  //int8_t direction;   //1=positive pressure, -1=negative pressure, 0=no pressure
  uint8_t mode;
  uint8_t lsenslist[LM75A_MAX_SENSORS];
  uint8_t nlsensors;
  uint8_t asenslist[MAX_ANALOG_SENSORS];
  uint8_t nasensors;
  uint8_t ssenslist[N_MAX_SOFT_TEMP_SENSORS];
  uint8_t nssensors;
  int8_t curve_temps[MAX_CURVE_NPOINTS];
  uint8_t curve_outputs[MAX_CURVE_NPOINTS];
  uint8_t ncurvepoints;
  uint8_t output;
  uint8_t pwm_counter_offset;
};

extern volatile struct fan fans[N_MAX_FANS];
extern uint8_t fanlist[N_MAX_FANS];
extern uint8_t nfans;

void initfans(void);
int8_t add_fan(const uint8_t id);
int8_t del_fan(const uint8_t id);
int8_t set_fan_specs(const uint8_t id, const uint16_t max_flow, const uint16_t max_rpm, const int8_t direction);
int8_t get_fan_voltage_response(const uint8_t id, uint16_t* v_no_out, int16_t* dvdout, int16_t* d2vdout2);
int8_t set_fan_voltage_response(const uint8_t id, const uint16_t v_no_out, const int16_t dvdout);
uint8_t get_fan_output(const uint8_t id);
int8_t set_fan_output(const uint8_t id, const uint8_t output);
int8_t switch_fan_control(const uint8_t id, const uint8_t mode);
int8_t fan_adc_calibration(const uint8_t id);
uint8_t* get_fan_data(const uint8_t id);
uint16_t get_fan_rpm(const uint8_t id);
int16_t get_fan_off_level(const uint8_t id);
uint16_t get_fan_voltage(const uint8_t id);
uint16_t get_fan_voltage_target(const uint8_t id);

#endif
