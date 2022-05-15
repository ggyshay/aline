#include "sequencerEngine.h"
#include "scheduler.h"
#include "outputBuffer.h"
#include "midiInterface.h"
#include "Interface.h"
#include "midiEvent.h"
#include "Arduino.h"

namespace std
{
    void __throw_bad_alloc();

    void __throw_length_error(char const *e);

    void __throw_bad_function_call();
    template class basic_string<char>;
    void __throw_logic_error(char const *err);
} // namespace std

MIDIInterface midiInterface;
SequencerEngine sequencer;
Scheduler scheduler;
OutputBuffer outputBuffer;
Interface interface;

bool fullUpdate = false;
void triggerSend()
{
    unsigned long timeToNextEventMicros = outputBuffer.sendNext();
    scheduler.registerTimeToNextEvent(timeToNextEventMicros);
}

void setup()
{
    Serial.begin(112500);
    while (!Serial)
        ;
    Serial.println("Serial started");
    scheduler.setTickCallback(triggerSend);
    scheduler.begin();
    Serial.println("schedule setup");

    midiInterface.begin();
    midiInterface.setHandleClock([]() -> void
                                 {
        scheduler.onMIDIClock();

        MIDIEvent *event_ptr = sequencer.getNextEvent();
        outputBuffer.registerEvent(event_ptr);
        unsigned long timeToNextEventMicros = outputBuffer.sendNext();
        scheduler.registerTimeToNextEvent(timeToNextEventMicros); });
    midiInterface.onStopClock = []() -> void
    {
        sequencer.resetSequencePosition();
    };
    Serial.println("midi interface setup");

    interface.onSelectionChange = [](char start, char end) -> void
    {
        Serial.printf("select: %d-%d\n", start, end);
        sequencer.setSelection(start, end);
    };
    interface.onMessage = [](byte t) -> void
    {
        sequencer.handleInterfaceMessage(t);
    };
    interface.stepPosition = &sequencer.currentNote;
    interface.sequenceLength = &sequencer.sequenceLength;
    interface.notes = sequencer.notes;
    interface.random_root = &sequencer.currentRoot;
    interface.random_scale = &sequencer.currentScale;
    interface.random_seed = &sequencer.currentSeed;
    interface.random_octaves = &sequencer.currentOctaves;
    interface.BPM = &(scheduler.bpmEstimate);
    sequencer.bpmEstimate = &(scheduler.bpmEstimate);
    interface.pagesLength = sequencer.pagesLength;
    interface.onGateChange = [](bool *gates, char currentPage) -> void
    { sequencer.changeGates(gates, currentPage); };
    interface.onChangeClockSource = [](int i) -> void {

    };
    interface.onActivateScaleLock = [](int i) -> void
    {
        sequencer.setScaleLock(i);
    };
    interface.onChangeScale = [](int i) -> void
    {
        sequencer.setScale(i);
    };
    interface.onChangeRoot = [](int i) -> void
    {
        sequencer.setRootNote(i);
    };
    interface.onChangeSequenceMode = [](int i) -> void {

    };
    interface.setup();
    Serial.println("interface setup");

    outputBuffer.setOutputCallback([](EdgeEvent e) -> void
                                   { midiInterface.sendMessage(e); });
    Serial.println("output buffer setup");

    Serial.println("Allsetup, printing splash");
    delay(500);
    interface.renderSplash();
}

void loop()
{
    if (!sequencer.nextEventReady)
    {
        sequencer.calculateNextEvent();
    }
    interface.update();
}