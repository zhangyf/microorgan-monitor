#include "I2C16X2.h"

microorgan_monitor::I2C16X2::I2C16X2(unsigned int addr)
{
    // since we use 4-bit mode,
    // we can combine (2), (3) as 0011 0011 = 0x33
    // and combine (4), (5) as 0011 0010 = 0x32
    SetAddr(addr);       // set lcd
    delay(15);               // (1) wait >15ms for LCD to power On
    LcdByte(0x33, LCD_CMD); // (2) and (3)
    LcdByte(0x32, LCD_CMD); // (4) and (5)

    // initial commands
    LcdByte(0x28, LCD_CMD); // 4-bit mode, 2 line, small font size
    LcdByte(0x0C, LCD_CMD); // Display ON, Cursor OFF, Cursor blink OFF
    LcdByte(0x06, LCD_CMD); // Cursor move direction left to right, no display shift
    LcdByte(0x01, LCD_CMD); // Clear display
}

// clear lcd and move cursor to home location 0x80 (LINE 1 ROW 0)
void microorgan_monitor::I2C16X2::LcdClear() {
	LcdByte(0x01, LCD_CMD);
}

// move cursor to location on LCD
// example: to go to column 3 of row 1 calls: lcdLoc(LINE1+3). Column start from 0
void microorgan_monitor::I2C16X2::LcdLoc(int line) {
	LcdByte(line, LCD_CMD);
}

// print char to LCD at current position
void microorgan_monitor::I2C16X2::PrintChar(char val) {
	LcdByte(val, LCD_CHR);
}

// print a string of any length to LCD (no text wrap)
void microorgan_monitor::I2C16X2::PrintString(const std::string& str) {
	for (char ch : str)
		LcdByte(ch, LCD_CHR);
}

// print a float number, up to 4 digits before decimal point and 2 digits after decimal point
void microorgan_monitor::I2C16X2::PrintFloat(float float_value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << float_value;
    std::string string_value = oss.str();
	PrintString(string_value);
}

// print integer as string
void microorgan_monitor::I2C16X2::PrintInt(int i) {
	PrintString(std::to_string(i));
}

void microorgan_monitor::I2C16X2::SetAddr(unsigned int addr)
{
    addr_ = addr;
    fd_ = wiringPiI2CSetup(addr);
}

unsigned int microorgan_monitor::I2C16X2::GetAddr()
{
    return addr_;
}

unsigned int &microorgan_monitor::I2C16X2::GetFd()
{
    return fd_;
}

void microorgan_monitor::I2C16X2::LcdByte(int bits, int mode)
{
    // Send byte to I2C
    // bits = the data
    // mode = 1 for character, 0 for command

    // Data is 1 byte,
    // left-most 4 bits are actual data nibble,
    // right-most 4 bits are control bits.
    // So, to send 1 byte to LCD, we must write 6 times to I2C:
    // data's high bits (left-most 4 bits of data) + LCD_EN HIGHT + LCD_EN LOW
    // data's low bits (right-most 4 bits of data) + LCD_EN HIGHT + LCD_EN LOW

    int bits_high;
    int bits_low;
    // uses the two half byte writes to LCD
    bits_high = mode | (bits & 0xF0) | LCD_BACKLIGHT;
    bits_low = mode | ((bits << 4) & 0xF0) | LCD_BACKLIGHT;

    // High bits
    wiringPiI2CWrite(GetFd(), bits_high);
    LcdToggleEnable(bits_high);

    // Low bits
    wiringPiI2CWrite(GetFd(), bits_low);
    LcdToggleEnable(bits_low);
}

void microorgan_monitor::I2C16X2::LcdToggleEnable(int bits)
{
    // Toggle enable pin on LCD display
    wiringPiI2CWrite(GetFd(), (bits | LCD_EN));  // EN HIGH
    delayMicroseconds(500);                      // HIGH for 500us
    wiringPiI2CWrite(GetFd(), (bits & ~LCD_EN)); // EN LOW
    delayMicroseconds(500);                      // Data needs >37us to settle, use 500us to make sure
}