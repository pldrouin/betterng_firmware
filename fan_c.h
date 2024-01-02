#include <math.h>
#include <stdlib.h>
#include "cmd.h"

#define LAST_TEMP_INVALID_VALUE (INT8_MAX)

static inline void initfan_data(void)
{
  uint8_t id;

  for(id=0U; id<N_MAX_FANS; ++id) {
    struct fan* const fan=fans+id;
    fan->prev_tach_ticks=INT16_MAX;
    fan->cur_tach_ticks=1;
    fan->cur_tach_phase=0;
    fan->vnoout=FAN_VNOOUT_DEFAULT_VALUE;
    fan->dvdout=FAN_DVDOUT_DEFAULT_VALUE;
    fan->d2vdout2=FAN_D2VDOUT2_DEFAULT_VALUE;
    fan->dcnoout=FAN_DCNOOUT_DEFAULT_VALUE;
    fan->ddcdout=FAN_DDCDOUT_DEFAULT_VALUE;
    fan->d2dcdout2=FAN_D2DCDOUT2_DEFAULT_VALUE;
    fan->mode=FAN_VOLTAGE_MODE;
    fan->nlsensors=0U;
    fan->nasensors=0U;
    fan->nssensors=0U;
    fan->last_temp=LAST_TEMP_INVALID_VALUE;
    fan->min_tach_ticks=FAN_MIN_PREV_TACH_TICKS_DEFAULT_VALUE;
    fan->max_tach_ticks=FAN_MAX_PREV_TACH_TICKS_DEFAULT_VALUE;
    fan->hysterisis=0U;
    fan->curve_temps[0]=0;
    fan->curve_outputs[0]=UINT8_MAX;
    fan->ncurvepoints=1U;
    set_fan_output(id, FAN_DEFAULT_OUTPUT_VALUE);
    fan->pwm_to_voltage_output=UINT8_MAX;
    fan->voltage_to_pwm_output=UINT8_MAX;
    fan->pwm_counter_offset=(uint8_t)(FAN_PWM_COUNTER_OFFSET*id);
    fan->last_fan_status=0;
    fan->prev_tach_osc=0;
  }
}

static inline void initfans(void)
{
  uint8_t id;

  for(id=0U; id<N_MAX_FANS; ++id) {
    struct fan* const fan=fans+id;
    set_fan_pin_as_input(ADC, id);
    set_fan_pin(ADC, id, false);
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(DC, id);

    set_fan_pin(TACH, id, false);
    set_fan_pin_as_input(TACH, id);

    set_fan_pin(RPM, id, false);
    set_fan_pin_as_output(RPM, id);

    const uint8_t loaded_mode = fan->mode;
    fan->mode = 0;
    switch_fan_mode(id, loaded_mode);

    if(!(fan->mode&FAN_AUTO_FLAG)) set_fan_output(id, fan->output);
    
    else set_fan_output_auto(id, fan->curve_outputs[fan->ncurvepoints-1]);
  }

  FAN_OUTPUT_COMPARE_REG = FAN_TIMER_ALARM;
  FAN_TIMER_CONTROL_REG = _BV(FAN_TIMER_WAVEFORM_MODE_BIT_1) | TIMER_FINE_8BIT_PRESCALER_SETTING(FAN_PWM_FREQ);
  sbi(FAN_TIMER_INTR_MASK_REG, FAN_TIMER_ENABLE_COMPARE_MATCH_INTR);
}

static inline uint8_t get_fan_list(void)
{
  uint8_t ret=0;
  uint8_t i;

  for(i=0; i<nfans; ++i) ret|=(1<<fanlist[i]);
  return ret;
}

static inline int8_t add_fan(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t i;

  for(i=0; i<nfans; ++i) if(fanlist[i]==id) return 0;

  fanlist[nfans]=id;
  ++nfans;
  return 0;
}

static inline int8_t del_fan(const uint8_t id)
{
  if(id>=N_MAX_FANS) return -1;
  uint8_t i;

  for(i=0; i<nfans; ++i) if(fanlist[i]==id) break;

  if(i==nfans) return -1;
  --nfans;

  memmove(fanlist+i, fanlist+i+1, nfans);
  return 0;
}

static inline uint8_t get_fan_lm75a_temp_sensor_list(const uint8_t fan_id)
{
  uint8_t ret=0;
  uint8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=0; i<fan->nlsensors; ++i) ret|=(1<<fan->lsenslist[i]);
  return ret;
}

static inline uint8_t get_fan_analog_temp_sensor_list(const uint8_t fan_id)
{
  uint8_t ret=0;
  uint8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=0; i<fan->nasensors; ++i) ret|=(1<<fan->asenslist[i]);
  return ret;
}

static inline uint8_t get_fan_soft_temp_sensor_list(const uint8_t fan_id)
{
  uint8_t ret=0;
  uint8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=0; i<fan->nssensors; ++i) ret|=(1<<fan->ssenslist[i]);
  return ret;
}


static inline int8_t add_fan_lm75a_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;

  if(sens_id>=LM75A_MAX_SENSORS) return -2;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nlsensors-1; i>=0; --i) if(fan->lsenslist[i]==sens_id) return 0;
  fan->lsenslist[fan->nlsensors]=sens_id;
  ++(fan->nlsensors);
  return 0;
}

static inline int8_t add_fan_analog_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;

  if(sens_id>=MAX_ANALOG_SENSORS) return -2;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nasensors-1; i>=0; --i) if(fan->asenslist[i]==sens_id) return 0;
  fan->asenslist[fan->nasensors]=sens_id;
  ++(fan->nasensors);
  return 0;
}

static inline int8_t add_fan_soft_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;

  if(sens_id>=N_MAX_SOFT_TEMP_SENSORS) return -2;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nssensors-1; i>=0; --i) if(fan->ssenslist[i]==sens_id) return 0;
  fan->ssenslist[fan->nssensors]=sens_id;
  ++(fan->nssensors);
  return 0;
}

static inline int8_t del_fan_lm75a_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nlsensors-1; i>=0; --i) if(fan->lsenslist[i]==sens_id) break;

  if(i==-1) return -1;
  --(fan->nlsensors);

  memmove(fan->lsenslist+i, fan->lsenslist+i+1, fan->nlsensors);
  return 0;
}

static inline int8_t del_fan_analog_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nasensors-1; i>=0; --i) if(fan->asenslist[i]==sens_id) break;

  if(i==-1) return -1;
  --(fan->nasensors);

  memmove(fan->asenslist+i, fan->asenslist+i+1, fan->nasensors);
  return 0;
}

static inline int8_t del_fan_soft_temp_sensor(const uint8_t fan_id, const uint8_t sens_id)
{
  if(fan_id>=N_MAX_FANS) return -1;
  int8_t i;
  struct fan* const fan=fans+fan_id;

  for(i=fan->nssensors-1; i>=0; --i) if(fan->ssenslist[i]==sens_id) break;

  if(i==-1) return -1;
  --(fan->nssensors);

  memmove(fan->ssenslist+i, fan->ssenslist+i+1, fan->nssensors);
  return 0;
}

static inline int8_t add_fan_curve_point(const uint8_t fan_id, const int8_t temp, const uint8_t output)
{
  if(fan_id>=N_MAX_FANS) return -1;
  struct fan* const fan=fans+fan_id;

  if(fan->ncurvepoints==MAX_CURVE_NPOINTS) return -2;
  uint8_t i=binary_search(temp, fan->curve_temps, fan->ncurvepoints);

  if(i < fan->ncurvepoints) {

    if(fan->curve_temps[i]==temp) {
      fan->curve_outputs[i]=output;
      return 0;
    }
    ++(fan->ncurvepoints);
    memmove(fan->curve_temps+i+1, fan->curve_temps+i, fan->ncurvepoints-i);
    memmove(fan->curve_outputs+i+1, fan->curve_outputs+i, fan->ncurvepoints-i);

  } else ++(fan->ncurvepoints);
  fan->curve_temps[i]=temp;
  fan->curve_outputs[i]=output;
  return 0;
}

static inline int8_t del_fan_curve_point(const uint8_t fan_id, const uint8_t index)
{
  if(fan_id>=N_MAX_FANS) return -1;
  struct fan* const fan=fans+fan_id;

  if(index>=fan->ncurvepoints) return -2;

  if(fan->ncurvepoints==1) {
    fan->curve_outputs[0]=UINT8_MAX;
    return 0;
  }
  --(fan->ncurvepoints);
  memmove(fan->curve_temps+index, fan->curve_temps+index+1, fan->ncurvepoints);
  memmove(fan->curve_outputs+index, fan->curve_outputs+index+1, fan->ncurvepoints);
  return 0;
}

static inline int8_t get_fan_n_curve_points(const uint8_t fan_id)
{
  if(fan_id>=N_MAX_FANS) return -1;
  return fans[fan_id].ncurvepoints;
}

static inline int8_t get_fan_curve_point(const uint8_t fan_id, const uint8_t index, int8_t* const temp, uint8_t* const output)
{
  if(fan_id>=N_MAX_FANS) return -1;
  struct fan* const fan=fans+fan_id;

  if(index>=fan->ncurvepoints) return -2;
  *temp=fan->curve_temps[index];
  *output=fan->curve_outputs[index];
  return 0;
}

static inline int8_t get_fan_duty_cycle_response(const uint8_t id, uint16_t* const dc_no_out, int16_t* const ddcdout, int16_t* const d2dcdout2)
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

static inline int8_t set_fan_duty_cycle_response(const uint8_t id, const uint16_t dc_no_out, const int16_t ddcdout, const int16_t d2dcdout2)
{
  if(id>=N_MAX_FANS) return -1;

  fans[id].dcnoout = dc_no_out;
  fans[id].ddcdout = ddcdout;
  fans[id].d2dcdout2 = d2dcdout2;
  return 0;
}

static inline int8_t get_fan_voltage_response(const uint8_t id, uint16_t* const v_no_out, int16_t* const dvdout, int16_t* const d2vdout2)
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

static inline int8_t set_fan_voltage_response(const uint8_t id, const uint16_t v_no_out, const int16_t dvdout, const int16_t d2vdout2)
{
  if(id>=N_MAX_FANS) return -1;

  fans[id].vnoout = v_no_out;
  fans[id].dvdout = dvdout;
  fans[id].d2vdout2 = d2vdout2;
  return 0;
}

static inline int8_t get_fan_mode_transitions(const uint8_t id, uint8_t* const pwm_to_voltage_output, uint8_t* const voltage_to_pwm_output)
{
  if(id>=N_MAX_FANS) return -1;
  *pwm_to_voltage_output = fans[id].pwm_to_voltage_output;
  *voltage_to_pwm_output = fans[id].voltage_to_pwm_output;
  return 0;
}

static inline int8_t set_fan_mode_transitions(const uint8_t id, const uint8_t pwm_to_voltage_output, const uint8_t voltage_to_pwm_output)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].pwm_to_voltage_output = pwm_to_voltage_output;
  fans[id].voltage_to_pwm_output = voltage_to_pwm_output;
  return 0;
}

static inline uint16_t get_fan_adc_value(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return adc_getValue(id);
}

static inline uint8_t get_fan_output(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].output;
}

static inline uint8_t get_fan_mode(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].mode;
}

static inline uint8_t get_fan_hysterisis(const uint8_t id)
{
  if(id>=N_MAX_FANS) return UINT8_MAX;
  return fans[id].hysterisis;
}

static inline int8_t set_fan_hysterisis(const uint8_t id, const uint8_t hysterisis)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].hysterisis = hysterisis;
  return 0;
}

static inline int16_t get_fan_min_tach_ticks(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].min_tach_ticks;
}

static inline int8_t set_fan_min_tach_ticks(const uint8_t id, const int16_t min_tach_ticks)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].min_tach_ticks = min_tach_ticks;
  return 0;
}

static inline int16_t get_fan_max_tach_ticks(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return fans[id].max_tach_ticks;
}

static inline int8_t set_fan_max_tach_ticks(const uint8_t id, const int16_t max_tach_ticks)
{
  if(id>=N_MAX_FANS) return -1;
  fans[id].max_tach_ticks = max_tach_ticks;
  return 0;
}

static inline int16_t get_fan_tach_ticks(const uint8_t id)
{
  if(id>=N_MAX_FANS) return INT16_MAX;
  return fans[id].prev_tach_ticks;
}

static inline void update_fans(void)
{
  int8_t index;

  if(!overtemp_pass()) {

    for(index=N_MAX_FANS-1; index>=0; --index) switch_fan_mode(index, FAN_DISABLED_MODE);
    device_overtemp_cmd();
    request_buzz_alarm();
    return;
  }

  uint8_t id;
  int8_t s;
  update_temp_values();

  for(index=nfans-1; index>=0; --index) {
    id=fanlist[index];
    struct fan* const fan=fans+id;

    if(fan->prev_tach_ticks==-INT16_MAX) {
      switch_fan_mode(id, FAN_DISABLED_MODE);
      request_buzz_alarm();

    }

    //If auto mode
    if((fan->mode&FAN_AUTO_FLAG) != 0) {

      if(!((fan->mode&FAN_STARTING_FLAG) && abs(fan->prev_tach_ticks) > fan->max_tach_ticks)) {
	int8_t maxtemp=INT8_MIN;

	for(s=fan->nlsensors-1; s>=0; --s) {
	  int8_t temp = (lsensors[fan->lsenslist[s]].value>>8);

	  if(temp > maxtemp) maxtemp=temp;
	}

	for(s=fan->nasensors-1; s>=0; --s) {
	  int8_t temp = (asensors[fan->asenslist[s]].value>>8);

	  if(temp > maxtemp) maxtemp=temp;
	}

	for(s=fan->nssensors-1; s>=0; --s) {
	  int8_t temp = (ssensors_values[fan->ssenslist[s]]>>8);

	  if(temp > maxtemp) maxtemp=temp;
	}

	if(maxtemp == INT8_MIN) {
	  set_fan_output_auto(id, fan->curve_outputs[fan->ncurvepoints-1]);
	  fan->last_temp = LAST_TEMP_INVALID_VALUE;

	} else {

	  if(abs(maxtemp - fan->last_temp) > fan->hysterisis) {
	    fan->last_temp = maxtemp;

	    if(maxtemp >= fan->curve_temps[fan->ncurvepoints-1]) set_fan_output_auto(id, fan->curve_outputs[fan->ncurvepoints-1]); 

	    else if(maxtemp <= fan->curve_temps[0]) set_fan_output_auto(id, fan->curve_outputs[0]);

	    else {
	      uint8_t ret=1;
	      uint8_t diff=fan->ncurvepoints-2;
	      uint8_t mid;
	      uint8_t hdiff;

	      while(diff>0) {
		hdiff=diff>>1;
		mid=ret+hdiff;

		if(maxtemp>=fan->curve_temps[mid]) {
		  ret=++mid;
		  diff-=hdiff+1;

		} else diff=hdiff;
	      }
	      set_fan_output_auto(id, (uint8_t)(fan->curve_outputs[ret-1]+((int16_t)fan->curve_outputs[ret]-fan->curve_outputs[ret-1])*(maxtemp-fan->curve_temps[ret-1])/(fan->curve_temps[ret]-fan->curve_temps[ret-1])));
	    }
	  }
	}

      } else fan->last_temp=LAST_TEMP_INVALID_VALUE;

      //Else if not auto mode
    } else {

      if((fan->mode&FAN_STARTING_FLAG) && abs(fan->prev_tach_ticks) <= fan->max_tach_ticks) switch_fan_mode(id, fan->mode&(~FAN_STARTING_FLAG));
    }
  }
}
