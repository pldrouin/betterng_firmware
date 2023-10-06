#include "defines.h"
#include <util/delay.h>

#include "fan.h"

#define INT16_MAX_VALUE (32767)
#define UINT16_MAX_VALUE (65535)
#define MAX_FLOW_INVALID_VALUE (UINT16_MAX_VALUE)
#define MIN_TACH_PERIOD_INVALID_VALUE (UINT16_MAX_VALUE)
#define OFF_LEVEL_INVALID_VALUE (UINT16_MAX_VALUE)
#define VNOOUT_INVALID_VALUE (INT16_MAX_VALUE)
#define DVDOUT_INVALID_VALUE (INT16_MAX_VALUE)
#define DTVDOUTT_INVALID_VALUE (INT16_MAX_VALUE)
#define LAST_TEMP_INVALID_VALUE (INT16_MAX_VALUE)

#define MAX_CURVE_NPOINTS (6)

#define set_fan_pin(MODE, ID, VALUE) (FAN_ ## MODE ## _PORT = (FAN_ ## MODE ## _PORT & ~_BV(FAN_ ## MODE ## _FIRST_NO+ID)) | ((VALUE!=0)<<(FAN_ ## MODE ## _FIRST_NO+ID)))
#define set_fan_output(MODE, ID) (FAN_ ## MODE ## _DDR |= _BV(FAN_ ## MODE ## _FIRST_NO+ID))
#define set_fan_input(MODE, ID) (FAN_ ## MODE ## _DDR &= ~_BV(FAN_ ## MODE ## _FIRST_NO+ID))

struct fan
{
  uint16_t max_flow;
  uint16_t min_tach_period; //Minimum fan tach period (inverse of max speed)
  uint16_t off_level; //Measured ADC voltage when MOSFET does not conduct (channel specific)
  uint16_t on_level;  //Measured ADC voltage when MOSFET in maximum conduction (channel specific)
  int16_t vnoout;     //Voltage when fan stops
  int16_t dvdout;     //Voltage derivative with output 
  int16_t d2vdout2;   //Second voltage derivative with output 
  int16_t last_temp;
  int16_t hysterisis;
  int8_t direction;   //1=positive pressure, -1=negative pressure, 0=no pressure
  bool pwm;
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

struct fan fans[N_MAX_FANS];
uint8_t fanlist[N_MAX_FANS];
uint8_t nfans=0;

void initfans(void)
{
  uint8_t id;

  for(id=0; id<N_MAX_FANS; ++id) {
    set_fan_input(ADC, id);
    set_fan_pin(ADC, id, false);

    fans[id].max_flow=MAX_FLOW_INVALID_VALUE;
    fans[id].min_tach_period=MIN_TACH_PERIOD_INVALID_VALUE;
    fans[id].off_level=OFF_LEVEL_INVALID_VALUE;
    fans[id].on_level=0;
    fans[id].vnoout=VNOOUT_INVALID_VALUE;
    fans[id].dvdout=DVDOUT_INVALID_VALUE;
    fans[id].d2vdout2=DTVDOUTT_INVALID_VALUE;
    fans[id].direction=0;
    fans[id].pwm=false;
    fans[id].nlsensors=0;
    fans[id].nasensors=0;
    fans[id].nssensors=0;
    fans[id].last_temp=LAST_TEMP_INVALID_VALUE;
    fans[id].hysterisis=0;
    fans[id].ncurvepoints=0;
  }
}

int8_t add_fan(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t i;

  for(i=0; i<nfans; ++i)
    if(fanlist[i]==id) return 0;

  fanlist[nfans]=id;
  ++nfans;
  return 0;
}

int8_t del_fan(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t i;

  for(i=0; i<nfans; ++i)
    if(fanlist[i]==id) break;

  if(i==nfans) return -1;
  --nfans;

  for(; i<nfans; ++i) fanlist[i]=fanlist[i+1];
  return 0;
}

int8_t set_fan_specs(const uint8_t id, const uint16_t max_flow, const uint16_t max_rpm, const int8_t direction)
{
  if(id>=N_MAX_FANS) return -1;

  if(max_rpm>16666) return FAN_INVALID_MAX_RPM;

  if(direction!=FAN_POSITIVE_PRESSURE && direction!=FAN_NEGATIVE_PRESSURE && direction!=FAN_NO_PRESSURE) return FAN_INVALID_DIRECTION;
  fans[id].max_flow=max_flow;
  fans[id].min_tach_period=1000000/max_rpm*60; //in usecs for now
  fans[id].direction=direction;
  return 0;
}

int8_t switch_fan_to_pwm_control(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  set_fan_pin(DC, id, false);
  set_fan_output(DC, id);
  set_fan_pin(PWM, id, false);
  set_fan_output(PWM, id);
  fans[id].pwm=true;
  return 0;
}

int8_t switch_fan_to_voltage_control(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].pwm=false;
  set_fan_pin(DC, id, false);
  set_fan_output(DC, id);
  set_fan_pin(PWM, id, false);
  set_fan_input(PWM, id);
  return 0;
}

int8_t fan_adc_calibration(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  bool prevmode=fans[id].pwm;
  uint16_t avalue;

  if(!prevmode) switch_fan_to_pwm_control(id);
  set_fan_pin(PWM, id, false);
  watchdogConfig(WATCHDOG_OFF);
  _delay_ms(10000);
  watchdogConfig(WATCHDOG_1S);
  fans[id].off_level=adc_getValue(id);

  do{
    avalue=fans[id].off_level;
    _delay_ms(100);
    fans[id].off_level=adc_getValue(id);
    watchdogReset();

  } while(fans[id].off_level>avalue);

  set_fan_pin(PWM, id, true);
  watchdogConfig(WATCHDOG_OFF);
  _delay_ms(10000);
  watchdogConfig(WATCHDOG_1S);
  fans[id].on_level=adc_getValue(id);

  do{
    avalue=fans[id].on_level;
    _delay_ms(100);
    fans[id].on_level=adc_getValue(id);
    watchdogReset();

  } while(fans[id].on_level<avalue);

  set_fan_pin(PWM, id, false);
  if(!prevmode) switch_fan_to_voltage_control(id);
  return 0;
}

uint8_t* get_fan_data(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return (uint8_t*)(fans+id);
}
