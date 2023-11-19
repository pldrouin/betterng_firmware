#include "cmd.h"

void ping_cmd(struct cmd* const cmd);
void reset_cmd(struct cmd* const cmd);

void get_fan_rpm_cmd(struct cmd* const cmd);
void get_fan_off_level_cmd(struct cmd* const cmd);
void get_fan_voltage_cmd(struct cmd* const cmd);
void get_fan_voltage_target_cmd(struct cmd* const cmd);

void fan_adc_calibration_cmd(struct cmd* const cmd);
void switch_fan_control_cmd(struct cmd* const cmd);
void get_fan_output_cmd(struct cmd* const cmd);
void set_fan_output_cmd(struct cmd* const cmd);
void get_fan_duty_cycle_response_cmd(struct cmd* const cmd);
void set_fan_duty_cycle_response_cmd(struct cmd* const cmd);
void get_fan_voltage_response_cmd(struct cmd* const cmd);
void set_fan_voltage_response_cmd(struct cmd* const cmd);
void ack_cmd(struct cmd* const cmd);

static inline void send_cmd(const struct cmd* const cmd)
{
  uart_send_bytes((const uint8_t*)cmd, cmd->nbytes+2);
}

//Outgoing commands 3 bytes
static inline void ack(const uint8_t id, const int8_t ret, struct cmd* const cmd)
{
  cmd->id=ACK_CMD_ID;
  cmd->nbytes=2;
  cmd->bytes[0]=id;
  cmd->bytes[1]=(uint8_t)ret;
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

struct input_cmd {
  void (*funct)(struct cmd*);
  uint8_t nbytes;
};

const __flash struct input_cmd input_cmds[] = {
    {ping_cmd, 0}, //  0
    {reset_cmd,0}, //  1
    {0,0}, //  2
    {0,0}, //  3
    {0,0}, //  4
    {0,0}, //  5
    {0,0}, //  6
    {0,0}, //  7
    {0,0}, //  8
    {0,0}, //  9
    {0,0}, // 10
    {0,0}, // 11
    {0,0}, // 12
    {0,0}, // 13
    {0,0}, // 14
    {0,0}, // 15
    {0,0}, // 16
    {0,0}, // 17
    {0,0}, // 18
    {0,0}, // 19
    {0,0}, // 20
    {0,0}, // 21
    {0,0}, // 22
    {0,0}, // 23
    {0,0}, // 24
    {0,0}, // 25
    {0,0}, // 26
    {0,0}, // 27
    {0,0}, // 28
    {0,0}, // 29
    {0,0}, // 30
    {0,0}, // 31
    {0,0}, // 32
    {0,0}, // 33
    {0,0}, // 34
    {0,0}, // 35
    {0,0}, // 36
    {0,0}, // 37
    {0,0}, // 38
    {0,0}, // 39
    {0,0}, // 40
    {0,0}, // 41
    {0,0}, // 42
    {0,0}, // 43
    {0,0}, // 44
    {0,0}, // 45
    {0,0}, // 46
    {0,0}, // 47
    {0,0}, // 48
    {0,0}, // 49
    {0,0}, // 50
    {0,0}, // 51
    {0,0}, // 52
    {0,0}, // 53
    {0,0}, // 54
    {0,0}, // 55
    {0,0}, // 56
    {0,0}, // 57
    {0,0}, // 58
    {0,0}, // 59
    {0,0}, // 60
    {0,0}, // 61
    {0,0}, // 62
    {0,0}, // 63
    {0,0}, // 64
    {0,0}, // 65
    {0,0}, // 66
    {0,0}, // 67
    {0,0}, // 68
    {0,0}, // 69
    {0,0}, // 70
    {0,0}, // 71
    {0,0}, // 72
    {0,0}, // 73
    {0,0}, // 74
    {0,0}, // 75
    {0,0}, // 76
    {0,0}, // 77
    {0,0}, // 78
    {0,0}, // 79
    {0,0}, // 80
    {0,0}, // 81
    {0,0}, // 82
    {0,0}, // 83
    {0,0}, // 84
    {0,0}, // 85
    {0,0}, // 86
    {0,0}, // 87
    {0,0}, // 88
    {0,0}, // 89
    {0,0}, // 90
    {0,0}, // 91
    {0,0}, // 92
    {0,0}, // 93
    {0,0}, // 94
    {0,0}, // 95
    {0,0}, // 96
    {0,0}, // 97
    {0,0}, // 98
    {0,0}, // 99
    {0,0}, //100
    {0,0}, //101
    {0,0}, //102
    {0,0}, //103
    {0,0}, //104
    {0,0}, //105
    {0,0}, //106
    {0,0}, //107
    {0,0}, //108
    {0,0}, //109
    {0,0}, //110
    {0,0}, //111
    {0,0}, //112
    {0,0}, //113
    {0,0}, //114
    {0,0}, //115
    {0,0}, //116
    {0,0}, //117
    {0,0}, //118
    {0,0}, //119
    {0,0}, //120
    {0,0}, //121
    {0,0}, //122
    {0,0}, //123
    {0,0}, //124
    {0,0}, //125
    {0,0}, //126
    {0,0}, //127
    {0,0}, //128
    {0,0}, //129
    {0,0}, //130
    {0,0}, //131
    {0,0}, //132
    {0,0}, //133
    {0,0}, //134
    {0,0}, //135
    {0,0}, //136
    {0,0}, //137
    {0,0}, //138
    {0,0}, //139
    {0,0}, //140
    {0,0}, //141
    {0,0}, //142
    {0,0}, //143
    {0,0}, //144
    {0,0}, //145
    {0,0}, //146
    {0,0}, //147
    {0,0}, //148
    {0,0}, //149
    {0,0}, //150
    {0,0}, //151
    {0,0}, //152
    {0,0}, //153
    {0,0}, //154
    {0,0}, //155
    {0,0}, //156
    {0,0}, //157
    {0,0}, //158
    {0,0}, //159
    {0,0}, //160
    {0,0}, //161
    {0,0}, //162
    {0,0}, //163
    {0,0}, //164
    {0,0}, //165
    {0,0}, //166
    {0,0}, //167
    {0,0}, //168
    {0,0}, //169
    {0,0}, //170
    {0,0}, //171
    {0,0}, //172
    {0,0}, //173
    {0,0}, //174
    {0,0}, //175
    {0,0}, //176
    {0,0}, //177
    {0,0}, //178
    {0,0}, //179
    {0,0}, //180
    {0,0}, //181
    {0,0}, //182
    {0,0}, //183
    {0,0}, //184
    {0,0}, //185
    {0,0}, //186
    {0,0}, //187
    {0,0}, //188
    {0,0}, //189
    {0,0}, //190
    {0,0}, //191
    {0,0}, //192
    {0,0}, //193
    {0,0}, //194
    {0,0}, //195
    {0,0}, //196
    {0,0}, //197
    {0,0}, //198
    {0,0}, //199
    {0,0}, //200
    {0,0}, //201
    {0,0}, //202
    {0,0}, //203
    {0,0}, //204
    {0,0}, //205
    {0,0}, //206
    {0,0}, //207
    {0,0}, //208
    {0,0}, //209
    {0,0}, //210
    {0,0}, //211
    {0,0}, //212
    {0,0}, //213
    {0,0}, //214
    {0,0}, //215
    {0,0}, //216
    {0,0}, //217
    {0,0}, //218
    {0,0}, //219
    {0,0}, //220
    {0,0}, //221
    {0,0}, //222
    {0,0}, //223
    {0,0}, //224
    {0,0}, //225
    {0,0}, //226
    {0,0}, //227
    {0,0}, //228
    {0,0}, //229
    {0,0}, //230
    {0,0}, //231
    {0,0}, //232
    {0,0}, //233
    {0,0}, //234
    {0,0}, //235
    {0,0}, //236
    {0,0}, //237
    {0,0}, //238
    {0,0}, //239
    {0,0}, //240
    {0,0}, //241
    {0,0}, //242
    {get_fan_rpm_cmd, 1}, //  243
    {get_fan_off_level_cmd,1}, //244
    {get_fan_voltage_cmd,1}, //245
    {get_fan_voltage_target_cmd,1}, //246
    {fan_adc_calibration_cmd, 1}, //247
    {switch_fan_control_cmd, 2}, //248
    {get_fan_output_cmd, 1}, //249
    {set_fan_output_cmd, 2}, //250
    {get_fan_duty_cycle_response_cmd, 1}, //251
    {set_fan_duty_cycle_response_cmd, 5}, //252
    {get_fan_voltage_response_cmd, 1}, //253
    {set_fan_voltage_response_cmd, 5}, //254
    {ack_cmd, 2} //255
};

static uint8_t last_ack_cmd=UINT8_MAX;
static int8_t last_ack_value=INT8_MIN;

int read_cmd(struct cmd* const cmd)
{
  if(!uart_receive_byte(&cmd->id)) return 0;
  cmd->nbytes=input_cmds[cmd->id].nbytes;
  toggle_pin(MCU_LED);

  if(uart_receive_bytes_timeout(cmd->bytes, cmd->nbytes+1)!=cmd->nbytes+1) return 0;
  //uart_blocking_receive_bytes(cmd->bytes, cmd->nbytes+1);

  //send_cmd(cmd);
  if(check_bytes(cmd) && input_cmds[cmd->id].funct) input_cmds[cmd->id].funct(cmd);
  return 0;
}

void ping_cmd(struct cmd* const cmd)
{
  send_cmd(cmd);
}

void reset_cmd(struct cmd* const cmd)
{
  cmd->nbytes=1;
  cmd->bytes[0]=0;
  calc_check_bytes(cmd);
  send_cmd(cmd);
  while(1);
}

void get_fan_rpm_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_RPM_CMD_RESP_ID;
  cmd->nbytes=2;
  *((int16_t*)&cmd->bytes[0])=htobe16(get_fan_rpm(cmd->bytes[0]));
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void get_fan_off_level_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_OFF_LEVEL_CMD_RESP_ID;
  cmd->nbytes=2;
  *((int16_t*)&cmd->bytes[0])=(int16_t)htobe16(get_fan_off_level(cmd->bytes[0]));
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void get_fan_voltage_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_VOLTAGE_CMD_RESP_ID;
  cmd->nbytes=2;
  *((uint16_t*)&cmd->bytes[0])=htobe16(get_fan_voltage(cmd->bytes[0]));
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void get_fan_voltage_target_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_VOLTAGE_TARGET_CMD_RESP_ID;
  cmd->nbytes=2;
  *((uint16_t*)&cmd->bytes[0])=htobe16(get_fan_voltage_target(cmd->bytes[0]));
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void ack_cmd(struct cmd* const cmd)
{
  last_ack_cmd=cmd->bytes[0];
  last_ack_value=(int8_t)cmd->bytes[1];
}

void switch_fan_control_cmd(struct cmd* const cmd)
{
  int8_t ret=switch_fan_control(cmd->bytes[0], cmd->bytes[1]);
  ack(cmd->id, ret, cmd);
}

void get_fan_output_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_OUTPUT_CMD_RESP_ID;
  cmd->nbytes=1;
  cmd->bytes[0]=get_fan_output(cmd->bytes[0]);
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void set_fan_output_cmd(struct cmd* const cmd)
{
  int8_t ret=set_fan_output(cmd->bytes[0], cmd->bytes[1]);
  ack(cmd->id, ret, cmd);
}

void get_fan_duty_cycle_response_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_DUTY_CYCLE_RESPONSE_CMD_RESP_ID;
  cmd->nbytes=6;
  uint16_t dcnoout;
  int16_t ddcdout;
  int16_t d2dcdout2;
  get_fan_duty_cycle_response(cmd->bytes[0], &dcnoout, &ddcdout, &d2dcdout2);
  cmd->bytes[0]=(uint8_t)(dcnoout>>8);
  cmd->bytes[1]=(uint8_t)dcnoout;
  cmd->bytes[2]=(uint8_t)(ddcdout>>8);
  cmd->bytes[3]=(uint8_t)ddcdout;
  cmd->bytes[4]=(uint8_t)(d2dcdout2>>8);
  cmd->bytes[5]=(uint8_t)d2dcdout2;
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void set_fan_duty_cycle_response_cmd(struct cmd* const cmd)
{
  int8_t ret=set_fan_duty_cycle_response(cmd->bytes[0], be16toh(*(uint16_t*)(cmd->bytes+1)), (int16_t)be16toh(*(uint16_t*)(cmd->bytes+3)));
  ack(cmd->id, ret, cmd);
}

void get_fan_voltage_response_cmd(struct cmd* const cmd)
{
  cmd->id=GET_FAN_VOLTAGE_RESPONSE_CMD_RESP_ID;
  cmd->nbytes=6;
  uint16_t vnoout;
  int16_t dvdout;
  int16_t d2vdout2;
  get_fan_voltage_response(cmd->bytes[0], &vnoout, &dvdout, &d2vdout2);
  cmd->bytes[0]=(uint8_t)(vnoout>>8);
  cmd->bytes[1]=(uint8_t)vnoout;
  cmd->bytes[2]=(uint8_t)(dvdout>>8);
  cmd->bytes[3]=(uint8_t)dvdout;
  cmd->bytes[4]=(uint8_t)(d2vdout2>>8);
  cmd->bytes[5]=(uint8_t)d2vdout2;
  calc_check_bytes(cmd);
  send_cmd(cmd);
}

void set_fan_voltage_response_cmd(struct cmd* const cmd)
{
  int8_t ret=set_fan_voltage_response(cmd->bytes[0], be16toh(*(uint16_t*)(cmd->bytes+1)), (int16_t)be16toh(*(uint16_t*)(cmd->bytes+3)));
  ack(cmd->id, ret, cmd);
}

void fan_adc_calibration_cmd(struct cmd* const cmd)
{
  uint8_t ret=fan_adc_calibration(cmd->bytes[0]);
  ack(cmd->id, ret, cmd);
}
