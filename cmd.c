#include <avr/pgmspace.h>
#include "cmd.h"

void ping_cmd(const struct cmd*);
void reset_cmd(const struct cmd*);

#define check_one_byte(cmd) (cmd->byte1==cmd->id)
#define check_two_bytes(cmd) (cmd->byte2==cmd->id+cmd->byte1)
#define check_three_bytes(cmd) (cmd->byte3==cmd->id+cmd->byte1+cmd->byte2)

typedef void (*FP)(const struct cmd*);
static const FP input_cmd_array[] PROGMEM = {
  ping_cmd, //0
  reset_cmd, //1
};

int read_cmd(struct cmd* cmd)
{
  if(uart_receive_byte(&cmd->id)) return 0;
  const uint8_t nbytes=CMD_NBYTES(cmd->id)-1;

  if(nbytes==1) {
    uart_blocking_receive_byte(&cmd->byte1);

    if(check_one_byte(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
    return 1;

  } else if(nbytes==2) {
    uart_blocking_receive_bytes(&cmd->byte1, 2);

    if(check_two_bytes(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
    return 2;
  }

  uart_blocking_receive_bytes(&cmd->byte1, 3);

  if(check_three_bytes(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
  return 2;
}

void ping_cmd(const struct cmd* cmd)
{
  send_cmd(cmd);
}

void reset_cmd(const struct cmd* cmd)
{
  send_cmd(cmd);
  while(1);
}
