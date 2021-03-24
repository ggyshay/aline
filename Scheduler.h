#include "IntervalTimer.h"
#include "Arduino.h"
#include "Note.h"

class Scheduler
{
private:
    IntervalTimer txTimer;
    int quarterNoteTime = 125000;
    // NoteEvent eventQueue[32];

public:
    int currentNote = 0;
    Note lastSentNote;
    Note *notes;
    unsigned char *sequenceLength;
    void sendNextNote();
    void sendNoteOff();
    void init();
    void resetPosition();
    // void updateSequence();
    void onClock();
};

extern Scheduler scheduler;

// class Scheduler
// {

// public:
//     bool hasBegun = false;
//     int notes[4];
//     static int currentNote;

//     IntervalTimer schedulerTimer;
//     void sendNextNote()
//     {
//         // Serial.println("test");
//         Serial.printf("sending note %d", notes[Scheduler::currentNote]);
//         // Scheduler::currentNote = (Scheduler::currentNote + 1) % 4;
//     }
//     void setSequence()
//     {
//         for (char i = 0; i < 4; i++)
//         {
//             notes[i] = 36 + 2 * i;
//         }
//     }

//     void begin()
//     {
//         // if (hasBegun)
//         //     return;
//         // return;
//         Serial.println("begin scheduler");
//         schedulerTimer.begin(sendNextNote, 1250000);
//         setSequence();
//         // hasBegun = true;
//     }
// };

// int Scheduler::currentNote = 0;