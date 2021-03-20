#include "Arduino.h"
#include <avr/pgmspace.h>
#include <i2c_t3.h>
#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_
class DisplayDriver
{
public:
    void cls(char); //clear the display (optionally with specific byte value)
    void init();
    void putScreen(const char *, const char *);
    void putScreen(const char *, float);

private:
    static unsigned int lastTransmit;
    static void blackLine();
    static void writeLine(char *);
    void sendCmd(unsigned char cmd); //send a single command (see datasheet)
    static char nextStrings[2][14];
    static void triggerTransmission();
    static void transmitScreen();
};

#endif