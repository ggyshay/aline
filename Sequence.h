#include "Note.h"

class Sequence
{
private:
    unsigned char selectionStart, selectionEnd;
    unsigned char copySelectionStart, copySelectionEnd;

public:
    Note notes[64];
    unsigned char sequenceLength = 16;
    void setSequenceLengthUp()
    {
        sequenceLength = sequenceLength + 1 > 64 ? 64 : sequenceLength + 1;
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
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].pitchUp();
        }
    }

    void selectionPitchDown()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].pitchDown();
        }
    }

    void selectionOctUp()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].octUp();
        }
    }

    void selectionOctDown()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
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
        for (unsigned char i = copySelectionStart; i <= copySelectionEnd; ++i)
        {
            notes[selectionStart + i - copySelectionStart] = notes[i];
        }
    }

    void setSelectionDurationUp()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].durationUp();
        }
    }

    void setSelectionDurationDown()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].durationDown();
        }
    }

    void setSelectionVelocityUp()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].velocityUp();
        }
    }

    void setSelectionVelocityDown()
    {
        for (unsigned char i = selectionStart; i <= selectionEnd; ++i)
        {
            notes[i].velocityDown();
        }
    }

    void eraseSequence()
    {
        for (unsigned char i = 0; i < 64; i++)
        {
            notes[i].reset();
            sequenceLength = 16;
        }
    }

    void changeGates(bool *newGates, unsigned char page)
    {
        for (unsigned char i = 0; i < 16; i++)
        {
            notes[i + 16 * page].gate = newGates[i];
        }
    }

    void easeSelection()
    {
        char vp = notes[selectionStart].velocity;
        char vq = notes[selectionEnd].velocity;
        for (unsigned char i = selectionStart; i <= selectionEnd; i++)
        {
            notes[i].velocity = (vp - vq) / (selectionStart - selectionEnd) * (i - selectionStart) + vp;
        }
    }

    String toString()
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
};