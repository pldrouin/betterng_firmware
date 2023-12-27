#include "serial.h"

volatile struct uart_buffer readbuf;
volatile struct uart_buffer writebuf;
uint16_t uart_byte_timeout=0;

uint8_t write_byte_to_uart_buf(struct uart_buffer volatile* buf, const uint8_t byte)
{
  uint8_t nextbyte = (buf->curwbyte + 1) % UART_BUFFER_SIZE;
  //curwbyte is never set to currbyte

  if(nextbyte != buf->currbyte) {
    buf->buf[buf->curwbyte] = byte;
    buf->curwbyte = nextbyte;
    return 1;
  }
  return 0;
}

uint8_t read_byte_from_uart_buf(struct uart_buffer volatile* buf, uint8_t* byte)
{
  //currbyte can be set to curwbyte
  if(buf->currbyte != buf->curwbyte) {
    *byte=buf->buf[buf->currbyte];
    buf->currbyte = (buf->currbyte + 1) % UART_BUFFER_SIZE;
    return 1;
  }
  return 0;
}

// Called when data received from USART
ISR(UART_RX_INTR_FUNC)
{
  // Read UDR register to reset flag
  uint8_t byte = UART_DATA_REG;

  // Check for error
  if((UART_CONTROL_REG_A & (_BV(FRAME_ERROR_BIT) | _BV(DATA_OVERRUN_BIT) | _BV(PARITY_ERROR_BIT))) == 0) {

    if(!write_byte_to_uart_buf(&readbuf, byte)) {
      //Disable interrupt if read buffer is full
      UART_CONTROL_REG_B &= ~_BV(ENABLE_RECEIVE_INTR);
    }
  }
}

//Called when the data register accepts new data
ISR(UART_DRE_INTR_FUNC)
{
  uint8_t byte;

  if(!read_byte_from_uart_buf(&writebuf, &byte)) UART_CONTROL_REG_B &= ~_BV(ENABLE_SEND_INTR);
  
  else UART_DATA_REG = byte;
}
