#include "defines.h"
#include <math.h>
#include <util/delay.h>

#include "fan.h"

#define MAX_FLOW_INVALID_VALUE (UINT16_MAX_VALUE)
#define MIN_TACH_PERIOD_INVALID_VALUE (UINT16_MAX_VALUE)
#define OFF_LEVEL_DEFAULT_VALUE (0x016D)
#define DIFF_LEVEL_DEFAULT_VALUE (OFF_LEVEL_DEFAULT_VALUE)
#define LAST_TEMP_INVALID_VALUE (INT16_MAX_VALUE)

volatile struct fan fans[N_MAX_FANS];
uint8_t fanlist[N_MAX_FANS];
uint8_t nfans=0U;

int8_t set_fan_output(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  if(output>0U) {
    float voltage = FAN_VNOOUT_SCALE*fans[id].vnoout + output * (FAN_DVDOUT_SCALE * fans[id].dvdout + output * FAN_D2VDOUT2_SCALE * fans[id].d2vdout2);
    fans[id].level = (uint16_t)(fans[id].off_level - (voltage>FAN_MAX_VOLTAGE_SCALE?FAN_MAX_VOLTAGE_SCALE:(voltage<0?0:voltage))/FAN_MAX_VOLTAGE_SCALE * fans[id].diff_level);

    if(fans[id].mode&DISABLED_MODE) switch_fan_control(id, fans[id].mode);

  //Completely turn off fan if desired output level is 0
  } else {
    fans[id].level = UINT16_MAX_VALUE;
    set_fan_pin(DC, id, false);
    set_fan_pin(PWM, id, false);
    set_fan_pin_as_output(PWM, id);
    fans[id].mode|=DISABLED_MODE;
  }
  fans[id].output = output;
  return 0;
}

void initfans(void)
{
  uint8_t id;

  for(id=0U; id<N_MAX_FANS; ++id) {
    set_fan_pin_as_input(ADC, id);
    set_fan_pin(ADC, id, false);
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(DC, id);

    fans[id].max_flow=MAX_FLOW_INVALID_VALUE;
    fans[id].min_tach_period=MIN_TACH_PERIOD_INVALID_VALUE;
    fans[id].off_level=OFF_LEVEL_DEFAULT_VALUE;
    fans[id].diff_level=DIFF_LEVEL_DEFAULT_VALUE;
    fans[id].vnoout=FAN_VNOOUT_DEFAULT_VALUE;
    fans[id].dvdout=FAN_DVDOUT_DEFAULT_VALUE;
    fans[id].d2vdout2=FAN_D2VDOUT2_DEFAULT_VALUE;
    fans[id].direction=0;
    fans[id].mode=VOLTAGE_MODE;
    fans[id].nlsensors=0U;
    fans[id].nasensors=0U;
    fans[id].nssensors=0U;
    fans[id].last_temp=LAST_TEMP_INVALID_VALUE;
    fans[id].hysterisis=0U;
    fans[id].ncurvepoints=0U;
    set_fan_output(id, FAN_DEFAULT_OUTPUT_VALUE);
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

int8_t set_fan_voltage_response(const uint8_t id, const float v_no_out, const float dvdout, const float d2vdout2)
{
  if(id>=N_MAX_FANS) return -1;

  if(v_no_out < 0 || dvdout <= 0) return -2;
  if(v_no_out+UINT8_MAX_VALUE*(dvdout + UINT8_MAX_VALUE*d2vdout2) > FAN_MAX_VOLTAGE_SCALE) return -3;
  if(ceil(v_no_out/FAN_VNOOUT_SCALE) > UINT16_MAX_VALUE || ceil(dvdout/FAN_DVDOUT_SCALE) > UINT16_MAX_VALUE || round(d2vdout2/FAN_D2VDOUT2_SCALE) > INT16_MAX_VALUE) return -4;
  fans[id].vnoout = (uint16_t)ceil(v_no_out/FAN_VNOOUT_SCALE);
  fans[id].dvdout = (uint16_t)ceil(dvdout/FAN_DVDOUT_SCALE);
  fans[id].d2vdout2 = (int16_t)round(d2vdout2/FAN_D2VDOUT2_SCALE);
  return 0;
}

int8_t switch_fan_control(const uint8_t id, uint8_t mode)
{
  if(id>=N_MAX_FANS) return -1;

  switch(mode) {
    case VOLTAGE_MODE:
      set_fan_pin(DC, id, false);
      set_fan_pin(PWM, id, false);
      set_fan_pin_as_input(PWM, id);
      break;

    case PWM_MODE:
      set_fan_pin(DC, id, false);
      set_fan_pin(PWM, id, false);
      set_fan_pin_as_output(PWM, id);
      break;

    case MANUAL_MODE:
      set_fan_pin(DC, id, false);
      set_fan_pin(PWM, id, false);
      set_fan_pin_as_output(PWM, id);
      break;

    default:
      return -1;
  }
  fans[id].mode=mode;
  return 0;
}

int8_t fan_adc_calibration(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t prevmode=fans[id].mode;
  uint16_t avalue;
  uint16_t on_level;

  switch_fan_control(id, MANUAL_MODE);
  watchdogConfig(WATCHDOG_OFF);
  _delay_ms(1000);
  watchdogConfig(WATCHDOG_1S);
  avalue=adc_getValue(id);

  do{
    fans[id].off_level=avalue;
    _delay_ms(100);
    avalue=adc_getValue(id);
    watchdogReset();

  } while(avalue>fans[id].off_level);

  set_fan_pin(PWM, id, true);
  watchdogConfig(WATCHDOG_OFF);
  _delay_ms(1000);
  watchdogConfig(WATCHDOG_1S);
  avalue=adc_getValue(id);

  do{
    on_level=avalue;
    _delay_ms(100);
    avalue=adc_getValue(id);
    watchdogReset();

  } while(avalue<on_level);
  fans[id].diff_level=fans[id].off_level-on_level;

  set_fan_pin(PWM, id, false);
  switch_fan_control(id, prevmode);
  return 0;
}

uint8_t* get_fan_data(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return (uint8_t*)(fans+id);
}
