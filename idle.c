#include <stdbool.h>
#include "idle.h"

volatile static bool idle_itr_set=false;

void idle_init(void)
{
  set_sleep_mode(SLEEP_MODE_IDLE);
  sbi(IDLE_TIMER_INTR_MASK_REG, IDLE_TIMER_ENABLE_COMPARE_MATCH_INTR);
}

void idle_delay_block_end(void)
{
    uint8_t oldSREG = SREG;
    cli();
    /*While the delay has not been triggered*/
    while(!idle_itr_set) {
	sleep_enable();
	SREG = oldSREG;
        sleep_cpu(); //Next instruction following setting the I bit is always executed before any interrupt, https://forum.arduino.cc/t/if-you-enable-interrupts-by-storing-to-sreg-is-the-next-instruction-executed/88918
        sleep_disable();
	oldSREG = SREG;
	cli();
    }
    idle_itr_set = false;
    IDLE_TIMER_CONTROL_REG = _BV(IDLE_TIMER_WAVEFORM_MODE_BIT_2) | TIMER_PRESCALER_OFF;
    SREG = oldSREG;
}

ISR(TIMER1_COMPA_vect)
{
  idle_itr_set = true;
}
