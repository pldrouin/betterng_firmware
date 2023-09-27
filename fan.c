#include "fan.h"

#define MAX_CURVE_NPOINTS (6)

struct fan
{
  uint16_t id;
  uint16_t max_flow;
  uint16_t min_tach_period; //Minimum fan tach period (inverse of max speed)
  uint16_t off_level; //Measured ADC voltage when MOSFET does not conduct (channel specific)
  int16_t vnoout;     //Voltage when fan stops
  int16_t dvdout;     //Voltage derivative with output 
  int16_t d2vdout2;   //Second voltage derivative with output 
  int8_t direction;   //1=positive pressure, -1=negative pressure, 0=no pressure
  bool pwm;
  uint8_t sensors[N_MAX_TEMP_SENSORS];
  uint8_t nsensors;
  int16_t last_temp;
  int16_t hysterisis;
  int8_t curve_temps[MAX_CURVE_NPOINTS];
  uint8_t curve_outputs[MAX_CURVE_NPOINTS];
};

struct fan fans[N_MAX_FANS];
uint8_t nfans=0;

void initfans(void)
{
  int8_t fi;

  for(fi=N_MAX_FANS-1; fi>=0; --fi) {
    FAN_ADC_DDR &= ~_BV(FAN_ADC_FIRST_NO+fi); //Set ADC as input
    //FAN_PWM_PORT &= ~_BV(FAN_PWM_FIRST_NO+fi); //Disable PWM pull-up/high output
    //FAN_PWM_DDR |= _BV(FAN_PWM_FIRST_NO+fi); //Set PWM as output
    //FAN_DC_PORT &= ~_BV(FAN_DC_FIRST_NO+fi); //Disable DC pull-up/high output
    //FAN_DC_DDR |= _BV(FAN_DC_FIRST_NO+fi); //Set DC as output
  }
}
