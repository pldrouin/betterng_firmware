#include "lm75a.h"

static uint8_t ids[LM75A_MAX_SENSORS];
static uint8_t nsensors=0;

void lm75a_begin(void)
{
  twi_init();
}

int8_t lm75a_add_sensor(const uint8_t id)
{
  if(nsensors==LM75A_MAX_SENSORS || id>=LM75A_MAX_SENSORS) return -1;

  uint8_t i;

  for(i=0; i<nsensors; ++i)

    if(ids[i]==id) return -1;

  ids[nsensors]=id;
  return nsensors++;
}

int8_t lm75a_del_sensor(const uint8_t index)
{
  if(index>=nsensors) return -1;

  int8_t i;
  --nsensors;

  for(i=index; i<nsensors; ++i) ids[i]=ids[i+1];
  return 0;
}

uint8_t lm75a_get_nsensors(void)
{
  return nsensors;
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

int16_t lm75a_getValue(const uint8_t index)
{
  //Returned value is the temperature in Celsius * 256
  int16_t value;

  if(index>=nsensors || !lm75a_read16bitRegister(LM75A_DEFAULT_ADDRESS+ids[index], (uint16_t*)&value)) return LM75A_INVALID_VALUE;

  return be16toh(value)/128;
}
