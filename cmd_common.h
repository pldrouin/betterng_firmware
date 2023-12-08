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
#define ADD_LM75A_TEMP_SENSOR_CMD_REQ_ID (207) // 2 bytes
#define DEL_LM75A_TEMP_SENSOR_CMD_REQ_ID (208) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (209) // 1 bytes
#define ADD_ANALOG_TEMP_SENSOR_CMD_REQ_ID (210) // 2 bytes
#define DEL_ANALOG_TEMP_SENSOR_CMD_REQ_ID (211) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (212) // 1 bytes
#define ADD_SOFT_TEMP_SENSOR_CMD_REQ_ID (213) // 2 bytes
#define DEL_SOFT_TEMP_SENSOR_CMD_REQ_ID (214) // 2 bytes
#define GET_SOFT_TEMP_SENSOR_LIST_CMD_REQ_ID (215) // 1 bytes
#define GET_LM75A_SENSOR_VALUE_CMD_REQ_ID (216) // 2 bytes
#define GET_ANALOG_SENSOR_VALUE_CMD_REQ_ID (217) // 2 bytes
#define GET_SOFT_SENSOR_VALUE_CMD_REQ_ID (218) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (219) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (220) // 2 bytes
#define SET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (221) // 8 bytes
#define SET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (222) // 8 bytes
#define SET_SOFT_TEMP_SENSOR_VALUE_CMD_REQ_ID (223) // 4 bytes
#define ADD_FAN_CMD_REQ_ID (224) // 2 bytes
#define DEL_FAN_CMD_REQ_ID (225) // 2 bytes
#define GET_FAN_LIST_CMD_REQ_ID (226) // 1 bytes
#define ADD_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (227) // 3 bytes
#define ADD_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (228) // 3 bytes
#define ADD_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (229) // 3 bytes
#define DEL_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (230) // 3 bytes
#define DEL_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (231) // 3 bytes
#define DEL_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (232) // 3 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (233) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (234) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_REQ_ID (235) // 2 bytes
#define ADD_FAN_CURVE_POINT_CMD_REQ_ID (236) // 4 bytes
#define DEL_FAN_CURVE_POINT_CMD_REQ_ID (237) // 3 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_REQ_ID (238) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_REQ_ID (239) // 3 bytes
#define GET_FAN_RPM_CMD_REQ_ID (240) // 2 bytes
#define GET_FAN_OFF_LEVEL_CMD_REQ_ID (241) // 2 bytes
#define GET_FAN_VOLTAGE_CMD_REQ_ID (242) // 2 bytes
#define GET_FAN_VOLTAGE_TARGET_CMD_REQ_ID (243) // 2 bytes
#define FAN_ADC_CALIBRATION_CMD_REQ_ID (244) // 2 bytes
#define SWITCH_FAN_CONTROL_CMD_REQ_ID (245) // 3 bytes
#define GET_FAN_OUTPUT_CMD_REQ_ID (246) //1 byte
#define SET_FAN_OUTPUT_CMD_REQ_ID (247) //3 bytes
#define SET_FAN_OUTPUT_AUTO_CMD_REQ_ID (248) //3 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (249) //2 bytes
#define SET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (250) //6 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (251) //2 bytes
#define SET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (252) //6 bytes
#define GET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (253) //2 bytes
#define SET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (254) //4 bytes

//Device commands
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (209) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (212) // 2 bytes
#define GET_SOFT_TEMP_SENSOR_LIST_CMD_RESP_ID (215) // 2 bytes
#define GET_LM75A_SENSOR_VALUE_CMD_RESP_ID (216) // 3 bytes
#define GET_ANALOG_SENSOR_VALUE_CMD_RESP_ID (217) // 3 bytes
#define GET_SOFT_SENSOR_VALUE_CMD_RESP_ID (218) // 3 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_RESP_ID (219) // 8 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_RESP_ID (220) // 8 bytes
#define GET_FAN_LIST_CMD_RESP_ID (226) // 2 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (233) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (234) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_RESP_ID (235) // 2 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_RESP_ID (238) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_RESP_ID (239) // 4 bytes
#define GET_FAN_RPM_CMD_RESP_ID (240) //3 bytes
#define GET_FAN_OFF_LEVEL_CMD_RESP_ID (241) //3 bytes
#define GET_FAN_VOLTAGE_CMD_RESP_ID (242) //3 bytes
#define GET_FAN_VOLTAGE_TARGET_CMD_RESP_ID (243) //3 bytes
#define GET_FAN_OUTPUT_CMD_RESP_ID (246) //7 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_RESP_ID (249) //7 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_RESP_ID (251) //7 bytes
#define GET_FAN_MODE_TRANSITIONS_CMD_RESP_ID (253) //3 bytes

enum {FAN_VOLTAGE_MODE=1, FAN_PWM_MODE=2, FAN_CUSTOM_MODE=4, FAN_AUTO_MODE=8, FAN_DISABLED_MODE=(1<<7), FAN_MODE_MASK=(FAN_VOLTAGE_MODE|FAN_PWM_MODE|FAN_CUSTOM_MODE)};
#define FAN_MAX_VOLTAGE_SCALE (12000) //in mV
#define FAN_SAFE_WORKING_VOLTAGE (4000)

#define FAN_SAFE_WORKING_DUTY_CYCLE (64)

#endif
