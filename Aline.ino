#include "Sequence.h"
#include "Interface.h"
#include "Scheduler.h"

Interface interface;
Sequence sequence;
Scheduler scheduler;
int clockCount = 0;
void handleClock()
{
  clockCount = (clockCount + 1) % 6;
  if (clockCount == 0)
    scheduler.onClock();
}
void handleStart()
{
  scheduler.resetPosition();
  clockCount = 0;
}
void setup()
{
  Serial.begin(12500);
  while (!Serial)
    ;
  Serial.println("Serial Started");
  interface.setup();
  interface.onSelectionChange = [](char start, char end) -> void {
    scheduler.notes = (sequence.notes);
    Serial.printf("selection changed start: %d  end: %d \n", start, end);
    Serial.print(sequence.toString());
    sequence.setSelection(start, end);
  };

  interface.onPitchUp = []() -> void {
    sequence.selectionPitchUp();
    Serial.print(sequence.toString());
  };

  interface.onPitchDown = []() -> void {
    sequence.selectionPitchDown();
    Serial.print(sequence.toString());
  };

  interface.onOctUp = []() -> void {
    sequence.selectionOctUp();
    Serial.print(sequence.toString());
  };

  interface.onOctDown = []() -> void {
    sequence.selectionOctDown();
    Serial.print(sequence.toString());
  };

  interface.onCopy = []() -> void {
    sequence.copySelection();
  };

  interface.onPaste = []() -> void {
    sequence.pasteToSelection();
  };

  interface.onDurationUp = []() -> void {
    sequence.setSelectionDurationUp();
  };
  interface.onDurationDown = []() -> void {
    sequence.setSelectionDurationDown();
  };
  interface.onVelocityUp = []() -> void {
    sequence.setSelectionVelocityUp();
  };
  interface.onVelocityDown = []() -> void {
    sequence.setSelectionVelocityDown();
  };
  interface.onLengthUp = []() -> void {
    sequence.setSequenceLengthUp();
  };
  interface.onLengthDown = []() -> void {
    sequence.setSequenceLengthDown();
    Serial.printf("echo inteface sequence length %d\n", *interface.sequenceLength);
  };
  interface.onErase = []() -> void {
    sequence.eraseSequence();
    Serial.println("erase issued");
  };
  interface.onGateChange = [](bool *gates) -> void {
    sequence.changeGates(gates);
  };
  interface.onEase = []() -> void {
    sequence.easeSelection();
  };
  interface.stepPosition = &scheduler.currentNote;
  interface.sequenceLength = &sequence.sequenceLength;
  interface.notes = sequence.notes;

  Serial.println("Interface setup done");

  scheduler.init();
  scheduler.notes = sequence.notes;
  scheduler.sequenceLength = &sequence.sequenceLength;

  usbMIDI.setHandleClock(handleClock);
  usbMIDI.setHandleStart(handleStart);
  interface.renderSplash();
}
float meanLoopTime = 0.0f;
int loops = 1;
void loop()
{
  // unsigned long startTime = micros();
  usbMIDI.read();
  interface.update();
  // meanLoopTime = meanLoopTime * loops / (loops + 1) + (millis() - startTime) / (loops + 1.0);
  // loops += 1;
  // if (loops % 1000 == 0)
  // {
  //   Serial.printf("mean loop time %d\n", micros() - startTime);
  // }
}
