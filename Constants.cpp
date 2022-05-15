#include "Constants.h"
String noteName[12] = {"C", "C#", "D", "D#",
                       "E", "F", "F#", "G", "G#", "A", "A#", "B"};

String scaleNames[7] = {"IONIAN", "DORIAN", "PHRYGIAN", "LYDIAN", "MIXOLYDIAN", "AEOLIAN", "LOCRIAN"};
char scales[7][12] = {
    {0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 9, 11},  // ionian
    {0, 0, 2, 3, 3, 5, 5, 7, 7, 9, 10, 10}, // dorian
    {0, 1, 1, 3, 4, 5, 5, 7, 8, 8, 10, 0},  // Phrygian
    {0, 0, 2, 2, 4, 4, 6, 7, 7, 9, 9, 11},  // Lydian
    {0, 0, 2, 2, 4, 5, 5, 7, 7, 9, 10, 10}, // Mixolydian
    {0, 0, 2, 3, 3, 5, 5, 7, 8, 8, 10, 10}, // aeolian
    {0, 1, 1, 3, 3, 5, 6, 6, 8, 8, 10, 0}   // Locrian
};

unsigned long MAX_U_LONG_VAL = 4294967295;