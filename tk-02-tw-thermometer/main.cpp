#include "mbed.h"
#include "codecool_lcd.h"
#include "codecool_i2c.h"
#include "codecool_serial.h"
#include "codecool_joystick.h"
#include "codecool_pwm.h"
#include "codecool_shield_names.h"
#define LM75_ADDRESS 0x90

void led_init();
void led_switch(float temp);
#define LM75_ADDRESS 0x90

int sender_main() {
    SERIAL_BAUD(9600);
    SERIAL_SET_NON_BLOCKING();

    I2C_FREQ(100000);

    uint8_t buffer[12];
    memset(buffer, 0x00, sizeof(buffer));

    float temp;

    LCD_CLS();
    LCD_LOCATE(0, 0);
    LCD_PRINTF("Starting...");

    while (1) {


    	buffer[0] = 0x00;
		I2C_WRITE(LM75_ADDRESS, buffer, 1);

		memset(buffer, 0x00, sizeof (buffer));
		I2C_READ(LM75_ADDRESS, buffer, 2);


		int8_t _int_part = (int8_t)buffer[0];
		temp = _int_part + 0.5f * ((buffer[1]&0x80)>>7);

		LCD_CLS();
		LCD_LOCATE(0, 0);
		LCD_PRINTF("Temperature: %0.1f\n", temp);

		wait(0.2);

    	if(JOYSTICK_PUSHED == 1){

            LCD_PRINTF("Sending data...");
            LCD_CLS();
            LCD_LOCATE(0, 0);
            SERIAL_SEND(buffer, 12);
            LCD_PRINTF("Data sent: %s", buffer);
            wait(1);

    	}
    }
}


int receiver_main(){
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

void led_init() {
	PinName leds[3] = {LED_GREEN_SHIELD, LED_RED_SHIELD, LED_BLUE_SHIELD};
	for (int i = 0; i < 3;i++) {
		PWM_INIT(leds[i]);
		PWM_FREQUENCY(leds[i], 2000);
		PWM_SET_PULSE_WIDTH(leds[i],0);
	}
}

void led_switch(float temp){
	led_init();
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
