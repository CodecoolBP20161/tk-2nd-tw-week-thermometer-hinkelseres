#include "mbed.h"
#include "codecool_lcd.h"
#include "codecool_i2c.h"
#include "codecool_serial.h"
#include "codecool_joystick.h"
#include "codecool_pwm.h"
#include "codecool_shield_names.h"


#define LM75_ADDRESS 0x90

void led_switch(float temp);

int main(){
   I2C_FREQ(100000);

   LCD_CLS();
   LCD_LOCATE(0, 0);
   LCD_PRINTF("Receiver application starting....");
   wait(2);
   SERIAL_BAUD(9600);
   SERIAL_SET_NON_BLOCKING();
   uint8_t buffer[12];
   float temp;

   while (true) {


       LCD_CLS();
       LCD_LOCATE(0, 0);
       LCD_PRINTF("Waiting for temperature...");
       wait(1);

       memset(buffer, 0x00, sizeof (buffer));
       if (JOYSTICK_PUSHED) {
    	   SERIAL_RECV(buffer, 12);
    	   uint8_t int_part = (int8_t)buffer[0];
    	   temp = int_part + 0.5f * ((buffer[1]&0x80)>>7);
    	   led_switch(temp);
       }
       LCD_CLS();
       LCD_LOCATE(0, 0);
       LCD_PRINTF("Got: %0.1f", temp);

       wait(5);
   }
}

void led_switch(float temp){
	PWM_INIT(LED_GREEN_SHIELD);
	PWM_INIT(LED_RED_SHIELD);
	PWM_INIT(LED_BLUE_SHIELD);
	PWM_FREQUENCY(LED_RED_SHIELD, 2000);
	PWM_FREQUENCY(LED_GREEN_SHIELD, 2000);
	PWM_FREQUENCY(LED_BLUE_SHIELD, 2000);
	PWM_SET_PULSE_WIDTH(LED_GREEN_SHIELD, 0);
	PWM_SET_PULSE_WIDTH(LED_RED_SHIELD, 0);
	PWM_SET_PULSE_WIDTH(LED_BLUE_SHIELD, 0);
	if (temp < 10){
		PWM_SET_PULSE_WIDTH(LED_BLUE_SHIELD, 100);
	} else if (temp >=10 && temp < 18) {
		PWM_SET_PULSE_WIDTH(LED_BLUE_SHIELD, 100);
		PWM_SET_PULSE_WIDTH(LED_GREEN_SHIELD, 100);
	} else if (temp >=18 && temp < 30){
		PWM_SET_PULSE_WIDTH(LED_GREEN_SHIELD, 100);
	} else if (temp >=30 && temp < 34) {
		PWM_SET_PULSE_WIDTH(LED_GREEN_SHIELD, 100);
		PWM_SET_PULSE_WIDTH(LED_RED_SHIELD, 100);
	} else {
		PWM_SET_PULSE_WIDTH(LED_RED_SHIELD, 100);
	}
}
