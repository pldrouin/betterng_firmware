#include "main.h"

int main(void)
{
    //uint8_t eebyte = eeprom_read_byte((uint8_t*)0);
    set_output(BUZZ_DDR, BUZZ_NO); //Set buzzer pin as an output
    set_output(MCU_LED_DDR, MCU_LED_NO); //Set buzzer pin as an output

    /* Initialization */    
    inituart(19200); // Initialize UART.
    sei();

    set_pin(BUZZ_PORT, BUZZ_NO, false);
    set_pin(MCU_LED_PORT, MCU_LED_NO, false);
    _delay_ms(1000);
    set_pin(BUZZ_PORT, BUZZ_NO, true);
    set_pin(MCU_LED_PORT, MCU_LED_NO, true);

    struct cmd cmd;
    watchdogConfig(WATCHDOG_1S);

    while(1) {

      //if(!read_pin(OVERTEMP_PORT, OVERTEMP_NO)) set_pin(BUZZ_PORT, BUZZ_NO, true);
      read_cmd(&cmd);
      //uart_blocking_send_bytes("Hello World!\n\r", 14);
      //uart_send_bytes("Hello World!\n\r", 14);
      watchdogReset();
    }
}
