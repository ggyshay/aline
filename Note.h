#ifndef NOTE_H
#define NOTE_H
#include "Constants.h"

class Note
{
public:
    char pitch = 36;
    char duration = 80;
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
        duration = duration > 99 ? 100 : duration + 1;
    }

    void durationDown()
    {
        duration = duration < 2 ? 1 : duration - 1;
    }

    void velocityUp()
    {
        velocity = velocity > 126 ? 127 : velocity + 1;
    }

    void velocityDown()
    {
        velocity = velocity < 1 ? 0 : velocity - 1;
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