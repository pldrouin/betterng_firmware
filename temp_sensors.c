#include "temp_sensors.h"

#define PARAM_SCALE (32767L)
#define N_MAX_TEMP_SENSORS (LM75A_MAX_SENSORS+MAX_ANALOG_SENSORS+N_MAX_SOFT_TEMP_SENSORS)

struct temp_sensor
{
  int16_t value;
  int16_t a2;
  int16_t a1;
  int16_t a0;
  uint8_t type;
};

static struct temp_sensor lsensors[LM75A_MAX_SENSORS];
static struct temp_sensor asensors[MAX_ANALOG_SENSORS];
static struct temp_sensor ssensors[N_MAX_SOFT_TEMP_SENSORS];
static struct temp_sensor* sensors[N_MAX_TEMP_SENSORS];

static uint8_t nsensors=0;

int8_t add_temp_sensor(const uint8_t type, const uint8_t id)
{
  int8_t ret;

  switch(type) {

    case LM75A_SENSOR:

      if((ret=lm75a_add_sensor(id))<0) return ret;
      sensors[nsensors]=lsensors+ret;
      lsensors[ret].type=LM75A_SENSOR;
      lsensors[ret].value=32767L;
      lsensors[ret].a2=0;
      lsensors[ret].a1=PARAM_SCALE;
      lsensors[ret].a0=0;
      break;

    case ANALOG_TEMP_SENSOR:

      if((ret=add_analog_sensor(id))<0) return ret;
      sensors[nsensors]=asensors+ret;
      asensors[ret].type=ANALOG_TEMP_SENSOR;
      asensors[ret].value=32767L;
      asensors[ret].a2=0;
      asensors[ret].a1=PARAM_SCALE;
      asensors[ret].a0=0;
      break;

    default: //SOFT_TEMP_SENSOR
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -1;

      uint8_t i;

      for(i=0; i<nsensors; ++i)

	if(sensors[i]==ssensors+id) return -1;
      sensors[nsensors]=ssensors+id;
      ssensors[id].type=SOFT_TEMP_SENSOR;
      ssensors[id].value=32767L;
      ssensors[id].a2=0;
      ssensors[id].a1=0;
      ssensors[id].a0=0;
  }

  return nsensors++;
}

int8_t del_temp_sensor(const uint8_t index)
{
  if(index>=nsensors) return -1;
  uint8_t i;

  switch(sensors[index]->type) {

    case LM75A_SENSOR:
      lm75a_del_sensor(sensors[index]-lsensors);
      uint8_t nlsensors=lm75a_get_nsensors()-1;

      for(i=sensors[index]-lsensors; i<nlsensors; ++i) lsensors[i]=lsensors[i+1];
      break;

    case ANALOG_TEMP_SENSOR:
      del_analog_sensor(sensors[index]-asensors);
      uint8_t nasensors=get_n_analog_sensors();

      for(i=sensors[index]-asensors; i<nasensors; ++i) asensors[i]=asensors[i+1];
      break;
  }
  --nsensors;

  for(i=index; i<nsensors; ++i) sensors[i]=sensors[i+1];
  return 0;
}

int8_t set_temp_sensor_calib(const uint16_t index, const float a2, const float a1, const float a0)
{
  if(index>=nsensors) return -1;
  sensors[index]->a0=a0;
  sensors[index]->a1=(int16_t)(a1*PARAM_SCALE);
  sensors[index]->a2=(int16_t)(a2*PARAM_SCALE*PARAM_SCALE);
  return 0;
}

void update_temp_values(void)
{
  uint8_t i;
  int16_t value;

  for(i=lm75a_get_nsensors()-1; i>=0; --i) {
    value=lm75a_getValue(i);
    lsensors[i].value=lsensors[i].a0 + (value*(lsensors[i].a1 + (((int32_t)value) * lsensors[i].a2)/PARAM_SCALE))/PARAM_SCALE;
  }

  for(i=get_n_analog_sensors()-1; i>=0; --i) {
    value=analog_sensor_getValue(i);
    asensors[i].value=asensors[i].a0 + (value*(asensors[i].a1 + (((int32_t)value) * asensors[i].a2)/PARAM_SCALE))/PARAM_SCALE;
  }
}

int8_t set_soft_sensor_value(const uint8_t id, const int16_t value)
{
      if(id>=N_MAX_SOFT_TEMP_SENSORS) return -1;
      ssensors[id].value=value;
      return 0;
}
