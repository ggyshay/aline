#include "Encoder.h"
#include "Button.h"
#include "HAL.h"
#include "DisplayDriver.h"
#include <stdlib.h>
#include <functional>
#define SELECTION_CHANGE_DELAY 700
#define LED_STEP_MODE 0
#define LED_SELECTION_MODE 1
#define LED_LENGTH_MODE 2
#define VISUALISATION_CHANGE_DEBOUNCE 2000

class Interface
{
private:
    Encoder encoders[5];
    Button *buttons[16];
    Button *auxButtons[8];
    DisplayDriver disp;

    int values[5] = {0, 0, 0, 0, 0};
    bool pressedButtons[16];
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
                changeWriteMode(LED_SELECTION_MODE);
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
            pressedAuxButtons[2] = 0;
        }
    }
    char visualisationMode = LED_STEP_MODE;
    char displayMode = ;
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

    void setupEncodersCallbacks()
    {
        encoders[0].onIncrement = [this]() -> void {
            onPitchUp();
            changeWriteMode(LED_SELECTION_MODE);
        };

        encoders[0].onDecrement = [this]() -> void {
            onPitchDown();
            changeWriteMode(LED_SELECTION_MODE);
        };
        encoders[0].onClick = [this]() -> void {
            Serial.printf("pitch click!\n");
        };
        encoders[1].onIncrement = [this]() -> void {
            onLengthUp();
            changeWriteMode(LED_LENGTH_MODE);
        };
        encoders[1].onDecrement = [this]() -> void {
            onLengthDown();
            changeWriteMode(LED_LENGTH_MODE);
        };
        encoders[2].onIncrement = [this]() -> void {
            onDurationUp();
            changeWriteMode(LED_SELECTION_MODE);
        };
        encoders[2].onDecrement = [this]() -> void {
            onDurationDown();
            changeWriteMode(LED_SELECTION_MODE);
        };
        encoders[3].onIncrement = [this]() -> void {
            onVelocityUp();
            changeWriteMode(LED_SELECTION_MODE);
        };
        encoders[3].onDecrement = [this]() -> void {
            onVelocityDown();
            changeWriteMode(LED_SELECTION_MODE);
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
        }

        for (char i = 0; i < 8; i++)
        {
            auxButtons[i] = new Button(pressedAuxButtons + i, true);
        }
        disp.init();
    }

    void renderSplash()
    {
        disp.putScreen("ALINE", " ");
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
            disp.putScreen(upperLine.c_str(), " ");
        }
        else
        {
            String upperLine = "FROM " + minNote.toString();
            String lowerLine = "TO   " + maxNote.toString();
            disp.putScreen(upperLine.c_str(), lowerLine.c_str());
        }
    }

    void writeToDisplay()
    {
        String line = "";
        if (visualisationMode == LED_SELECTION_MODE)
        {
            printSequenceMode();
        }
        else if (visualisationMode == LED_STEP_MODE)
        {
            line = "STEP";
        }
        else
        {
            line = "LENGTH";
            disp.putScreen(line.c_str(), (float)*sequenceLength);
        }
    }

    void changeWriteMode(int newMode)
    {
        visualisationMode = newMode;
        lastVisualisationChange = millis();
        writeToDisplay();
    }

    void writeLedModes(char i)
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
