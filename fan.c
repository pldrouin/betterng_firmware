#include "defines.h"
#include <math.h>
#include <stdlib.h>

#include "fan.h"

struct fan fans[N_MAX_FANS];
uint8_t fanlist[N_MAX_FANS];
uint8_t nfans=0U;

int8_t set_fan_output(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  struct fan* const fan=fans+id;

  if(output>0U) {
#define FAN_VOLTAGE ((uint16_t)(fan->vnoout + (int16_t)((((int32_t)output)*fan->dvdout)>>8) + (int16_t)((((int32_t)output)*fan->d2vdout2*output)>>16)))
    fan->level = (int16_t)(FAN_OFF_LEVEL_DEFAULT_VALUE - ((((int32_t)FAN_OFF_LEVEL_DEFAULT_VALUE)*FAN_VOLTAGE)>>FAN_MAX_VOLTAGE_SCALE_SHIFT));
    //Assertion: level <= off_level

    fan->duty_cycle = (uint8_t)((fan->dcnoout + (int16_t)((((int32_t)output)*fan->ddcdout)>>8) + (int16_t)((((int32_t)output)*fan->d2dcdout2*output)>>16))>>6);

    if(fan->mode & FAN_OFF_FLAG) {
      fan->prev_tach_ticks=INT16_MAX;
      fan->cur_tach_ticks = 1;
      fan->prev_tach_osc = 0;
      fan->mode = (fan->mode&(~FAN_OFF_FLAG))|FAN_STARTING_FLAG;
      set_fan_pin(PWM, id, true);
    }

  //Completely turn off fan if desired output level is 0
  } else {
    fan->mode|=FAN_OFF_FLAG;
    fan->level = INT16_MAX;
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(PWM, id);
    set_fan_pin(PWM, id, false);
    fan->prev_tach_ticks=INT16_MAX;
  }
  fan->output = output;
  return 0;
}

int8_t set_fan_output_auto(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  struct fan* const fan=fans+id;

  if(output>0U) {

    if(output < fan->voltage_to_pwm_output && output >= fan->pwm_to_voltage_output) {
      fan->level = (int16_t)(FAN_OFF_LEVEL_DEFAULT_VALUE - ((((int32_t)FAN_OFF_LEVEL_DEFAULT_VALUE)*FAN_VOLTAGE)>>FAN_MAX_VOLTAGE_SCALE_SHIFT));
      //Assertion: level <= off_level

      if(fan->mode & FAN_OFF_FLAG) {
        fan->prev_tach_ticks=INT16_MAX;
	fan->cur_tach_ticks = 1;
	fan->prev_tach_osc = 0;
	set_fan_pin(PWM, id, true);
	fan->mode = (fan->mode&(~FAN_OFF_FLAG))|FAN_STARTING_FLAG;

      } else switch_fan_control(id, FAN_VOLTAGE_MODE|FAN_AUTO_FLAG);

    } else {

      fan->duty_cycle = (uint8_t)((fan->dcnoout + (int16_t)((((int32_t)output)*fan->ddcdout)>>8) + (int16_t)((((int32_t)output)*fan->d2dcdout2*output)>>16))>>6);

      if(fan->mode & FAN_OFF_FLAG) {
        fan->prev_tach_ticks=INT16_MAX;
	fan->cur_tach_ticks = 1;
	fan->prev_tach_osc = 0;
	set_fan_pin(PWM, id, true);
	fan->mode = (fan->mode&(~FAN_OFF_FLAG))|FAN_STARTING_FLAG;

      } else switch_fan_control(id, FAN_PWM_MODE|FAN_AUTO_FLAG);
    }

  //Completely turn off fan if desired output level is 0
  } else {
    fan->mode|=FAN_OFF_FLAG;
    fan->level = INT16_MAX;
    set_fan_pin(DC, id, false);
    set_fan_pin_as_output(PWM, id);
    set_fan_pin(PWM, id, false);
    fan->prev_tach_ticks=INT16_MAX;
  }
  fan->output = output;
  return 0;
}

int8_t switch_fan_control(const uint8_t id, uint8_t mode)
{
  if(id>=N_MAX_FANS) return -1;

  if(mode != fans[id].mode) {
    fans[id].mode=mode;

    switch(mode&FAN_MODE_MASK) {
      case FAN_VOLTAGE_MODE:
	set_fan_pin(DC, id, false);
	set_fan_pin_as_input(PWM, id);
	set_fan_pin(PWM, id, false);
	break;

      case FAN_PWM_MODE:
      case FAN_DISABLED_MODE:
	set_fan_pin(DC, id, false);
	set_fan_pin_as_output(PWM, id);
	set_fan_pin(PWM, id, false);
	break;

      case FAN_CUSTOM_MODE:
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

    if(fan->mode&(FAN_DISABLED_MODE|FAN_OFF_FLAG)) continue;

    ++(fan->cur_tach_phase);
    
    if(fan->cur_tach_phase > abs(fan->prev_tach_ticks)) {
      toggle_fan_pin(RPM, id);
      fan->cur_tach_phase-=abs(fan->prev_tach_ticks);
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
	  if(fan->cur_tach_ticks < FAN_TACH_MEASUREMENT_MAX_TICKS && fan->prev_tach_osc<UINT8_MAX) {
	    ++(fan->cur_tach_ticks);
	    ++(fan->prev_tach_osc);
	    fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | (fan->last_fan_status&FAN_TACH_ACCURATE_RPM);

	    //Else if we cannot wait for another oscillation
	  } else {
	    //Corrects the period using the average expected time for the
	    //tach change
	    fan->prev_tach_ticks = -(int16_t)((fan->cur_tach_ticks - ((UINT8_MAX - fan->duty_cycle)>>1)) / (fan->prev_tach_osc + 1));
	    fan->cur_tach_ticks = 1 + ((UINT8_MAX - fan->duty_cycle)>>1);
	    fan->prev_tach_osc = 0;
	    fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP;
	  }

	  //Else if the power was previously active, then the end of the tach signal is accurate
	} else {

	  if(fan->last_fan_status&FAN_TACH_ACCURATE_RPM) fan->prev_tach_ticks = fan->cur_tach_ticks / (fan->prev_tach_osc + 1);

	  else fan->prev_tach_ticks = -(int16_t)(fan->cur_tach_ticks / (fan->prev_tach_osc + 1));
	  fan->cur_tach_ticks = 1;
	  fan->prev_tach_osc = 0;
	  fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | FAN_TACH_ACCURATE_RPM;
	}

	//Else if the tach pin did not change since the last time power was active
      } else {

	if(fan->cur_tach_ticks >= FAN_TACH_NO_TICK_TIMEOUT) {
	  fan->prev_tach_ticks = -INT16_MAX;
	  fan->cur_tach_ticks = 1;
	  fan->prev_tach_osc = 0;

	} else ++(fan->cur_tach_ticks);
	fan->last_fan_status = ((uint8_t)cur_tach_status) | FAN_LAST_POWER_UP | (fan->last_fan_status&FAN_TACH_ACCURATE_RPM);
      }

      //Else if the fan power is not active
    } else {
      
      //if((fan->cur_tach_ticks%abs(fan->prev_tach_ticks) == 0)
      //  toggle_fan_pin(RPM, id);

      if(fan->cur_tach_ticks >= FAN_TACH_NO_TICK_TIMEOUT) {
	fan->prev_tach_ticks = -INT16_MAX;
	fan->cur_tach_ticks = 1;
	fan->prev_tach_osc = 0;
	fan->last_fan_status = 0;

      } else {
	++(fan->cur_tach_ticks);
	fan->last_fan_status &= ~FAN_LAST_POWER_UP;
      }
    }

    //If fan is running in PWM mode
    if((fan->mode&FAN_MODE_MASK) == FAN_PWM_MODE) {

      if((uint8_t)(++pwm_counter+fan->pwm_counter_offset) > fan->duty_cycle) set_fan_pin(PWM, id, false);

      else set_fan_pin(PWM, id, true);
    }
  }
}
