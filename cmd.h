#ifndef _CMD_
#define _CMD_
#include <stdbool.h>
#include "serial.h"
#include "fan.h"
#include "cmd_common.h"

int read_cmd(struct cmd* const cmd);

#endif
