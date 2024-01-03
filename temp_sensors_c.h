#include "cmd.h"

static inline void init_temp_sensor_data(void)
{
  uint8_t i;
  struct temp_sensor* sens;

  for(i=0; i<LM75A_MAX_SENSORS; ++i) {
    sens=lsensors+i;
    sens->a2=0;
    sens->a1=(1<<14);
    sens->a0=0;
    sens->alarm_value=TEMP_SENSOR_DEFAULT_ALARM_VALUE;
    sens->value=TEMP_SENSOR_INVALID_VALUE;
  }

  for(i=0; i<MAX_ANALOG_SENSORS; ++i) {
    sens=asensors+i;
    sens->a2=0;
    sens->a1=(1<<14);
    sens->a0=0;
    sens->alarm_value=TEMP_SENSOR_DEFAULT_ALARM_VALUE;
    sens->value=TEMP_SENSOR_INVALID_VALUE;
  }

  for(i=0; i<N_MAX_SOFT_TEMP_SENSORS; ++i) {
    ssensors_alarm_values[i]=TEMP_SENSOR_DEFAULT_ALARM_VALUE;
    ssensors_values[i]=-INT16_MAX;
  }
}

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

  for(i=0; i<*nsens; ++i) if(senslist[i]==id) return 0;

  senslist[*nsens]=id;
  struct temp_sensor* sens=allsensors+id;
  sens->a2=0;
  sens->a1=(1<<14);
  sens->a0=0;
  sens->alarm_value=TEMP_SENSOR_DEFAULT_ALARM_VALUE;
  sens->value=TEMP_SENSOR_INVALID_VALUE;
  ++*nsens;
  return 0;
}

static inline int8_t del_temp_sensor(const uint8_t id, const uint8_t nmaxids, uint8_t* const senslist, uint8_t* const nsens)
{
  if(id>=nmaxids) return -1;
  uint8_t i;

  for(i=0; i<*nsens; ++i) if(senslist[i]==id) break;

  if(i==*nsens) return -1;
  --*nsens;

  for(; i<*nsens; ++i) senslist[i]=senslist[i+1];
  return 0;
}

static inline uint8_t get_lm75a_temp_sensor_list(void)
{
  return get_temp_sensor_list(lsenslist, nlsensors);
}

static inline int8_t add_lm75a_temp_sensor(const uint8_t id)
{
  return add_temp_sensor(id, LM75A_MAX_SENSORS, lsensors, lsenslist, &nlsensors);
}

static inline int8_t del_lm75a_temp_sensor(const uint8_t id)
{
  return del_temp_sensor(id, LM75A_MAX_SENSORS, lsenslist, &nlsensors);
}

static inline uint8_t get_analog_temp_sensor_list(void)
{
  return get_temp_sensor_list(asenslist, nasensors);
}

static inline int8_t add_analog_temp_sensor(const uint8_t id)
{
  return add_temp_sensor(id, MAX_ANALOG_SENSORS, asensors, asenslist, &nasensors);
}

static inline int8_t del_analog_temp_sensor(const uint8_t id)
{
  return del_temp_sensor(id, MAX_ANALOG_SENSORS, asenslist, &nasensors);
}

static inline int16_t get_lm75a_temp_sensor_value(const uint8_t id)
{
  if(id>=LM75A_MAX_SENSORS) return TEMP_SENSOR_INVALID_VALUE;
  return lsensors[id].value;
}

static inline int16_t get_analog_temp_sensor_value(const uint8_t id)
{
  if(id>=MAX_ANALOG_SENSORS) return TEMP_SENSOR_INVALID_VALUE;
  return asensors[id].value;
}

static inline int16_t get_soft_temp_sensor_value(const uint8_t id)
{
  if(id>=N_MAX_SOFT_TEMP_SENSORS) return TEMP_SENSOR_INVALID_VALUE;
  return ssensors_values[id];
}

static inline int8_t get_temp_sensor_calib(const uint8_t id, const uint8_t nmaxids, struct temp_sensor* const allsensors, int16_t* a0, int16_t* a1, int16_t* a2)
{
  if(id>=nmaxids) return -1;
  *a0=allsensors[id].a0;
  *a1=allsensors[id].a1;
  *a2=allsensors[id].a2;
  return 0;
}

static inline int8_t get_lm75a_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2)
{
  return get_temp_sensor_calib(id, LM75A_MAX_SENSORS, lsensors, a2, a1, a0);
}

static inline int8_t get_analog_temp_sensor_calib(const uint8_t id, int16_t* a0, int16_t* a1, int16_t* a2)
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

static inline int8_t set_lm75a_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2)
{
  return set_temp_sensor_calib(id, LM75A_MAX_SENSORS, lsensors, a2, a1, a0);
}

static inline int8_t set_analog_temp_sensor_calib(const uint8_t id, const int16_t a0, const int16_t a1, const int16_t a2)
{
  return set_temp_sensor_calib(id, MAX_ANALOG_SENSORS, asensors, a2, a1, a0);
}

static inline int8_t set_soft_temp_sensor_value(const uint8_t id, const int16_t value)
{
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -1;
      ssensors_values[id]=value;
      return 0;
}

static inline int16_t get_lm75a_temp_sensor_alarm_value(const uint8_t id)
{
      if(id>=LM75A_MAX_SENSORS) return -INT16_MAX;
      return lsensors[id].alarm_value;
}

static inline int16_t get_analog_temp_sensor_alarm_value(const uint8_t id)
{
      if(id>=MAX_ANALOG_SENSORS) return -INT16_MAX;
      return asensors[id].alarm_value;
}

static inline int16_t get_soft_temp_sensor_alarm_value(const uint8_t id)
{
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -INT16_MAX;
      return ssensors_alarm_values[id];
}

static inline int8_t set_lm75a_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value)
{
      if(id>=LM75A_MAX_SENSORS) return -1;
      lsensors[id].alarm_value = alarm_value;
      return 0;
}

static inline int8_t set_analog_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value)
{
      if(id>=MAX_ANALOG_SENSORS) return -1;
      asensors[id].alarm_value = alarm_value;
      return 0;
}

static inline int8_t set_soft_temp_sensor_alarm_value(const uint8_t id, const int16_t alarm_value)
{
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -1;
      ssensors_alarm_values[id] = alarm_value;
      return 0;
}

static inline void update_temp_values(void)
{
  uint8_t i;
  int16_t value;
  bool alarm=false;

  for(i=0; i<nlsensors; ++i) {
    const uint8_t id=lsenslist[i];
    value=lm75a_get_value(id);
    struct temp_sensor* const sens=lsensors+id;
    sens->value=sens->a0 + ((((int32_t)value)*(sens->a1 + ((((int32_t)value) * sens->a2)>>14)))>>14);

    if(sens->value >= sens->alarm_value) {
      alarm=true;
      lm75a_temp_sensor_alarm_cmd(i);
    }
  }

  for(i=0; i<nasensors; ++i) {
    const uint8_t id=asenslist[i];
    value=analog_sensor_get_ADC_value(id);
    struct temp_sensor* const sens=asensors+id;
    sens->value=sens->a0 + ((((int32_t)value)*(sens->a1 + ((((int32_t)value) * sens->a2)>>14)))>>14);

    if(sens->value >= sens->alarm_value) {
      analog_temp_sensor_alarm_cmd(i);
      alarm=true;
    }
  }

  for(i=0; i<N_MAX_SOFT_TEMP_SENSORS; ++i) {

    if(ssensors_values[i] >= ssensors_alarm_values[i]) {
      soft_temp_sensor_alarm_cmd(i);
      alarm=true;
    }
  }

  if(alarm) buzz_signal(250);
}
