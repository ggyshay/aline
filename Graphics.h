#include "DisplayDriver.h"
#include <avr/pgmspace.h>
#include "Arduino.h"
class Graphics
{
private:
    DisplayDriver disp;
    int cursor = 0;

public:
    void blackLine();
    void selectionLine(unsigned char start, unsigned char end, char dataLength);
    void writePrettyLine();
    void plotGraph(unsigned const char *data, char dataLength, bool isThin);
    void plotGraph(int *data, char dataLength, bool isThin);
    void writeLine(const char *s);
    void init();
    void buildTwoStringScreen(const char *str1, const char *str2);
    void titlePlot(const char *, unsigned char *, char, bool, unsigned char, unsigned char);
    void titlePlot(const char *, int *, char, bool, unsigned char, unsigned char);
    void buildSplash();
    void beginSession();
    void endSession();
};