#ifndef _CMD_COMMON_
#define _CMD_COMMON_

#define N_MAX_CMD_BYTES 8

struct cmd{
  uint8_t id;
  uint8_t bytes[N_MAX_CMD_BYTES];
  uint8_t nbytes; //Excludes command ID and checksum bytes
} __attribute__((packed));

#define MAX_PACKET_SIZE (sizeof(struct cmd))
#define DEFAULT_CMD_US_TIMEOUT (100000)

#define check_bytes(cmd) ({uint8_t check=(cmd)->id; uint8_t i; for(i=0; i<(cmd)->nbytes; ++i) check+=(cmd)->bytes[i]; ((cmd)->bytes[(cmd)->nbytes]==(uint8_t)~check);})
#define calc_check_bytes(cmd) ({uint8_t check=(cmd)->id; uint8_t i; for(i=0; i<(cmd)->nbytes; ++i) check+=(cmd)->bytes[i]; (cmd)->bytes[(cmd)->nbytes]=(uint8_t)~check;})

//Host/device commands
#define PING_CMD_ID (0)
#define RESET_CMD_ID (1)
#define ACK_CMD_ID (255)

//Host commands
#define EEPROM_SAVE_CMD_REQ_ID (2) // 1 bytes
#define ADD_LM75A_TEMP_SENSOR_CMD_REQ_ID (212) // 2 bytes
#define DEL_LM75A_TEMP_SENSOR_CMD_REQ_ID (213) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (214) // 1 bytes
#define ADD_ANALOG_TEMP_SENSOR_CMD_REQ_ID (215) // 2 bytes
#define DEL_ANALOG_TEMP_SENSOR_CMD_REQ_ID (216) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (217) // 1 bytes
#define GET_LM75A_SENSOR_VALUE_CMD_REQ_ID (218) // 2 bytes
#define GET_ANALOG_SENSOR_VALUE_CMD_REQ_ID (219) // 2 bytes
#define GET_SOFT_SENSOR_VALUE_CMD_REQ_ID (220) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (221) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (222) // 2 bytes
#define SET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (223) // 8 bytes
#define SET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (224) // 8 bytes
#define SET_SOFT_TEMP_SENSOR_VALUE_CMD_REQ_ID (225) // 4 bytes
#define ADD_FAN_CMD_REQ_ID (226) // 2 bytes
#define DEL_FAN_CMD_REQ_ID (227) // 2 bytes
#define GET_FAN_LIST_CMD_REQ_ID (228) // 1 bytes
#define ADD_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (229) // 3 bytes
#define ADD_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (230) // 3 bytes
#define ADD_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (231) // 3 bytes
#define DEL_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (232) // 3 bytes
#define DEL_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (233) // 3 bytes
#define DEL_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (234) // 3 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (235) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (236) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_REQ_ID (237) // 2 bytes
#define ADD_FAN_CURVE_POINT_CMD_REQ_ID (238) // 4 bytes
#define DEL_FAN_CURVE_POINT_CMD_REQ_ID (239) // 3 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_REQ_ID (240) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_REQ_ID (241) // 3 bytes
#define GET_FAN_RPM_CMD_REQ_ID (242) // 2 bytes
#define GET_FAN_MODE_CMD_REQ_ID (243) //2 byte
#define SWITCH_FAN_CONTROL_CMD_REQ_ID (244) // 3 bytes
#define GET_FAN_ADC_VALUE_CMD_REQ_ID (245) //2 byte
#define GET_FAN_OUTPUT_CMD_REQ_ID (246) //2 byte
#define SET_FAN_OUTPUT_CMD_REQ_ID (247) //3 bytes
#define SET_FAN_OUTPUT_AUTO_CMD_REQ_ID (248) //3 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (249) //2 bytes
#define SET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (250) //6 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (251) //2 bytes
#define SET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (252) //6 bytes
#define GET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (253) //2 bytes
#define SET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (254) //4 bytes

//Host commands custom timeout
#define EEPROM_SAVE_CMD_US_TIMEOUT (2000000)

//Device respond commands
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (216) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (217) // 2 bytes
#define GET_LM75A_SENSOR_VALUE_CMD_RESP_ID (218) // 3 bytes
#define GET_ANALOG_SENSOR_VALUE_CMD_RESP_ID (219) // 3 bytes
#define GET_SOFT_SENSOR_VALUE_CMD_RESP_ID (220) // 3 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_RESP_ID (221) // 8 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_RESP_ID (222) // 8 bytes
#define GET_FAN_LIST_CMD_RESP_ID (228) // 2 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (235) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (236) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_RESP_ID (237) // 2 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_RESP_ID (240) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_RESP_ID (241) // 4 bytes
#define GET_FAN_RPM_CMD_RESP_ID (242) //3 bytes
#define GET_FAN_MODE_CMD_RESP_ID (243) //2 bytes
#define GET_FAN_ADC_VALUE_CMD_RESP_ID (245) //3 bytes
#define GET_FAN_OUTPUT_CMD_RESP_ID (246) //2 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_RESP_ID (249) //7 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_RESP_ID (251) //7 bytes
#define GET_FAN_MODE_TRANSITIONS_CMD_RESP_ID (253) //3 bytes

//Device commands
#define DEVICE_OVERTEMP_CMD_ID (63) // 1 bytes
#define LM75A_TEMP_SENSOR_ALARM_CMD_ID (125) // 2 bytes
#define ANALOG_TEMP_SENSOR_ALARM_CMD_ID (126) // 2 bytes
#define SOFT_TEMP_SENSOR_ALARM_CMD_ID (127) // 2 bytes

enum {FAN_VOLTAGE_MODE=1, FAN_PWM_MODE=2, FAN_DISABLED_MODE=4, FAN_CUSTOM_MODE=8, FAN_AUTO_FLAG=32, FAN_OFF_FLAG=64, FAN_STARTING_FLAG=128, FAN_MODE_MASK=(FAN_VOLTAGE_MODE|FAN_PWM_MODE|FAN_DISABLED_MODE|FAN_CUSTOM_MODE)};
#define FAN_MAX_VOLTAGE_SCALE (12000) //in mV
#define FAN_CORRECTED_MAX_VOLTAGE_SCALE ((int16_t)ceil(FAN_MAX_VOLTAGE_SCALE*256./UINT8_MAX))
#define FAN_OFF_LEVEL_DEFAULT_VALUE (0x016D)
#define FAN_SAFE_WORKING_VOLTAGE (4000)

#define FAN_SAFE_WORKING_DUTY_CYCLE (64)

#endif
