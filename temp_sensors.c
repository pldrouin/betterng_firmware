#include "temp_sensors.h"
#include "cmd.h"

struct temp_sensor lsensors[LM75A_MAX_SENSORS];
struct temp_sensor asensors[MAX_ANALOG_SENSORS];
int16_t ssensors_values[N_MAX_SOFT_TEMP_SENSORS];
int16_t ssensors_alarm_values[N_MAX_SOFT_TEMP_SENSORS];

uint8_t lsenslist[LM75A_MAX_SENSORS];
uint8_t nlsensors=0U;
uint8_t asenslist[MAX_ANALOG_SENSORS];
uint8_t nasensors=0U;
