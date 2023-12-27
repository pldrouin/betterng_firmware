#ifndef _SERIAL_
#define _SERIAL_

#include <avr/interrupt.h>
#include "defines.h"
#include "timer.h"

struct uart_buffer{
  uint8_t buf[UART_BUFFER_SIZE];
  uint8_t curwbyte;
  uint8_t currbyte;
};

extern volatile struct uart_buffer readbuf;
extern volatile struct uart_buffer writebuf;
extern uint16_t uart_byte_timeout;

//static inline void inituart(long baudrate);
uint8_t write_byte_to_uart_buf(struct uart_buffer volatile* buf, const uint8_t byte);
uint8_t read_byte_from_uart_buf(struct uart_buffer volatile* buf, uint8_t* byte);
//static inline uint8_t uart_send_byte(const uint8_t byte);
//static inline uint8_t uart_receive_byte(uint8_t* byte);
//static inline unsigned int uart_send_bytes(const uint8_t* buf, const unsigned int len);
//static inline unsigned int uart_receive_bytes(uint8_t* buf, const unsigned int len);

//static inline void uart_clear_receive(void);
//static inline void uart_blocking_flush_send(void);

//static inline void uart_blocking_send_byte(const uint8_t byte);
//static inline void uart_blocking_receive_byte(uint8_t* byte);
//static inline void uart_blocking_send_bytes(const uint8_t* buf, const unsigned int len);
//static inline void uart_blocking_receive_bytes(uint8_t* buf, const unsigned int len);

//static inline uint8_t uart_send_byte_timeout(const uint8_t byte);
//static inline uint8_t uart_receive_byte_timeout(uint8_t* byte);
//static inline unsigned int uart_send_bytes_timeout(const uint8_t* buf, const unsigned int len);
//static inline unsigned int uart_receive_bytes_timeout(uint8_t* buf, const unsigned int len);

#include "serial_c.h"

#endif
