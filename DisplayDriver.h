#include "Arduino.h"

#include <i2c_t3.h>
#ifndef _DISPLAY_DRIVER_H_
#define _DISPLAY_DRIVER_H_
class DisplayDriver
{
public:
    void init();
    void putScreen();
    static char dataBuffer[1024];

private:
    static unsigned int lastTransmit;
    static bool pendingTransmission;
    void sendCmd(unsigned char cmd); //send a single command (see datasheet)
    static void triggerTransmission();
    static void transmitScreen();
    static void transmitBuffer();
};

#endif