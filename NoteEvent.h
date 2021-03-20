#include "Note.h"
#define NOTEON_ET 0
#define NOTEOFF_ET 1

class NoteEvent
{
public:
    int pitch;
    int velocity;
    int eventType;
    int transportTime;
    int relativeTime;
    void setDataFromNote(Note n, bool isBegining, int _transportTime)
    {
        pitch = n.pitch;
        velocity = 127;
        eventType = isBegining ? NOTEON_ET : NOTEOFF_ET;
        transportTime = _transportTime;
    }
};
