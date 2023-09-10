#ifndef _SERIAL_
#define _SERIAL_

#include <avr/interrupt.h>
#include "defines.h"

struct uart_buffer{
  uint8_t buf[UART_BUFFER_SIZE];
  uint8_t curwbyte;
  uint8_t currbyte;
};

void inituart( long baudrate );
unsigned int uart_send_byte( uint8_t byte );
unsigned int uart_receive_byte( uint8_t* byte );
unsigned int uart_send_bytes( uint8_t* buf, const unsigned int len );
unsigned int uart_receive_bytes( uint8_t* buf, const unsigned int len );

#endif
