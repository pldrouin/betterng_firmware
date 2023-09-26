#ifndef _CMD_
#define _CMD_
#include <stdbool.h>
#include "serial.h"

#define FIRST_TWO_BYTE_CMD_INDEX 2
#define FIRST_THREE_BYTE_CMD_INDEX 2

#define CMD_NBYTES(id) (2+(id>=FIRST_TWO_BYTE_CMD_INDEX)+(id>=FIRST_THREE_BYTE_CMD_INDEX))

struct cmd{
  uint8_t id;
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
};

static inline void send_cmd(const struct cmd* cmd) {uart_blocking_send_bytes((const uint8_t*)cmd, CMD_NBYTES(cmd->id));}
int read_cmd(struct cmd* cmd);

#endif
