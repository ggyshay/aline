#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "Note.h"

class Sequence
{
private:
    unsigned char selectionStart, selectionEnd;
    unsigned char copySelectionStart, copySelectionEnd;
    char noteToScale(char);

public:
    unsigned char currentScale = 0;
    unsigned char currentRoot = 0;
    unsigned char currentOctaves = 1;
    unsigned char sequenceLength = 16;
    char currentSeed = 0;
    char scale = 0;
    char rootNote = 0;
    bool scaleLockMode = false;
    Note notes[64];
    Note copyBuffer[16];
    void setSequenceLengthUp();
    void setSequenceLengthDown();
    void setSelection(char start, char end);
    void selectionPitchUp();
    void selectionPitchDown();
    void selectionOctUp();
    void selectionOctDown();
    void copySelection();
    void pasteToSelection();
    void setSelectionDurationUp();
    void setSelectionDurationDown();
    void setSelectionVelocityUp();
    void setSelectionVelocityDown();
    void eraseSequence();
    void changeGates(bool *newGates, unsigned char page);
    void easeSelection();
    String toString();

    void setSeedUp();
    void setSeedDown();
    void setScaleUp();
    void setScaleDown();
    void setRootUp();
    void setRootDown();
    void setOctavesUp();
    void setOctavesDown();
    void randomize();
    void setScaleLock(int);
    void setScale(int);
    void setRootNote(int);
};
#endif