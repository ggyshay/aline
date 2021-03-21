#include "Encoder.h"
#include "Button.h"
#include "HAL.h"
#include "Graphics.h"
#include <stdlib.h>
#include <functional>
#define SELECTION_CHANGE_DELAY 700
#define LED_STEP_MODE 0
#define LED_SELECTION_MODE 1
#define LED_LENGTH_MODE 2
#define DISPLAY_SELECTION_MODE 0
#define DISPLAY_VELOCITY_MODE 1
#define DISPLAY_DURATION_MODE 2
#define DISPLAY_GATE_MODE 3
#define VISUALISATION_CHANGE_DEBOUNCE 2000

class Interface
{
private:
    Encoder encoders[5];
    Button *buttons[16];
    Button *auxButtons[8];
    Graphics disp;

    int values[5] = {0, 0, 0, 0, 0};
    bool pressedButtons[16];
    bool gateButtons[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    bool gateMode = false;
    bool pressedAuxButtons[8];
    char selectionStart = 0;
    char selectionEnd = 0;
    unsigned long lastSelectionChange = 0;
    void detectSelecion()
    {
        int newSelectionStart = 17;
        int newSelectionEnd = -1;
        for (char i = 0; i < 16; i++)
        {
            if (pressedButtons[i] && i < newSelectionStart)
                newSelectionStart = i;
            if (pressedButtons[i] && i > newSelectionEnd)
                newSelectionEnd = i;
        }

        if ((newSelectionEnd == -1) && (newSelectionStart == 17))
        {
            return;
        }

        if ((newSelectionStart != selectionStart) || (newSelectionEnd != selectionEnd))
        {
            if ((millis() - lastSelectionChange > SELECTION_CHANGE_DELAY) || (newSelectionStart != newSelectionEnd))
            {
                lastSelectionChange = millis();
                onSelectionChange(newSelectionStart, newSelectionEnd);
                selectionStart = newSelectionStart;
                selectionEnd = newSelectionEnd;
                changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
            }
        }
    }

    void detectCommands()
    {
        if (pressedAuxButtons[1])
        {
            if (pressedAuxButtons[0])
            {
                onPaste();
                changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
            }
            else
            {
                onCopy();
            }
        }
        if (pressedAuxButtons[2])
        {

            if (pressedAuxButtons[0])
            {
                onOctDown();
            }
            else
            {
                onOctUp();
            }
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
            pressedAuxButtons[2] = 0;
        }
        if (pressedAuxButtons[3])
        {
            if (pressedAuxButtons[0])
            {
                onErase();
                onSelectionChange(0, 15);
                selectionStart = 0;
                selectionEnd = 15;
                for (char i = 0; i < 16; i++)
                {
                    gateButtons[i] = true;
                }
                changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
            }
            else
            {
                onEase();
                changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
            }

            pressedAuxButtons[3] = 0;
        }
    }
    char visualisationMode = LED_STEP_MODE;
    char displayMode = 1;
    unsigned long lastVisualisationChange = 0;

public:
    int *stepPosition;
    char *sequenceLength;
    Note *notes;
    std::function<void(void)> onLengthUp = nullptr;
    std::function<void(void)> onLengthDown = nullptr;
    std::function<void(void)> onDurationUp = nullptr;
    std::function<void(void)> onDurationDown = nullptr;
    std::function<void(void)> onVelocityUp = nullptr;
    std::function<void(void)> onVelocityDown = nullptr;
    std::function<void(void)> onPitchUp = nullptr;
    std::function<void(void)> onPitchDown = nullptr;
    std::function<void(void)> onOctUp = nullptr;
    std::function<void(void)> onOctDown = nullptr;
    std::function<void(char, char)> onSelectionChange = nullptr;
    std::function<void(void)> onCopy = nullptr;
    std::function<void(void)> onPaste = nullptr;
    std::function<void(void)> onErase = nullptr;
    std::function<void(bool *)> onGateChange = nullptr;
    std::function<void(void)> onEase = nullptr;

    void setupEncodersCallbacks()
    {
        encoders[0].onIncrement = [this]() -> void {
            onPitchUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        };

        encoders[0].onDecrement = [this]() -> void {
            onPitchDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        };
        encoders[0].onClick = [this]() -> void {
            gateMode = !gateMode;
            for (char i = 0; i < 16; ++i)
            {
                buttons[i]->setPointer(gateMode ? gateButtons + i : pressedButtons + i);
                buttons[i]->isReleaseSensitive = !(buttons[i]->isReleaseSensitive);
            }
            changeWriteMode(LED_LENGTH_MODE, gateMode ? DISPLAY_GATE_MODE : DISPLAY_SELECTION_MODE);
        };
        encoders[1].onIncrement = [this]() -> void {
            onLengthUp();
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_SELECTION_MODE);
        };
        encoders[1].onDecrement = [this]() -> void {
            onLengthDown();
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_SELECTION_MODE);
        };
        encoders[2].onIncrement = [this]() -> void {
            onDurationUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_DURATION_MODE);
        };
        encoders[2].onDecrement = [this]() -> void {
            onDurationDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_DURATION_MODE);
        };
        encoders[3].onIncrement = [this]() -> void {
            onVelocityUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        };
        encoders[3].onDecrement = [this]() -> void {
            onVelocityDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        };
    }

    void setup()
    {
        setupPins();
        for (char i = 4; i < 5; i++)
        {
            encoders[i].onIncrement = [this, i]() -> void {
                values[i]++;
                Serial.printf("e%d  increment: %d \n", i, values[i]);
            };

            encoders[i].onDecrement = [this, i]() -> void {
                values[i]--;
                Serial.printf("e%d  decrement: %d \n", i, values[i]);
            };

            encoders[i].onClick = [i]() -> void {
                Serial.printf("e%d click!\n", i);
            };
        }
        setupEncodersCallbacks();

        for (char i = 0; i < 16; i++)
        {
            buttons[i] = new Button(pressedButtons + i, true);
            buttons[i]->onToggle = [this]() -> void {
                onGateChange(gateButtons);
            };
        }

        for (char i = 0; i < 8; i++)
        {
            auxButtons[i] = new Button(pressedAuxButtons + i, true);
        }
        disp.init();
    }

    void renderSplash()
    {
        disp.buildTwoStringScreen("ALINE", " ");
    }

    void printSequenceMode()
    {
        Note minNote = notes[selectionStart];
        Note maxNote = notes[selectionStart];
        for (char i = selectionStart + 1; i <= selectionEnd; i++)
        {
            minNote = notes[i].pitch < minNote.pitch ? notes[i] : minNote;
            maxNote = notes[i].pitch > maxNote.pitch ? notes[i] : maxNote;
        }
        if (selectionStart == selectionEnd)
        {
            String upperLine = "NOTE " + minNote.toString();
            char pitches[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                pitches[i] = notes[i].pitch;
            }

            disp.titlePlot(upperLine.c_str(), pitches, *sequenceLength, false);
        }
        else
        {
            String upperLine = minNote.toString() + " TO " + maxNote.toString();
            char pitches[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                pitches[i] = notes[i].pitch;
            }

            disp.titlePlot(upperLine.c_str(), pitches, *sequenceLength, false);
        }
    }

    void printVelocityMode()
    {
        Note minNote = notes[selectionStart];
        Note maxNote = notes[selectionStart];
        for (char i = selectionStart + 1; i <= selectionEnd; i++)
        {
            minNote = notes[i].velocity < minNote.velocity ? notes[i] : minNote;
            maxNote = notes[i].velocity > maxNote.velocity ? notes[i] : maxNote;
        }
        if (selectionStart == selectionEnd)
        {
            String upperLine = "NOTE " + minNote.toString();
            char velocities[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                velocities[i] = notes[i].velocity;
            }

            disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true);
        }
        else
        {
            String upperLine = minNote.toString() + " TO " + maxNote.toString();
            char velocities[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                velocities[i] = notes[i].velocity;
            }

            disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true);
        }
    }
    void printDurationMode()
    {
        Note minNote = notes[selectionStart];
        Note maxNote = notes[selectionStart];
        for (char i = selectionStart + 1; i <= selectionEnd; i++)
        {
            minNote = notes[i].duration < minNote.duration ? notes[i] : minNote;
            maxNote = notes[i].duration > maxNote.duration ? notes[i] : maxNote;
        }
        if (selectionStart == selectionEnd)
        {
            String upperLine = "NOTE " + minNote.toString();
            char durations[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                durations[i] = notes[i].duration;
            }

            disp.titlePlot(upperLine.c_str(), durations, *sequenceLength, true);
        }
        else
        {
            String upperLine = minNote.toString() + " TO " + maxNote.toString();
            char durations[16] = {};
            for (char i = 0; i < *sequenceLength; i++)
            {
                durations[i] = notes[i].duration;
            }

            disp.titlePlot(upperLine.c_str(), durations, *sequenceLength, true);
        }
    }

    void writeToDisplay()
    {
        String line = "";
        if (displayMode == DISPLAY_SELECTION_MODE)
        {
            printSequenceMode();
        }
        else if (displayMode == DISPLAY_VELOCITY_MODE)
        {
            printVelocityMode();
        }
        else if (displayMode == DISPLAY_DURATION_MODE)
        {
            printDurationMode();
        }
        else if (displayMode == DISPLAY_GATE_MODE)
        {

            disp.buildTwoStringScreen("GATE MODE", " ");
        }
        else
        {
            line = "LENGTH";
            disp.buildTwoStringScreen(line.c_str(), ""); //(float)*sequenceLength
        }
    }

    void changeWriteMode(int newMode, int newDisplayMode)
    {
        visualisationMode = newMode;
        displayMode = newDisplayMode;
        lastVisualisationChange = millis();
        writeToDisplay();
    }

    void writeLedModes(char i)
    {
        if (gateMode)
        {
            writeLed(gateButtons[i]);
        }
        else
        {
            switch (visualisationMode)
            {
            case LED_STEP_MODE:
                writeLed(i == (*stepPosition));
                break;
            case LED_SELECTION_MODE:
                writeLed((i >= selectionStart) && (i <= selectionEnd));
                break;
            case LED_LENGTH_MODE:
                writeLed(i < (*sequenceLength));
                break;
            default:
                break;
            }
        }
    }

    void update()
    {
        if (millis() - lastVisualisationChange > VISUALISATION_CHANGE_DEBOUNCE)
        {
            visualisationMode = LED_STEP_MODE;
        }
        for (char i = 0; i < 16; i++)
        {
            sendBits(i);
            writeLedModes(i);
            //le botoes
            buttons[i]->setReading(getButtons());
            if (i < 8)
            {
                auxButtons[i]->setReading(getAuxButtons());
            }

            if (i < 5)
            {

                encoders[i].setReading(getEncodersA(), getEncodersB(), getEncodersC());
            }
            delayMicroseconds(100);
            detectSelecion();
            detectCommands();
            writeLed(0);
        }
    }
};
