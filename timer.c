#include <stdbool.h>
#include "timer.h"

volatile static bool timer_itr_set=false;

void timer_init(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sbi(TIMER_INTR_MASK_REG, TIMER_ENABLE_COMPARE_MATCH_INTR);
}

bool timer_delay_verify(void)
{
  if(timer_itr_set) {
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    timer_itr_set = false;
    return true;
  }
  return false;
}

bool timer_delay_end(void)
{
  if(timer_itr_set) {
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    timer_itr_set = false;
    return true;
  }
  TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
  timer_itr_set = false;
  return false;
}

void idle_timer_delay_apply(void)
{
    uint8_t oldSREG = SREG;
    cli();
    /*While the delay has not been triggered*/
    while(!timer_itr_set) {
	sleep_enable();
	SREG = oldSREG;
        sleep_cpu(); //Next instruction following setting the I bit is always executed before any interrupt, https://forum.arduino.cc/t/if-you-enable-interrupts-by-storing-to-sreg-is-the-next-instruction-executed/88918
        sleep_disable();
	oldSREG = SREG;
	cli();
    }
    SREG = oldSREG;
    TIMER_CONTROL_REG = _BV(TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    timer_itr_set = false;
}

ISR(TIMER1_COMPA_vect) {
  timer_itr_set = true;
}
