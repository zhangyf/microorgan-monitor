#ifndef INCLUDE_I2C16X2_H_
#define INCLUDE_I2C16X2_H_

#include "common.h"

namespace microorgan_monitor
{
#define LCD_CHR 1            // RS = 1 = Character.
#define LCD_CMD 0            // RS = 0 = Command.
#define LCD_RW 0b0010        // RW = 0 = Write; RW 1 = Read. However we should NOT use Read mode.
#define LCD_EN 0b0100        // Enable bit. Must be pulse HIGH for at least 450ns after sending 4 bits.
#define LCD_BACKLIGHT 0b1000 // Backlight control. 0 = OFF, 1 = ON.
#define LINE1 0x80           // 1st line
#define LINE2 0xC0           // 2nd line

    class I2C16X2
    {
    public:
        I2C16X2(unsigned int addr);
        ~I2C16X2() {};
        void SetAddr(unsigned int addr);
        void PrintInt(int i);              // print integer
        void PrintFloat(float float_value);    // print float
        void PrintString(const std::string& str);   // print string
        void PrintChar(char val);          // print a char
        void LcdLoc(int line);             // move cursor
        void LcdClear();               // clr LCD return home

    private:
        void LcdByte(int bits, int mode); // send a byte of data
        void LcdToggleEnable(int bits);  // toggle Enable bit
        unsigned int GetAddr();
        unsigned int& GetFd();
        unsigned int addr_;
        unsigned int fd_;
    };
}

#endif