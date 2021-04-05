#include "IntervalTimer.h"
#include "Arduino.h"
#include <vector>
#include <algorithm>
#include "Note.h"

struct NoteOffEvt
{
    Note note;
    int timeout;
    int delta;
    char channel;
};

bool noteOffSorter(NoteOffEvt a, NoteOffEvt b);
class Scheduler
{
private:
    IntervalTimer txTimer;
    IntervalTimer internalClockTimer;
    bool usignInternalClock = false;
    void scheduleNoteOffBuffer();
    void sendPendingNoteOffs();
    void deleteFutureNoteOff(char pitch, char velocity, char channel);

public:
    bool multiMode = false;
    int currentNote = 0;
    int sixteenth = 120;
    Note *notes;
    int clocks = 0;
    long unsigned int periodStart = 0;
    unsigned char *sequenceLength;
    char *pagesLength;
    unsigned char currentPageNote[4] = {0, 0, 0, 0};
    std::vector<NoteOffEvt> noteOffBuffer;
    void sendNextNote();
    void resetPosition();
    // void updateSequence();
    void onClock();
    void onCrudeClock();
    void setClockSource(int i);
    void onBPMChange();
    void internalHandleClock();
    void bufferTimeout();
    float BPM = 120;
};

extern Scheduler scheduler;