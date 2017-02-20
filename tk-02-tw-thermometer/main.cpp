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
       LCD_PRINTF("Waiting for msg");
       wait(1);

       memset(buffer, 0x00, sizeof (buffer));
       if (JOYSTICK_PUSHED) {
    	   SERIAL_RECV(buffer, 12);
       }
       LCD_CLS();
       LCD_LOCATE(0, 0);
       LCD_PRINTF("Get: %s", buffer);

       wait(5);
   }
}
