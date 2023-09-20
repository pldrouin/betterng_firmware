#include "cmd.h"

void ping_cmd(const struct cmd*);
void reset_cmd(const struct cmd*);

static inline bool check_one_byte(const struct cmd* cmd){return (cmd->byte1==cmd->id);}
static inline bool check_two_bytes(const struct cmd* cmd){return (cmd->byte2==cmd->id+cmd->byte1);}

static void (*input_cmd_array[255])(const struct cmd*)={
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
  }

  uart_blocking_receive_bytes(&cmd->byte1, 2);

  if(check_two_bytes(cmd) && input_cmd_array[cmd->id]) input_cmd_array[cmd->id](cmd);
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
