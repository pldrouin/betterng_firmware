static inline void inituart(void)
{
  readbuf.currbyte=readbuf.curwbyte=0;
  writebuf.currbyte=writebuf.curwbyte=0;
  uint16_t prescale;
 
  if(UART_BAUDRATE<=19200) {
    UART_CONTROL_REG_A &= ~_BV(ENABLE_DOUBLE_SPEED);
    prescale = ((F_CPU / (UART_BAUDRATE * 16UL))) - 1;

  } else {
    UART_CONTROL_REG_A |= _BV(ENABLE_DOUBLE_SPEED);
    prescale = ((F_CPU / (UART_BAUDRATE * 8UL))) - 1;
  }
  BAUD_RATE_LOW_REG = prescale;
  BAUD_RATE_HIGH_REG = (prescale >> 8);
  UART_CONTROL_REG_B = _BV(ENABLE_RECEIVER_BIT) |
                     _BV(ENABLE_TRANSMITTER_BIT) |
		     _BV(ENABLE_RECEIVE_INTR);
  // 8 databits
  // Even parity
  // 1 stopbit
  //UART_CONTROL_REG_C = _BV(REGISTER_SELECT) | _BV(ENABLE_EVEN_PARITY) | _BV(ENABLE_CHARACTER_SIZE_1) | _BV(ENABLE_CHARACTER_SIZE_0);
  UART_CONTROL_REG_C = _BV(REGISTER_SELECT) | _BV(ENABLE_CHARACTER_SIZE_1) | _BV(ENABLE_CHARACTER_SIZE_0);
}

static inline void flush_uart_buf(volatile struct uart_buffer* buf)
{
  buf->currbyte = buf->curwbyte;
}

static inline uint8_t uart_buf_non_empty(volatile struct uart_buffer* buf)
{
  return (buf->currbyte != buf->curwbyte);
}

//Sends to UART buffer (non-blocking)
static inline uint8_t uart_send_byte(const uint8_t byte)
{
  uint8_t ret=write_byte_to_uart_buf(&writebuf, byte);
  UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
  return ret;
}

//Receives from UART buffer (non-blocking)
static inline uint8_t uart_receive_byte(uint8_t* byte)
{
  uint8_t ret=read_byte_from_uart_buf(&readbuf, byte);
  UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
  return ret;
}

//Sends to UART buffer (non-blocking)
static inline unsigned int uart_send_bytes(const uint8_t* buf, const unsigned int len)
{
  unsigned int i=0;

  while(i<len && write_byte_to_uart_buf(&writebuf, buf[i])) ++i;
  UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
  return i;
}

//Receives from UART buffer (non-blocking)
static inline unsigned int uart_receive_bytes(uint8_t* buf, const unsigned int len)
{
  if(len==0) return 0;
  unsigned int i=0;

  while(i<len && read_byte_from_uart_buf(&readbuf, buf+i)) ++i;
  UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
  return i;
}

static inline void uart_clear_receive(void)
{
  flush_uart_buf(&readbuf);
}

//Blocks until send buffer is not empty
static inline void uart_blocking_flush_send(void)
{
  //Blocks while buffer is not empty
  while(uart_buf_non_empty(&writebuf));
}

//Sends to UART buffer
static inline void uart_blocking_send_byte(const uint8_t byte)
{
  //Blocks if buffer is full
  while(!write_byte_to_uart_buf(&writebuf, byte));
  UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);

  //Blocks while buffer is not empty
  //while(uart_buf_non_empty(&writebuf));
}

//Receives from UART buffer
static inline void uart_blocking_receive_byte(uint8_t* byte)
{
  //Blocks if buffer is empty
  while(!read_byte_from_uart_buf(&readbuf, byte));
  UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
}

//Sends to UART buffer
static inline void uart_blocking_send_bytes(const uint8_t* buf, const unsigned int len)
{
  unsigned int i;

  for(i=0; i<len; ++i) {
    //Blocks if buffer is full
    while(!write_byte_to_uart_buf(&writebuf, buf[i]));
    UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
  }

  //Blocks while buffer is not empty
  //while(uart_buf_non_empty(&writebuf));
}

//Receives from UART buffer
static inline void uart_blocking_receive_bytes(uint8_t* buf, const unsigned int len)
{
  if(len==0) return;
  unsigned int i;

  for(i=0; i<len; ++i) {

    //Blocks if buffer is empty
    while(!read_byte_from_uart_buf(&readbuf, buf+i));
    UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
  }
}

static inline uint8_t uart_send_byte_timeout(const uint8_t byte)
{
  
  if(!write_byte_to_uart_buf(&writebuf, byte)) {
    timer_minimum_micros_start(SERIAL_UART_BYTE_TIMEOUT);

    while(!timer_delay_verify()) {

      if(write_byte_to_uart_buf(&writebuf, byte)) {
        UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
	timer_delay_end();
	return 1U;
      }
    }
    return 0U;
  }
  UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
  return 1U;
}

static inline uint8_t uart_receive_byte_timeout(uint8_t* byte)
{
 
  if(!read_byte_from_uart_buf(&readbuf, byte)) {
    timer_minimum_micros_start(SERIAL_UART_BYTE_TIMEOUT);

    while(!timer_delay_verify()) {

      if(read_byte_from_uart_buf(&readbuf, byte)) {
        UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
	timer_delay_end();
	return 1U;
      }
    }
    return 0U;
  }
  UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
  return 1U;
}

static inline unsigned int uart_send_bytes_timeout(const uint8_t* buf, const unsigned int len)
{
  unsigned int i;

  for(i=0; i<len; ++i) {

    if(!write_byte_to_uart_buf(&writebuf, buf[i])) {
      timer_minimum_micros_start(SERIAL_UART_BYTE_TIMEOUT);

      while(!timer_delay_verify()) {

	if(write_byte_to_uart_buf(&writebuf, buf[i])) {
	  timer_delay_end();
	  goto send_bytes_timeout_recovered;
	}
      }
      return i;
    }
send_bytes_timeout_recovered:
    UART_CONTROL_REG_B |= _BV(ENABLE_SEND_INTR);
  }
  return i;
}

static inline unsigned int uart_receive_bytes_timeout(uint8_t* buf, const unsigned int len)
{
  if(len==0) return 0;
  unsigned int i;

  for(i=0; i<len; ++i) {

    if(!read_byte_from_uart_buf(&readbuf, buf+i)) {
      timer_minimum_micros_start(SERIAL_UART_BYTE_TIMEOUT);

      while(!timer_delay_verify()) {

	if(read_byte_from_uart_buf(&readbuf, buf+i)) {
	  timer_delay_end();
	  goto receive_bytes_timeout_recovered;
	}
      }
      return i;
    }
receive_bytes_timeout_recovered:
    UART_CONTROL_REG_B |= _BV(ENABLE_RECEIVE_INTR);
  }
  return i;
}
