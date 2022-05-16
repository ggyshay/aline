#include "midiEvent.h"
#include "arduino.h"
#include "Constants.h"
#include "Note.h"

class SequencerEngine
{
public:
    void calculateNextEvent();
    bool nextEventReady;
    MIDIEvent *getNextEvent();
    float *bpmEstimate;
    void handleInterfaceMessage(byte t);
    void setSelection(char start, char end);
    void changeGates(bool *newGates, byte page);
    void resetSequencePosition();
    void setScaleLock(int);
    void setScale(int);
    void setRootNote(int);

    byte currentNote = 0;

    byte sequenceLength = 16;
    Note notes[64];
    char pagesLength[4] = {16, 16, 16, 16};
    byte currentScale = 0;
    byte currentRoot = 0;
    byte currentOctaves = 1;
    char currentSeed = 0;

private:
    MIDIEvent nextEvent;
    byte selectionStart, selectionEnd;
    byte copySelectionStart, copySelectionEnd;
    byte lastCalculatedNote = 0;
    char scale = 0;
    char rootNote = 0;
    bool scaleLockMode = false;
    void copySelection();
    void pasteToSelection();
    void setSequenceLengthUp();
    void setSequenceLengthDown();
    void selectionPitchUp();
    void selectionPitchDown();
    void selectionOctUp();
    void selectionOctDown();
    void setSelectionDurationUp();
    void setSelectionDurationDown();
    void setSelectionVelocityUp();
    void setSelectionVelocityDown();
    void eraseSequence();
    void easeSelection();
    void setSeedUp();
    void setSeedDown();
    void setScaleUp();
    void setScaleDown();
    void setRootUp();
    void setRootDown();
    void setOctavesUp();
    void setOctavesDown();
    void randomize();
    char noteToScale(char);

    String toString();
};