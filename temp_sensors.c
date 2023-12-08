#include "temp_sensors.h"

struct temp_sensor lsensors[LM75A_MAX_SENSORS];
struct temp_sensor asensors[MAX_ANALOG_SENSORS];
struct temp_sensor ssensors[N_MAX_SOFT_TEMP_SENSORS];

static uint8_t lsenslist[LM75A_MAX_SENSORS];
static uint8_t nlsensors=0U;
static uint8_t asenslist[MAX_ANALOG_SENSORS];
static uint8_t nasensors=0U;
static uint8_t ssenslist[N_MAX_SOFT_TEMP_SENSORS];
static uint8_t nssensors=0U;

static inline uint8_t get_temp_sensor_list(uint8_t const* const senslist, uint8_t const nsens)
{
  uint8_t ret=0;
  uint8_t i;

  for(i=0; i<nsens; ++i) {
    ret|=(1<<senslist[i]);
  }
  return ret;
}

static inline int8_t add_temp_sensor(const uint8_t id, const uint8_t nmaxids, struct temp_sensor* const allsensors, uint8_t* const senslist, uint8_t* const nsens)
{
  if(id>=nmaxids) return -1;
  uint8_t i;

  for(i=0; i<*nsens; ++i)
    if(senslist[i]==id) return 0;

  senslist[*nsens]=id;
  allsensors[*nsens].value=TEMP_SENSOR_INVALID_VALUE;
  allsensors[*nsens].a2=0;
  allsensors[*nsens].a1=(1<<14);
  allsensors[*nsens].a0=0;
  ++*nsens;
  return 1;
}

static inline int8_t del_temp_sensor(const uint8_t id, const uint8_t nmaxids, struct temp_sensor* const allsensors, uint8_t* const senslist, uint8_t* const nsens)
{
  if(id>=nmaxids) return -1;
  uint8_t i;

  for(i=0; i<*nsens; ++i)
    if(senslist[i]==id) break;

  if(i==*nsens) return -1;
  --*nsens;

  for(; i<*nsens; ++i) senslist[i]=senslist[i+1];
  return 0;
}

uint8_t get_lm75a_temp_sensor_list(void)
{
  return get_temp_sensor_list(lsenslist, nlsensors);
}

int8_t add_lm75a_temp_sensor(const uint8_t id)
{
  return add_temp_sensor(id, LM75A_MAX_SENSORS, lsensors, lsenslist, &nlsensors);
}

int8_t del_lm75a_temp_sensor(const uint8_t id)
{
  return del_temp_sensor(id, LM75A_MAX_SENSORS, lsensors, lsenslist, &nlsensors);
}

uint8_t get_analog_temp_sensor_list(void)
{
  return get_temp_sensor_list(asenslist, nasensors);
}

int8_t add_analog_temp_sensor(const uint8_t id)
{
  return add_temp_sensor(id, MAX_ANALOG_SENSORS, asensors, asenslist, &nasensors);
}

int8_t del_analog_temp_sensor(const uint8_t id)
{
  return del_temp_sensor(id, MAX_ANALOG_SENSORS, asensors, asenslist, &nasensors);
}

uint8_t get_soft_temp_sensor_list(void)
{
  return get_temp_sensor_list(ssenslist, nssensors);
}

int8_t add_soft_temp_sensor(const uint8_t id)
{
  return add_temp_sensor(id, N_MAX_SOFT_TEMP_SENSORS, ssensors, ssenslist, &nssensors);
}

int8_t del_soft_temp_sensor(const uint8_t id)
{
  return del_temp_sensor(id, N_MAX_SOFT_TEMP_SENSORS, ssensors, ssenslist, &nssensors);
}

static inline int8_t get_temp_sensor_calib(const uint8_t id, const uint8_t nmaxids, struct temp_sensor* const allsensors, int16_t* a0, int16_t* a1, int16_t* a2)
{
  if(id>=nmaxids) return -1;
  *a0=allsensors[id].a0;
  *a1=allsensors[id].a1;
  *a2=allsensors[id].a2;
  return 0;
}

int8_t get_lm75a_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2)
{
  return get_temp_sensor_calib(id, LM75A_MAX_SENSORS, lsensors, a2, a1, a0);
}

int8_t get_analog_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2)
{
  return get_temp_sensor_calib(id, MAX_ANALOG_SENSORS, asensors, a2, a1, a0);
}

static inline int8_t set_temp_sensor_calib(const uint8_t id, const uint8_t nmaxids, struct temp_sensor* const allsensors, const int16_t a0, const int16_t a1, const int16_t a2)
{
  if(id>=nmaxids) return -1;
  allsensors[id].a0=a0;
  allsensors[id].a1=a1;
  allsensors[id].a2=a2;
  return 0;
}

int8_t set_lm75a_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2)
{
  return set_temp_sensor_calib(id, LM75A_MAX_SENSORS, lsensors, a2, a1, a0);
}

int8_t set_analog_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2)
{
  return set_temp_sensor_calib(id, MAX_ANALOG_SENSORS, asensors, a2, a1, a0);
}

void update_temp_values(void)
{
  uint8_t i;
  int16_t value;

  for(i=0; i<nlsensors; ++i) {
    value=lm75a_getValue(lsenslist[i]);
    lsensors[lsenslist[i]].value=lsensors[lsenslist[i]].a0 + ((((int32_t)value)*(lsensors[lsenslist[i]].a1 + ((((int32_t)value) * lsensors[lsenslist[i]].a2)>>14)))>>14);
  }

  for(i=0; i<nasensors; ++i) {
    value=analog_sensor_getValue(asenslist[i]);
    asensors[asenslist[i]].value=asensors[asenslist[i]].a0 + ((((int32_t)value)*(asensors[asenslist[i]].a1 + ((((int32_t)value) * asensors[asenslist[i]].a2)>>14)))>>14);
  }
}

int8_t set_soft_temp_sensor_value(const uint8_t id, const int16_t value)
{
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -1;
      ssensors[id].value=value;
      return 0;
}
