#include "Interface.h"

void getMinMaxNotes(Note *notes, std::function<unsigned char(Note n)> extractor, Note *minNote, Note *maxNote, unsigned char start, unsigned char end)
{
    *minNote = notes[start];
    *maxNote = notes[start];
    for (unsigned char i = start + 1; i <= end; i++)
    {
        *minNote = extractor(notes[i]) < extractor(*minNote) ? notes[i] : *minNote;
        *maxNote = extractor(notes[i]) > extractor(*maxNote) ? notes[i] : *maxNote;
    }
}

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
        return;

    if ((newSelectionStart != selectionStart) || (newSelectionEnd != selectionEnd))
    {
        if ((millis() - lastSelectionChange > SELECTION_CHANGE_DELAY) || (newSelectionStart != newSelectionEnd) || ((newSelectionStart == newSelectionEnd) && (selectionStart == selectionEnd)))
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
        onEase();
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        pressedAuxButtons[3] = 0;
    }
    if (pressedAuxButtons[4])
    {
        if (pressedAuxButtons[0])
        {
            lastSelectionChange = millis();
            onSelectionChange(0, 15);
            selectionStart = 0;
            selectionEnd = 15;
        }
        else if (currentPage != 0)
        {
            currentPage = 0;
            updatePageContext();
        }
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        pressedAuxButtons[4] = false;
    }
    if (pressedAuxButtons[5])
    {
        if (pressedAuxButtons[0])
        {
            lastSelectionChange = millis();
            onSelectionChange(0, 31);
            selectionStart = 0;
            selectionEnd = 31;
        }
        else if (currentPage != 1)
        {
            currentPage = 1;
            updatePageContext();
        }
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        pressedAuxButtons[5] = false;
    }
    if (pressedAuxButtons[6])
    {
        if (pressedAuxButtons[0])
        {
            lastSelectionChange = millis();
            onSelectionChange(0, 47);
            selectionStart = 0;
            selectionEnd = 47;
        }
        else if (currentPage != 2)
        {
            currentPage = 2;
            updatePageContext();
        }
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        pressedAuxButtons[6] = false;
    }
    if (pressedAuxButtons[7])
    {
        if (pressedAuxButtons[0])
        {
            lastSelectionChange = millis();
            onSelectionChange(0, 63);
            selectionStart = 0;
            selectionEnd = 63;
        }
        else if (currentPage != 3)
        {
            currentPage = 3;
            updatePageContext();
        }
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        pressedAuxButtons[7] = false;
    }
}

void Interface::setupEncodersCallbacks()
{
    encoders[0].onIncrement = [this]() -> void {
        if (randomMode)
        {
            setRootUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_ROOT_MODE);
        }
        else
        {
            onPitchUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        }
    };

    encoders[0].onDecrement = [this]() -> void {
        if (randomMode)
        {
            setRootDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_ROOT_MODE);
        }
        else
        {
            onPitchDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
        }
    };
    encoders[0].onClick = [this]() -> void {
        randomMode = !randomMode;
        changeWriteMode(LED_LENGTH_MODE, randomMode ? DISPLAY_RANDOM_ROOT_MODE : DISPLAY_SELECTION_MODE);
    };
    encoders[1].onIncrement = [this]() -> void {
        if (randomMode)
        {
            setScaleUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_SCALE_MODE);
        }
        else if (multiMode)
        {
            pagesLength[currentPage] = pagesLength[currentPage] + 1 > 16 ? 16 : pagesLength[currentPage] + 1;
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_LENGTH_MODE);
        }
        else
        {
            onLengthUp();
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_LENGTH_MODE);
        }
    };
    encoders[1].onDecrement = [this]() -> void {
        if (randomMode)
        {
            setScaleDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_SCALE_MODE);
        }
        else if (multiMode)
        {
            pagesLength[currentPage] = pagesLength[currentPage] - 1 < 1 ? 1 : pagesLength[currentPage] - 1;
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_LENGTH_MODE);
        }
        else
        {
            onLengthDown();
            changeWriteMode(LED_LENGTH_MODE, DISPLAY_LENGTH_MODE);
        }
    };
    encoders[1].onClick = [this]() -> void {
        disp.buildTwoStringScreen("PAU NO CU", "DO BOLSONARO");
    };
    encoders[2]
        .onIncrement = [this]() -> void {
        if (randomMode)
        {
            setSeedUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_SEED_MODE);
        }
        else
        {
            onDurationUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_DURATION_MODE);
        }
    };
    encoders[2].onDecrement = [this]() -> void {
        if (randomMode)
        {
            setSeedDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_SEED_MODE);
        }
        else
        {
            onDurationDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_DURATION_MODE);
        }
    };
    encoders[2].onClick = [this]() -> void {
        disp.buildTwoStringScreen("PAU NO CU", "DO BOLSONARO");
    };
    encoders[3]
        .onIncrement = [this]() -> void {
        if (randomMode)
        {
            setOctavesUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_OCTAVES_MODE);
        }
        else
        {
            onVelocityUp();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        }
    };
    encoders[3].onDecrement = [this]() -> void {
        if (randomMode)
        {
            setOctavesDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_RANDOM_OCTAVES_MODE);
        }
        else
        {
            onVelocityDown();
            changeWriteMode(LED_SELECTION_MODE, DISPLAY_VELOCITY_MODE);
        }
    };
    encoders[3].onClick = [this]() -> void {
        gateMode = !gateMode;
        for (unsigned char i = 0; i < 16; ++i)
        {
            buttons[i]->setPointer(gateMode ? &(notes[i + 16 * currentPage].gate) : pressedButtons + i);
            buttons[i]->isReleaseSensitive = !(buttons[i]->isReleaseSensitive);
        }
        changeWriteMode(LED_LENGTH_MODE, gateMode ? DISPLAY_GATE_MODE : DISPLAY_SELECTION_MODE);
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
    menu.onLoad = [this](std::vector<int> *stack) -> void { onLoad((*stack)[1]); };
    menu.onSave = [this](std::vector<int> *stack) -> void { onSave((*stack)[1]); };
    menu.onMask = [this](std::vector<int> *stack) -> void { onMask((*stack)[1]); };
    menu.onChangeClockSource = [this](std::vector<int> *stack) -> void { onChangeClockSource((*stack)[2]); };
    menu.onChangeBPM = [this](std::vector<int> *stack) -> void { onChangeBPM(); };
    menu.onActivateScaleLock = [this](std::vector<int> *stack) -> void { onActivateScaleLock((*stack)[2]); };
    menu.onChangeScale = [this](std::vector<int> *stack) -> void { onChangeScale((*stack)[2]); };
    menu.onChangeRoot = [this](std::vector<int> *stack) -> void { onChangeRoot((*stack)[2]); };
    menu.onErase = [this](std::vector<int> *stack) -> void {
        onErase();
        onSelectionChange(0, 15);
        selectionStart = 0;
        selectionEnd = 15;
        changeWriteMode(LED_SELECTION_MODE, DISPLAY_SELECTION_MODE);
    };
    menu.onChangeSequenceMode = [this](std::vector<int> *stack) -> void { onChangeSequenceMode((*stack)[1]); multiMode = (*stack)[1] == 1; };
    menu.setBPMPointer(BPM);
    menu.init();
}

void Interface::drawSequenceGraph()
{
    unsigned char pitches[64];
    unsigned char length = multiMode ? pagesLength[currentPage] : *sequenceLength;
    for (unsigned char i = 0; i < *sequenceLength; i++)
        pitches[i] = multiMode ? notes[16 * currentPage + i].pitch : notes[i].pitch;
    disp.plotGraph(pitches, length, false);
}

void Interface::renderSplash()
{
    disp.buildSplash();
}

void Interface::printSequenceMode()
{
    unsigned char pitches[64] = {};
    unsigned char length = 0;
    Note minSelectionNote;
    Note maxSelectionNote;
    minSelectionNote = notes[selectionStart];
    maxSelectionNote = notes[selectionStart];
    for (unsigned char i = selectionStart; i <= selectionEnd; i++)
    {
        minSelectionNote = notes[i].pitch < minSelectionNote.pitch ? notes[i] : minSelectionNote;
        maxSelectionNote = notes[i].pitch > maxSelectionNote.pitch ? notes[i] : maxSelectionNote;
    }
    if (multiMode)
    {
        length = pagesLength[currentPage];
        for (unsigned char i = 0; i < length; i++)
        {
            pitches[i] = notes[i + 16 * currentPage].pitch;
        }
        String upperLine;
        if (selectionStart == selectionEnd)
            upperLine = "NOTE " + minSelectionNote.toString();
        else
            upperLine = minSelectionNote.toString() + " TO " + maxSelectionNote.toString();

        disp.titlePlot(upperLine.c_str(), pitches, length, false, selectionStart, selectionEnd);
    }
    else
    {
        disp.beginSession();
        String upperLine;
        if (selectionStart == selectionEnd)
        {
            upperLine = "NOTE " + minSelectionNote.toString();
        }
        else
        {
            upperLine = minSelectionNote.toString() + " TO " + maxSelectionNote.toString();
        }
        disp.writeLine(upperLine.c_str());
        disp.blackLine();
        drawSequenceGraph();
        disp.selectionLine(selectionStart, selectionEnd, *sequenceLength);
        disp.endSession();
    }
}
void Interface::printLengthMode()
{
    disp.beginSession();
    unsigned char length = multiMode ? pagesLength[currentPage] : *sequenceLength;
    String upperLine = "LENGTH " + String((int)length);
    disp.writeLine(upperLine.c_str());
    disp.blackLine();
    drawSequenceGraph();
    disp.selectionLine(selectionStart, selectionEnd, length);
    disp.endSession();
}

void Interface::printVelocityMode()
{
    Note minNote, maxNote;
    getMinMaxNotes(
        notes, [](Note n) -> unsigned char { return n.velocity; }, &minNote, &maxNote, selectionStart, selectionEnd);

    unsigned char velocities[64] = {};
    for (unsigned char i = 0; i < *sequenceLength; i++)
    {
        velocities[i] = notes[i].velocity;
    }
    if (selectionStart == selectionEnd)
    {
        String upperLine = "VELOCITY " + String((int)minNote.velocity);
        disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true, selectionStart, selectionEnd);
    }
    else
    {
        String upperLine = String((int)minNote.velocity) + " TO " + String((int)maxNote.velocity);
        disp.titlePlot(upperLine.c_str(), velocities, *sequenceLength, true, selectionStart, selectionEnd);
    }
}
void Interface::printDurationMode()
{
    Note minNote, maxNote;
    getMinMaxNotes(
        notes, [](Note n) -> unsigned char { return n.duration; }, &minNote, &maxNote, selectionStart, selectionEnd);

    int durations[64] = {};
    for (unsigned char i = 0; i < *sequenceLength; i++)
        durations[i] = notes[i].duration;

    String upperLine;
    if (selectionStart == selectionEnd)
    {
        upperLine = "DURATION " + String(minNote.duration);
    }
    else
    {
        upperLine = String(minNote.duration) + " TO " + String(maxNote.duration);
    }
    disp.titlePlot(upperLine.c_str(), durations, *sequenceLength, true, selectionStart, selectionEnd);
}

void Interface::writeToDisplay()
{

    switch (displayMode)
    {
    case DISPLAY_SELECTION_MODE:
        printSequenceMode();
        break;
    case DISPLAY_VELOCITY_MODE:
        printVelocityMode();
        break;
    case DISPLAY_DURATION_MODE:
        printDurationMode();
        break;
    case DISPLAY_GATE_MODE:
        disp.buildTwoStringScreen("GATE MODE", " ");
        break;
    case DISPLAY_LENGTH_MODE:
        printLengthMode();
        break;
    case DISPLAY_RANDOM_ROOT_MODE:
        disp.buildTwoStringScreen("R ROOT", noteName[*random_root].c_str());
        break;
    case DISPLAY_RANDOM_SCALE_MODE:
        disp.buildTwoStringScreen("R SCALE", scaleNames[*random_scale].c_str());
        break;
    case DISPLAY_RANDOM_SEED_MODE:
        disp.buildTwoStringScreen("R SEED", String((int)*random_seed).c_str());
        break;
    case DISPLAY_RANDOM_OCTAVES_MODE:
        disp.buildTwoStringScreen("R OCTAVES", String(*random_octaves).c_str());
        break;
    default:
        break;
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
            if (multiMode)
            {
                writeLed((i) < (pagesLength[currentPage]));
            }
            else
            {
                writeLed((i + 16 * currentPage) < (*sequenceLength));
            }
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
