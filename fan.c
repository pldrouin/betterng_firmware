#include "defines.h"
#include <math.h>

#include "fan.h"

#define MAX_FLOW_INVALID_VALUE (UINT16_MAX)
#define OFF_LEVEL_DEFAULT_VALUE (0x016D)
#define DIFF_LEVEL_DEFAULT_VALUE (OFF_LEVEL_DEFAULT_VALUE)
#define LAST_TEMP_INVALID_VALUE (INT16_MAX)

volatile struct fan fans[N_MAX_FANS];
uint8_t fanlist[N_MAX_FANS];
uint8_t nfans=0U;

volatile static uint8_t pwm_counter=0;

int8_t set_fan_output(const uint8_t id, const uint8_t output)
{
  if(id>=N_MAX_FANS) return -1;

  if(output>0U) {
    float voltage = FAN_VNOOUT_SCALE*fans[id].vnoout + output * (FAN_DVDOUT_SCALE * fans[id].dvdout + output * FAN_D2VDOUT2_SCALE * fans[id].d2vdout2);
    //fans[id].level = (uint16_t)(fans[id].off_level - (voltage>FAN_MAX_VOLTAGE_SCALE?FAN_MAX_VOLTAGE_SCALE:(voltage<0?0:voltage))/FAN_MAX_VOLTAGE_SCALE * fans[id].diff_level);
    fans[id].level = (uint16_t)(fans[id].off_level * (1. - (voltage>FAN_MAX_VOLTAGE_SCALE?FAN_MAX_VOLTAGE_SCALE:(voltage<0?0:voltage))/FAN_MAX_VOLTAGE_SCALE));

    if(fans[id].mode&DISABLED_MODE) switch_fan_control(id, fans[id].mode);

  //Completely turn off fan if desired output level is 0
  } else {
    fans[id].level = UINT16_MAX;
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

    set_fan_pin(TACH, id, false);
    set_fan_pin_as_input(TACH, id);

    set_fan_pin(RPM, id, false);
    set_fan_pin_as_output(RPM, id);

    //fans[id].max_flow=MAX_FLOW_INVALID_VALUE;
    fans[id].prev_tach_pwm_ticks=0L;
    fans[id].cur_tach_pwm_ticks=0L;
    fans[id].off_level=OFF_LEVEL_DEFAULT_VALUE;
    //fans[id].diff_level=DIFF_LEVEL_DEFAULT_VALUE;
    fans[id].vnoout=FAN_VNOOUT_DEFAULT_VALUE;
    fans[id].dvdout=FAN_DVDOUT_DEFAULT_VALUE;
    fans[id].d2vdout2=FAN_D2VDOUT2_DEFAULT_VALUE;
    //fans[id].direction=0;
    fans[id].mode=VOLTAGE_MODE;
    fans[id].nlsensors=0U;
    fans[id].nasensors=0U;
    fans[id].nssensors=0U;
    fans[id].last_temp=LAST_TEMP_INVALID_VALUE;
    fans[id].hysterisis=0U;
    fans[id].ncurvepoints=0U;
    set_fan_output(id, FAN_DEFAULT_OUTPUT_VALUE);
    fans[id].pwm_counter_offset=(uint8_t)(((FAN_MAX_OUTPUT_VALUE+1.)*id)/N_MAX_FANS);
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

int8_t set_fan_voltage_response(const uint8_t id, const float v_no_out, const float dvdout, const float d2vdout2)
{
  if(id>=N_MAX_FANS) return -1;

  if(v_no_out < 0 || dvdout <= 0) return -2;
  if(v_no_out+UINT8_MAX*(dvdout + UINT8_MAX*d2vdout2) > FAN_MAX_VOLTAGE_SCALE) return -3;
  if(ceil(v_no_out/FAN_VNOOUT_SCALE) > UINT16_MAX || ceil(dvdout/FAN_DVDOUT_SCALE) > UINT16_MAX || round(d2vdout2/FAN_D2VDOUT2_SCALE) > INT16_MAX) return -4;
  fans[id].vnoout = (uint16_t)ceil(v_no_out/FAN_VNOOUT_SCALE);
  fans[id].dvdout = (uint16_t)ceil(dvdout/FAN_DVDOUT_SCALE);
  fans[id].d2vdout2 = (int16_t)round(d2vdout2/FAN_D2VDOUT2_SCALE);
  return 0;
}

int8_t switch_fan_control(const uint8_t id, uint8_t mode)
{
  if(id>=N_MAX_FANS) return -1;
  mode&=MODE_MASK;

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
  int8_t i;
  //uint16_t on_level;

  switch_fan_control(id, MANUAL_MODE);
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

volatile uint16_t get_fan_rpm(const uint8_t id)
{
  if(id>=N_MAX_FANS) return 0;
  return convert_fan_rpm(fans[id].prev_tach_pwm_ticks);
}

ISR(TIMER2_COMP_vect)
{
  //Note: In PWM mode, a real tach signal is when the tach line is pulled low. A line
  //turning high following the end of the power pulse should be ignored.
  int8_t index;
  uint8_t id;

  ++pwm_counter;

  for(index=nfans; index>=0; --index) {
    id=fanlist[index];
    //toggle_fan_pin(RPM, id);

    //If the tach pin went low during the current cycle
    if(fans[id].cur_tach_pwm_ticks>0) {
      ++fans[id].cur_tach_pwm_ticks;

      if(fans[id].cur_tach_pwm_ticks == INT16_MAX) {
	fans[id].prev_tach_pwm_ticks = INT16_MAX;
	fans[id].cur_tach_pwm_ticks = 0;

	//If the tach pin just went high
      } else if(read_fan_pin(TACH, id)!=0) {
	fans[id].prev_tach_pwm_ticks = fans[id].cur_tach_pwm_ticks;
	fans[id].cur_tach_pwm_ticks = 0;
	set_fan_pin(RPM, id, true);
      }

      //Else if the tach pin has not gone low yet
    } else {
      --fans[id].cur_tach_pwm_ticks;

      //If the tach has not changed state and the counter is overflowing
      if(fans[id].cur_tach_pwm_ticks == -INT16_MAX) {
	fans[id].prev_tach_pwm_ticks = INT16_MIN;
	fans[id].cur_tach_pwm_ticks = 0;

	//If the trigger just went low, and not because PWM is active and the
	//PWM output is low, flip its sign
      } else if(read_fan_pin(TACH, id)==0 && ((fans[id].mode&PWM_MODE)==0 || read_fan_pin(PWM, id))) {
	fans[id].cur_tach_pwm_ticks = -fans[id].cur_tach_pwm_ticks;
	set_fan_pin(RPM, id, false);
      }
    }

    //If fan is running in PWM mode
    if(fans[id].mode&PWM_MODE) {

      if(pwm_counter+fans[id].pwm_counter_offset >= fans[id].output) set_fan_pin(PWM, id, false);

      else set_fan_pin(PWM, id, true);
    }
  }
}
