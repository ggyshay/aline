#include <stdlib.h>
#include <functional>

#include "midiInterface.h"

static void handleMIDIClockOuter()
{
    midiInterface.midiTick();
}
static void handleMIDIClockStopOuter()
{
    midiInterface.stopClock();
}

static void handleScheduleMIDIReadTick()
{
    midiInterface.update();
}

void MIDIInterface::begin()
{
    usbMIDI.setHandleClock(handleMIDIClockOuter);
    usbMIDI.setHandleStop(handleMIDIClockStopOuter);
    internalClockTimer.begin(handleScheduleMIDIReadTick, 250);
}

void MIDIInterface::midiTick()
{
    if (counter == 0)
    {
        onClockCallback();
    }
    counter = (counter + 1) % 6;
}

void MIDIInterface::stopClock()
{
    counter = 0;
    onStopClock();
}

void MIDIInterface::setHandleClock(std::function<void(void)> callback)
{
    onClockCallback = callback;
}

void MIDIInterface::sendMessage(EdgeEvent event)
{
    usbMIDI.sendNoteOn(event.note, event.velocity, event.channel);
    usbMIDI.send_now();
}

void MIDIInterface::update()
{
    usbMIDI.read();
}