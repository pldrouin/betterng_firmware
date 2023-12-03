#include "defines.h"
#include <math.h>
#include <stdlib.h>

#include "fan.h"

#define MAX_FLOW_INVALID_VALUE (UINT16_MAX)
#define OFF_LEVEL_DEFAULT_VALUE (0x016D)
#define DIFF_LEVEL_DEFAULT_VALUE (OFF_LEVEL_DEFAULT_VALUE)
#define LAST_TEMP_INVALID_VALUE (INT16_MAX)

struct fan fans[N_MAX_FANS];
static uint8_t fanlist[N_MAX_FANS];
static uint8_t nfans=0U;

uint8_t get_fan_output(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].output;
}

int8_t set_fan_output(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  if(output>0U) {
    fans[id].voltage = (uint16_t)(fans[id].vnoout + ((int32_t)output)*fans[id].dvdout/UINT8_MAX + ((int32_t)output)*fans[id].d2vdout2*output/(((uint16_t)UINT8_MAX)*UINT8_MAX));
    fans[id].level = (int16_t)(fans[id].off_level - ((int32_t)fans[id].off_level)*fans[id].voltage/FAN_MAX_VOLTAGE_SCALE);
    //Assertion: level <= off_level

    fans[id].duty_cycle = (uint8_t)((fans[id].dcnoout + ((int32_t)output)*fans[id].ddcdout/UINT8_MAX + ((int32_t)output)*fans[id].d2dcdout2*output/(((uint16_t)UINT8_MAX)*UINT8_MAX))/64);

    switch_fan_control(id, fans[id].mode);

  //Completely turn off fan if desired output level is 0
  } else {
    fans[id].level = INT16_MAX;
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(PWM, id);
    set_fan_pin(PWM, id, false);
    fans[id].mode|=FAN_DISABLED_MODE;
  }
  fans[id].output = output;
  return 0;
}

int8_t set_fan_output_auto(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  if(output>0U) {

    if(output < fans[id].voltage_to_pwm_output && output >= fans[id].pwm_to_voltage_output) {
      fans[id].voltage = (uint16_t)(fans[id].vnoout + ((int32_t)output)*fans[id].dvdout/UINT8_MAX + ((int32_t)output)*fans[id].d2vdout2*output/(((uint16_t)UINT8_MAX)*UINT8_MAX));
      fans[id].level = (int16_t)(fans[id].off_level - ((int32_t)fans[id].off_level)*fans[id].voltage/FAN_MAX_VOLTAGE_SCALE);
      //Assertion: level <= off_level
      switch_fan_control(id, FAN_VOLTAGE_MODE);

    } else {

      fans[id].duty_cycle = (uint8_t)((fans[id].dcnoout + ((int32_t)output)*fans[id].ddcdout/UINT8_MAX + ((int32_t)output)*fans[id].d2dcdout2*output/(((uint16_t)UINT8_MAX)*UINT8_MAX))/64);
      switch_fan_control(id, FAN_PWM_MODE);
    }


  //Completely turn off fan if desired output level is 0
  } else {
    fans[id].level = INT16_MAX;
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(PWM, id);
    set_fan_pin(PWM, id, false);
    fans[id].mode|=FAN_DISABLED_MODE;
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
    set_fan_pin_as_input(PWM, id);
    set_fan_pin(PWM, id, false);
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(DC, id);

    set_fan_pin(TACH, id, false);
    set_fan_pin_as_input(TACH, id);

    set_fan_pin(RPM, id, false);
    set_fan_pin_as_output(RPM, id);

    //fans[id].max_flow=MAX_FLOW_INVALID_VALUE;
    fans[id].prev_tach_pwm_ticks=-INT16_MAX;
    fans[id].cur_tach_pwm_ticks=1;
    fans[id].cur_tach_phase=0;
    fans[id].off_level=OFF_LEVEL_DEFAULT_VALUE;
    //fans[id].diff_level=DIFF_LEVEL_DEFAULT_VALUE;
    fans[id].vnoout=FAN_VNOOUT_DEFAULT_VALUE;
    fans[id].dvdout=FAN_DVDOUT_DEFAULT_VALUE;
    fans[id].d2vdout2=FAN_D2VDOUT2_DEFAULT_VALUE;
    fans[id].dcnoout=FAN_DCNOOUT_DEFAULT_VALUE;
    fans[id].ddcdout=FAN_DDCDOUT_DEFAULT_VALUE;
    fans[id].d2dcdout2=FAN_D2DCDOUT2_DEFAULT_VALUE;
    //fans[id].direction=0;
    fans[id].mode=FAN_VOLTAGE_MODE;
    fans[id].nlsensors=0U;
    fans[id].nasensors=0U;
    fans[id].nssensors=0U;
    fans[id].last_temp=LAST_TEMP_INVALID_VALUE;
    fans[id].hysterisis=0U;
    fans[id].ncurvepoints=0U;
    set_fan_output(id, FAN_DEFAULT_OUTPUT_VALUE);
    fans[id].pwm_to_voltage_output=UINT8_MAX;
    fans[id].voltage_to_pwm_output=UINT8_MAX;
    fans[id].pwm_counter_offset=(uint8_t)(((UINT8_MAX+1.)*id)/N_MAX_FANS);
    fans[id].last_fan_status=0;
    fans[id].prev_tach_osc=0;
  }

  FAN_OUTPUT_COMPARE_REG = FAN_TIMER_ALARM;
  FAN_TIMER_CONTROL_REG = _BV(FAN_TIMER_WAVEFORM_MODE_BIT_1) | TIMER_FINE_8BIT_PRESCALER_SETTING(FAN_PWM_FREQ);
  sbi(FAN_TIMER_INTR_MASK_REG, FAN_TIMER_ENABLE_COMPARE_MATCH_INTR);
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
  //fans[id].max_flow=max_flow;
  //fans[id].direction=direction;
  return 0;
}

int8_t get_fan_duty_cycle_response(const uint8_t id, uint16_t* const dc_no_out, int16_t* const ddcdout, int16_t* const d2dcdout2)
{
  if(id>=N_MAX_FANS) {
    *dc_no_out = 0;
    *ddcdout = 0;
    *d2dcdout2 = 0;
    return -1;
  }
  *dc_no_out = fans[id].dcnoout;
  *ddcdout = fans[id].ddcdout;
  *d2dcdout2 = fans[id].d2dcdout2;
  return 0;
}

int8_t set_fan_duty_cycle_response(const uint8_t id, const uint16_t dc_no_out, const int16_t ddcdout)
{
  if(id>=N_MAX_FANS) return -1;

  fans[id].dcnoout = dc_no_out;
  fans[id].ddcdout = ddcdout;
  fans[id].d2dcdout2 = calc_d2dcdout2(dc_no_out, ddcdout);
  return 0;
}

int8_t get_fan_voltage_response(const uint8_t id, uint16_t* const v_no_out, int16_t* const dvdout, int16_t* const d2vdout2)
{
  if(id>=N_MAX_FANS) {
    *v_no_out = 0;
    *dvdout = 0;
    *d2vdout2 = 0;
    return -1;
  }
  *v_no_out = fans[id].vnoout;
  *dvdout = fans[id].dvdout;
  *d2vdout2 = fans[id].d2vdout2;
  return 0;
}

int8_t set_fan_voltage_response(const uint8_t id, const uint16_t v_no_out, const int16_t dvdout)
{
  if(id>=N_MAX_FANS) return -1;

  fans[id].vnoout = v_no_out;
  fans[id].dvdout = dvdout;
  fans[id].d2vdout2 = calc_d2vdout2(v_no_out, dvdout);
  return 0;
}

int8_t get_fan_mode_transitions(const uint8_t id, uint8_t* const pwm_to_voltage_output, uint8_t* const voltage_to_pwm_output)
{
  if(id>=N_MAX_FANS) return -1;
  *pwm_to_voltage_output = fans[id].pwm_to_voltage_output;
  *voltage_to_pwm_output = fans[id].voltage_to_pwm_output;
  return 0;
}

int8_t set_fan_mode_transitions(const uint8_t id, const uint8_t pwm_to_voltage_output, const uint8_t voltage_to_pwm_output)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].pwm_to_voltage_output = pwm_to_voltage_output;
  fans[id].voltage_to_pwm_output = voltage_to_pwm_output;
  return 0;
}

int8_t switch_fan_control(const uint8_t id, uint8_t mode)
{
  if(id>=N_MAX_FANS) return -1;
  mode&=FAN_MODE_MASK;

  if(mode != fans[id].mode) {
    fans[id].mode=mode;

    switch(mode) {
      case FAN_VOLTAGE_MODE:
	set_fan_pin_as_input(PWM, id);
	set_fan_pin(PWM, id, false);
	break;

      case FAN_PWM_MODE:
	set_fan_pin(DC, id, false);
	set_fan_pin_as_output(PWM, id);
	break;

      case FAN_MANUAL_MODE:
	set_fan_pin(DC, id, false);
	set_fan_pin_as_output(PWM, id);
	set_fan_pin(PWM, id, false);
	break;

      default:
	return -1;
    }
  }
  return 0;
}

int8_t fan_adc_calibration(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t prevmode=fans[id].mode;
  int16_t avalue;
  int8_t i;
  //uint16_t on_level;

  switch_fan_control(id, FAN_MANUAL_MODE);
  for(i=99; i>=0; --i) {
    idle_timer_delay_millis(100);
    watchdogReset();
  }
  avalue=adc_getValue(id);

  do{
    fans[id].off_level=avalue;
    idle_timer_delay_millis(100);
    avalue=adc_getValue(id);
    watchdogReset();

  } while(avalue>fans[id].off_level);

  set_fan_pin(PWM, id, false);
  switch_fan_control(id, prevmode);
  return 0;
}

uint8_t* get_fan_data(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return (uint8_t*)(fans+id);
}

int16_t get_fan_rpm(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  uint16_t rpm = convert_fan_rpm(fans[id].prev_tach_pwm_ticks);
  return (abs(rpm)==convert_fan_rpm(INT16_MAX)?0:rpm);
}

int16_t get_fan_off_level(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].off_level; 
}

uint16_t get_fan_voltage(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  int16_t adc_level=adc_getValue(id);
  return (adc_level < fans[id].off_level?(uint16_t)((uint32_t)(fans[id].off_level - adc_level) * FAN_MAX_VOLTAGE_SCALE / fans[id].off_level):0U); 
}

uint16_t get_fan_voltage_target(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  //Assertion: level <= off_level, so voltage is positive
  //return (uint16_t)((uint32_t)(fans[id].off_level - fans[id].level) * FAN_MAX_VOLTAGE_SCALE / fans[id].off_level); 
  return fans[id].voltage;
}

ISR(TIMER2_COMP_vect)
{
  static uint8_t pwm_counter=0;
  static struct fan* fan;
  bool cur_tach_status;

  //Note: In PWM mode, a real tach signal is when the tach line is pulled low. A line
  //turning high following the end of the power pulse should be ignored.
  int8_t index;
  uint8_t id;

  for(index=nfans-1; index>=0; --index) {
    id=fanlist[index];
    fan=fans+id;

    ++(fan->cur_tach_phase);
    
    if(fan->cur_tach_phase > abs(fan->prev_tach_pwm_ticks)) {
      toggle_fan_pin(RPM, id);
      fan->cur_tach_phase-=abs(fan->prev_tach_pwm_ticks);
    }

    //If fan power is active
    if((fan->mode&FAN_PWM_MODE)==0 || read_fan_pin(PWM, id)) {
      cur_tach_status=(read_fan_pin(TACH, id)>0);

      //If the tach pin changed since the last time power was active
      if(((fan->last_fan_status&FAN_LAST_TACH_UP)!=0) ^ cur_tach_status) {
        //set_fan_pin(RPM, id, cur_tach_status);

	//If the power just became active, the tach signal is not accurate
	if(!(fan->last_fan_status&FAN_LAST_POWER_UP)) {

	  //If we can wait for another tach oscillation
	  if(fan->cur_tach_pwm_ticks < FAN_TACH_MEASUREMENT_MAX_TICKS && fan->prev_tach_osc<UINT8_MAX) {
	    ++(fan->cur_tach_pwm_ticks);
	    ++(fan->prev_tach_osc);
	    fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | (fan->last_fan_status&FAN_TACH_ACCURATE_RPM);

	    //Else if we cannot wait for another oscillation
	  } else {
	    //Corrects the period using the average expected time for the
	    //tach change
	    fan->prev_tach_pwm_ticks = -(int16_t)((fan->cur_tach_pwm_ticks - (UINT8_MAX - fan->duty_cycle)/2) / (fan->prev_tach_osc + 1));
	    fan->cur_tach_pwm_ticks = 1 + (UINT8_MAX - fan->duty_cycle)/2;
	    fan->prev_tach_osc = 0;
	    fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP;
	  }

	  //Else if the power was previously active, then the end of the tach signal is accurate
	} else {

	  if(fan->last_fan_status&FAN_TACH_ACCURATE_RPM) fan->prev_tach_pwm_ticks = fan->cur_tach_pwm_ticks / (fan->prev_tach_osc + 1);

	  else fan->prev_tach_pwm_ticks = -(int16_t)(fan->cur_tach_pwm_ticks / (fan->prev_tach_osc + 1));
	  fan->cur_tach_pwm_ticks = 1;
	  fan->prev_tach_osc = 0;
	  fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | FAN_TACH_ACCURATE_RPM;
	}

	//Else if the tach pin did not change since the last time power was active
      } else {

	if(fan->cur_tach_pwm_ticks >= FAN_TACH_NO_TICK_TIMEOUT) {
	  fan->prev_tach_pwm_ticks = -INT16_MAX;
	  fan->cur_tach_pwm_ticks = 1;
	  fan->prev_tach_osc = 0;

	} else ++(fan->cur_tach_pwm_ticks);
	fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | (fan->last_fan_status&FAN_TACH_ACCURATE_RPM);
      }

      //Else if the fan power is not active
    } else {
      
      //if((fan->cur_tach_pwm_ticks%abs(fan->prev_tach_pwm_ticks) == 0)
      //  toggle_fan_pin(RPM, id);

      if(fan->cur_tach_pwm_ticks >= FAN_TACH_NO_TICK_TIMEOUT) {
	fan->prev_tach_pwm_ticks = -INT16_MAX;
	fan->cur_tach_pwm_ticks = 1;
	fan->prev_tach_osc = 0;
	fan->last_fan_status = 0;

      } else {
	++(fan->cur_tach_pwm_ticks);
	fan->last_fan_status &= ~FAN_LAST_POWER_UP;
      }
    }

    //If fan is running in PWM mode
    if(fan->mode==FAN_PWM_MODE) {

      if((uint8_t)(++pwm_counter+fan->pwm_counter_offset) > fan->duty_cycle) set_fan_pin(PWM, id, false);

      else set_fan_pin(PWM, id, true);
    }
  }
}
