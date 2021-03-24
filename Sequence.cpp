
#include "Sequence.h"

void Sequence::setSequenceLengthUp()
{
    sequenceLength = sequenceLength + 1 > 64 ? 64 : sequenceLength + 1;
    Serial.printf("new length %d \n", sequenceLength);
}
void Sequence::setSequenceLengthDown()
{
    sequenceLength = sequenceLength - 1 < 1 ? 1 : sequenceLength - 1;
    Serial.printf("new length %d \n", sequenceLength);
}
void Sequence::setSelection(char start, char end)
{
    selectionStart = start;
    selectionEnd = end;
}

void Sequence::selectionPitchUp()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].pitchUp();
    }
}

void Sequence::selectionPitchDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].pitchDown();
    }
}

void Sequence::selectionOctUp()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octUp();
    }
}

void Sequence::selectionOctDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octDown();
    }
}

void Sequence::copySelection()
{
    copySelectionStart = selectionStart;
    copySelectionEnd = selectionEnd;
}

void Sequence::pasteToSelection()
{
    for (unsigned char i = copySelectionStart; i <= copySelectionEnd; ++i)
    {
        notes[selectionStart + i - copySelectionStart] = notes[i];
    }
}

void Sequence::setSelectionDurationUp()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].durationUp();
    }
}

void Sequence::setSelectionDurationDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].durationDown();
    }
}

void Sequence::setSelectionVelocityUp()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].velocityUp();
    }
}

void Sequence::setSelectionVelocityDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].velocityDown();
    }
}

void Sequence::eraseSequence()
{
    for (unsigned char i = 0; i < 64; i++)
    {
        notes[i].reset();
        sequenceLength = 16;
    }
}

void Sequence::changeGates(bool *newGates, unsigned char page)
{
    for (unsigned char i = 0; i < 16; i++)
    {
        notes[i + 16 * page].gate = newGates[i];
    }
}

void Sequence::easeSelection()
{
    char vp = notes[selectionStart].velocity;
    char vq = notes[selectionEnd].velocity;
    for (unsigned char i = selectionStart; i <= selectionEnd; i++)
    {
        notes[i].velocity = (vp - vq) / (selectionStart - selectionEnd) * (i - selectionStart) + vp;
    }
}

String Sequence::toString()
{
    String res;
    res = "";
    for (unsigned char i = 0; i < sequenceLength; i++)
    {
        res += notes[i].toString();
    }
    res += "\n";
    return res;
}