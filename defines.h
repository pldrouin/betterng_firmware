/* definitions generated by preprocessor, copy into defines.h */
#ifndef	PPINC
#define	PPINC

#include <avr/sfr_defs.h>

#define	_ATMEGA16	// device select: _ATMEGAxxxx
#ifdef	__ICCAVR__
#include	"iom16.h"
#endif
#if	__GNUC__
#include	<avr/io.h>
#endif

/* baud rate register value calculation */
#define	CPU_FREQ	16000000

#define WDTCSR	    WDTCR
#define WDCE        WDTOE

#define WATCHDOG_OFF    (0)
#define WATCHDOG_16MS   (_BV(WDE))
#define WATCHDOG_32MS   (_BV(WDP0) | _BV(WDE))
#define WATCHDOG_64MS   (_BV(WDP1) | _BV(WDE))
#define WATCHDOG_125MS  (_BV(WDP1) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_250MS  (_BV(WDP2) | _BV(WDE))
#define WATCHDOG_500MS  (_BV(WDP2) | _BV(WDP0) | _BV(WDE))
#define WATCHDOG_1S     (_BV(WDP2) | _BV(WDP1) | _BV(WDE))
#define WATCHDOG_2S     (_BV(WDP2) | _BV(WDP1) | _BV(WDP0) | _BV(WDE))

// Watchdog functions. These are only safe with interrupts turned off.
static inline void watchdogReset(void)
{
  __asm__ __volatile__ (
    "wdr\n"
  );
}

static inline void watchdogConfig(uint8_t x)
{
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = x;
}

/* definitions for UART control */
#define	BAUD_RATE_LOW_REG	UBRRL
#define	BAUD_RATE_HIGH_REG	UBRRH
#define	UART_CONTROL_REG_A	UCSRA
#define ENABLE_DOUBLE_SPEED	U2X
#define	FRAME_ERROR_BIT		FE
#define	DATA_OVERRUN_BIT	DOR
#define	PARITY_ERROR_BIT	PE
#define	UART_CONTROL_REG_B	UCSRB
#define	ENABLE_TRANSMITTER_BIT	TXEN
#define	ENABLE_RECEIVER_BIT	RXEN
#define	ENABLE_RECEIVE_INTR	RXCIE
#define	ENABLE_SEND_INTR	UDRIE
#define	UART_CONTROL_REG_C	UCSRC
#define	REGISTER_SELECT		URSEL
#define	ENABLE_EVEN_PARITY	UPM1
#define	ENABLE_CHARACTER_SIZE_1	UCSZ1
#define	ENABLE_CHARACTER_SIZE_0	UCSZ0
#define	UART_STATUS_REG	UCSRA
#define	TRANSMIT_COMPLETE_BIT	TXC
#define	RECEIVE_COMPLETE_BIT	RXC
#define	UART_DATA_REG	UDR
#define UART_RX_INTR_FUNC	USART_RXC_vect
#define UART_DRE_INTR_FUNC	USART_UDRE_vect

#define UART_BUFFER_SIZE	32

#define BUZZ_DDR        DDRD
#define BUZZ_PORT       PORTD
#define BUZZ_NO         PD6

#define OVERTEMP_PORT	PORTC
#define OVERTEMP_NO	PC2

#define MCU_LED_DDR        DDRD
#define MCU_LED_PORT       PORTD
#define MCU_LED_NO         PD7

#define set_output(DDR, PIN) DDR |= _BV(PIN)
#define set_input(DDR, PIN) DDR &= ~_BV(PIN)
#define set_pin(PORT, PIN, VALUE) PORT = (PORT & ~_BV(PIN)) | ((VALUE==0)<<PIN)
#define toggle_pin(PORT, PIN) PORT ^= _BV(PIN)
#define read_pin(PORT, PIN) bit_is_set(PORT, PIN)

#endif
