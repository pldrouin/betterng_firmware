#include "main.h"

int main(void)
{
    int8_t i;
    //uint8_t eebyte = eeprom_read_byte((uint8_t*)0);

    /* Initialization */    
    timer_init();
    inituart(115200); // Initialize UART.
    initfans(); //Initialize fans.
    initadc(); // Initialize UART.
    sei();

    set_pin_as_output(BUZZ); //Set buzzer pin as an output
    set_pin_as_output(MCU_LED); //Set buzzer pin as an output
    set_pin(BUZZ, true);
    set_pin(MCU_LED, true);
    idle_timer_delay_millis(1000);
    set_pin(BUZZ, false);
    set_pin(MCU_LED, false);
    watchdogConfig(WATCHDOG_1S);

    lm75a_begin();

    watchdogConfig(WATCHDOG_1S);
    watchdogReset();

    for(i=0; i<N_MAX_FANS; ++i) add_fan(i);
    watchdogReset();

    //set_fan_voltage_response(0, 3., (12.-3.)/255, 0);
    //set_fan_voltage_response(1, 3., (12.-3.)/255, 0);
    //set_fan_voltage_response(2, 3., (12.-3.)/255, 0);
    //set_fan_voltage_response(3, 3000, (12000-3000)*250/255, 0);

    struct cmd cmd;
    uint16_t wval;

    while(1) {
      read_cmd(&cmd);
      watchdogReset();
    }
}
