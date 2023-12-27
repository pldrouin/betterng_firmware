#ifndef _CMD_
#define _CMD_
#include "serial.h"
#include "cmd_common.h"

int read_cmd(struct cmd* const cmd);

static inline void send_cmd(const struct cmd* const cmd)
{
  uart_send_bytes((const uint8_t*)cmd, cmd->nbytes+2);
}

static inline void device_overtemp_cmd(void){
  struct cmd cmd={DEVICE_OVERTEMP_CMD_ID, {}, 0};
  calc_check_bytes(&cmd);
  send_cmd(&cmd);
}

static inline void lm75a_temp_sensor_alarm_cmd(uint8_t id){
  struct cmd cmd={LM75A_TEMP_SENSOR_ALARM_CMD_ID, {id}, 1};
  calc_check_bytes(&cmd);
  send_cmd(&cmd);
}

static inline void analog_temp_sensor_alarm_cmd(uint8_t id){
  struct cmd cmd={ANALOG_TEMP_SENSOR_ALARM_CMD_ID, {id}, 1};
  calc_check_bytes(&cmd);
  send_cmd(&cmd);
}

static inline void soft_temp_sensor_alarm_cmd(uint8_t id){
  struct cmd cmd={SOFT_TEMP_SENSOR_ALARM_CMD_ID, {id}, 1};
  calc_check_bytes(&cmd);
  send_cmd(&cmd);
}

#endif
