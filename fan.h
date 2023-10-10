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

#define MAX_CURVE_NPOINTS (6U)

#define set_fan_pin(MODE, ID, VALUE) (FAN_ ## MODE ## _PORT = (FAN_ ## MODE ## _PORT & ~_BV(FAN_ ## MODE ## _FIRST_NO+ID)) | ((VALUE!=0)<<(FAN_ ## MODE ## _FIRST_NO+ID)))
#define set_fan_pin_as_output(MODE, ID) (FAN_ ## MODE ## _DDR |= _BV(FAN_ ## MODE ## _FIRST_NO+ID))
#define set_fan_pin_as_input(MODE, ID) (FAN_ ## MODE ## _DDR &= ~_BV(FAN_ ## MODE ## _FIRST_NO+ID))

#define FAN_DEFAULT_OUTPUT_VALUE (128U)
enum {VOLTAGE_MODE=_BV(0), PWM_MODE=_BV(1), MANUAL_MODE=_BV(2), DISABLED_MODE=_BV(7)};

#define FAN_MAX_VOLTAGE_SCALE (12.)
#define FAN_VNOOUT_SCALE (0.0002)    //in volts
#define FAN_DVDOUT_SCALE (7.8125e-7) //in volts/output
#define FAN_D2VDOUT2_SCALE (6.1295e-7) //in volts/output
#define FAN_SAFE_WORKING_VOLTAGE (4.)
#define FAN_VNOOUT_DEFAULT_VALUE ((uint16_t)(FAN_SAFE_WORKING_VOLTAGE/FAN_VNOOUT_SCALE))
#define FAN_DVDOUT_DEFAULT_VALUE ((uint16_t)((FAN_MAX_VOLTAGE_SCALE-FAN_SAFE_WORKING_VOLTAGE)/(FAN_DVDOUT_SCALE*UINT8_MAX_VALUE)))
#define FAN_D2VDOUT2_DEFAULT_VALUE ((int16_t)(0/(FAN_D2VDOUT2_SCALE*UINT8_MAX_VALUE*UINT8_MAX_VALUE)))

struct fan
{
  uint16_t max_flow;
  uint16_t min_tach_period; //Minimum fan tach period (inverse of max speed)
  uint16_t off_level; //Measured (maximum) ADC voltage when MOSFET does not conduct (channel specific)
  uint16_t diff_level;//Measured ADC maximum voltage range
  uint16_t vnoout;     //Voltage when fan stops (units are VNOOUT_SCALE)
  uint16_t dvdout;     //Voltage derivative with output (units are DVDOUT_SCALE)
  int16_t d2vdout2;    //Second voltage derivative with output (units are D2VDOUT2_SCALE)
  int16_t last_temp;
  uint16_t hysterisis;
  uint16_t level; //Calculated target ADC level
  int8_t direction;   //1=positive pressure, -1=negative pressure, 0=no pressure
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
};

extern volatile struct fan fans[N_MAX_FANS];
extern uint8_t fanlist[N_MAX_FANS];
extern uint8_t nfans;

void initfans(void);
int8_t add_fan(const uint8_t id);
int8_t del_fan(const uint8_t id);
int8_t set_fan_specs(const uint8_t id, const uint16_t max_flow, const uint16_t max_rpm, const int8_t direction);
int8_t set_fan_voltage_response(const uint8_t id, const float v_no_out, const float dvdout, const float d2vdout2);
int8_t set_fan_output(const uint8_t id, const uint8_t output);
int8_t switch_fan_control(const uint8_t id, const uint8_t mode);
int8_t fan_adc_calibration(const uint8_t id);
uint8_t* get_fan_data(const uint8_t id);

#endif
