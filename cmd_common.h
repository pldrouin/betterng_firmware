#ifndef _CMD_IDS_
#define _CMD_IDS_

struct cmd{
  uint8_t id;
  uint8_t byte1;
  uint8_t byte2;
  uint8_t byte3;
} __attribute__((packed));

#define MAX_PACKET_SIZE (sizeof(struct cmd))
#define CMD_US_TIMEOUT (100000)

#define check_one_byte(cmd) (((cmd)->byte1)==(uint8_t)(~((cmd)->id)))
#define check_two_bytes(cmd) (((cmd)->byte2)==(uint8_t)(~((cmd)->id+(cmd)->byte1)))
#define check_three_bytes(cmd) (((cmd)->byte3)==(uint8_t)(~((cmd)->id+(cmd)->byte1+(cmd)->byte2)))

#define calc_check_one_byte(cmd) ((cmd)->byte1=~((cmd)->id))
#define calc_check_two_bytes(cmd) ((cmd)->byte2=~((cmd)->id+(cmd)->byte1))
#define calc_check_three_bytes(cmd) ((cmd)->byte3=~((cmd)->id+(cmd)->byte1+(cmd)->byte2))

//Host/device commands
#define PING_CMD_ID (0)
#define RESET_CMD_ID (1)
#define ACK_CMD_ID (255)

//Host commands
#define GET_FAN_RPM_CMD_REQ_ID (2)
#define SWITCH_FAN_CONTROL_CMD_REQ_ID (253)
#define SET_FAN_OUTPUT_CMD_REQ_ID (254)

//Device commands
#define GET_FAN_RPM_CMD_RESP_ID (254)

#define FIRST_TWO_BYTE_HOST_CMD_INDEX 2
#define FIRST_THREE_BYTE_HOST_CMD_INDEX 253

#define HOST_CMD_NBYTES(id) (2+(id>=FIRST_TWO_BYTE_HOST_CMD_INDEX)+(id>=FIRST_THREE_BYTE_HOST_CMD_INDEX))

#define FIRST_TWO_BYTE_DEV_CMD_INDEX 2
#define FIRST_THREE_BYTE_DEV_CMD_INDEX 254

#define DEV_CMD_NBYTES(id) (2+(id>=FIRST_TWO_BYTE_DEV_CMD_INDEX)+(id>=FIRST_THREE_BYTE_DEV_CMD_INDEX))

#endif
