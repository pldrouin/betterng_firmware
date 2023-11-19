#ifndef _CMD_COMMON_
#define _CMD_COMMON_

#define N_MAX_CMD_BYTES 8

struct cmd{
  uint8_t id;
  uint8_t bytes[N_MAX_CMD_BYTES];
  uint8_t nbytes; //Excludes command ID and checksum bytes
} __attribute__((packed));

#define MAX_PACKET_SIZE (sizeof(struct cmd))
#define CMD_US_TIMEOUT (100000)

#define check_bytes(cmd) ({uint8_t check=(cmd)->id; uint8_t i; for(i=0; i<(cmd)->nbytes; ++i) check+=(cmd)->bytes[i]; ((cmd)->bytes[(cmd)->nbytes]==(uint8_t)~check);})
#define calc_check_bytes(cmd) ({uint8_t check=(cmd)->id; uint8_t i; for(i=0; i<(cmd)->nbytes; ++i) check+=(cmd)->bytes[i]; (cmd)->bytes[(cmd)->nbytes]=(uint8_t)~check;})

//Host/device commands
#define PING_CMD_ID (0)
#define RESET_CMD_ID (1)
#define ACK_CMD_ID (255)

//Host commands
#define GET_FAN_RPM_CMD_REQ_ID (243) // 2 bytes
#define GET_FAN_OFF_LEVEL_CMD_REQ_ID (244) // 2 bytes
#define GET_FAN_VOLTAGE_CMD_REQ_ID (245) // 2 bytes
#define GET_FAN_VOLTAGE_TARGET_CMD_REQ_ID (246) // 2 bytes
#define FAN_ADC_CALIBRATION_CMD_REQ_ID (247) // 2 bytes
#define SWITCH_FAN_CONTROL_CMD_REQ_ID (248) // 3 bytes
#define GET_FAN_OUTPUT_CMD_REQ_ID (249) //1 byte
#define SET_FAN_OUTPUT_CMD_REQ_ID (250) //3 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (251) //2 bytes
#define SET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (252) //6 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (253) //2 bytes
#define SET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (254) //6 bytes

//Device commands
#define GET_FAN_RPM_CMD_RESP_ID (243) //3 bytes
#define GET_FAN_OFF_LEVEL_CMD_RESP_ID (244) //3 bytes
#define GET_FAN_VOLTAGE_CMD_RESP_ID (245) //3 bytes
#define GET_FAN_VOLTAGE_TARGET_CMD_RESP_ID (246) //3 bytes
#define GET_FAN_OUTPUT_CMD_RESP_ID (249) //7 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_RESP_ID (251) //7 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_RESP_ID (253) //7 bytes

enum {FAN_VOLTAGE_MODE=0, FAN_PWM_MODE=1, FAN_MANUAL_MODE=2, FAN_DISABLED_MODE=(1<<7), FAN_MODE_MASK=~FAN_DISABLED_MODE};
#define FAN_MAX_VOLTAGE_SCALE (12000) //in mV
#define FAN_SAFE_WORKING_VOLTAGE (4000)

#define FAN_SAFE_WORKING_DUTY_CYCLE (64)

#endif
