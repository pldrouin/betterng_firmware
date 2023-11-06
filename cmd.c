#include <avr/pgmspace.h>
#include "cmd.h"

//Incoming commands 1 byte
void ping_cmd(struct cmd* cmd);
void reset_cmd(struct cmd* cmd);

//Incoming commands 2 bytes
void get_fan_rpm_cmd(struct cmd* cmd);

//Incoming commands 3 bytes
void switch_fan_control_cmd(struct cmd* cmd);
void set_fan_output_cmd(struct cmd* cmd);
void ack_cmd(struct cmd* cmd);

static inline void send_cmd(const struct cmd* cmd)
{
  uart_send_bytes((const uint8_t*)cmd, DEV_CMD_NBYTES(cmd->id));
}

//Outgoing commands 3 bytes
static inline void ack(const uint8_t id, const int8_t ret, struct cmd* cmd)
{
  cmd->id=ACK_CMD_ID;
  cmd->byte1=id;
  cmd->byte2=(uint8_t)ret;
  calc_check_three_bytes(cmd);
  send_cmd(cmd);
}

typedef void (*FP)(struct cmd*);
const __flash FP input_cmd_array[] = {
  ping_cmd, //  0
  reset_cmd, //  1
  0, //  2
  0, //  3
  0, //  4
  0, //  5
  0, //  6
  0, //  7
  0, //  8
  0, //  9
  0, // 10
  0, // 11
  0, // 12
  0, // 13
  0, // 14
  0, // 15
  0, // 16
  0, // 17
  0, // 18
  0, // 19
  0, // 20
  0, // 21
  0, // 22
  0, // 23
  0, // 24
  0, // 25
  0, // 26
  0, // 27
  0, // 28
  0, // 29
  0, // 30
  0, // 31
  0, // 32
  0, // 33
  0, // 34
  0, // 35
  0, // 36
  0, // 37
  0, // 38
  0, // 39
  0, // 40
  0, // 41
  0, // 42
  0, // 43
  0, // 44
  0, // 45
  0, // 46
  0, // 47
  0, // 48
  0, // 49
  0, // 50
  0, // 51
  0, // 52
  0, // 53
  0, // 54
  0, // 55
  0, // 56
  0, // 57
  0, // 58
  0, // 59
  0, // 60
  0, // 61
  0, // 62
  0, // 63
  0, // 64
  0, // 65
  0, // 66
  0, // 67
  0, // 68
  0, // 69
  0, // 70
  0, // 71
  0, // 72
  0, // 73
  0, // 74
  0, // 75
  0, // 76
  0, // 77
  0, // 78
  0, // 79
  0, // 80
  0, // 81
  0, // 82
  0, // 83
  0, // 84
  0, // 85
  0, // 86
  0, // 87
  0, // 88
  0, // 89
  0, // 90
  0, // 91
  0, // 92
  0, // 93
  0, // 94
  0, // 95
  0, // 96
  0, // 97
  0, // 98
  0, // 99
  0, //100
  0, //101
  0, //102
  0, //103
  0, //104
  0, //105
  0, //106
  0, //107
  0, //108
  0, //109
  0, //110
  0, //111
  0, //112
  0, //113
  0, //114
  0, //115
  0, //116
  0, //117
  0, //118
  0, //119
  0, //120
  0, //121
  0, //122
  0, //123
  0, //124
  0, //125
  0, //126
  0, //127
  0, //128
  0, //129
  0, //130
  0, //131
  0, //132
  0, //133
  0, //134
  0, //135
  0, //136
  0, //137
  0, //138
  0, //139
  0, //140
  0, //141
  0, //142
  0, //143
  0, //144
  0, //145
  0, //146
  0, //147
  0, //148
  0, //149
  0, //150
  0, //151
  0, //152
  0, //153
  0, //154
  0, //155
  0, //156
  0, //157
  0, //158
  0, //159
  0, //160
  0, //161
  0, //162
  0, //163
  0, //164
  0, //165
  0, //166
  0, //167
  0, //168
  0, //169
  0, //170
  0, //171
  0, //172
  0, //173
  0, //174
  0, //175
  0, //176
  0, //177
  0, //178
  0, //179
  0, //180
  0, //181
  0, //182
  0, //183
  0, //184
  0, //185
  0, //186
  0, //187
  0, //188
  0, //189
  0, //190
  0, //191
  0, //192
  0, //193
  0, //194
  0, //195
  0, //196
  0, //197
  0, //198
  0, //199
  0, //200
  0, //201
  0, //202
  0, //203
  0, //204
  0, //205
  0, //206
  0, //207
  0, //208
  0, //209
  0, //210
  0, //211
  0, //212
  0, //213
  0, //214
  0, //215
  0, //216
  0, //217
  0, //218
  0, //219
  0, //220
  0, //221
  0, //222
  0, //223
  0, //224
  0, //225
  0, //226
  0, //227
  0, //228
  0, //229
  0, //230
  0, //231
  0, //232
  0, //233
  0, //234
  0, //235
  0, //236
  0, //237
  0, //238
  0, //239
  0, //240
  0, //241
  0, //242
  0, //243
  0, //244
  0, //245
  0, //246
  0, //247
  0, //248
  0, //249
  0, //250
  0, //251
  get_fan_rpm_cmd, //  252
  switch_fan_control_cmd, //253
  set_fan_output_cmd, //254
  ack_cmd, //255
};

static uint8_t last_ack_cmd=UINT8_MAX;
static int8_t last_ack_value=INT8_MIN;

int read_cmd(struct cmd* cmd)
{
  if(!uart_receive_byte(&cmd->id)) return 0;
  const uint8_t nbytes=HOST_CMD_NBYTES(cmd->id)-1;

  if(nbytes==1) {

    if(uart_receive_byte_timeout(&cmd->byte1)!=1) return 0;

    if(check_one_byte(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
    /*
    if(check_one_byte(cmd)) {

      switch(cmd->id) {
	case PING_CMD_ID:	 ping_cmd(cmd); break;
	case RESET_CMD_ID:	reset_cmd(cmd); break;
      }
    }
    */
    return 1;

  } else if(nbytes==2) {

    if(uart_receive_bytes_timeout(&cmd->byte1, 2)!=2) return 0;

    if(check_two_bytes(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
    /*
    if(check_two_bytes(cmd)) {

      switch(cmd->id) {
	case GET_FAN_RPM_CMD_REQ_ID: get_fan_rpm_cmd(cmd); break;
      }
    }
    */
    return 2;
  }

  if(uart_receive_bytes_timeout(&cmd->byte1, 3)!=3) return 0;

  if(check_three_bytes(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
  /*
  if(check_three_bytes(cmd)) {

    switch(cmd->id) {
      case SWITCH_FAN_CONTROL_CMD_REQ_ID:	switch_fan_control_cmd(cmd); break;
      case SET_FAN_OUTPUT_CMD_REQ_ID:		set_fan_output_cmd(cmd); break;
      case ACK_CMD_ID:				ack_cmd(cmd); break;
    }
  }
  */
  return 3;
}

void ping_cmd(struct cmd* cmd)
{
  send_cmd(cmd);
}

void reset_cmd(struct cmd* cmd)
{
  send_cmd(cmd);
  while(1);
}

void get_fan_rpm_cmd(struct cmd* cmd)
{
  cmd->id=GET_FAN_RPM_CMD_RESP_ID;
  *((uint16_t*)&cmd->byte1)=htobe16(get_fan_rpm(cmd->byte1));
  calc_check_three_bytes(cmd);
  send_cmd(cmd);
}

void ack_cmd(struct cmd* cmd)
{
  last_ack_cmd=cmd->byte1;
  last_ack_value=(int8_t)cmd->byte2;
}

void switch_fan_control_cmd(struct cmd* cmd)
{
  uint8_t ret=switch_fan_control(cmd->byte1, cmd->byte2);
  ack(cmd->id, ret, cmd);
}

void set_fan_output_cmd(struct cmd* cmd)
{
  uint8_t ret=set_fan_output(cmd->byte1, cmd->byte2);
  ack(cmd->id, ret, cmd);
}
