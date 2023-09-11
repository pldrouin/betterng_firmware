#ifndef _CMD_
#define _CMD_
#include <stdbool.h>
#include "serial.h"

#define FIRST_TWO_BYTE_CMD_INDEX 255

struct cmd{
  uint8_t id;
  uint8_t byte1;
  uint8_t byte2;
};

static inline void send_cmd(const struct cmd* cmd) {uart_blocking_send_bytes((const uint8_t*)cmd, 2+(cmd->id>=FIRST_TWO_BYTE_CMD_INDEX));}
int read_cmd(struct cmd* cmd);

#endif
