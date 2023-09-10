#include "main.h"

int main(void)
{
    uint8_t eebyte = eeprom_read_byte((uint8_t*)0);
    const char* msg="Bonjour le monde!\n";
    uint8_t i;
    //BUZZPORT ^= _BV(BUZZ_NO);
    //while(1) _delay_ms(500);
    BUZZ_DDR = _BV(BUZZ_NO); //Set buzzer pin as an output

    /* Initialization */    
    inituart(115200); // Initialize UART.


    BUZZPORT ^= _BV(BUZZ_NO);
    _delay_ms(1000);
    BUZZPORT ^= _BV(BUZZ_NO);
    while(1) {

      for(i=0; msg[i]!=0; ++i) uart_send_byte(msg[i]);
      _delay_ms(1000);
    }
}
