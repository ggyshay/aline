#ifndef NOTE_H
#define NOTE_H
#include "Constants.h"

class Note
{
public:
    char pitch = 36;
    int duration = 80;
    char velocity = 100;
    bool gate = true;
    void reset()
    {
        pitch = 36;
        duration = 80;
        velocity = 100;
        gate = true;
    }
    void pitchUp()
    {
        pitch++;
    }

    void pitchDown()
    {
        pitch--;
    }

    void octUp()
    {
        pitch += 12;
    }

    void octDown()
    {
        pitch -= 12;
    }

    void durationUp()
    {
        int newDuration = duration + 4 * (duration / 100 + 1);
        duration = newDuration > 1600 ? 1600 : newDuration;
    }

    void durationDown()
    {
        int newDuration = duration - 4 * (duration / 100 + 1);
        duration = newDuration < 1 ? 1 : newDuration;
    }

    void velocityUp()
    {
        velocity = velocity > 123 ? 127 : velocity + 4;
    }

    void velocityDown()
    {
        velocity = velocity < 4 ? 0 : velocity - 4;
    }

    String toString()
    {
        String res = "";
        res += noteName[pitch % 12];
        res += (pitch / 12 - 3);
        // res += " ";
        return res;
    }
};
#endif