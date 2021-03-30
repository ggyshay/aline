#ifndef NOTE_H
#define NOTE_H
#include "Constants.h"

class Note
{
public:
    int duration = 80;
    char pitch = 36;
    char velocity = 100;
    bool gate = true;
    void reset()
    {
        pitch = 36;
        duration = 80;
        velocity = 100;
        gate = true;
    }
    void pitchUp(bool isScaleLocked, int scale, int root)
    {
        if (isScaleLocked)
        {
            char newPitch = pitch;
            while (((scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12) == pitch)
            {
                newPitch++;
            }
            pitch = (scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12;
        }
        else
        {
            pitch++;
        }
    }

    void pitchDown(bool isScaleLocked, int scale, int root)
    {
        if (isScaleLocked)
        {
            char newPitch = pitch;
            while (((scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12) == pitch)
            {
                newPitch--;
            }
            pitch = (scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12;
        }
        else
        {
            pitch--;
        }
    }

    void octUp(bool isScaleLocked, int scale, int root)
    {
        if (isScaleLocked)
        {
            char newPitch = pitch;
            while (((scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12) == pitch)
            {
                newPitch += 12;
            }
            pitch = (scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12;
        }
        else
        {
            pitch += 12;
        }
    }

    void octDown(bool isScaleLocked, int scale, int root)
    {
        if (isScaleLocked)
        {
            char newPitch = pitch;
            while (((scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12) == pitch)
            {
                newPitch -= 12;
            }
            pitch = (scales[scale][(12 + newPitch - root) % 12] + root) % 12 + (newPitch / 12) * 12;
        }
        else
        {
            pitch -= 12;
        }
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