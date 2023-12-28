#include "main.h"

uint8_t csr=0;

ISR(BADISR_vect)
{

  for(;;) UART_DATA_REG='!';
}

ISR(INT0_vect)
{

  for(;;) UART_DATA_REG='*';
}

int main(void)
{
    csr=MCU_CONTROL_STATUS_REG;
    MCU_CONTROL_STATUS_REG=0;

    // Initialization
    GENERAL_INTERRUPT_CONTROL_REG &= ~(_BV(ENABLE_INT0_INTR)|_BV(ENABLE_INT1_INTR)|_BV(ENABLE_INT2_INTR));
    inituart(); // Initialize UART.

    if(csr) {

      if(csr & _BV(PORF)) UART_DATA_REG='P';
      else if(csr & _BV(EXTRF)) UART_DATA_REG='E';
      else if(csr & _BV(BORF)) UART_DATA_REG='B';
      else if(csr & _BV(WDRF)) UART_DATA_REG='W';
      else if(csr & _BV(JTRF)) UART_DATA_REG='J';
      else for(;;) UART_DATA_REG='?';
    }
    timer_init();
    init_temp_sensor_data();
    initfan_data(); //Initialize fans.

    initovertemp();

    set_pin_as_output(BUZZ); //Set buzzer pin as an output
    set_pin_as_output(MCU_LED); //Set buzzer pin as an output

    int ret=eeprom_load();
    initfans(); //Initialize fans.
    initadc(); // Initialize UART.
    sei();

    if(ret) {
      buzz_signal(500);
      idle_timer_delay_millis(500);
    }

    set_pin(BUZZ, true);
    set_pin(MCU_LED, true);
    idle_timer_delay_millis(1000);
    set_pin(BUZZ, false);
    set_pin(MCU_LED, false);
    watchdogConfig(WATCHDOG_1S);

    lm75a_begin();

    watchdogConfig(WATCHDOG_1S);
    watchdogReset();

    struct cmd cmd;

    watchdogReset();

    while(1) {
      reset_buzz_alarm();

      if(!read_cmd(&cmd)) update_fans();
      update_buzz_alarm();
      watchdogReset();
    }
}
