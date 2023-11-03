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

void inituart(long baudrate);
uint8_t uart_send_byte(const uint8_t byte);
uint8_t uart_receive_byte(uint8_t* byte);
unsigned int uart_send_bytes(const uint8_t* buf, const unsigned int len);
unsigned int uart_receive_bytes(uint8_t* buf, const unsigned int len);

void uart_clear_receive(void);
void uart_blocking_flush_send(void);

void uart_blocking_send_byte(const uint8_t byte);
void uart_blocking_receive_byte(uint8_t* byte);
void uart_blocking_send_bytes(const uint8_t* buf, const unsigned int len);
void uart_blocking_receive_bytes(uint8_t* buf, const unsigned int len);

uint8_t uart_send_byte_timeout(const uint8_t byte);
uint8_t uart_receive_byte_timeout(uint8_t* byte);
unsigned int uart_send_bytes_timeout(const uint8_t* buf, const unsigned int len);
unsigned int uart_receive_bytes_timeout(uint8_t* buf, const unsigned int len);

#endif
