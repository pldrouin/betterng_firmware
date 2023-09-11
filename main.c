#include "main.h"

int main(void)
{
    uint8_t eebyte = eeprom_read_byte((uint8_t*)0);
    BUZZ_DDR = _BV(BUZZ_NO); //Set buzzer pin as an output

    /* Initialization */    
    inituart(115200); // Initialize UART.


    BUZZPORT ^= _BV(BUZZ_NO);
    _delay_ms(1000);
    BUZZPORT ^= _BV(BUZZ_NO);

    struct cmd cmd;
    watchdogConfig(WATCHDOG_1S);

    while(1) {
      read_cmd(&cmd);
      watchdogReset();
    }
}
