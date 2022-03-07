include "mbed.h"
#include "lcd_DOGM204_i2c.h"
 
DOGM204I2C  lcd(P0_0, P0_1, 0, 100000);
 
int main() {
 
    lcd.init();  // Initialize display
    lcd.cls();   // Clear display
 
    lcd.display_set(DOGM204I2C::LCD_DISPLAY_ON);  // optional | LCD_CURSOR_ON | LCD_BLINK_ON
 
    lcd.set_pos(DOGM204I2C::LCD_LINE1);
    lcd.write((char *)"*** Hello world *** ");
 
    lcd.set_pos(DOGM204I2C::LCD_LINE2);
    lcd.write((char *)"01234567890123456789");
 
    lcd.set_pos(DOGM204I2C::LCD_LINE3);
    lcd.write((char *)"ABCDEFGHIJKLMNOPQRST");
 
    lcd.set_pos(DOGM204I2C::LCD_LINE4);
    lcd.write((char *)"abcdefghijklmnopqrst");
 
}
