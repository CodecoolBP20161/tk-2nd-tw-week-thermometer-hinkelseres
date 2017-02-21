#include "mbed.h"
#include "codecool_serial.h"
#include "codecool_joystick.h"
#include "codecool_lcd.h"
#include "codecool_i2c.h"

#define LM75_ADDRESS 0x90


int main() {
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
