#include "HAL.h"

void setupPins()
{
    pinMode(S0, OUTPUT);
    pinMode(S1, OUTPUT);
    pinMode(S2, OUTPUT);
    pinMode(S3, OUTPUT);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTONS_PIN, INPUT);
    pinMode(AUXBUTTONS_PIN, INPUT);
    pinMode(ENCODERS_A_PIN, INPUT);
    pinMode(ENCODERS_B_PIN, INPUT);
    pinMode(ENCODERS_C_PIN, INPUT);
}

void sendBits(char i)
{
    digitalWriteFast(S0, i & 1);
    digitalWriteFast(S1, i & 2);
    digitalWriteFast(S2, i & 4);
    digitalWriteFast(S3, i & 8);
    delayMicroseconds(100);
}

bool getButtons()
{
    return digitalReadFast(BUTTONS_PIN);
}

bool getAuxButtons()
{
    return digitalReadFast(AUXBUTTONS_PIN);
}

bool getEncodersA()
{
    return digitalReadFast(ENCODERS_A_PIN);
}

bool getEncodersB()
{
    return digitalReadFast(ENCODERS_B_PIN);
}
bool getEncodersC()
{
    return digitalReadFast(ENCODERS_C_PIN);
}

void writeLed(bool value)
{
    digitalWriteFast(LED_PIN, value);
}