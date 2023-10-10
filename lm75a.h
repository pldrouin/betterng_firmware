#ifndef _LM75A_
#define _LM75A_

#include <stdint.h>
#include "defines.h"
#include "utility/twi.h"

#define LM75A_DEFAULT_ADDRESS           0x48            // Address is configured with pins A0-A2

#define LM75A_REGISTER_TEMP                     0                       // Temperature register (read-only)
#define LM75A_REGISTER_CONFIG           1                       // Configuration register
#define LM75A_REGISTER_THYST            2                       // Hysterisis register
#define LM75A_REGISTER_TOS                      3                       // OS register
#define LM75A_REGISTER_PRODID           7                       // Product ID register - Only valid for Texas Instruments

#define LM75_CONF_OS_COMP_INT           1                       // OS operation mode selection
#define LM75_CONF_OS_POL                        2                       // OS polarity selection
#define LM75_CONF_OS_F_QUE                      3                       // OS fault queue programming

#define LM75A_INVALID_VALUE       32767L        // Just an arbritary value outside of the sensor limits

#define LM75A_MAX_SENSORS	8U

void lm75a_begin(void);
int16_t lm75a_getValue(const uint8_t id);

#endif
