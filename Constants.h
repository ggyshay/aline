#include "Arduino.h"
#ifndef CONSTANTS_H
#define CONSTANTS_H
#define MSG_SEQ_LEN_UP 0
#define MSG_SEQ_LEN_DOWN 1
#define MSG_SEL_PITCH_UP 2
#define MSG_SEL_PITCH_DOWN 3
#define MSG_SEL_OCT_UP 4
#define MSG_SEL_OCT_DOWN 5
#define MSG_SEL_DURATION_UP 6
#define MSG_SEL_DURATION_DOWN 7
#define MSG_SEL_VELOCITY_UP 8
#define MSG_SEL_VELOCITY_DOWN 9
#define MSG_ERASE_SEQ 10
#define MSG_EASE_SEL 11
#define MSG_SEED_UP 12
#define MSG_SEED_DOWN 13
#define MSG_SCALE_UP 14
#define MSG_SCALE_DOWN 15
#define MSG_ROOT_UP 16
#define MSG_ROOT_DOWN 17
#define MSG_OCT_UP 18
#define MSG_OCT_DOWN 19
#define MSG_RANDOMIZE 20
#define MSG_COPY 21
#define MSG_PASTE 22

extern unsigned long MAX_U_LONG_VAL;
extern String noteName[12];
extern char scales[7][12];
extern String scaleNames[7];
#endif