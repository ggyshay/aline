#include "IntervalTimer.h"
#include "Arduino.h"
#include "Note.h"

class Scheduler
{
private:
    IntervalTimer txTimer;
    // int quarterNoteTime = 125000;
    // NoteEvent eventQueue[32];
    bool pendingNote = false;

public:
    int currentNote = 0;
    int sixteenth = 120;
    Note lastSentNote;
    Note *notes;
    int clocks = 0;
    long unsigned int periodStart = 0;
    unsigned char *sequenceLength;
    void sendNextNote();
    void sendNoteOff();
    void init();
    void resetPosition();
    // void updateSequence();
    void onClock();
    void onCrudeClock();
    void onStart();
};

extern Scheduler scheduler;