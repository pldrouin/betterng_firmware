#include<stdint.h>
#include<avr/eeprom.h>

#include "eeprom.h"
#include "fan.h"

#define EEPROM_DATA_VERSION (0x00)
#define EEPROM_DATA_SIZE (5 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + (LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS)*(TEMP_SENSOR_DATA_SAVED_SIZE+1)+N_MAX_SOFT_TEMP_SENSORS)

#if EEPROM_DATA_SIZE > EEPROM_SIZE
#error Attempting to write too much data to EEPROM!
#endif

#define EEPROM_FREE_SPACE (EEPROM_SIZE - EEPROM_DATA_SIZE)
#define EEPROM_N_DATA_SLOTS (EEPROM_DATA_SIZE - 1 - EEPROM_FREE_SPACE)
#define EEPROM_FREE_SPACE_START (2+(EEPROM_DATA_VERSION%EEPROM_N_DATA_SLOTS))
#define EEPROM_FREE_SPACE_OVER (EEPROM_FREE_SPACE_START+EEPROM_FREE_SPACE)

static inline int eeprom_gentle_write_byte(uint8_t *addr, uint8_t value)
{
  if(addr >= (uint8_t*)EEPROM_FREE_SPACE_START) addr += EEPROM_FREE_SPACE;

  if(eeprom_read_byte(addr) != value) eeprom_write_byte(addr, value);

  if(eeprom_read_byte(addr) != value) return -1;
  return 0;
}

static inline int eeprom_gentle_write_block(const void *pointer_ram, size_t eeprom_byte, size_t n)
{
  size_t i;

  if(eeprom_byte >= (size_t)EEPROM_FREE_SPACE_START) {
    eeprom_byte += EEPROM_FREE_SPACE;

    for(i=0; i<n; ++i) {
      uint8_t* addr=(uint8_t*)(eeprom_byte+i);
      uint8_t value=*((uint8_t*)pointer_ram+i);

      if(eeprom_read_byte(addr) != value) eeprom_write_byte(addr, value);

      if(eeprom_read_byte(addr) != value) return -1;
    }

  } else if(eeprom_byte+n <= (size_t)EEPROM_FREE_SPACE_START) {

    for(i=0; i<n; ++i) {
      uint8_t* addr=(uint8_t*)(eeprom_byte+i);
      uint8_t value=*((uint8_t*)pointer_ram+i);

      if(eeprom_read_byte(addr) != value) eeprom_write_byte(addr, value);

      if(eeprom_read_byte(addr) != value) return -1;
    }

  } else {

    for(i=eeprom_byte; i<EEPROM_FREE_SPACE_START; ++i) {
      uint8_t value=*((uint8_t*)pointer_ram+i-eeprom_byte);

      if(eeprom_read_byte((uint8_t*)i) != value) eeprom_write_byte((uint8_t*)i, value);

      if(eeprom_read_byte((uint8_t*)i) != value) return -1;
    }

    for(i=EEPROM_FREE_SPACE_START-eeprom_byte; i<n; ++i) {
      size_t j=EEPROM_FREE_SPACE_OVER+i;
      uint8_t value=*((uint8_t*)pointer_ram+i);

      if(eeprom_read_byte((uint8_t*)j) != value) eeprom_write_byte((uint8_t*)j, value);

      if(eeprom_read_byte((uint8_t*)j) != value) return -1;
    }
  }
  return 0;
}

static inline uint8_t eeprom_gentle_read_byte(uint8_t* addr)
{
  return eeprom_read_byte((addr >= (uint8_t*)EEPROM_FREE_SPACE_START)?addr+EEPROM_FREE_SPACE:addr);
}

static inline void eeprom_gentle_read_block(void *pointer_ram, size_t eeprom_byte, size_t n)
{
  if(eeprom_byte >= (size_t)EEPROM_FREE_SPACE_START) {
    eeprom_byte += EEPROM_FREE_SPACE;
    eeprom_read_block(pointer_ram, (void*)eeprom_byte, n);

  } else if(eeprom_byte+n <= (size_t)EEPROM_FREE_SPACE_START) {
    eeprom_read_block(pointer_ram, (void*)eeprom_byte, n);

  } else {
    const size_t nprior=EEPROM_FREE_SPACE_START-eeprom_byte;
    eeprom_read_block(pointer_ram, (void*)eeprom_byte, nprior);
    eeprom_read_block(pointer_ram+nprior, (void*)EEPROM_FREE_SPACE_OVER, n-nprior);
  }
}

static inline int eeprom_load(void)
{
  uint16_t version;

  version=eeprom_read_byte((uint8_t*)0)|(eeprom_read_byte((uint8_t*)1)<<8);

  if(version != EEPROM_DATA_VERSION) {
    buzz_signal(1000);
    return -1;
  }

  uint8_t i;

  for(i=0; i<N_MAX_FANS; ++i) {
    eeprom_gentle_read_block(fans+i, (2 + i*FAN_DATA_SAVED_SIZE), FAN_DATA_SAVED_SIZE);

    if(!(fans[i].mode&FAN_AUTO_FLAG)) set_fan_output(i, fans[i].output);
  }

  nfans = eeprom_gentle_read_byte((uint8_t*)(2 + N_MAX_FANS*FAN_DATA_SAVED_SIZE));

  eeprom_gentle_read_block(fanlist, (3 + N_MAX_FANS*FAN_DATA_SAVED_SIZE), nfans);

  for(i=0; i<LM75A_MAX_SENSORS; ++i) eeprom_gentle_read_block(lsensors+i, (3 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + i*TEMP_SENSOR_DATA_SAVED_SIZE), TEMP_SENSOR_DATA_SAVED_SIZE);

  nlsensors = eeprom_gentle_read_byte((uint8_t*)(3 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE));

  eeprom_gentle_read_block(lsenslist, (4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nlsensors);

  for(i=0; i<MAX_ANALOG_SENSORS; ++i) eeprom_gentle_read_block(asensors+i, (4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + i*TEMP_SENSOR_DATA_SAVED_SIZE), TEMP_SENSOR_DATA_SAVED_SIZE);

  nasensors = eeprom_gentle_read_byte((uint8_t*)(4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + MAX_ANALOG_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE));

  eeprom_gentle_read_block(asenslist, (5 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + MAX_ANALOG_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nasensors);

  eeprom_gentle_read_block(ssensors_alarm_values, (5 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + (LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS)*(TEMP_SENSOR_DATA_SAVED_SIZE+1)), N_MAX_SOFT_TEMP_SENSORS*2);

  return 0;
}

static inline int eeprom_save(void)
{
#define eeprom_save_exit(err) ({error=err; goto eeprom_save_catch;});
  uint8_t i;
  int8_t error=0;
  watchdogConfig(WATCHDOG_OFF);

  if(eeprom_gentle_write_byte((uint8_t*)0, (uint8_t)(EEPROM_DATA_VERSION))) eeprom_save_exit(-1);

  if(eeprom_gentle_write_byte((uint8_t*)1, (uint8_t)(EEPROM_DATA_VERSION>>8))) eeprom_save_exit(-2);

  for(i=0; i<N_MAX_FANS; ++i) {
    //Excludes output at first
    if(eeprom_gentle_write_block(fans+i, (2 + i*FAN_DATA_SAVED_SIZE), FAN_DATA_SAVED_SIZE-1)) eeprom_save_exit(-3);
    //Only saves output if custom mode
    
    if(!(fans[i].mode&FAN_AUTO_FLAG) && eeprom_gentle_write_byte((uint8_t*)(1 + (i+1)*FAN_DATA_SAVED_SIZE), fans[i].output)) eeprom_save_exit(-4);
  }

  if(eeprom_gentle_write_byte((uint8_t*)(2 + N_MAX_FANS*FAN_DATA_SAVED_SIZE), nfans)) eeprom_save_exit(-5);

  if(eeprom_gentle_write_block(fanlist, (3 + N_MAX_FANS*FAN_DATA_SAVED_SIZE), nfans)) eeprom_save_exit(-6);

  for(i=0; i<LM75A_MAX_SENSORS; ++i) if(eeprom_gentle_write_block(lsensors+i, (3 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + i*TEMP_SENSOR_DATA_SAVED_SIZE), TEMP_SENSOR_DATA_SAVED_SIZE)) eeprom_save_exit(-7);

  if(eeprom_gentle_write_byte((uint8_t*)(3 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nlsensors)) eeprom_save_exit(-8);

  if(eeprom_gentle_write_block(lsenslist, (4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nlsensors)) eeprom_save_exit(-9);

  for(i=0; i<MAX_ANALOG_SENSORS; ++i) if(eeprom_gentle_write_block(asensors+i, (4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + i*TEMP_SENSOR_DATA_SAVED_SIZE), TEMP_SENSOR_DATA_SAVED_SIZE)) eeprom_save_exit(-10);

  if(eeprom_gentle_write_byte((uint8_t*)(4 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + MAX_ANALOG_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nasensors)) eeprom_save_exit(-11);

  if(eeprom_gentle_write_block(asenslist, (5 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + LM75A_MAX_SENSORS*(TEMP_SENSOR_DATA_SAVED_SIZE+1) + MAX_ANALOG_SENSORS*TEMP_SENSOR_DATA_SAVED_SIZE), nasensors)) eeprom_save_exit(-12);

  if(eeprom_gentle_write_block(ssensors_alarm_values, (5 + N_MAX_FANS*(FAN_DATA_SAVED_SIZE+1) + (LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS)*(TEMP_SENSOR_DATA_SAVED_SIZE+1)), N_MAX_SOFT_TEMP_SENSORS*2)) eeprom_save_exit(-13);

eeprom_save_catch:
  watchdogReset();
  watchdogConfig(WATCHDOG_1S);
  return error;
}
