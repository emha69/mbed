	

/** EA DOGM204 LCD class
 *
 * Provides access to the I2C connected Electronic Assembly DOGM204 LCD display
 * with SSD1803A controller (www.lcd-module.de)
 *
 * Version: 1.00.000
 * Date :   02.01.2020
 * Author:  Marjan Hanc, www.m-hub.eu
 *
 * Note: This library does not support SPI and 4/8-Bit I/O modes of the display.
 *      It is assumed, that the RS address selection pin is statically connected
 *      either to the GND (SA=0) or VCC (SA=1).
 **/
 
#include "lcd_DOGM204_i2c.h"
#include "mbed.h"
 
/*\brief Initialises the LCD with respective I2C interface
  \param sda        I2C SDA pin mapping
  \param scl        I2C SCL pin mapping
  \param SA         SA0 (selector) address of the LCD display
  \param freq       Frequency of the I2C clock
*/
 
DOGM204I2C::DOGM204I2C(PinName sda, PinName scl, char SA, int frequency) : i2c(sda,scl) {
 
     if (SA > 1)
            error("DOGM204I2C: SA is out of range, must be 0..1\n");
     else
         _baseAdr = LCD_ADR + (SA << 1); // sets the base address of the LCD
 
     if (frequency > Frequency_400KHz)
         error("DOGM204I2C: I2C frequency out of range, must be less than 400 kHz\n");
 
     i2c.frequency(frequency);
}
 
/*\brief Sends command with one data byte to the LCD
  \param cmd        LCD command
  \param dta        Data byte
  \return status    Returns true, if the command was successful
*/
 
bool DOGM204I2C::lcd_i2c_write(char cmd, char dta)
{
    char data[] = {cmd, dta};
 
    if (i2c.write(_baseAdr, data, 2)) {
 
        return false;
    }
 
    return true;
}
 
/*\brief Reads one byte (command status) of the LCD
  \param cmd        LCD command
  \return data      Data byte
*/
 
char DOGM204I2C::lcd_i2c_read(char cmd)
{
    char data[] = {cmd};
 
    i2c.write(_baseAdr, data, 1);    // note, that the base address occupies bits b1..b7
    i2c.read(_baseAdr + 1, data, 1); // and that the b0 is R=1/W=0
 
    return(data[0]);
}
 
/*\brief Writes single command to the LCD by checking the display status first
  \note  The command is not fail safe. If the display doesn't return "not busy" it might hang forever
  \param cmd    LCD command
  \return stat  Returns true if successful
*/
 
bool DOGM204I2C::lcd_write_cmd(char data)
{
    //check and wait, if LCD is busy
    while(lcd_i2c_read(LCD_STATUS) & LCD_BUSY);
 
    // write command, control byte C0=0 & D/C = 0
    return lcd_i2c_write(0x80, data);
}
 
/*\brief Writes data byte to the LCD by checking the display's busy flag BF first
  \note  The command is not fail safe. If the display doesn't return "not busy" it might hang forever.
  \param data   LCD data
  \return stat  Returns true if successful
*/
bool DOGM204I2C::lcd_write_data(char data)
{
    //check and wait, if LCD is busy
    while(lcd_i2c_read(LCD_STATUS) & LCD_BUSY);
    //write data, control byte C0=0 & D/C = 1
    return lcd_i2c_write(0x40, data);
}
 
// -- Public functions --
 
/* \brief Sets the LCD display to the provided mode
 * \param mode LCD_DISPLAY_ON|OFF LCD_CURSOR_ON|OFF LCD_BLINK_ON|OFF
 */
void DOGM204I2C::display_set(char mode)
{
    if (!lcd_write_cmd(0x08+mode))
        error("DOGM204I2C: Write command in lcd_display_set() failed!\n");
}
 
/* \brief Sets the cursor to the given position counted from the origin
 * \param pos  position
 */
void DOGM204I2C::set_pos(char pos)
{
    if (!lcd_write_cmd(LCD_HOME+pos))
        error("DOGM204I2C: Write command in lcd_set_pos() failed!\n");
}
 
/* \brief Writes a single character at given position in the given line
 * \param pos LCD_LINEx+position
 */
void DOGM204I2C::write_char(char line, char pos, char ch)
{
    char lcdpos = pos + 0x20 * line;
 
    if (!lcd_write_cmd(lcdpos))
        error("DOGM204I2C: Write command in lcd_write_char() failed!\n");
    else
        if (!lcd_write_data(ch))
            error("DOGM204I2C: Write data in lcd_write_char() failed!\n");
}
 
/* \brief Writes null terminated string to the LCD
 * \param s Null terminated string
 */
void DOGM204I2C::write(char *s)
{
    while(*s)
    {
        if (!lcd_write_data(*s++)) {
            error("DOGM204I2C: Write data in lcd_write() failed!\n");
            break;
        }
    }
}
 
/* \brief Clears the LCD display and sets the cursor to the top-left position
 */
void DOGM204I2C::cls()
{
    lcd_write_cmd(LCD_CLEAR);   // Clear display
    lcd_write_cmd(LCD_RTHOME);  // Set cursor position to home (top left)
}
 
/* \brief Initializes LCD display
 * \note  This sequence will set 4 lines, 5 dots, and top orientation at maximum 
    contrast with cursor off
 */
void DOGM204I2C::init()
{
    lcd_write_cmd(0x3A);        // 8-Bit data length, extension Bit RE=1; REV=0
    lcd_write_cmd(LCD_4LINE_MODE | LCD_FONT_5DOT ); // 4 lines, 5 dots charset
 
    lcd_write_cmd(0x80);        // Pixel shift 0
 
    lcd_write_cmd(LCD_TOPVIEW); // Set LCD orientation
    lcd_write_cmd(0x1E);        // Bias setting BS1=1
 
    lcd_write_cmd(0x39);        // 8-Bit data length extension Bit RE=0; IS=1
    lcd_write_cmd(0x1B);        // BS0=1 -> Bias=1/6
 
    lcd_write_cmd(0x6E);        // Divider ON and set value
    lcd_write_cmd(0x57);        // Booster ON and set contrast (BB1=C5, DB0=C4)
    lcd_write_cmd(0x7B);        // Set optimum contrast (DB3-DB0=C3-C0)
 
    lcd_write_cmd(0x38);        // 8-Bit data length extension Bit RE=0; IS=0
}
