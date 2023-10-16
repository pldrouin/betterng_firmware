#include "main.h"

int main(void)
{
    //uint8_t eebyte = eeprom_read_byte((uint8_t*)0);

    /* Initialization */    
    idle_init();
    inituart(115200); // Initialize UART.
    initfans(); //Initialize fans.
    initadc(); // Initialize UART.
    sei();

    set_pin_as_output(BUZZ); //Set buzzer pin as an output
    set_pin_as_output(MCU_LED); //Set buzzer pin as an output
    set_pin(BUZZ, true);
    set_pin(MCU_LED, true);
    idle_delay_millis(1000);
    set_pin(BUZZ, false);
    set_pin(MCU_LED, false);

    lm75a_begin();

    add_fan(0);
    add_fan(1);
    add_fan(2);
    add_fan(3);

    fan_adc_calibration(0);
    uart_send_bytes(get_fan_data(0),20);
    fan_adc_calibration(1);
    uart_send_bytes(get_fan_data(1),20);
    fan_adc_calibration(2);
    uart_send_bytes(get_fan_data(2),20);
    fan_adc_calibration(3);
    uart_send_bytes(get_fan_data(3),20);
    //set_fan_voltage_response(0, 3., (12.-3.)/255, 0);
    //set_fan_voltage_response(1, 3., (12.-3.)/255, 0);
    //set_fan_voltage_response(2, 3., (12.-3.)/255, 0);
    set_fan_voltage_response(3, 3., (12.-3.)/255, 0);
    set_fan_output(0, 1);
    set_fan_output(1, 1);
    set_fan_output(2, 1);
    set_fan_output(3, 127);
    idle_delay_millis(100);
    uart_send_bytes(get_fan_data(0),20);
    idle_delay_millis(100);
    uart_send_bytes(get_fan_data(1),20);
    idle_delay_millis(100);
    uart_send_bytes(get_fan_data(2),20);
    idle_delay_millis(100);
    uart_send_bytes(get_fan_data(3),20);
    idle_delay_millis(100);


    //struct cmd cmd;
    //watchdogConfig(WATCHDOG_1S);
    uint16_t wval;

    //cmd.id=2;

    while(1) {
      //if(!read_pin(OVERTEMP_PORT, OVERTEMP_NO)) set_pin(BUZZ_PORT, BUZZ_NO, true);
      //read_cmd(&cmd);
      //cmd.byte1=lm75a_getValue(1)>>8;
      //cmd.byte2=cmd.id+cmd.byte1;
      //send_cmd(&cmd);
      //uart_blocking_send_bytes("Hello World!\n\r", 14);
      //uart_send_bytes("Hello World!\n\r", 14);
      /*
	 uart_send_byte(0xAA);
	 uart_send_bytes(get_fan_data(0),8);
	 uart_send_byte(0xBB);
	 fan_adc_calibration(0);
	 uart_send_byte(0xCC);
	 uart_send_bytes(get_fan_data(0),8);
	 */
      /*
	 wval=analog_sensor_getValue(0);
	 wval=htobe16(wval);
	 uart_send_bytes((uint8_t*)&wval,2);
      //uart_send_byte(adc_getCurrentChannel());
      uart_send_byte(0xDD);
      _delay_us(500000);
      */
      idle_delay_block_minimum_millis_start(100);
      wval=get_fan_rpm(0);
      wval=htobe16(wval);
      uart_send_bytes((uint8_t*)&wval,2);
      wval=get_fan_rpm(1);
      wval=htobe16(wval);
      uart_send_bytes((uint8_t*)&wval,2);
      wval=get_fan_rpm(2);
      wval=htobe16(wval);
      uart_send_bytes((uint8_t*)&wval,2);
      wval=get_fan_rpm(3);
      wval=htobe16(wval);
      uart_send_bytes((uint8_t*)&wval,2);
      idle_delay_block_end();

      watchdogReset();
    }
}
