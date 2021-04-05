
#include "Scheduler.h"

static void handleNoteTimeout();
static void handleClockOuter();
static void handleBufferTimeout();
bool noteOffSorter(NoteOffEvt a, NoteOffEvt b)
{
    return a.timeout <= b.timeout;
}
void Scheduler::init()
{
    // txTimer.begin(handleNoteTimeout, 125000);
}

// void Scheduler::recalculateAllDeltas()
// {
//     noteOffBuffer[0].delta = noteOffBuffer[0].timeout - micros();
//     for (unsigned char i = 1; i < noteOffBuffer.size(); i++)
//     {
//         noteOffBuffer[i].delta = noteOffBuffer[i].timeout - noteOffBuffer[i - 1].timeout;
//     }
// }
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
    for (int i = 0; i < noteOffBuffer.size(); ++i)
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
    Serial.println("scheduler: scheduleNoteOffBuffer");
    if (noteOffBuffer.size() == 0)
    {
        txTimer.end();
        return;
    }
    std::sort(noteOffBuffer.begin(), noteOffBuffer.begin() + noteOffBuffer.size(), noteOffSorter);
    // for (unsigned char i = 1; i < noteOffBuffer.size(); i++)
    // {
    //     noteOffBuffer[i].delta = noteOffBuffer[i].timeout - noteOffBuffer[i - 1].timeout;
    // }
    // noteOffBuffer[0].delta = noteOffBuffer[0].timeout - micros();
    txTimer.update(noteOffBuffer[0].timeout - now);
    Serial.printf("noteOffBuffer:\n    now: %d\n", now);
    for (auto e : noteOffBuffer)
    {
        Serial.printf("    t=%d\n", e.timeout);
    }
    Serial.println();
    // while (!noteOffBuffer.empty())
    // {
    //     noteOffBuffer.pop_back();
    // }
}

void Scheduler::bufferTimeout()
{
    int now = micros();
    Serial.printf("buffertimeout, %d evts waiting\n", noteOffBuffer.size());
    Serial.printf("noteOffBuffer:\n    now: %d\n", now);
    for (auto e : noteOffBuffer)
    {
        Serial.printf("    t=%d dchannel=%d\n", e.timeout, e.channel);
    }
    Serial.println();
    if (noteOffBuffer.size() == 0)
    {
        Serial.println("buffer empty, stopping");
        txTimer.end();
        return;
    }
    sendPendingNoteOffs();
    txTimer.update(noteOffBuffer[0].timeout - micros());

    // if (noteOffBuffer[0].timeout < now)
    // {
    //     //send note off
    //     Serial.printf("sending note off t=%d\n", noteOffBuffer[0].timeout);
    //     usbMIDI.sendNoteOff(noteOffBuffer[0].note.pitch, noteOffBuffer[0].note.velocity, noteOffBuffer[0].channel);
    //     // pop 0
    //     noteOffBuffer.erase(noteOffBuffer.begin());
    //     // recalculateAllDeltas();
    //     while ((noteOffBuffer.size() != 0) && (noteOffBuffer[0].timeout <= now))
    //     {
    //         Serial.println("sending simultaneous noteoff");
    //         usbMIDI.sendNoteOff(noteOffBuffer[0].note.pitch, noteOffBuffer[0].note.velocity, noteOffBuffer[0].channel);
    //         noteOffBuffer.erase(noteOffBuffer.begin());
    //     }
    //     // set next timeout
    //     txTimer.update(noteOffBuffer[0].timeout - now);
    // }
    // else
    // {
    //     txTimer.update(noteOffBuffer[0].timeout - micros());
    // }
}

void Scheduler::sendNextNote()
{
    if (multiMode)
    {
        sendPendingNoteOffs();
        unsigned int now = micros();
        for (unsigned char i = 0; i < 4; i++)
        {

            int length = pagesLength[i];
            currentPageNote[i] = (currentPageNote[i] + 1) % length;
            if (notes[currentPageNote[i] + 16 * i].gate)
            {
                // if (pendingNote)
                //     sendNoteOff();
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
                // txTimer.begin(handleNoteTimeout, );
                // pendingNote = true;
            }
        }
        scheduleNoteOffBuffer();
    }
    else
    {
        Serial.println("send next note single mode");
        int length = *sequenceLength;
        currentNote = (currentNote + 1) % length;
        if (notes[currentNote].gate)
        {
            if (pendingNote)
                sendNoteOff();
            usbMIDI.sendNoteOn(notes[currentNote].pitch, notes[currentNote].velocity, 1);
            txTimer.begin(handleNoteTimeout, 10 * sixteenth * notes[currentNote].duration);
            pendingNote = true;
            lastSentNote = notes[currentNote];
        }
    }
}

void Scheduler::resetPosition()
{
    currentNote = 0;
    clocks = 0;
    for (char i = 0; i < 4; i++)
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
    // Serial.printf("clocks=%d sixteenth=%d delta=%d durationp=%d duration=%ld \n", clocks, sixteenth, millis() - periodStart, (int)notes[currentNote].duration, 10 * sixteenth * notes[currentNote].duration);

    sendNextNote();
}

void Scheduler::onClock()
{
    if (!usignInternalClock)
        internalHandleClock();
}

void Scheduler::sendNoteOff()
{
    txTimer.end();
    if (lastSentNote.gate)
    {
        usbMIDI.sendNoteOff(lastSentNote.pitch, lastSentNote.velocity, 1);
        pendingNote = false;
    }
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

static void handleNoteTimeout()
{
    scheduler.sendNoteOff();
}
static void handleClockOuter()
{
    scheduler.internalHandleClock();
}

static void handleBufferTimeout()
{
    scheduler.bufferTimeout();
}