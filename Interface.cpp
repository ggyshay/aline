#include "Interface.h"

void Interface::detectSelecion()
{
    int newSelectionStart = 1000;
    int newSelectionEnd = -1;
    for (unsigned char i = 0; i < 16; i++)
    {
        if (pressedButtons[i] && (16 * currentPage + i < newSelectionStart))
            newSelectionStart = 16 * currentPage + i;
        if (pressedButtons[i] && (16 * currentPage + i > newSelectionEnd))
            newSelectionEnd = 16 * currentPage + i;
    }

    if ((newSelectionEnd == -1) && (newSelectionStart == 1000))
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

void Interface::updatePageContext()
{
    for (unsigned char i = 0; i < 16; i++)
        buttons[i]->setPointer(gateMode ? &(notes[i + 16 * currentPage].gate) : pressedButtons + i);
}

void Interface::detectCommands()
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
        pressedAuxButtons[1] = 0;
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
            // for (char i = 0; i < 16; i++)
            // {
            //     gateButtons[i] = true;
            // }
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        }
        else
        {
            onEase();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        }

        pressedAuxButtons[3] = 0;
    }
    if (pressedAuxButtons[4] && currentPage != 0)
    {
        currentPage = 0;
        updatePageContext();
    }
    if (pressedAuxButtons[5] && currentPage != 1)
    {
        currentPage = 1;
        updatePageContext();
    }
    if (pressedAuxButtons[6] && currentPage != 2)
    {
        currentPage = 2;
        updatePageContext();
    }
    if (pressedAuxButtons[7] && currentPage != 3)
    {
        currentPage = 3;
        updatePageContext();
    }
}

void Interface::setupEncodersCallbacks()
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
        for (unsigned char i = 0; i < 16; ++i)
        {
            buttons[i]->setPointer(gateMode ? &(notes[i + 16 * currentPage].gate) : pressedButtons + i);
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
    encoders[4].onIncrement = [this]() -> void {
        menu.onIncrement();
    };
    encoders[4].onDecrement = [this]() -> void {
        menu.onDecrement();
    };
    encoders[4].onClick = [this]() -> void {
        menu.onClick();
    };
}

void Interface::setup()
{
    setupPins();
    setupEncodersCallbacks();

    for (unsigned char i = 0; i < 16; i++)
    {
        buttons[i] = new Button(pressedButtons + i, true);
    }

    for (unsigned char i = 0; i < 8; i++)
    {
        auxButtons[i] = new Button(pressedAuxButtons + i, true);
    }
    disp.init();
    menu.disp = &disp;
    menu.setGraphicsPointer(&disp);
    menu.onLoad = [this](std::vector<int> *stack) -> void {
        onLoad((*stack)[0]);
        Serial.printf("menu onLoad callback from interface %d\n", (*stack)[0]);
    };
    menu.onSave = [this](std::vector<int> *stack) -> void {
        onSave((*stack)[0]);
        Serial.printf("menu onSave callback from interface %d\n", (*stack)[0]);
    };
    menu.init();
}

void Interface::renderSplash()
{
    disp.buildTwoStringScreen("ALINE", " ");
}

void Interface::printSequenceMode()
{
    Note minNote = notes[0];
    Note maxNote = notes[0];
    for (unsigned char i = 0; i <= *sequenceLength; i++)
    {
        minNote = notes[i].pitch < minNote.pitch ? notes[i] : minNote;
        maxNote = notes[i].pitch > maxNote.pitch ? notes[i] : maxNote;
    }
    Note minSelectionNote = notes[selectionStart];
    Note maxSelectionNote = notes[selectionStart];
    for (unsigned char i = selectionStart; i <= selectionEnd; i++)
    {
        minSelectionNote = notes[i].pitch < minSelectionNote.pitch ? notes[i] : minSelectionNote;
        maxSelectionNote = notes[i].pitch > maxSelectionNote.pitch ? notes[i] : maxSelectionNote;
    }
    unsigned char pitches[64] = {};
    for (unsigned char i = 0; i < *sequenceLength; i++)
    {
        pitches[i] = notes[i].pitch;
    }
    if (selectionStart == selectionEnd)
    {
        String upperLine = "NOTE " + minSelectionNote.toString();
        Serial.println(upperLine);

        disp.titlePlot(upperLine.c_str(), pitches, *sequenceLength, false);
    }
    else
    {
        String upperLine = minSelectionNote.toString() + " TO " + maxSelectionNote.toString();
        Serial.println(upperLine);

        disp.titlePlot(upperLine.c_str(), pitches, *sequenceLength, false);
    }
}

void Interface::printVelocityMode()
{
    Note minNote = notes[selectionStart];
    Note maxNote = notes[selectionStart];
    for (unsigned char i = selectionStart + 1; i <= selectionEnd; i++)
    {
        minNote = notes[i].velocity < minNote.velocity ? notes[i] : minNote;
        maxNote = notes[i].velocity > maxNote.velocity ? notes[i] : maxNote;
    }
    unsigned char velocities[64] = {};
    for (unsigned char i = 0; i < *sequenceLength; i++)
    {
        velocities[i] = notes[i].velocity;
    }
    if (selectionStart == selectionEnd)
    {
        String upperLine = "VELOCITY " + String((int)minNote.velocity);
        Serial.println(upperLine);
        disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true);
    }
    else
    {
        String upperLine = String((int)minNote.velocity) + " TO " + String((int)maxNote.velocity);
        Serial.println(upperLine);

        disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true);
    }
}
void Interface::printDurationMode()
{
    Note minNote = notes[selectionStart];
    Note maxNote = notes[selectionStart];
    for (unsigned char i = selectionStart + 1; i <= selectionEnd; i++)
    {
        minNote = notes[i].duration < minNote.duration ? notes[i] : minNote;
        maxNote = notes[i].duration > maxNote.duration ? notes[i] : maxNote;
    }
    unsigned char durations[64] = {};
    for (unsigned char i = 0; i < *sequenceLength; i++)
    {
        durations[i] = notes[i].duration;
    }
    if (selectionStart == selectionEnd)
    {
        String upperLine = "DURATION " + String((int)minNote.duration);
        Serial.println(upperLine);

        disp.titlePlot(upperLine.c_str(), durations, *sequenceLength, true);
    }
    else
    {
        String upperLine = String((int)minNote.duration) + " TO " + String((int)maxNote.duration);
        Serial.println(upperLine);

        disp.titlePlot(upperLine.c_str(), durations, *sequenceLength, true);
    }
}

void Interface::writeToDisplay()
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

void Interface::changeWriteMode(int newMode, int newDisplayMode)
{
    visualisationMode = newMode;
    displayMode = newDisplayMode;
    lastVisualisationChange = millis();
    writeToDisplay();
}

void Interface::writeLedModes(unsigned char i)
{
    if (gateMode)
    {
        writeLed(notes[i + 16 * currentPage].gate);
    }
    else
    {
        switch (visualisationMode)
        {
        case LED_STEP_MODE:
            writeLed((i + 16 * currentPage) == (*stepPosition));
            break;
        case LED_SELECTION_MODE:
            writeLed(((i + 16 * currentPage) >= selectionStart) && ((i + 16 * currentPage) <= selectionEnd));
            break;
        case LED_LENGTH_MODE:
            writeLed((i + 16 * currentPage) < (*sequenceLength));
            break;
        default:
            break;
        }
    }
}

void Interface::update()
{
    if (millis() - lastVisualisationChange > VISUALISATION_CHANGE_DEBOUNCE)
    {
        visualisationMode = LED_STEP_MODE;
    }
    for (unsigned char i = 0; i < 16; i++)
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
