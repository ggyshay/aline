#include "DisplayDriver.h"
#include <avr/pgmspace.h>
#include "Arduino.h"
class Graphics
{
private:
    DisplayDriver disp;
    int cursor = 0;
    void blackLine();
    void writeLine(const char *s);
    void plotGraph(unsigned const char *data, char dataLength, bool isThin);
    void plotGraph(int *data, char dataLength, bool isThin);
    void selectionLine(unsigned char, unsigned char, char dataLength);
    void writePrettyLine();

public:
    void init();
    void buildTwoStringScreen(const char *str1, const char *str2);
    void titlePlot(const char *, unsigned char *, char, bool, unsigned char, unsigned char);
    void titlePlot(const char *, int *, char, bool, unsigned char, unsigned char);
    void buildSplash();
};