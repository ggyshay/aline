#include "IntervalTimer.h"
#include "Arduino.h"
#include "Note.h"

class Scheduler
{
private:
    IntervalTimer txTimer;
    IntervalTimer internalClockTimer;
    bool usignInternalClock = false;
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
    void setClockSource(int i);
    void onBPMChange();
    void internalHandleClock();
    float BPM = 120;
};

extern Scheduler scheduler;