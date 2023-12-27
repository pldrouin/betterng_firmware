#ifndef _BUZZER_
#define _BUZZER_

#include "defines.h"
#include "timer.h"

#define buzz_signal(millis) ({\
    if(!read_pin(BUZZ)) {\
      set_pin(BUZZ, true);\
      idle_timer_delay_millis(1000);\
      set_pin(BUZZ, false);\
    }\
})

#define buzz_alarm() (set_pin(BUZZ, true))
#define silence_alarm() (set_pin(BUZZ, false))
#endif
