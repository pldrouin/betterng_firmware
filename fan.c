#include "fan.h"

void initfans(void)
{
  int8_t fi;

  for(fi=NFANS-1; fi>=0; --fi) {
    FAN_ADC_DDR &= ~_BV(FAN_ADC_FIRST_NO+fi); //Set ADC as input
    //FAN_PWM_PORT &= ~_BV(FAN_PWM_FIRST_NO+fi); //Disable PWM pull-up/high output
    //FAN_PWM_DDR |= _BV(FAN_PWM_FIRST_NO+fi); //Set PWM as output
    //FAN_DC_PORT &= ~_BV(FAN_DC_FIRST_NO+fi); //Disable DC pull-up/high output
    //FAN_DC_DDR |= _BV(FAN_DC_FIRST_NO+fi); //Set DC as output
  }
}
