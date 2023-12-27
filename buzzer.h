#ifndef _BUZZER_
#define _BUZZER_

#include<stdbool.h>

#include "defines.h"
#include "timer.h"

extern bool buzz_requested;

#define buzz_signal(millis) ({\
    if(!read_pin(BUZZ)) {\
      set_pin(BUZZ, true);\
      idle_timer_delay_millis(1000);\
      set_pin(BUZZ, false);\
    }\
})

#define request_buzz_alarm() ({buzz_requested=true;})
#define reset_buzz_alarm() ({buzz_requested=false;})
#define update_buzz_alarm() (set_pin(BUZZ, buzz_requested))
#endif
