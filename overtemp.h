#ifndef _OVERTEMP_
#define _OVERTEMP_

#include "defines.h"

static inline void initovertemp(void)
{
    set_pin_as_input(OVERTEMP);
    set_pin(OVERTEMP, false);
}

static inline bool overtemp_pass(void)
{
  if(!read_pin(OVERTEMP)) return false;
  return true;
}

#endif
