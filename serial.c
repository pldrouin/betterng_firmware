#include "serial.h"

volatile struct uart_buffer readbuf;
volatile struct uart_buffer sendbuf;

static inline int write_byte_to_uart_buf(volatile struct uart_buffer* buf, const uint8_t byte)
{
  uint8_t nextbyte = (buf->curwbyte + 1) % UART_BUFFER_SIZE;

  if(nextbyte != buf->currbyte) {
    buf->buf[buf->curwbyte] = byte;
    buf->curwbyte = nextbyte;
    return 0;
  }
  return 1;
}

static inline int read_byte_from_uart_buf(volatile struct uart_buffer* buf, uint8_t* byte)
{
  if(buf->currbyte != buf->curwbyte) {
    *byte=buf->buf[buf->currbyte];
    buf->currbyte = (buf->currbyte + 1) % UART_BUFFER_SIZE;
    return 0;
  }
  return 1;
}

void inituart(long baudrate)
{
  readbuf.curwbyte=0;
  readbuf.currbyte=UART_BUFFER_SIZE;
  sendbuf.curwbyte=0;
  sendbuf.currbyte=UART_BUFFER_SIZE;
  uint16_t prescale = ((CPU_FREQ / (baudrate * 16UL))) - 0.5;
  BAUD_RATE_LOW_REG = prescale;
  BAUD_RATE_HIGH_REG = (prescale >> 8);
  UART_CONTROL_REG_B = (1 << ENABLE_RECEIVER_BIT) |
                     (1 << ENABLE_TRANSMITTER_BIT) |
		     (1 << ENABLE_RECEIVE_INTR);
  // 8 databits
  // Even parity
  // 1 stopbit
  UART_CONTROL_REG_C = (1 << ENABLE_EVEN_PARITY) | (1 << ENABLE_CHARACTER_SIZE_1) | (1 << ENABLE_CHARACTER_SIZE_0);
}

// Called when data received from USART
ISR(UART_RX_INTR_FUNC)
{
  // Read UDR register to reset flag
  uint8_t byte = UART_DATA_REG;

  // Check for error
  if((UART_CONTROL_REG_A & ((1 << FRAME_ERROR_BIT) | (1 << DATA_OVERRUN_BIT) | (1 << PARITY_ERROR_BIT))) == 0) {

    if(write_byte_to_uart_buf(&readbuf, byte)) {
      //Disable interrupt when command complete
      UART_CONTROL_REG_B &= ~(1 << ENABLE_RECEIVE_INTR);
    }
  }
}

//Called when the data register accepts new data
ISR(UART_DRE_INTR_FUNC)
{
  uint8_t byte;

  if(read_byte_from_uart_buf(&sendbuf, &byte)) UART_CONTROL_REG_B &= ~(1 << ENABLE_SEND_INTR);
  
  else UART_DATA_REG = byte;
}

//Sends to UART buffer (non-blocking)
unsigned int uart_send_byte( uint8_t byte)
{
  unsigned int ret=write_byte_to_uart_buf(&sendbuf, byte);
  UART_CONTROL_REG_B |= (1 << ENABLE_SEND_INTR);
  return ret;
}

//Receives from UART buffer (non-blocking)
unsigned int uart_receive_byte( uint8_t* byte )
{
  unsigned int ret=read_byte_from_uart_buf(&readbuf, byte);
  UART_CONTROL_REG_B |= (1 << ENABLE_RECEIVE_INTR);
  return ret;
}

//Sends to UART buffer (non-blocking)
unsigned int uart_send_bytes( uint8_t* buf, const unsigned int len )
{
  unsigned int i=0;

  while(i<len && !write_byte_to_uart_buf(&sendbuf, buf[i])) ++i;
  UART_CONTROL_REG_B |= (1 << ENABLE_SEND_INTR);
  return i;
}

//Receives from UART buffer (non-blocking)
unsigned int uart_receive_bytes( uint8_t* buf, const unsigned int len )
{
  unsigned int i=0;

  while(i<len && !read_byte_from_uart_buf(&readbuf, buf+i)) ++i;
  UART_CONTROL_REG_B |= (1 << ENABLE_RECEIVE_INTR);
  return i;
}
