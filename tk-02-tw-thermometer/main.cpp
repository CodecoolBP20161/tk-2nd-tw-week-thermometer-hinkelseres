#include "mbed.h"
#include "codecool_lcd.h"
#include "codecool_i2c.h"
#include "codecool_serial.h"
#include "codecool_joystick.h"


#define LM75_ADDRESS 0x90

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
       }
       LCD_CLS();
       LCD_LOCATE(0, 0);
       LCD_PRINTF("Got: %0.1f", temp);

       wait(5);
   }
}
