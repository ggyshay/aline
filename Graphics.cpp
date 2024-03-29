
#include "Graphics.h"
static const char font_bmap[18 * 47] PROGMEM =
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2, 0x2, 0x2, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0xe0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x30, 0xc, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x23, 0x20, 0x18, 0x7, 0x0, 0x0, 0x0, 0x20, 0x10, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x0, 0x0, 0x0, 0x0, 0x30, 0x10, 0x10, 0x30, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x28, 0x24, 0x23, 0x21, 0x0, 0x0, 0x0, 0x0, 0x20, 0x10, 0x10, 0xa0, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x21, 0x21, 0x13, 0x1e, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x60, 0xf0, 0xf0, 0x0, 0x0, 0x0, 0xc, 0xb, 0x8, 0x8, 0x3f, 0x3f, 0x8, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x31, 0x21, 0x21, 0x33, 0x1e, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x30, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x3, 0x1f, 0x31, 0x21, 0x21, 0x1a, 0xc, 0x0, 0x0, 0x10, 0x10, 0x10, 0x10, 0x90, 0x70, 0x10, 0x0, 0x0, 0x0, 0x20, 0x38, 0xe, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x10, 0x10, 0x10, 0xe0, 0x0, 0x0, 0x0, 0xc, 0x1e, 0x21, 0x21, 0x21, 0x1e, 0xc, 0x0, 0x0, 0xc0, 0xe0, 0x10, 0x10, 0x30, 0xe0, 0x80, 0x0, 0x0, 0x0, 0x21, 0x22, 0x22, 0x32, 0x1f, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x80, 0x80, 0x40, 0x20, 0x0, 0x0, 0x0, 0x0, 0x1, 0x2, 0x6, 0x4, 0x8, 0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0, 0x0, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x4, 0x0, 0x0, 0x0, 0x20, 0x40, 0x40, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x8, 0x4, 0x4, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0, 0x20, 0x10, 0x10, 0xb0, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x36, 0x3, 0x1, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x20, 0x10, 0x90, 0x90, 0x20, 0xc0, 0x0, 0x0, 0x3f, 0x40, 0x9b, 0x90, 0x9f, 0x90, 0xc, 0x0, 0x0, 0x0, 0x0, 0xe0, 0x30, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x38, 0xf, 0x5, 0x4, 0x5, 0xf, 0x38, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x21, 0x21, 0x21, 0x12, 0xc, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x20, 0x20, 0x10, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x30, 0x60, 0xc0, 0x0, 0x0, 0x0, 0x3f, 0x20, 0x20, 0x30, 0x18, 0xf, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x21, 0x21, 0x21, 0x21, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x22, 0x32, 0x1e, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x3f, 0x20, 0x20, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x20, 0x20, 0x20, 0x20, 0x1f, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x80, 0x40, 0x20, 0x10, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x3, 0x6, 0x18, 0x30, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x20, 0x20, 0x20, 0x20, 0x0, 0x0, 0x0, 0xf0, 0x70, 0xc0, 0x0, 0xc0, 0x30, 0xf0, 0x0, 0x0, 0x3f, 0x0, 0x0, 0x3, 0x1, 0x0, 0x3f, 0x0, 0x0, 0x0, 0xf0, 0xc0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x0, 0x3, 0xc, 0x3f, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x20, 0x18, 0xf, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xf0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x60, 0x10, 0x10, 0x10, 0x60, 0xc0, 0x0, 0x0, 0xf, 0x18, 0x20, 0x20, 0x60, 0x98, 0x8f, 0x0, 0x0, 0x0, 0xf0, 0x10, 0x10, 0x10, 0xe0, 0x40, 0x0, 0x0, 0x0, 0x3f, 0x1, 0x3, 0x7, 0x18, 0x30, 0x0, 0x0, 0x0, 0xe0, 0x10, 0x10, 0x10, 0x20, 0x0, 0x0, 0x0, 0x0, 0x30, 0x21, 0x21, 0x22, 0x1e, 0x0, 0x0, 0x0, 0x10, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0x20, 0x20, 0x20, 0x1f, 0x0, 0x0, 0x0, 0x70, 0x80, 0x0, 0x0, 0x0, 0x80, 0x70, 0x0, 0x0, 0x0, 0x3, 0x1c, 0x30, 0x1c, 0x3, 0x0, 0x0, 0x10, 0xf0, 0x0, 0x0, 0x80, 0x0, 0x0, 0xf0, 0x10, 0x0, 0x1f, 0x38, 0xf, 0x3, 0xf, 0x38, 0x1f, 0x0, 0x0, 0x10, 0x60, 0xc0, 0x0, 0x80, 0x60, 0x10, 0x0, 0x0, 0x20, 0x18, 0xc, 0x3, 0x6, 0x18, 0x30, 0x0, 0x0, 0x10, 0x60, 0xc0, 0x0, 0xc0, 0x60, 0x10, 0x0, 0x0, 0x0, 0x0, 0x1, 0x3f, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x10, 0x10, 0xd0, 0x70, 0x10, 0x0, 0x0, 0x20, 0x38, 0x2c, 0x23, 0x20, 0x20, 0x20, 0x0};
void Graphics::init()
{
    disp.init();
}
void Graphics::blackLine()
{
    byte col = 0;
    while (col < 128)
    {
        disp.dataBuffer[cursor++] = 0x0;
        ++col;
    }
}

void Graphics::selectionLine(byte s, byte e, char dataLength)
{
    int xScale = 16 * (1 + (dataLength - 1) / 16);
    int xDivisor = 128 / xScale;

    for (byte col = 0; col < 128; ++col)
    {
        disp.dataBuffer[cursor++] = ((col / xDivisor) >= s && (col / xDivisor) <= e) ? 0xE0 : 0;
    }
}

void Graphics::writeLine(const char *s)
{
    for (byte page = 0; page < 2; ++page)
    {
        byte col = 0;
        for (; col < 9; ++col)
        {

            disp.dataBuffer[cursor++] = 0x0; // 9 columns padding
        }

        byte index = 0;
        char c;
        while (*(s + index) != 0x00 && index < 15)
        {
            c = (*(s + index) - 0x2c);
            if (*(s + index) == 0x20)
            {
                for (char i = 0; i < 9; i++)
                {
                    disp.dataBuffer[cursor++] = 0x0;
                    ++col;
                }
            }
            else if (*(s + index) == 0x23)
            {
                for (char i = 0; i < 9; i++)
                {
                    disp.dataBuffer[cursor++] = cursor % 4 == 3 ? 0xff : 0x10;
                    ++col;
                }
            }
            else
            {
                for (char i = 0; i < 9; i++)
                {
                    disp.dataBuffer[cursor++] = pgm_read_byte(&(font_bmap[18 * c + i + page * 9]));
                    ++col;
                }
            }
            ++index;
        }
        while (col < 128)
        {
            disp.dataBuffer[cursor++] = 0x0;
            ++col;
        }
    }
}

void Graphics::plotGraph(unsigned const char *data, char dataLength, bool isThin)
{
    int min = 1000;
    int max = -1;
    int xScale = 16 * (1 + (dataLength - 1) / 16);
    int xDivisor = 128 / xScale;
    for (int i = 0; i < dataLength; ++i)
    {
        min = data[i] < min ? data[i] : min;
        max = data[i] > max ? data[i] : max;
    }
    unsigned long int scaled[64] = {};
    for (int i = 0; i < dataLength; ++i)
    {
        byte value = (data[i] - min) * 31 / (max - min);
        scaled[i] = ((1 << 32) - 1) - ((1 << (32 - value - 1)) - 1);
    }
    for (int i = dataLength; i < 16; i++)
    {
        scaled[i] = 0;
    }

    for (byte page = 0; page < 4; page++)
    {
        for (byte col = 0; col < 128; ++col)
        {
            byte columnValue;
            if (isThin)
            {
                columnValue = col % xDivisor == (xDivisor / 2) ? (scaled[col / xDivisor] & (0xFF << (8 * page))) >> (8 * page) : 0;
            }
            else
            {
                columnValue = col % xDivisor == (xDivisor - 1) ? 0 : (scaled[col / xDivisor] & (0xFF << (8 * page))) >> (8 * page);
            }
            disp.dataBuffer[cursor++] = columnValue;
        }
    }
}
void Graphics::plotGraph(int *data, char dataLength, bool isThin)
{
    int min = 1000;
    int max = -1;
    int xScale = 16 * (1 + (dataLength - 1) / 16);
    int xDivisor = 128 / xScale;
    for (int i = 0; i < dataLength; ++i)
    {
        min = data[i] < min ? data[i] : min;
        max = data[i] > max ? data[i] : max;
    }
    unsigned long int scaled[64] = {};
    for (int i = 0; i < dataLength; ++i)
    {
        int value = (data[i] - min) * 31 / (max - min);
        scaled[i] = ((1 << 32) - 1) - ((1 << (32 - value - 1)) - 1);
    }
    for (int i = dataLength; i < 16; i++)
    {
        scaled[i] = 0;
    }

    for (byte page = 0; page < 4; page++)
    {
        for (byte col = 0; col < 128; ++col)
        {
            byte columnValue;
            if (isThin)
            {
                columnValue = col % xDivisor == (xDivisor / 2) ? (scaled[col / xDivisor] & (0xFF << (8 * page))) >> (8 * page) : 0;
            }
            else
            {
                columnValue = col % xDivisor == (xDivisor - 1) ? 0 : (scaled[col / xDivisor] & (0xFF << (8 * page))) >> (8 * page);
            }
            disp.dataBuffer[cursor++] = columnValue;
        }
    }
}

void Graphics::titlePlot(const char *str1, byte *data, char dataLength, bool isThin, byte selectionStart, byte selectionEnd)
{
    cursor = 0;
    writeLine(str1);
    blackLine();
    plotGraph(data, dataLength, isThin);
    selectionLine(selectionStart, selectionEnd, dataLength);
    disp.putScreen();
}
void Graphics::titlePlot(const char *str1, int *data, char dataLength, bool isThin, byte selectionStart, byte selectionEnd)
{
    cursor = 0;
    writeLine(str1);
    blackLine();
    plotGraph(data, dataLength, isThin);
    selectionLine(selectionStart, selectionEnd, dataLength);
    disp.putScreen();
}

void Graphics::buildTwoStringScreen(const char *str1, const char *str2)
{
    cursor = 0;
    blackLine();
    writeLine(str1);
    blackLine();
    writeLine(str2);
    blackLine();
    blackLine();
    disp.putScreen();
}

void Graphics::writePrettyLine()
{
    char circle[8] = {0xff, 0xc3, 0x99, 0xa5, 0xa5, 0x99, 0xc3, 0xff};
    for (byte i = 0; i < 16; i++)
    {
        for (byte j = 0; j < 8; j++)
            disp.dataBuffer[cursor++] = circle[j];
    }
}

void Graphics::buildSplash()
{
    cursor = 0;
    writePrettyLine();
    writePrettyLine();
    blackLine();
    writeLine("   ALINE   ");
    blackLine();
    writePrettyLine();
    writePrettyLine();
    disp.putScreen();
}

void Graphics::beginSession()
{
    cursor = 0;
}
void Graphics::endSession()
{
    disp.putScreen();
}
