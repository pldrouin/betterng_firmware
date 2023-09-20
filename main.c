#include "main.h"

int main(void)
{
    //uint8_t eebyte = eeprom_read_byte((uint8_t*)0);
    BUZZ_DDR = _BV(BUZZ_NO); //Set buzzer pin as an output

    /* Initialization */    
    inituart(19200); // Initialize UART.
    sei();

    BUZZPORT ^= _BV(BUZZ_NO);
    _delay_ms(1000);
    BUZZPORT ^= _BV(BUZZ_NO);

    struct cmd cmd;
    watchdogConfig(WATCHDOG_1S);

    while(1) {
      read_cmd(&cmd);
      //uart_blocking_send_bytes("Hello World!\n\r", 14);
      //uart_send_bytes("Hello World!\n\r", 14);
      watchdogReset();
    }
}
