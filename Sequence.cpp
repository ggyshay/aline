
#include "Sequence.h"

void Sequence::setSequenceLengthUp()
{
    sequenceLength = sequenceLength + 1 > 64 ? 64 : sequenceLength + 1;
}
void Sequence::setSequenceLengthDown()
{
    sequenceLength = sequenceLength - 1 < 1 ? 1 : sequenceLength - 1;
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
        notes[i].pitchUp(scaleLockMode, scale, rootNote);
    }
}

void Sequence::selectionPitchDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].pitchDown(scaleLockMode, scale, rootNote);
    }
}

void Sequence::selectionOctUp()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octUp(scaleLockMode, scale, rootNote);
    }
}

void Sequence::selectionOctDown()
{
    for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
    {
        notes[i].octDown(scaleLockMode, scale, rootNote);
    }
}

void Sequence::copySelection()
{
    copySelectionStart = selectionStart;
    copySelectionEnd = selectionEnd;
    for (unsigned char i = selectionStart; i <= selectionEnd; i++)
    {
        copyBuffer[i - selectionStart] = notes[i];
    }
}

void Sequence::pasteToSelection()
{
    for (unsigned char i = 0; i <= copySelectionEnd - copySelectionStart; ++i)
    {
        notes[selectionStart + i] = copyBuffer[i];
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

char Sequence::noteToScale(char n)
{
    return currentRoot + scales[currentScale][n % 12] + 12 * (n / 12);
}

void Sequence::setSeedUp()
{
    currentSeed++;
    randomize();
}

void Sequence::setSeedDown()
{
    currentSeed--;
    randomize();
}

void Sequence::randomize()
{
    randomSeed(currentSeed);
    for (unsigned char i = 0; i < 64; ++i)
    {
        notes[i].pitch = noteToScale((char)random(36, currentOctaves * 12 + 36));
    }
}
void Sequence::setScaleUp()
{
    currentScale = (currentScale + 1) % 7;
    randomize();
}
void Sequence::setScaleDown()
{

    currentScale = (currentScale + 6) % 7;
    randomize();
}
void Sequence::setRootUp()
{
    currentRoot = (currentRoot + 1) % 12;
    for (unsigned char i = 0; i < 64; i++)
    {
        notes[i].pitch++;
    }
}
void Sequence::setRootDown()
{
    currentRoot = (currentRoot + 11) % 12;
    for (unsigned char i = 0; i < 64; i++)
    {
        notes[i].pitch--;
    }
}

void Sequence::setOctavesUp()
{
    currentOctaves++;
    randomize();
}

void Sequence::setOctavesDown()
{
    currentOctaves = currentOctaves - 1 <= 1 ? 1 : currentOctaves - 1;
    randomize();
}

void Sequence::setScaleLock(int i)
{
    scaleLockMode = i == 1;
    // for (unsigned char i = 0; i < 64; ++i)
    // {
    //     notes[i].toScale(scaleLockMode, scale, rootNote);
    // }
}
void Sequence::setScale(int i)
{
    scale = i;
}
void Sequence::setRootNote(int i)
{
    rootNote = i;
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