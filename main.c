#include "main.h"

int main(void)
{
    //uint8_t eebyte = eeprom_read_byte((uint8_t*)0);
    set_output(BUZZ); //Set buzzer pin as an output
    set_output(MCU_LED); //Set buzzer pin as an output

    /* Initialization */    
    inituart(115200); // Initialize UART.
    initadc(); // Initialize UART.
    sei();

    set_pin(BUZZ, false);
    set_pin(MCU_LED, false);
    _delay_ms(1000);
    set_pin(BUZZ, true);
    set_pin(MCU_LED, true);

    struct cmd cmd;
    watchdogConfig(WATCHDOG_1S);

    while(1) {
      //if(!read_pin(OVERTEMP_PORT, OVERTEMP_NO)) set_pin(BUZZ_PORT, BUZZ_NO, true);
      read_cmd(&cmd);
      //uart_blocking_send_bytes("Hello World!\n\r", 14);
      //uart_send_bytes("Hello World!\n\r", 14);
      //uart_send_byte(0xAA);
      //_delay_ms(1000);
      watchdogReset();
    }
}
