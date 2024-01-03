#ifndef _CMD_COMMON_
#define _CMD_COMMON_

#define F_CPU   (16000000)
#define UART_BAUDRATE (115200)
#define FAN_PWM_FREQ (20000UL)
#define FAN_TACH_RPM_DIVIDER (2)

#include "timer_defs.h"

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
#define ADD_LM75A_TEMP_SENSOR_CMD_REQ_ID (200) // 2 bytes
#define DEL_LM75A_TEMP_SENSOR_CMD_REQ_ID (201) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (202) // 1 bytes
#define ADD_ANALOG_TEMP_SENSOR_CMD_REQ_ID (203) // 2 bytes
#define DEL_ANALOG_TEMP_SENSOR_CMD_REQ_ID (204) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (205) // 1 bytes
#define GET_LM75A_TEMP_SENSOR_VALUE_CMD_REQ_ID (206) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_VALUE_CMD_REQ_ID (207) // 2 bytes
#define GET_SOFT_TEMP_SENSOR_VALUE_CMD_REQ_ID (208) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (209) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (210) // 2 bytes
#define SET_LM75A_TEMP_SENSOR_CALIB_CMD_REQ_ID (211) // 8 bytes
#define SET_ANALOG_TEMP_SENSOR_CALIB_CMD_REQ_ID (212) // 8 bytes
#define SET_SOFT_TEMP_SENSOR_VALUE_CMD_REQ_ID (213) // 4 bytes
#define GET_LM75A_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (214) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (215) // 2 bytes
#define GET_SOFT_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (216) // 2 bytes
#define SET_LM75A_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (217) // 4 bytes
#define SET_ANALOG_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (218) // 4 bytes
#define SET_SOFT_TEMP_SENSOR_ALARM_VALUE_CMD_REQ_ID (219) // 4 bytes
#define ADD_FAN_CMD_REQ_ID (220) // 2 bytes
#define DEL_FAN_CMD_REQ_ID (221) // 2 bytes
#define GET_FAN_LIST_CMD_REQ_ID (222) // 1 bytes
#define ADD_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (223) // 3 bytes
#define ADD_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (224) // 3 bytes
#define ADD_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (225) // 3 bytes
#define DEL_FAN_LM75A_TEMP_SENSOR_CMD_REQ_ID (226) // 3 bytes
#define DEL_FAN_ANALOG_TEMP_SENSOR_CMD_REQ_ID (227) // 3 bytes
#define DEL_FAN_SOFT_TEMP_SENSOR_CMD_REQ_ID (228) // 3 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_REQ_ID (229) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_REQ_ID (230) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_REQ_ID (231) // 2 bytes
#define ADD_FAN_CURVE_POINT_CMD_REQ_ID (232) // 4 bytes
#define DEL_FAN_CURVE_POINT_CMD_REQ_ID (233) // 3 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_REQ_ID (234) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_REQ_ID (235) // 3 bytes
#define GET_FAN_MIN_TACH_TICKS_CMD_REQ_ID (236) // 2 bytes
#define SET_FAN_MIN_TACH_TICKS_CMD_REQ_ID (237) // 4 bytes
#define GET_FAN_MAX_TACH_TICKS_CMD_REQ_ID (238) // 2 bytes
#define SET_FAN_MAX_TACH_TICKS_CMD_REQ_ID (239) // 4 bytes
#define GET_FAN_TACH_TICKS_CMD_REQ_ID (240) // 2 bytes
#define GET_FAN_HYSTERISIS_CMD_REQ_ID (241) // 2 bytes
#define SET_FAN_HYSTERISIS_CMD_REQ_ID (242) // 3 bytes
#define GET_FAN_MODE_CMD_REQ_ID (243) //2 byte
#define SWITCH_FAN_MODE_CMD_REQ_ID (244) // 3 bytes
#define GET_FAN_ADC_VALUE_CMD_REQ_ID (245) //2 byte
#define GET_FAN_OUTPUT_CMD_REQ_ID (246) //2 byte
#define SET_FAN_OUTPUT_CMD_REQ_ID (247) //3 bytes
#define SET_FAN_OUTPUT_AUTO_CMD_REQ_ID (248) //3 bytes
#define GET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (249) //2 bytes
#define SET_FAN_DUTY_CYCLE_RESPONSE_CMD_REQ_ID (250) //8 bytes
#define GET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (251) //2 bytes
#define SET_FAN_VOLTAGE_RESPONSE_CMD_REQ_ID (252) //8 bytes
#define GET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (253) //2 bytes
#define SET_FAN_MODE_TRANSITIONS_CMD_REQ_ID (254) //4 bytes

//Host commands custom timeout
#define EEPROM_SAVE_CMD_US_TIMEOUT (10000000)

//Device respond commands
#define GET_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (202) // 2 bytes
#define GET_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (205) // 2 bytes
#define GET_LM75A_TEMP_SENSOR_VALUE_CMD_RESP_ID (206) // 3 bytes
#define GET_ANALOG_TEMP_SENSOR_VALUE_CMD_RESP_ID (207) // 3 bytes
#define GET_SOFT_TEMP_SENSOR_VALUE_CMD_RESP_ID (208) // 3 bytes
#define GET_LM75A_TEMP_SENSOR_CALIB_CMD_RESP_ID (209) // 8 bytes
#define GET_ANALOG_TEMP_SENSOR_CALIB_CMD_RESP_ID (210) // 8 bytes
#define GET_LM75A_TEMP_SENSOR_ALARM_VALUE_CMD_RESP_ID (214) // 3 bytes
#define GET_ANALOG_TEMP_SENSOR_ALARM_VALUE_CMD_RESP_ID (215) // 3 bytes
#define GET_SOFT_TEMP_SENSOR_ALARM_VALUE_CMD_RESP_ID (216) // 3 bytes
#define GET_FAN_LIST_CMD_RESP_ID (222) // 2 bytes
#define GET_FAN_LM75A_TEMP_SENSOR_LIST_CMD_RESP_ID (229) // 2 bytes
#define GET_FAN_ANALOG_TEMP_SENSOR_LIST_CMD_RESP_ID (230) // 2 bytes
#define GET_FAN_SOFT_TEMP_SENSOR_LIST_CMD_RESP_ID (231) // 2 bytes
#define GET_FAN_N_CURVE_POINTS_CMD_RESP_ID (234) // 2 bytes
#define GET_FAN_CURVE_POINT_CMD_RESP_ID (235) // 4 bytes
#define GET_FAN_MIN_TACH_TICKS_CMD_RESP_ID (236) //3 bytes
#define GET_FAN_MAX_TACH_TICKS_CMD_RESP_ID (238) //3 bytes
#define GET_FAN_TACH_TICKS_CMD_RESP_ID (240) //3 bytes
#define GET_FAN_HYSTERISIS_CMD_RESP_ID (241) // 2 bytes
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

#define FAN_TIMER_PRESCALE TIMER_FINE_8BIT_PRESCALE(FAN_PWM_FREQ)
#define FAN_TIMER_ALARM TIMER_FINE_8BIT_ALARM(FAN_PWM_FREQ)
#define convert_fan_rpm(TACH_TICKS) ((int16_t)((F_CPU/FAN_TIMER_PRESCALE)*(int32_t)30/(FAN_TIMER_ALARM+1)/TACH_TICKS/FAN_TACH_RPM_DIVIDER))
#define convert_fan_ticks(RPM) ((int16_t)((F_CPU/FAN_TIMER_PRESCALE)*(int32_t)30/(FAN_TIMER_ALARM+1)/RPM/FAN_TACH_RPM_DIVIDER))

#define FAN_MAX_REAL_VOLTAGE (12000) //in mV
#define FAN_MAX_VOLTAGE_SCALE_SHIFT (14)
#define FAN_MAX_VOLTAGE_SCALE (1<<FAN_MAX_VOLTAGE_SCALE_SHIFT) //2^FAN_MAX_VOLTAGE_SCALE_SHIFT
#define FAN_CORRECTED_MAX_VOLTAGE_SCALE ((int16_t)ceil(FAN_MAX_VOLTAGE_SCALE*256./UINT8_MAX))
#define FAN_OFF_LEVEL_DEFAULT_VALUE (0x02E6)
#define FAN_MIN_PREV_TACH_TICKS_DEFAULT_VALUE (convert_fan_ticks(15000))
#define FAN_MAX_PREV_TACH_TICKS_DEFAULT_VALUE (convert_fan_ticks(240))
#define FAN_SAFE_WORKING_VOLTAGE ((int16_t)round(4000./FAN_MAX_REAL_VOLTAGE*FAN_MAX_VOLTAGE_SCALE))
#define calc_d2vdout2(v_no_out, dvdout) (((int16_t)ceil((FAN_MAX_VOLTAGE_SCALE - v_no_out - (int16_t)((((int32_t)dvdout)*UINT8_MAX)>>8))*(65536./65025))))

#define FAN_SAFE_WORKING_DUTY_CYCLE (64)
#define calc_d2dcdout2(dc_no_out, ddcdout) (((int16_t)ceil((((uint16_t)UINT8_MAX)*64 - dc_no_out - (int16_t)((((int32_t)ddcdout)*UINT8_MAX)>>8))*(65536./65025))))
#define calc_dcnoout(ddcdout, d2dcdout2) (((int16_t)(((uint16_t)UINT8_MAX)*64 - (int16_t)((((int32_t)ddcdout)*UINT8_MAX)>>8) - (int16_t)((((int32_t)UINT8_MAX)*d2dcdout2*UINT8_MAX)>>16))))

#endif
