#include "midiEvent.h"
#include "sequencerEngine.h"

void SequencerEngine::calculateNextEvent()
{
    Note curr_note = notes[lastCalculatedNote];
    byte velocity = curr_note.gate ? curr_note.velocity : 0;
    float bpm = *bpmEstimate;
    int duration = curr_note.duration * 150000.0 / bpm;
    // Serial.printf("bpm estimate: %f, duration: %d\n", bpm, duration);
    nextEvent.set(0, curr_note.pitch, velocity, duration);
    nextEventReady = true;
    lastCalculatedNote = (lastCalculatedNote + 1) % sequenceLength;
}

MIDIEvent *SequencerEngine::getNextEvent()
{
    if (nextEventReady == false)
    {
        Serial.println("tryied sending none event");
    }
    currentNote = (currentNote + 1) % sequenceLength;
    nextEventReady = false;
    return &nextEvent;
}

void SequencerEngine::resetSequencePosition()
{
    lastCalculatedNote = 0;
    currentNote = 0;
}

void SequencerEngine::handleInterfaceMessage(byte t)
{
    switch (t)
    {
    case MSG_SEQ_LEN_UP:
        setSequenceLengthUp();
        break;
    case MSG_SEQ_LEN_DOWN:
        setSequenceLengthDown();
        break;
    case MSG_SEL_PITCH_UP:
        selectionPitchUp();
        break;
    case MSG_SEL_PITCH_DOWN:
        selectionPitchDown();
        break;
    case MSG_SEL_OCT_UP:
        selectionOctUp();
        break;
    case MSG_SEL_OCT_DOWN:
        selectionOctDown();
        break;
    case MSG_SEL_DURATION_UP:
        setSelectionDurationUp();
        break;
    case MSG_SEL_DURATION_DOWN:
        setSelectionDurationDown();
        break;
    case MSG_SEL_VELOCITY_UP:
        setSelectionVelocityUp();
        break;
    case MSG_SEL_VELOCITY_DOWN:
        setSelectionVelocityDown();
        break;
    case MSG_ERASE_SEQ:
        eraseSequence();
        break;
    case MSG_EASE_SEL:
        easeSelection();
        break;
    case MSG_SEED_UP:
        setSeedUp();
        break;
    case MSG_SEED_DOWN:
        setSeedDown();
        break;
    case MSG_SCALE_UP:
        setScaleUp();
        break;
    case MSG_SCALE_DOWN:
        setScaleDown();
        break;
    case MSG_ROOT_UP:
        setRootUp();
        break;
    case MSG_ROOT_DOWN:
        setRootDown();
        break;
    case MSG_OCT_UP:
        setOctavesUp();
        break;
    case MSG_OCT_DOWN:
        setOctavesDown();
        break;
    case MSG_RANDOMIZE:
        randomize();
        break;
    case MSG_COPY:
        copySelection();
        break;
    case MSG_PASTE:
        pasteToSelection();
        break;
    }
}

void SequencerEngine::setSequenceLengthUp()
{
    sequenceLength = sequenceLength + 1 > 64 ? 64 : sequenceLength + 1;
}
void SequencerEngine::setSequenceLengthDown()
{
    sequenceLength = sequenceLength - 1 < 1 ? 1 : sequenceLength - 1;
}
void SequencerEngine::setSelection(char start, char end)
{
    selectionStart = start;
    selectionEnd = end;
}

void SequencerEngine::selectionPitchUp()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].pitchUp(scaleLockMode, scale, rootNote);
    }
}

void SequencerEngine::selectionPitchDown()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].pitchDown(scaleLockMode, scale, rootNote);
    }
}

void SequencerEngine::selectionOctUp()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octUp(scaleLockMode, scale, rootNote);
    }
}

void SequencerEngine::selectionOctDown()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octDown(scaleLockMode, scale, rootNote);
    }
}

void SequencerEngine::copySelection()
{
    copySelectionStart = selectionStart;
    copySelectionEnd = selectionEnd;
}

void SequencerEngine::pasteToSelection()
{
    for (byte i = 0; i <= copySelectionEnd - copySelectionStart; ++i)
    {
        notes[selectionStart + i] = notes[copySelectionStart + i];
    }
}

void SequencerEngine::setSelectionDurationUp()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].durationUp();
    }
}

void SequencerEngine::setSelectionDurationDown()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].durationDown();
    }
}

void SequencerEngine::setSelectionVelocityUp()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].velocityUp();
    }
}

void SequencerEngine::setSelectionVelocityDown()
{
    for (byte i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].velocityDown();
    }
}

void SequencerEngine::eraseSequence()
{
    for (byte i = 0; i < 64; i++)
    {
        notes[i].reset();
        sequenceLength = 16;
    }
}

void SequencerEngine::changeGates(bool *newGates, byte page)
{
    for (byte i = 0; i < 16; i++)
    {
        notes[i + 16 * page].gate = newGates[i];
    }
}

void SequencerEngine::easeSelection()
{
    char vp = notes[selectionStart].velocity;
    char vq = notes[selectionEnd].velocity;
    for (byte i = selectionStart; i <= selectionEnd; i++)
    {
        notes[i].velocity = (vp - vq) / (selectionStart - selectionEnd) * (i - selectionStart) + vp;
    }
}

char SequencerEngine::noteToScale(char n)
{
    return currentRoot + scales[currentScale][n % 12] + 12 * (n / 12);
}

void SequencerEngine::setSeedUp()
{
    currentSeed++;
    randomize();
}

void SequencerEngine::setSeedDown()
{
    currentSeed--;
    randomize();
}

void SequencerEngine::randomize()
{
    randomSeed(currentSeed);
    for (byte i = 0; i < 64; ++i)
    {
        notes[i].pitch = noteToScale((char)random(36, currentOctaves * 12 + 36));
    }
}
void SequencerEngine::setScaleUp()
{
    currentScale = (currentScale + 1) % 7;
    randomize();
}
void SequencerEngine::setScaleDown()
{

    currentScale = (currentScale + 6) % 7;
    randomize();
}
void SequencerEngine::setRootUp()
{
    currentRoot = (currentRoot + 1) % 12;
    for (byte i = 0; i < 64; i++)
    {
        notes[i].pitch++;
    }
}
void SequencerEngine::setRootDown()
{
    currentRoot = (currentRoot + 11) % 12;
    for (byte i = 0; i < 64; i++)
    {
        notes[i].pitch--;
    }
}

void SequencerEngine::setOctavesUp()
{
    currentOctaves++;
    randomize();
}

void SequencerEngine::setOctavesDown()
{
    currentOctaves = currentOctaves - 1 <= 1 ? 1 : currentOctaves - 1;
    randomize();
}

void SequencerEngine::setScaleLock(int i)
{
    scaleLockMode = i == 1;
}
void SequencerEngine::setScale(int i)
{
    scale = i;
}
void SequencerEngine::setRootNote(int i)
{
    rootNote = i;
}

String SequencerEngine::toString()
{
    String res;
    res = "";
    for (byte i = 0; i < sequenceLength; i++)
    {
        res += notes[i].toString();
    }
    res += "\n";
    return res;
}