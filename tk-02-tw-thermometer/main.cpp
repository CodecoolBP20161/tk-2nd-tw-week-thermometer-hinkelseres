#include "mbed.h"
#include "codecool_serial.h"
#include "codecool_joystick.h"
#include "codecool_lcd.h"

int main() {
    SERIAL_BAUD(9600);
    SERIAL_SET_NON_BLOCKING();

    uint8_t buffer[64];
    memset(buffer, 0x00, sizeof(buffer));

    LCD_CLS();
    LCD_LOCATE(0, 0);
    LCD_PRINTF("Starting...");

    while (1) {
    	if(JOYSTICK_PUSHED == 1){
            strncpy((char*)buffer, "Hello there!", 12);
            LCD_PRINTF("Sending data...");
            LCD_CLS();
            LCD_LOCATE(0, 0);
            SERIAL_SEND(buffer, strlen((char*)buffer));
            LCD_PRINTF("Data sent: %s", buffer);
            wait(1);

    	}


//        while(SERIAL_AVAILABLE() < 12 );
//
//        memset(buffer, 0x00, sizeof(buffer));
//        SERIAL_RECV(buffer, 12);
//        D("Received: %s", buffer);
//
//        wait(5);
    }
}
