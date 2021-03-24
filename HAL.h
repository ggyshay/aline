#include "Arduino.h"
#include <string>
#define S0 9
#define S1 10
#define S2 11
#define S3 12
#define LED_PIN 24
#define BUTTONS_PIN 25
#define AUXBUTTONS_PIN 26
#define ENCODERS_A_PIN 28
#define ENCODERS_B_PIN 29
#define ENCODERS_C_PIN 27

void setupPins();

void sendBits(char i);

bool getButtons();
bool getAuxButtons();

bool getEncodersA();

bool getEncodersB();
bool getEncodersC();

void writeLed(bool value);