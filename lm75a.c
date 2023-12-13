#include "lm75a.h"

void lm75a_begin(void)
{
  twi_init();
}

bool lm75a_read16bitRegister(const uint8_t address, uint16_t* response)
{
  uint8_t reg=LM75A_REGISTER_TEMP;
  uint16_t data;

  if(twi_writeTo(address, &reg, 1, 1, true)) return false;

  if(twi_readFrom(address, (uint8_t*)&data, sizeof(data), true)!=sizeof(data)) return false;

  *response=data;
  return true;
}

int16_t lm75a_getValue(const uint8_t id)
{
  //Returned value is the temperature in Celsius * 256
  uint16_t value;

  if(id>=LM75A_MAX_SENSORS || !lm75a_read16bitRegister(LM75A_DEFAULT_ADDRESS+id, &value)) return LM75A_INVALID_VALUE;

  return (int16_t)be16toh(value);
}
