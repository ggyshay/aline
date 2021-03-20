// #include <iostream>
// #include <sstream>
#include "Note.h"
// using namespace std;

class Sequence
{
private:
    char selectionStart, selectionEnd;
    char copySelectionStart, copySelectionEnd;

public:
    Note notes[16];
    char sequenceLength = 16;
    void setSequenceLengthUp()
    {
        sequenceLength = sequenceLength + 1 > 16 ? 16 : sequenceLength + 1;
        Serial.printf("new length %d \n", sequenceLength);
    }
    void setSequenceLengthDown()
    {
        sequenceLength = sequenceLength - 1 < 1 ? 1 : sequenceLength - 1;
        Serial.printf("new length %d \n", sequenceLength);
    }
    void setSelection(char start, char end)
    {
        selectionStart = start;
        selectionEnd = end;
    }

    void selectionPitchUp()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].pitchUp();
        }
    }

    void selectionPitchDown()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].pitchDown();
        }
    }

    void selectionOctUp()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].octUp();
        }
    }

    void selectionOctDown()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].octDown();
        }
    }

    void copySelection()
    {
        copySelectionStart = selectionStart;
        copySelectionEnd = selectionEnd;
    }

    void pasteToSelection()
    {
        for (char i = copySelectionStart; i <= copySelectionEnd; ++i)
        {
            notes[selectionStart + i - copySelectionStart] = notes[i];
        }
    }

    void setSelectionDurationUp()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].durationUp();
        }
    }

    void setSelectionDurationDown()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].durationDown();
        }
    }

    void setSelectionVelocityUp()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].velocityUp();
        }
    }

    void setSelectionVelocityDown()
    {
        for (char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].velocityDown();
        }
    }

    String toString()
    {
        String res;
        res = "";
        for (char i = 0; i < 16; i++)
        {
            res += notes[i].toString();
        }
        res += "\n";
        return res;
    }
};