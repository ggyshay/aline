
#include "Scheduler.h"

static void handleClockOuter();
static void handleBufferTimeout();
bool noteOffSorter(NoteOffEvt a, NoteOffEvt b)
{
    return a.timeout <= b.timeout;
}

void Scheduler::sendPendingNoteOffs()
{
    int now = micros();
    std::sort(noteOffBuffer.begin(), noteOffBuffer.begin() + noteOffBuffer.size(), noteOffSorter);
    while ((noteOffBuffer.size() > 0) && noteOffBuffer[0].timeout < now)
    {
        usbMIDI.sendNoteOff(noteOffBuffer[0].note.pitch, noteOffBuffer[0].note.velocity, noteOffBuffer[0].channel);
        noteOffBuffer.erase(noteOffBuffer.begin());
    }
}

void Scheduler::deleteFutureNoteOff(char pitch, char velocity, char channel)
{
    for (unsigned int i = 0; i < noteOffBuffer.size(); ++i)
    {
        if ((noteOffBuffer[i].note.pitch == pitch) && (noteOffBuffer[i].note.velocity == velocity) && (noteOffBuffer[i].channel == channel))
        {
            noteOffBuffer.erase(noteOffBuffer.begin() + i);
            return;
        }
    }
}
void Scheduler::scheduleNoteOffBuffer()
{
    int now = micros();
    // Serial.println("scheduler: scheduleNoteOffBuffer");
    if (noteOffBuffer.size() == 0)
    {
        txTimer.end();
        return;
    }
    std::sort(noteOffBuffer.begin(), noteOffBuffer.begin() + noteOffBuffer.size(), noteOffSorter);
    txTimer.update(noteOffBuffer[0].timeout - now);
    // Serial.printf("noteOffBuffer:\n    now: %d\n", now);
    // for (auto e : noteOffBuffer)
    // {
    //     Serial.printf("    t=%d\n", e.timeout);
    // }
    // Serial.println();
}

void Scheduler::bufferTimeout()
{
    // Serial.printf("buffertimeout, %d evts waiting\n", noteOffBuffer.size());
    // Serial.printf("noteOffBuffer:\n    now: %d\n", now);
    // for (auto e : noteOffBuffer)
    // {
    //     Serial.printf("    t=%d dchannel=%d\n", e.timeout, e.channel);
    // }
    // Serial.println();
    if (noteOffBuffer.size() == 0)
    {
        // Serial.println("buffer empty, stopping");
        txTimer.end();
        return;
    }
    sendPendingNoteOffs();
    txTimer.update(noteOffBuffer[0].timeout - micros());
}

void Scheduler::sendNextNote()
{
    unsigned int now = micros();
    sendPendingNoteOffs();
    if (multiMode)
    {
        for (unsigned char i = 0; i < 4; i++)
        {
            int length = pagesLength[i];
            currentPageNote[i] = (currentPageNote[i] + 1) % length;
            if (notes[currentPageNote[i] + 16 * i].gate)
            {
                deleteFutureNoteOff(notes[currentPageNote[i] + 16 * i].pitch, notes[currentPageNote[i] + 16 * i].velocity, i + 1);
                usbMIDI.sendNoteOn(notes[currentPageNote[i] + 16 * i].pitch, notes[currentPageNote[i] + 16 * i].velocity, i + 1);
                NoteOffEvt e;
                e.note = notes[currentPageNote[i] + 16 * i];
                e.timeout = 10 * sixteenth * notes[currentPageNote[i] + 16 * i].duration + now;
                e.channel = i + 1;
                noteOffBuffer.push_back(e);
                if (noteOffBuffer.size() == 1)
                {
                    txTimer.begin(handleBufferTimeout, noteOffBuffer[0].timeout - now);
                }
            }
        }
    }
    else
    {
        // Serial.println("send next note single mode");
        int length = *sequenceLength;
        currentNote = (currentNote + 1) % length;
        if (notes[currentNote].gate)
        {
            deleteFutureNoteOff(notes[currentNote].pitch, notes[currentNote].velocity, 1);
            usbMIDI.sendNoteOn(notes[currentNote].pitch, notes[currentNote].velocity, 1);
            NoteOffEvt e;
            e.note = notes[currentNote];
            e.timeout = now + 10 * sixteenth * notes[currentNote].duration;
            e.channel = 1;
            noteOffBuffer.push_back(e);
            if (noteOffBuffer.size() == 1)
            {
                txTimer.begin(handleBufferTimeout, noteOffBuffer[0].timeout - now);
            }
        }
    }
    scheduleNoteOffBuffer();
}

void Scheduler::resetPosition()
{
    currentNote = 0;
    clocks = 0;
    for (unsigned char i = 0; i < 4; i++)
    {
        currentPageNote[i] = 0;
    }
}

void Scheduler::internalHandleClock()
{
    if (clocks == 0)
    {
        periodStart = millis();
    }
    sixteenth = usignInternalClock ? 15000 / BPM : round(clocks == 0 ? 125 : 1.0f * (millis() - periodStart) / clocks);
    clocks++;

    sendNextNote();
}

void Scheduler::onClock()
{
    if (!usignInternalClock)
        internalHandleClock();
}

void Scheduler::setClockSource(int i)
{
    usignInternalClock = i == 1;
    if (usignInternalClock)
    {
        internalClockTimer.begin(handleClockOuter, 15000 / BPM * 1000);
    }
    else
    {
        internalClockTimer.end();
    }
}
void Scheduler::onBPMChange()
{
    internalClockTimer.update(15000 / BPM * 1000);
}
static void handleClockOuter()
{
    scheduler.internalHandleClock();
}

static void handleBufferTimeout()
{
    scheduler.bufferTimeout();
}