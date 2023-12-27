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

static inline void lm75a_begin(void)
{
  twi_init();
}

static inline bool lm75a_read16bitRegister(const uint8_t address, uint16_t* response)
{
  uint8_t reg=LM75A_REGISTER_TEMP;
  uint16_t data;

  if(twi_writeTo(address, &reg, 1, 1, true)) return false;

  if(twi_readFrom(address, (uint8_t*)&data, sizeof(data), true)!=sizeof(data)) return false;

  *response=data;
  return true;
}

static inline int16_t lm75a_getValue(const uint8_t id)
{
  //Returned value is the temperature in Celsius * 256
  uint16_t value;

  if(id>=LM75A_MAX_SENSORS || !lm75a_read16bitRegister(LM75A_DEFAULT_ADDRESS+id, &value)) return LM75A_INVALID_VALUE;

  return (int16_t)be16toh(value);
}
#endif
