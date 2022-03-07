/** EA DOGM204 LCD header file
 *
 * Provides access to the I2C connected Electronic Assembly DOGM204 LCD display
 * with SSD1803A controller (www.lcd-module.de)
 *
 * Version: 1.00.000
 * Date :   02.01.2020
 * Author:  Marjan Hanc, www.m-hub.eu
 *
 * Note: This library does not support SPI and 4/8-Bit I/O modes of display.
 *      It is assumed, that the RS address selection pin is statically connected
 *      either to the GND (SA0=0) or VCC (SA0=1).
 **/
 
#ifndef MBED_LCD_DOGM204
#define MBED_LCD_DOGM204
 
#include "mbed.h"
 
class DOGM204I2C {
 
    public:
 
        enum LCD_Commands {
            LCD_ADR    = 0x78,  // SA0=0, 0x7A when SA0=1
            LCD_CLEAR  = 0x00,  // Clear Display
            LCD_RTHOME = 0x02   // Return home
        };
 
        enum LCD_Status {
            LCD_STATUS = 0x00,
            LCD_DATA   = 0x40,
            LCD_BUSY   = 0x80
        };
 
        enum LCD_Charset {
            LCD_ROMA = 0x00,
            LCD_ROMB = 0x04,
            LCD_ROMC = 0x0C
        };
 
        enum LCD_Mode {
            LCD_CURSOR_LINE  = 0x00,
            LCD_CURSOR_BLOCK = 0x02,
            LCD_TOPVIEW      = 0x05,
            LCD_BOTVIEW      = 0x06,
            LCD_2LINE_MODE   = 0x08, // Base setting for 1 & 2 line mode
            LCD_4LINE_MODE   = 0x09, // Base setting for 3 & 4 line mode
            LCD_FONT_5DOT    = 0x00,
            LCD_FONT_6DOT    = 0x04
        };
 
        enum LCD_Settings {
            LCD_DISPLAY_ON  = 0x04,
            LCD_DISPLAY_OFF = 0x03,
            LCD_CURSOR_ON   = 0x02,
            LCD_CURSOR_OFF  = 0x05,
            LCD_BLINK_ON    = 0x01,
            LCD_BLINK_OFF   = 0x06
        };
 
        enum LCD_Positions {
            LCD_HOME  = 0x80,
            LCD_LINE1 = 0x00,
            LCD_LINE2 = 0x20,
            LCD_LINE3 = 0x40,
            LCD_LINE4 = 0x60
        };
 
        enum Frequency {
            Frequency_100KHz = 100000,
            Frequency_400KHz = 400000
        };
 
        DOGM204I2C(PinName sda, PinName scl, char SA, int frequency);
 
        void display_set(char mode);
        void set_pos(char pos);
        void write_char(char line, char pos, char ch);
        void write(char *s);
        void cls();
        void init();
 
    private:
        char _baseAdr;  // LCD base address (0x78 or 0x7A, depends on SA0)
 
        I2C  i2c;  // I2C interface
 
        bool lcd_i2c_write(char cmd, char dta);
        char lcd_i2c_read(char cmd);
        bool lcd_write_cmd(char data);
        bool lcd_write_data(char data);
};
 
 
#endif
