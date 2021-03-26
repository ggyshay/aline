
#include "Scheduler.h"

static void handleNoteTimeout();

void Scheduler::init()
{
    // txTimer.begin(handleNoteTimeout, 125000);
}

void Scheduler::sendNextNote()
{
    pendingNote = true;
    int length = *sequenceLength;
    currentNote = (currentNote + 1) % length;
    if (notes[currentNote].gate)
    {
        usbMIDI.sendNoteOn(notes[currentNote].pitch, notes[currentNote].velocity, 0);
    }
    lastSentNote = notes[currentNote];
}

void Scheduler::resetPosition()
{
    currentNote = 0;
}

// void Scheduler::updateSequence()
// {
//     for (char i = 0; i < 32; i++)
//     {
//         eventQueue[i].setDataFromNote(notes[i / 2], i % 2 == 0, quarterNoteTime * i / 2 + 80 * (i - 1) / 2); // 0, d, q, q + d
//         if (i > 0)
//         {
//             eventQueue[i].relativeTime = eventQueue[i].transportTime - eventQueue[i - 1].transportTime;
//         }
//     }
//     //TODO: sort event queue
// }

void Scheduler::onClock()
{
    if (pendingNote)
        sendNoteOff();
    if (clocks == 0)
    {
        periodStart = millis();
    }
    sixteenth = round(clocks == 0 ? 125 : 1.0f * (millis() - periodStart) / clocks);
    clocks++;
    // Serial.printf("clocks=%d sixteenth=%d delta=%d durationp=%d duration=%ld \n", clocks, sixteenth, millis() - periodStart, (int)notes[currentNote].duration, 10 * sixteenth * notes[currentNote].duration);

    txTimer.begin(handleNoteTimeout, 10 * sixteenth * (int)notes[currentNote].duration);
    sendNextNote();
}

void Scheduler::sendNoteOff()
{
    txTimer.end();
    if (lastSentNote.gate)
    {
        usbMIDI.sendNoteOff(lastSentNote.pitch, lastSentNote.velocity, 0);
    }
    pendingNote = false;
}

static void handleNoteTimeout()
{
    scheduler.sendNoteOff();
}

// void Scheduler::onCrudeClock()
// {

// }

void Scheduler::onStart()
{
    clocks = 0;
    periodStart = millis();
}